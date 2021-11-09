#pragma once
#include <vector>

namespace ogl {
	

	template<typename T>
	struct MemView {

		using iterator = T*;
		using const_iterator = const T*;

		MemView() : mBegin(nullptr), mEnd(nullptr) {}
		MemView(T* begin, T* end) : mBegin(begin), mEnd(end) {}
		MemView(T* begin, size_t size) : mBegin(begin), mEnd(begin + size) {}

		T& operator[](size_t index) {
			return mBegin[index];
		}

		const T& operator[](size_t index) const {
			return mBegin[index];
		}

		inline size_t size() const { return mEnd - mBegin; }

		inline iterator begin() { return mBegin; }
		inline iterator end() { return mEnd; }
		inline const_iterator begin() const { return mBegin; }
		inline const_iterator end() const { return mEnd; }
		inline iterator rbegin() { return mEnd - 1; }
		inline iterator rend() { return mBegin - 1; }

		inline const_iterator cbegin() const { return mBegin; }
		inline const_iterator cend() const { return mEnd; }
		inline const_iterator crbegin() const { return mEnd - 1; }
		inline const_iterator crend() const { return mBegin - 1; }

		operator bool() const {
			return mBegin == nullptr || mEnd == nullptr;
		}

		T* mBegin;
		T* mEnd;
	};
}
