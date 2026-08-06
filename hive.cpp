/* Performance test from cppreference.com.
 * Taken from the example from the https://en.cppreference.com/cpp/container/hive page.
 * As of August 6, 2026, clang does not support std::hive.
 *
 * Notes:
 *  std::hive is like a hyrbid between std::list and std::vector. 
 *  The container is essentially a linked list of contiguous blocks
 *  of memory. The purpose is to have the good cache locality of a
 *  vector, and combine that with the quick insert/deletion and 
 *  pointer stability of a list.
 *
 * Sources:
 *  https://en.cppreference.com/cpp/container/hive page
 *  https://lemire.me/blog/2026/08/02/how-fast-is-c26s-stdhive/
 *  https://towardsdev.com/cpp26-std-hive-deep-dive-tutorial-5bdaa44f4d94
 */
#include <chrono>
#include <cstddef>
#include <hive>
#include <iomanip>
#include <ios>
#include <iostream>
#include <list>


int main() {
  constexpr auto iterations{10'000'000};
  auto benchmark = [=](auto& container, auto rem) {
    const auto t0{std::chrono::system_clock::now()};
    auto it{container.cbegin()};
    for (auto n{0}; n != iterations; ++n)
         container.insert(it, n);
    while (container.size())
      container.erase(container.cbegin());
    const std::chrono::duration<double> diff{std::chrono::system_clock::now() - t0};
    std::cout << std::fixed << std::setprecision(3) << diff << rem << std::endl;
  };

  std::list<std::size_t> list;
  benchmark(list, " (cold list)");
  benchmark(list, " (warm list)");

  std::hive<std::size_t> hive;
  benchmark(hive, " (cold hive)");
  benchmark(hive, " (warm hive)");

}
