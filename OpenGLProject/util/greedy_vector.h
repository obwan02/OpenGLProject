#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <memory>
#include <unordered_map>	
#include "core.h"

#ifndef OGL_GREEDY_VECTOR_DEFAULT_ALLOCATION_SIZE
#define OGL_GREEDY_VECTOR_DEFAULT_ALLOCATION_SIZE 2
#endif

namespace ogl {

	// Same as a normal vector but never deallocates space unless specifically asked to
	template<typename T, typename Allocator = std::allocator<T>>
	class GreedyVector {
	public:

		// Iterators
		using iterator = T*;
		using const_iterator = const T*;

		GreedyVector() : m_Data(nullptr), m_End(nullptr) {}
		
		GreedyVector(const GreedyVector<T>& other) : m_Allocator(other.m_Allocator) {
			m_Data = m_Allocator.allocate(other.size());
			std::uninitialized_copy(other.m_Data, other.m_End, m_Data);
		}
		GreedyVector(GreedyVector<T>&& other) : m_Data(other.m_Data), m_End(other.m_End), m_Allocator(static_cast<Allocator&&>(other.m_Allocator)) {
			other.m_Data = nullptr; other.m_End = nullptr;
		}
			
		GreedyVector(const Allocator& alloc) : m_Data(nullptr), m_End(nullptr), m_Allocator(alloc) { }
		GreedyVector(Allocator&& alloc) : m_Data(nullptr), m_End(nullptr), m_Allocator(static_cast<Allocator&&>(alloc)) { }
		
		GreedyVector(size_t cap) { m_Data = m_Allocator.allocate(cap); m_End = m_Data; };
		GreedyVector(size_t cap, const Allocator& alloc) : m_Allocator(alloc) { m_Data = m_Allocator.allocate(cap); m_End = m_Data; };
		GreedyVector(size_t cap, Allocator&& alloc) : m_Allocator(static_cast<Allocator&&>(alloc)) { m_Data = m_Allocator.allocate(cap); m_End = m_Data; };
		
		~GreedyVector() { m_Allocator.deallocate(m_Data, m_Capacity); }

		void reserve(size_t count) {
			if(count <= m_Capacity) return;
			T* newBuffer = m_Allocator.allocate(count);
			std::uninitialized_move(m_Data, m_End, newBuffer);
			m_Allocator.deallocate(m_Data, m_Capacity);
			
			m_Capacity = count;
			m_End = newBuffer + (m_End - m_Data);
			m_Data = newBuffer;
		}

		// Same as reserve but 'adds' memory, instead of specifying a certain amount
		// of memory to reserve.
		void reserve_extra(size_t extra) {
			T* newBuffer = m_Allocator.allocate(m_Capacity + extra);
			std::uninitialized_move(m_Data, m_End, newBuffer);
			m_Allocator.deallocate(m_Data, m_Capacity);

			m_Capacity += extra;
			m_End = newBuffer + (m_End - m_Data);
			m_Data = newBuffer;
		}

		void push_back(const T& item) {
			if (m_End - m_Data >= m_Capacity) { reserve_extra(m_AllocationSize); }
			::new(m_End) T(item);
			m_End++;
		}

		void push_back(T&& item) {
			if (m_End - m_Data >= m_Capacity) { reserve_extra(m_AllocationSize); }
			::new(m_End) T(item);
			m_End++;
		}

		void push_back_many(size_t count, const T& value) noexcept {
			if ((m_End - m_Data) + count >= m_Capacity) { reserve_extra(count); }
			std::uninitialized_fill(m_End, m_End + count, value);
			m_End += count;
		}
		
		template<typename ...Args> 
		void emplace_back(Args&&... args) {
			if (m_End - m_Data >= m_Capacity) { reserve_extra(m_AllocationSize); }
			m_Allocator.construct(m_End++, static_cast<Args&&>(args)...);
		}

		template<typename ...Args>
		void emplace_back_many(size_t count, Args&&... args) {
			if ((m_End - m_Data) + count >= m_Capacity) { reserve_extra(count); }
			for (; m_End != m_End + count; m_End++)
				m_Allocator.construct(m_End, static_cast<Args&&>(args)...);
		}

		void pop_back() {
			OGL_DEBUG_ASSERT(m_End != m_Data);
			m_End--;
			if constexpr (std::is_destructible_v<T>)
				m_Allocator.destroy(m_End);
		}

		void pop_back_many(size_t num) {
			OGL_DEBUG_ASSERT(m_End - num >= m_Data);
			if constexpr (std::is_destructible_v<T>)
				std::destroy(m_End - num, m_End);
			m_End -= num;
		}

		void pop_all() {
			if constexpr (std::is_destructible_v<T>)
				std::destroy(m_Data, m_End);
			m_End = m_Data;
		}

		// reallocates the internal buffer to make cap = size
		void shrink_to_size() {
			size_t currentSize = size();
			if(currentSize == cap()) return;

			T* newBuffer = m_Allocator.allocate(currentSize);
			std::uninitialized_move(m_Data, size(), newBuffer);
		}

		// reallocates the internal buffer so that cap = size + padding
		void shrink_to_size(size_t padding) {
			if(size() + padding == cap()) return;
		}

		void resize(size_t size, const T& val) {
			if(this->size() == size) return;
			if (this->size() < size) {
				reserve(size);
				std::uninitialized_fill(m_End, m_Data + size, val);
				m_End = m_Data + size;
				return;
			}

			pop_back_many(this->size() - size);
		}

		void set_allocation_size(size_t size) { m_AllocationSize = size; }
		size_t get_allocation_size() const { return m_AllocationSize; }

		T& first() { return *m_Data; }
		const T& first() const { return *m_Data; }
		T& last() { return *(m_End - 1); }
		const T& last() const { return *(m_Data - 1); }

		T* data() { return m_Data; }
		const T* data() const { return m_Data; }

		inline size_t size() const { return m_End - m_Data; }
		inline size_t cap() const { return m_Capacity; }
		inline bool at_capacity() const{ return cap() == size(); }
		inline bool empty() const { return m_End == m_Data; }

		T& operator[](size_t index) { return m_Data[index]; }
		const T& operator[](size_t index) const { return m_Data[index]; }

		inline iterator begin() { return m_Data; }
		inline iterator end() { return m_End; }
		inline const_iterator begin() const { return m_Data; }
		inline const_iterator end() const { return m_End; }

		inline iterator rbegin() { return m_End - 1; }
		inline iterator rend() { return m_Data - 1; }
		
		inline const_iterator cbegin() const { return m_Data; }
		inline const_iterator cend() const { return m_End; }
		inline const_iterator crbegin() const { return m_End - 1; }
		inline const_iterator crend() const { return m_Data - 1; }

	private:
		Allocator m_Allocator;
		T *m_Data, *m_End;
		size_t m_Capacity = 0;
		size_t m_AllocationSize = OGL_GREEDY_VECTOR_DEFAULT_ALLOCATION_SIZE;
	};
}
