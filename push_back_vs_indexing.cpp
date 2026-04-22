/* Test to see if it's faster to reserve and then push_back to a vector,
 * or to construct a vector of a set size and then just overwriting the default
 * constructed elements via direct indexing.
 *
 * NOTE: I'm pretty sure this is dependent on compiler optimization flag.
 *       In theory, direct indexing should be faster since push_back has the overhead
 *       of checking capacity, incrementing the size, and then writing.
 *       Direct indexing woould just be a direct memory access followed by a write.
 * */
#include <vector>
#include <iostream>
#include <chrono>

template<typename F>
double measure(F func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();

  return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
  const int N = 10000000; // 10,000,000
  
  double direct_indexing = measure([&](){
    std::vector<int> v(N);
    for (int i = 0; i < N; i++)
      v[i] = 7;
  });

  double pushing_back = measure([](){
    std::vector<int> v;
    v.reserve(N);
    for (int i = 0; i < N; i++)
      v.push_back(7);
  });

  std::cout << "Testing direct indexing vs push_back for 10,000,000 vector writes, both initialized as to not have to resize the vector\n";
  std::cout << "Direct indexing: " << direct_indexing << " ms\n";
  std::cout << "Push back: " << pushing_back << " ms\n";

  return 0;
}
