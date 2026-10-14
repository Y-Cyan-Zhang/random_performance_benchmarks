/* Using to test simple branch prediction performance differences.
 * Mainly for pedagolical reasons.
 */
#include <iostream>
#include <chrono>
#include <functional>
#include <vector>
#include <random>

using namespace std::chrono;
using ull = unsigned long long;

/* Maybe try the config variables below with 
 * just standard global vars, i.e.
 * int ARRAY_SIZE, since the compiler can
 * likely optimize away code that involves 
 * constexpr and preprocessor macros
 */
constexpr int ARRAY_SIZE = 100000000; // 100 million
constexpr int RUNS = 5;

static void clobber() {
  asm volatile("" ::: "memory");
}

std::pair<ull, volatile ull> benchmark(std::function<unsigned long long(std::vector<int>&)> func,
                             std::vector<int>& v) {

  volatile unsigned long long sink = 0;
  unsigned long long run_time = 0;

  for (int i = 0; i < RUNS; i++) {
    auto start = high_resolution_clock::now();
    
    clobber();
    sink += func(v);
    clobber();

    auto end = high_resolution_clock::now();
    run_time += duration_cast<milliseconds>(end - start).count();
  }

  return {run_time, sink};
}

unsigned long long no_mispredicts(std::vector<int>& data) {
  unsigned long long result = 0;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (i % 2) {
      result += 1;
    }
  }
  return result;
}

// function with a lot of mispredicts
unsigned long long lot_mispredicts(std::vector<int>& data) {
  unsigned long long result = 0;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (data[i] % 2 == 1) {
      result += 1;
    }
  }
  return result;
}

// function with ~2% mispredicts
unsigned long long some_mispredicts(std::vector<int>& data) {
  unsigned long long result = 0;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    if (data[i] < 250) {
      result += 1;
    }
  }
  return result;
}

int main() {

  // generating random numbers uniformly
  // in the ranges of [0, 255] for the vector
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 255);

  // populating the array with the random values
  std::vector<int> data(ARRAY_SIZE);
  for (auto& x : data) {
    x = dist(gen);
  }

  auto [t1, res1] = benchmark(no_mispredicts, data); 
  auto [t2, res2] = benchmark(some_mispredicts, data); 
  auto [t3, res3] = benchmark(lot_mispredicts, data); 


  std::cout << "No branch mispredictions: " << t1 / RUNS << std::endl;
  std::cout << "Some branch mispredictions: " << t2 / RUNS << std::endl;
  std::cout << "A ton of branch mispredictions: " << t3 / RUNS << std::endl;
}
