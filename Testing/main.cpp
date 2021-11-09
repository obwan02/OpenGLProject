#include <assert.h>
#include <chrono>
#include <cstring>
#include <iterator>
#include <math.h>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#define OGL_ASSERT(x) assert(x)

namespace ogl {

template <typename T, size_t Alignment = alignof(T)> class StackAllocator {
public:
 
  template<typename U, size_t Alignment2> 
  friend class StackAllocator;

  using pointer = T *;
  using const_pointer = const T *;
  using void_pointer = void *;
  using const_void_pointer = const void *;
  using value_type = T;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  template <typename U> struct rebind {
    using other = StackAllocator<U, Alignment>;
  };

  // Always give more memory then you need to account for alignment.
  StackAllocator(uint8_t *buffer, size_t size)
      : m_Buffer(reinterpret_cast<T*>(buffer)), m_Ptr(m_Buffer), m_Size(size), m_Disabled(false) {
    if ((size_t)buffer % Alignment)
      m_Ptr += (size_t)buffer % Alignment;
  }

  template<typename U>
  StackAllocator(const StackAllocator<U, Alignment> &other)
      : m_Buffer(reinterpret_cast<T*>(other.m_Buffer)), m_Ptr(reinterpret_cast<T*>(other.m_Ptr)), m_Size(other.m_Size) {
    other.m_Disabled = true;
    if ((size_t)m_Ptr % Alignment)
        m_Ptr += (size_t)m_Ptr % Alignment;
  }; // We need to make sure on copy that the previous allocator is disabled
  StackAllocator(StackAllocator<T, Alignment> &&) = default;
  StackAllocator<T, Alignment>& operator=(const StackAllocator<T, Alignment> &) = delete;
  StackAllocator<T, Alignment>& operator=(StackAllocator<T, Alignment> &&other) {
    m_Buffer = other.m_Buffer;
    m_Ptr = other.m_Ptr;
    m_Size = other.m_Size;
  }

  T *allocate(size_t n) {
    printf("Allocation %lu bytes\n", n * sizeof(T));
    OGL_ASSERT(!m_Disabled);
    if (m_Ptr + n <= m_Buffer + m_Size) {
      T *p = m_Ptr;
      m_Ptr += n;
      return p;
    }

    puts("Ran out of memory in StackAllocator");
    abort();
  }

  void deallocate(T *ptr, size_t n) noexcept {
    printf("Deallocating %lu bytes\n", n * sizeof(T));
    if (ptr + n == m_Ptr && !m_Disabled)
      m_Ptr -= n;
  }

  template <typename... Args> void construct(T *ptr, Args &&... args) {
    ::new (ptr) T(std::forward<Args>(args)...);
  }

  void destroy(T *ptr) { ptr->~T(); }
  inline size_t max_size() noexcept { return m_Size; }

  bool operator==(StackAllocator<T> stackAlloc) {
    return stackAlloc.m_Buffer == m_Buffer;
  }

  bool operator!=(StackAllocator<T> stackAlloc) {
    return !(stackAlloc == *this);
  }

private:
  T *m_Buffer, *m_Ptr;
  size_t m_Size;
  mutable bool m_Disabled;
};
} // namespace ogl
int main() {

  // Time StackVector
  uint8_t buffer[4500];
  using Alloc = ogl::StackAllocator<std::pair<const int, std::string>>;
  Alloc stackAlloc(buffer, 4500);
  std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>, Alloc> map(stackAlloc);

  for (int i = 0; i < 1000; i++) {
    map[i] = "this is a string";
  }

  printf("20th value: %s", map[19].c_str());
  return 0;
}