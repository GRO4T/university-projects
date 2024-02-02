//
// Created by damian on 8/19/20.
//

#ifndef ZOMBIESHOOTER_INCLUDE_UTILS_H_
#define ZOMBIESHOOTER_INCLUDE_UTILS_H_

#include <filesystem>
#include <vector>
#include <iostream>

namespace ten_thousand_chambers {

#define CLAMP(x, lower, upper) (std::min(upper, std::max(x, lower)))

class Utils {
 public:
  template<typename T>
  static T Clamp(T value, T lower_bound, T upper_bound) {
    return std::min(upper_bound, std::max(value, lower_bound));
  }

  static void ReadDirectory(const std::string &name, std::vector<std::string> &file_list);

  constexpr const static float kEpsilon = std::numeric_limits<float>::epsilon();

  template<typename T>
  static void PrintVector(const std::vector<T> &vec) {
    for (auto elem : vec) {
      std::cout << elem << std::endl;
    }
  }

  template<typename T>
  class Trigger {
   public:
    Trigger(T default_value) { this->default_value = default_value; }

    void Set(T value) { this->value = value; }
    T Get() {
      T temp = value;
      value = default_value;
      return temp;
    }

    T value;
    T default_value;
  };

 private:
  struct path_leaf_string {
    std::string operator()(const std::filesystem::directory_entry &entry) const {
      return entry.path().filename().string();
    }
  };
};

}

#endif //ZOMBIESHOOTER_INCLUDE_UTILS_H_
