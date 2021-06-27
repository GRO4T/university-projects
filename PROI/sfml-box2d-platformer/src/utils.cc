//
// Created by DamianPC on 9/14/2020.
//

#include <utils.h>
#include <algorithm>

namespace ten_thousand_chambers{

void Utils::ReadDirectory(const std::string &name, std::vector<std::string>& file_list) {
  std::filesystem::path p(name);
  std::filesystem::directory_iterator start(p);
  std::filesystem::directory_iterator end;
  std::transform(start, end, std::back_inserter(file_list), path_leaf_string());
}

}

