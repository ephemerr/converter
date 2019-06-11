#include <cstdlib>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <algorithm>
#include <vector>
#include <climits>
#include <cfloat>

std::vector<std::string> units;

std::random_device seed;
std::mt19937 prng(seed());
std::uniform_int_distribution<> percent(0, 100), save(0,20), rand_int(0,INT_MAX);

int main() {
  std::ifstream file("/usr/share/dict/words");
  std::string line="", u2="";

  for(std::size_t n = 0; std::getline(file, line) && units.size() < 999; n++) {
    if( !std::all_of( line.begin(), line.end(), islower )
        || line.find('\'') != std::string::npos
      ) continue;

    if (percent(prng) >= 1) continue;

    if ( u2 == "") {
      if (!units.size() || save(prng) < 1) {
        u2 = line;
        continue;
      }
      std::uniform_int_distribution<> u2_dist(0,units.size()-1);
      u2 = units[u2_dist(prng)];
    } else {
      units.push_back(u2);
    }

    auto v1 = percent(prng)*10 + 1;
    auto v2 = percent(prng)*10 + 1;
    auto &u1 = line;
    std::cout << v1 << " " << u1 << " = " << v2 << " " << u2 << "\n";
    units.push_back(u1);
    u2 = "";
  }
  std::uniform_int_distribution<> rand_unit(0,units.size()-1);
  for(int i=0; i<99; i++) {
    auto u1 = units[rand_unit(prng)];
    auto u2 = units[rand_unit(prng)];
    float v1 = percent(prng);
    std::cout << std::fixed << v1 << " " << u1 << " = ? " << u2 << "\n";
  }
}
