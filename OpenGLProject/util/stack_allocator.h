#pragma once
#include <stdint.h>
#include <utility>
#include <vector>

#include "core.h"

namespace ogl {

	template<typename T, size_t Alignment = alignof(T)>
	class StackAllocator {
	public:
		using pointer = T*;
		using const_pointer = const T*;
		using void_pointer = void*;
		using const_void_pointer = const void*;
		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		template<typename U> struct rebind { using other = StackAllocator<U, Alignment>; };

		// Always give more memory then you need to account for alignment.
		StackAllocator(uint8_t* buffer, size_t size) : m_Buffer(buffer), m_Ptr(buffer), m_Size(size), m_Disabled(false) {
			if ((size_t)buffer % Alignment) m_Ptr += (size_t)buffer % Alignment;
		}

		StackAllocator(const StackAllocator<T, Alignment>& other) : m_Buffer(other.m_Buffer), m_Ptr(other.m_Ptr), m_Size(other.m_Size) { other.m_Disabled = true; }; // We need to make sure on copy that the previous allocator is disabled
		StackAllocator(StackAllocator<T, Alignment>&&) = default;
		StackAllocator<T, Alignment>& operator=(const StackAllocator<T, Alignment>&) = delete;
		StackAllocator<T, Alignment>& operator=(StackAllocator<T, Alignment>&& other) {
			m_Buffer = other.m_Buffer; m_Ptr = other.m_Ptr; m_Size = other.m_Size;
		}

		T* allocate(size_t n) {
			OGL_ASSERT(!m_Disabled);
			if(m_Ptr + n * sizeof(T) <= m_Buffer + m_Size) {
				T* p = reinterpret_cast<T*>(m_Ptr);
				m_Ptr += n * sizeof(T);
				return p;
			}

			OGL_ASSERT(false, "Ran out of memory in StackAllocator.");
		}

		void deallocate(T* ptr, size_t n) noexcept {
			if (reinterpret_cast<uint8_t*>(ptr) + n * sizeof(T) == m_Ptr && !m_Disabled)
				m_Ptr -= n * sizeof(T);
		}

		template<typename ...Args>
		void construct(T* ptr, Args&&... args) {
			::new(ptr) T(std::forward<Args>(args)...);
		}

		void destroy(T* ptr) { ptr->~T(); }
		inline size_t max_size() noexcept { return m_Size; }

		bool operator==(StackAllocator<T> stackAlloc) {
			return stackAlloc.m_Buffer == m_Buffer;
		}

		bool operator!=(StackAllocator<T> stackAlloc) {
			return !(stackAlloc == *this);
		}

	private:
		T* m_Buffer, *m_Ptr;
		size_t m_Size;
		mutable bool m_Disabled;
	};
}