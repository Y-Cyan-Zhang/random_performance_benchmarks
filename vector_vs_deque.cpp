#include <iostream>
#include <vector>
#include <deque>
#include <chrono>

template<typename F>
double measure(F func) {
  auto start = std::chrono::high_resolution_clock::now();
  func(); // this calls the passed in lambda function
  auto end = std::chrono::high_resolution_clock::now();

  return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
  const int N = 10000000;

  // Test 1: Push Back
  double vec_push_back = measure([&]() {
    std::vector<int> v;
    for (int i = 0; i < N; i++) { v.push_back(i); }
  });

  double deq_push_back = measure([&]() {
    std::deque<int> d;
    for (int i = 0; i < N; i++) { d.push_back(i); }
  });
  
  // Test 2: Random Access
  std::vector<int> v(N);
  std::deque<int> d(N);

  double vec_access = measure([&]() {
    volatile int sum = 0;
    for (int i = 0; i < N; i++) { sum += v[i]; }
  });

  double deq_access = measure([&]() {
    volatile int sum = 0;
    for (int i = 0; i < N; i++) { sum += d[i]; }
  });

  std::cout << "Testing vector vs deque for 10 million elements\n";
  std::cout << "Push Back: vector=" << vec_push_back << "ms, deque=" << deq_push_back << "ms\n";
  std::cout << "Random Access: vector=" << vec_access << "ms, deque=" << deq_access << "ms\n";

  return 0;
}
