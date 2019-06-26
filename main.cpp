#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdio>

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

using namespace std;

typedef struct {
  int chain;
  double proportion;
} Unit;

map<string, Unit> unit_sys;

void format_out(double val) {
  if (val >= 1000000 || val < .1)
    cout << scientific << val;
  else
    cout << fixed << val;
}

#include <sys/time.h>
typedef unsigned long long timestamp_t;

static timestamp_t get_timestamp () {
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

timestamp_t t0,t1,t2;


int main() {
  t0 = get_timestamp();

  bool study = true;
  int chain_count = 0;
  string v1, v2, u1, u2, eq;
  while (cin >> v1 >> u1 >> eq >> v2 >> u2)  {
    if (study && v2 == "?") {
      study = false;
      t1 = get_timestamp();
    }
    if (study) {
      DEBUG_MSG ( v1 << u1 << eq << v2  << u2 );
      double val1 = stod(v1);
      double val2 = stod(v2);
      auto chain1 = unit_sys.find(u1);
      auto chain2 = unit_sys.find(u2);
      if (chain1 == unit_sys.end() && chain2 == unit_sys.end()) {
        DEBUG_MSG ( "New chain " << chain_count );
        chain_count++;
        unit_sys[u1] = {chain_count-1,1};
        unit_sys[u2] = {chain_count-1,val2/val1};
      } else {
        if (chain1 == unit_sys.end()) {
          auto old_chain = chain2->second.chain;
          auto k = chain2->second.proportion;
          DEBUG_MSG ( "New unit 1 " << old_chain << k);
          unit_sys[u1] = {old_chain,val1/val2*k};
        } else if (chain2 == unit_sys.end()) {
          auto old_chain = chain1->second.chain;
          auto k = chain1->second.proportion;
          DEBUG_MSG ( "New unit 2 " << old_chain << k);
          unit_sys[u2] = {old_chain,val2/val1*k};
        } else {
          DEBUG_MSG ( "Both units already present" );
          continue;
        }
      }
    } else {
      double val1 = stod(v1);
      auto chain1 = unit_sys.find(u1);
      auto chain2 = unit_sys.find(u2);
      if (chain1 == unit_sys.end() || chain2 == unit_sys.end()) {
        DEBUG_MSG ( "Missing unit" );
        cout << "No conversion is possible." << "\n";
      } else if (chain1->second.chain != chain2->second.chain) {
        DEBUG_MSG ( "Unconvertable units" );
        cout << "No conversion is possible." << "\n";
      } else {
        auto prop1 = chain1->second.proportion;
        auto prop2 = chain2->second.proportion;
        double val2 = val1/prop1*prop2;
        DEBUG_MSG ( "prop1 " << prop1 << " prop2 " << prop2 << " val1 " << val1 << " chain " << chain1->second.chain);
        format_out(val1);
        cout << " " << u1 << " = ";
        format_out(val2);
        cout << " " << u2 << "\n";
      }
    }
  }
  t2 = get_timestamp();
  // cout << "study/parse: " << t1-t0 << " " << t2-t1 << "\n";
  return 0;
}
