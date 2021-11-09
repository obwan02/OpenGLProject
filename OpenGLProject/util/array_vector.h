#pragma once
#include <stdlib.h>
#include <string.h>
#include <memory>

#include "core.h"

namespace ogl {

	template<typename T, size_t Size, size_t Alignment = alignof(T)>
	class ArrayVector {
	public:
		// Iterators
		using iterator = T*;
		using const_iterator = const T*;

		ArrayVector() : m_Begin((T*)m_Buffer), m_End((T*)m_Buffer) {}

		template<size_t Size2>
		ArrayVector(const ArrayVector<T, Size2>& other) {
			size_t thisSize = m_End - m_Begin;
			size_t otherSize = other.m_End - other.m_Begin;

			size_t copySize = (thisSize <= otherSize ? thisSize : otherSize);
			std::copy(other.m_Begin, other.m_End + copySize, m_Buffer);

			m_End = m_Begin + copySize;
		}

		template<size_t Size2>
		ArrayVector(ArrayVector<T, Size2>&& other) {
			size_t thisSize = m_End - m_Begin;
			size_t otherSize = other.m_End - other.m_Begin;

			size_t copySize = (thisSize <= otherSize ? thisSize : otherSize);
			std::copy(other.m_Begin, other.m_End + copySize, m_Buffer);

			m_End = m_Begin + copySize;
		}

		void push_back(T&& value) {
			OGL_DEBUG_ASSERT(m_End - m_Begin < Size);
			::new(m_End) T(std::forward<T>(value));
			m_End++;
		}

		void push_back(const T& value) {
			OGL_DEBUG_ASSERT(m_End - m_Begin < Size);
			::new(m_End) T(value);
			m_End++;
		}

		template<typename ...Args>
		void emplace_back(Args&&... args) noexcept {
			OGL_DEBUG_ASSERT(m_End - m_Begin < Size);
			::new(m_End) T(static_cast<Args&&>(args)...);
			m_End++;
		}

		void push_back_many(size_t count, const T& value) noexcept {
			OGL_DEBUG_ASSERT(m_End - m_Begin + count <= Size);
			std::uninitialized_fill(m_End, m_End + count, value);
			m_Begin += count;
		}

		template<typename ...Args>
		void emplace_back_many(size_t count, Args&&... args) {
			OGL_DEBUG_ASSERT(m_End - m_Begin + count <= Size);
			for (; m_End != m_End + count; m_End++)
				::new(m_End) T(static_cast<Args&&>(args)...);
		}

		void pop_back() {
			OGL_DEBUG_ASSERT(m_Begin != m_End);
			m_End--;
			if constexpr (std::is_destructible_v<T>)
				m_End->~T();
		}

		void pop_back(size_t num) {
			OGL_DEBUG_ASSERT(m_End - num >= m_Begin);
			if constexpr (std::is_destructible_v<T>)
				std::destroy(m_End - num, m_End);
			m_End -= num;
		}

		void pop_all() {
			if constexpr (std::is_destructible_v<T>)
				std::destroy(m_Begin, m_End);
			m_End = m_Begin;
		}

		T& first() { return *m_Begin; }
		const T& first() const { return *m_Begin; }
		T& last() { return *(m_End - 1); }
		const T& last() const { return *(m_End - 1); }


		size_t size() const { return m_End - m_Begin; }
		constexpr size_t cap() const { return Size; }
		constexpr size_t alignment() const { return Alignment; }

		T& operator[](size_t i) {
			OGL_DEBUG_ASSERT(m_Begin + i < m_End);
			return m_Begin[i];
		}

		const T& operator[](size_t i) const {
			OGL_DEBUG_ASSERT(m_Begin + i < m_End);
			return m_Begin[i];
		}

		iterator begin() { return m_Begin; }
		iterator end() { return m_End; }
		const_iterator begin() const { return m_Begin; }
		const_iterator end() const { return m_End; }

		iterator rbegin() { return m_End - 1; }
		iterator rend() { return m_Begin - 1; }

		const_iterator cbegin() const { return m_Begin; }
		const_iterator cend() const { return m_End; }
		const_iterator crbegin() const { return m_End - 1; }
		const_iterator crend() const { return m_Begin - 1; }
	private:
		T* m_Begin, * m_End;
		alignas(Alignment) uint8_t m_Buffer[Size * sizeof(T)];
	};
}
