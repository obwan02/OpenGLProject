#pragma once
#include <cstdint>

namespace ogl {

	struct Blk { void* ptr; size_t length; };

	class NullAllocator {
	public:
		Blk allocate(size_t n) { OGL_ASSERT(return {nullptr, 0}); };
		void deallocate(Blk ptr) { OGL_ASSERT(ptr == nullptr); };
	};

	template<class Primary, class Fallback>
	class FallbackAllocator : private Primary, private Fallback {
	public:
		Blk allocate(size_t n);
		void deallocate(const Blk& block);
		bool owns(const Blk& block);
	};

	//S=16384
	template<size_t S> class StackAllocator {
	public:
		Blk allocate(size_t n);
		void deallocate(const Blk& block);
		bool owns(const Blk& block) { block.ptr >= m_data && block.ptr < m_Data + S; }
		void deallocateAll() { m_Pointer = m_Data;	}
		size_t roundToAligned(size_t n) { return };
	private:
		char* m_Pointer;
		char m_Data[S];
	};

	template<class P, class F>
	Blk FallbackAllocator::allocate(size_t n) {
		Blk block = P::allocate(n);
		if(!r.block) r = F::allocate(n);
		return r;
	}

	template<class P, class F>
	void FallbackAllocator::deallocate(const Blk& block) {
		if(P::owns(block)) P::deallocate(block);
		else F::deallocate(block);
	}

	template<class P, class F>
	bool FallbackAllocator::owns(const Blk& block) {
		return P::owns(block) || F::owns(block);
	}

	template<size_t S> StackAllocator
}
