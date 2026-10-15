/* Branchless vs branchful loop benchmark.
 * Mainly for pedagogical reasons.
 *
 * Both loops compute the same thing: the sum of every element >= 128.
 * Each is run on random data (branch is a coin flip -> ~50% mispredicts)
 * and on sorted data (branch is perfectly predictable).
 *
 * Expected result: the branchful loop is fast on sorted data and slow on
 * random data, while the branchless loop takes about the same time on both.
 *
 * Compile instructions (copy and paste):
    clang++ -std=c++17 -O0 branchless_vs_branchful.cpp && ./a.out
    clang++ -std=c++17 -O1 branchless_vs_branchful.cpp && ./a.out
    clang++ -std=c++17 -O2 branchless_vs_branchful.cpp && ./a.out
    clang++ -std=c++17 -O3 branchless_vs_branchful.cpp && ./a.out
 *
 * Note: at -O1 and above the compiler loves to turn a simple if into a
 * cmov/csel (i.e. make it branchless for us) or vectorize it, which hides
 * the effect. To keep a real branch, the branchful loop puts an empty
 * asm volatile inside the if body, and both loops disable vectorization
 * so we compare scalar code against scalar code.
 *
 * Written by Claude
 */
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace std::chrono;
using ull = unsigned long long;

constexpr int ARRAY_SIZE = 50000000; // 50 million
constexpr int RUNS = 5;
constexpr int THRESHOLD = 128;

static void clobber() {
  asm volatile("" ::: "memory");
}

// real conditional jump on every element
ull branchful(const std::vector<int>& data) {
  ull sum = 0;
#pragma clang loop vectorize(disable) interleave(disable)
  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (data[i] >= THRESHOLD) {
      asm volatile(""); // keeps the compiler from converting this to a cmov
      sum += data[i];
    }
  }
  return sum;
}

// no jump: build an all-ones / all-zeros mask from the comparison
ull branchless(const std::vector<int>& data) {
  ull sum = 0;
#pragma clang loop vectorize(disable) interleave(disable)
  for (int i = 0; i < ARRAY_SIZE; i++) {
    int x = data[i];
    int mask = -static_cast<int>(x >= THRESHOLD); // 0xFFFFFFFF or 0
    sum += static_cast<ull>(x & mask);
  }
  return sum;
}

template <typename F>
std::pair<double, ull> benchmark(F func, const std::vector<int>& data) {
  volatile ull sink = 0;
  double run_time = 0.0;

  // untimed warm-up so caches are warm
  clobber();
  sink = sink + func(data);
  clobber();

  for (int i = 0; i < RUNS; i++) {
    auto start = high_resolution_clock::now();

    clobber();
    sink = sink + func(data);
    clobber();

    auto end = high_resolution_clock::now();
    run_time += duration<double, std::milli>(end - start).count();
  }

  return {run_time / RUNS, sink};
}

int main() {
  // uniform random values in [0, 255], so x >= 128 is a coin flip
  std::mt19937 gen(42);
  std::uniform_int_distribution<int> dist(0, 255);

  std::vector<int> random_data(ARRAY_SIZE);
  for (auto& x : random_data) {
    x = dist(gen);
  }

  std::vector<int> sorted_data = random_data;
  std::sort(sorted_data.begin(), sorted_data.end());

  auto [t1, s1] = benchmark(branchful, random_data);
  auto [t2, s2] = benchmark(branchful, sorted_data);
  auto [t3, s3] = benchmark(branchless, random_data);
  auto [t4, s4] = benchmark(branchless, sorted_data);

  std::cout << "Branchful,  random data: " << t1 << " ms\n";
  std::cout << "Branchful,  sorted data: " << t2 << " ms\n";
  std::cout << "Branchless, random data: " << t3 << " ms\n";
  std::cout << "Branchless, sorted data: " << t4 << " ms\n";

  // all four sinks should be identical multiples of the true sum
  if (!(s1 == s2 && s2 == s3 && s3 == s4)) {
    std::cerr << "Mismatched results!\n";
    return 1;
  }
}
