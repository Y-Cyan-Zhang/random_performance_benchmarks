#include <string>
#include <string_view>
#include <unordered_set>
#include <bitset>
#include <iostream>
#include <random>

int unordered_set_solution(std::string_view s) {
  if (s.size() < 3)
    return 0;

  std::unordered_set<std::string> unique;
  unique.reserve(s.size() - 2);
  for (size_t i = 0; i + 2 < s.size(); ++i) {
    unique.emplace(s.substr(i, 3)); 
  }

  return static_cast<int>(unique.size());
}

int bitset_solution(std::string_view s) {
  if (s.size() < 3)
    return 0;
  std::bitset<26 * 26 * 26> unique;
  int count = 0;

  for (size_t i = 0; i + 2 < s.size(); ++i) {
    unsigned a = s[i] - 'a';
    unsigned b = s[i+1] - 'a';
    unsigned c = s[i+2] - 'a';

    size_t idx = (a * 26 + b) * 26 + c;
    if (!unique.test(idx)) {
      unique.set(idx);
      ++count;
    }
  }
  return count;
}


// ============== Simple Unit Tests ==============

void run_unit_tests() {
    struct TestCase {
        std::string input;
        int expected;
    };
    
    std::vector<TestCase> tests = {
        {"", 0},
        {"a", 0},
        {"ab", 0},
        {"abc", 1},
        {"aaa", 1},
        {"abcabc", 3},
        {"aaaaa", 1},
    };
    
    int passed = 0;
    for (const auto& tc : tests) {
        int r1 = unordered_set_solution(tc.input);
        int r2 = bitset_solution(tc.input);
        
        bool ok = (r1 == tc.expected) && (r2 == tc.expected);
        if (ok) {
            ++passed;
            std::cout << "✓ PASS: \"" << tc.input << "\"\n";
        } else {
            std::cout << "✗ FAIL: \"" << tc.input << "\" "
                      << "expected=" << tc.expected 
                      << " unordered_set=" << r1 
                      << " bitset=" << r2 << "\n";
        }
    }
    std::cout << "\nPassed: " << passed << "/" << tests.size() << "\n\n";
}

// ============== Simple Performance Test ==============

template<typename Func>
double benchmark(Func f, std::string_view s, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        volatile int result = f(s);  // volatile prevents optimization
        (void)result;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / static_cast<double>(iterations);
}

void run_performance_tests() {
    std::cout << "Performance Tests\n";
    std::cout << "=================\n\n";
    
    // Generate test strings
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis('a', 'z');
    
    std::vector<size_t> sizes = {100, 1000, 10000, 100000, 1000000};
    
    for (size_t size : sizes) {
        std::string s;
        s.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            s += static_cast<char>(dis(gen));
        }
        
        int iterations = std::max(1, static_cast<int>(1000000 / size));
        
        double time_set = benchmark(unordered_set_solution, s, iterations);
        double time_bit = benchmark(bitset_solution, s, iterations);
        
        std::cout << "Size: " << size << "\n";
        std::cout << "  unordered_set: " << time_set << " µs\n";
        std::cout << "  bitset:        " << time_bit << " µs\n";
        std::cout << "  speedup:       " << time_set / time_bit << "x\n\n";
    }
}

int main() {
    run_unit_tests();
    run_performance_tests();
    return 0;
}
