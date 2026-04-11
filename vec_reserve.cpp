#include <iostream>
#include <vector>
#include <chrono>


class complexObject {
  
};

template<typename F>
double measure(F func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();

  return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
  const int N = 10000000; // 10 mil

  double not_reserved_push_back = measure([&](){
    std::vector<int> v;
    for (int i = 0; i < N; i++) { v.push_back(i); }
  });
  
  std::vector<int> reserved;
  reserved.reserve(N);
  double reserved_push_back = measure([&](){
    for (int i = 0; i < N; i++) { reserved.push_back(i); }
  });

  std::cout << "Not reserved vector, 10 million push_backs: " << not_reserved_push_back << "ms\n";
  std::cout << "Reserved vector, 10 million push_backs: " << reserved_push_back << "ms\n";

  return 0;
}
