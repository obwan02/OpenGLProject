#pragma once
#include <vector>
#include <memory>

#include "util/greedy_vector.h"
#include "core.h"

#define OGL_STORAGE_PAGE_SIZE 1024u

namespace ogl {

	typedef size_t storage_id;
	inline constexpr storage_id empty_storage_id = -1U;

	
	template<typename T>
	struct DefaultStorageAllocators {
		using PackedObjAllocator = std::allocator<T>;

		using PackedIDAllocator = std::allocator<storage_id>;
		using FreeListAllocator = std::allocator<storage_id>;
		using SparsePagePointerAllocator = std::allocator<size_t*>;
		using PageAllocator = std::allocator<size_t>;
	};

	

	
	// Basically a sparse set with that has a list of objects associated with
	// the packed array.
	template<typename T, class Allocators = DefaultStorageAllocators<T>>
	class Storage {
	public:

		using iterator = typename GreedyVector<T, typename Allocators::PackedObjAllocator>::iterator;
		using const_iterator = typename GreedyVector<T, typename Allocators::PackedObjAllocator>::const_iterator;

		Storage() {
			m_PackedIds.set_allocation_size(32); 
			m_FreeList.set_allocation_size(32);
			m_PackedObjs.set_allocation_size(32); 
			m_SparsePages.set_allocation_size(1); 
			ensure_page(0); 
		}

		~Storage() {
			for (auto& item : m_SparsePages) {
				if(item != nullptr)
					m_PageAllocator.deallocate(item, OGL_STORAGE_PAGE_SIZE);
			}
		}

		template<typename ...Args>
		storage_id create(Args&&... args) {
			storage_id newId;
			if (!m_FreeList.empty()) {
				newId = m_FreeList.last();
				m_FreeList.pop_back();
				m_SparsePages[page(newId)][offset(newId)] = m_PackedIds.size();
			}
			else {
				newId = incr_counter();
				ensure_page(page(newId))[offset(newId)] = m_PackedIds.size();
			}

			m_PackedIds.push_back(newId);
			m_PackedObjs.emplace_back(static_cast<Args&&>(args)...);
			return newId;
		}

		bool valid(storage_id id) const {
			return page(id) < m_SparsePages.size() && m_SparsePages[page(id)] != nullptr && m_SparsePages[page(id)][offset(id)] != empty_storage_id;
		}

		void remove(storage_id id) {
			OGL_DEBUG_ASSERT(valid(id));

			// quick speed up if you're just popping the last element
			storage_id last = m_PackedIds.last();
			if (last == id) {
				m_PackedIds.pop_back();
				m_PackedObjs.pop_back();
				m_SparsePages[page(id)][offset(id)] = empty_storage_id;
				return;
			}

			// Change the index the last id is pointing to
			m_SparsePages[page(last)][offset(last)] = std::exchange(m_SparsePages[page(id)][offset(id)], empty_storage_id);
			
			// Swap the last in the packed with the element to be removed
			m_PackedIds[m_SparsePages[page(last)][offset(last)]] = std::move(m_PackedIds.last());
			m_PackedObjs[m_SparsePages[page(last)][offset(last)]] = std::move(m_PackedObjs.last());
			
			// remove the last elements
			m_PackedIds.pop_back(); m_PackedObjs.pop_back();

			// push back the removed id into the free list
			m_FreeList.push_back(id);
		}

		// try_remove will try to remove the storage_id only if the 
		// id is valid for this storage. Returns if the item is contained in the set.
		bool try_remove(storage_id id) {
			if (valid(id)) {
				remove(id);
				return true; 
			}
			return false;
		}

		template<typename F>
		void for_each(F&& func) {
			static_assert(std::is_invocable_v<F, T> || std::is_invocable_v<F, T&> || std::is_invocable_v<F, const T&>, 
				"F must be a invokable type that takes a T, T& or a const T& ");
			for (auto& item : m_PackedObjs)
				func(item);
		}

		template<typename F>
		void for_each(F&& func) const {
			static_assert(std::is_invocable_v<F, T> || std::is_invocable_v<F, T&> || std::is_invocable_v<F, const T&>,
				"F must be a invokable type that takes a T, T& or a const T& ");
			for (const auto& item : m_PackedObjs)
				func(item);
		}

		inline T& get(storage_id id) { OGL_DEBUG_ASSERT(valid(id)); return m_PackedObjs[m_SparsePages[page(id)][offset(id)]]; }
		inline const T& get(storage_id id) const { OGL_DEBUG_ASSERT(valid(id));  return m_PackedObjs[m_SparsePages[page(id)][offset(id)]]; }
		
		inline T* try_get(storage_id id) { return valid(id) ? &get(id) : nullptr; }
		inline const T* try_get(storage_id id) const { return valid(id) ? &get(id) : nullptr; }

		inline T& operator[](storage_id id) { return get(id); }
		inline const T& operator[](storage_id id) const { return get(id); }

		inline iterator begin() { return m_PackedObjs.begin(); }
		inline iterator end() { return m_PackedObjs.end(); }
		inline iterator rbegin() { return m_PackedObjs.rbegin(); }
		inline iterator rend() { return m_PackedObjs.rend(); }

		inline const_iterator cbegin() { return m_PackedObjs.cbegin(); }
		inline const_iterator cend() { return m_PackedObjs.cend(); }
		inline const_iterator crbegin() { return m_PackedObjs.crbegin(); }
		inline const_iterator crend() { return m_PackedObjs.crend(); }


	private:
		constexpr inline storage_id page(storage_id index) const { return index / OGL_STORAGE_PAGE_SIZE; }
		constexpr inline storage_id offset(storage_id index) const { return index % OGL_STORAGE_PAGE_SIZE;  }

		inline void allocate_page(storage_id page) {
			m_SparsePages[page] = m_PageAllocator.allocate(OGL_STORAGE_PAGE_SIZE);
			std::uninitialized_fill(m_SparsePages[page], m_SparsePages[page] + OGL_STORAGE_PAGE_SIZE, empty_storage_id);
		}

		inline void deallocate_page(storage_id page) {
			m_PageAllocator.deallocate(m_SparsePages[page], OGL_STORAGE_PAGE_SIZE);
			m_SparsePages[page] = nullptr;
		}

		size_t* ensure_page(storage_id page) {
			if (page >= m_SparsePages.size()) {
				m_SparsePages.reserve(page + 1);
				m_SparsePages.push_back_many(page + 1 - m_SparsePages.size(), nullptr);
				allocate_page(page);
				return m_SparsePages[page];
			}

			if (m_SparsePages[page] == nullptr) allocate_page(page);
			return m_SparsePages[page];
		}

		inline storage_id incr_counter() {
			return ++counter;
		}

	private:
		GreedyVector<storage_id, typename Allocators::PackedIDAllocator> m_PackedIds;
		GreedyVector<T, typename Allocators::PackedObjAllocator> m_PackedObjs;

		GreedyVector<storage_id, typename Allocators::FreeListAllocator> m_FreeList;
		GreedyVector<size_t*, typename Allocators::SparsePagePointerAllocator> m_SparsePages;
		typename Allocators::PageAllocator m_PageAllocator;
	
		storage_id counter = 0;
	};
}
