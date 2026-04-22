/* Test to see if it's faster to reserve and then push_back to a vector,
 * or to construct a vector of a set size and then just overwriting the default
 * constructed elements via direct indexing.
 *
 * NOTE: I'm pretty sure this is dependent on compiler optimization flag.
 *       In theory, direct indexing should be faster since push_back has the overhead
 *       of checking capacity, incrementing the size, and then writing.
 *       Direct indexing woould just be a direct memory access followed by a write.
 * NOTE: (After running), direct indexing is consistently faster, but I was wrong
 *       about direct indexing being a direct memory access. Since it's on the
 *       std::vector container, it actually calls the operator[] function, which
 *       has it's own overhead. However, for optimization levels above -O0, this 
 *       is not true, as it gets inlined to direct memory access.
 *        e.g. vec[i] = 7 -> *(vec + i) = 7
 * */
#include <vector>
#include <iostream>
#include <chrono>

template<typename F>
double measure(F func) {
  auto start = std::chrono::high_resolution_clock::now();
  auto result = func(); // store result so the compiler doesn't optimize away the dead code
  auto end = std::chrono::high_resolution_clock::now();
  volatile auto sink = result;  // Prevent optimization
  return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
  const int N = 10000000; // 10,000,000
  const int runs = 20;
  
  double cum_direct_indexing = 0;
  double cum_push_back = 0;
  for (int run = 0; run < runs; run++) {
    double direct_indexing = measure([&](){
      std::vector<int> v(N);
      for (int i = 0; i < N; i++)
        v[i] = 7;
      return v[N/2];  // Force compiler to keep the work
    });

    double pushing_back = measure([](){
      std::vector<int> v;
      v.reserve(N);
      for (int i = 0; i < N; i++)
        v.push_back(7);
      return v[N/2];  // Force compiler to keep the work
    });

    cum_direct_indexing += direct_indexing;
    cum_push_back += pushing_back;
  }

  std::cout << "Testing direct indexing vs push_back for 10,000,000 vector writes averaged of 20 runs. (Both initialized as to not have to resize the vector)\n";
  std::cout << "Direct indexing: " << cum_direct_indexing / runs << " ms\n";
  std::cout << "Push back: " << cum_push_back / runs << " ms\n";

  return 0;
}
