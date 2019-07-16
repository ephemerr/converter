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

typedef map<string, Unit>::iterator UnitIterator;

map<string, Unit> unit_sys;

map<int, vector<UnitIterator>*> chains;

int chain_count = 0;

void insert_unit(const string& key, const Unit &unit) {
    auto ret = unit_sys.insert(pair<string, Unit>(key,unit));
    chains[unit.chain]->push_back(ret.first);
    DEBUG_MSG ( "Insert unit " <<  key << " " << ret.second);

}

void new_chain() {
  chains.insert(pair<int, vector<UnitIterator>*>(chain_count,new vector<UnitIterator>));
  chain_count++;
}

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
        DEBUG_MSG ( "New chain " << chain_count);
        new_chain();
        insert_unit(u1,{chain_count-1,1});
        insert_unit(u2,{chain_count-1,val2/val1});
      } else {
        if (chain1 == unit_sys.end()) {
          auto old_chain = chain2->second.chain;
          auto k = chain2->second.proportion;
          DEBUG_MSG ( "New unit 1 " << old_chain << k);
          insert_unit(u1,{old_chain,val1/val2*k});
        } else if (chain2 == unit_sys.end()) {
          auto old_chain = chain1->second.chain;
          auto k = chain1->second.proportion;
          DEBUG_MSG ( "New unit 2 " << old_chain << k);
          insert_unit(u1,{old_chain,val2/val1*k});
        } else if (chain1 != chain2){
          DEBUG_MSG ( "Merge chains" << chain1->second.chain << " " << chain2->second.chain );
          auto k1 = chain1->second.proportion;
          auto k2 = chain2->second.proportion;
          for (auto u: *chains[chain2->second.chain]) {
            auto unit = &u->second;
            unit->chain = chain1->second.chain;
            unit->proportion = unit->proportion / k2 * k1 * val2 / val1;
            chains.erase(chain2->second.chain);
          }
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
