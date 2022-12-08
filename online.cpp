#include <iostream>
#include <unordered_map>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <bitset>
#include <sstream>
#include <map>
#include <unordered_set>
using namespace std;

// vector< vector<int> > records;
map< int, unordered_set<int> > records;

// void testdata() {
//   map<int, vector<int>> temp_map; // set i -> [elements j]
//   for(int i = 0 ; i < 50; i++) {
//     for(int j = 0 ; j < 50; j++) {
//       if (rand() % 2  == 0)
//         temp_map[i].push_back(j);
//     }
//   }
//   for (auto&it : temp_map) {
//     records.push_back(it.second);
//   }
// }

void testdata_map() {
  map< int, unordered_set<int> > temp_map; // set i -> [elements j]
  for(int i = 0 ; i < 10; i++) {
    for(int j = 0 ; j < 10; j++) {
      if (rand() % 2  == 0)
        temp_map[i].insert(j);
    }
    records[i] = temp_map[i];
  }
}

int access(int u, int v) { 
  // input: two sets (u, v) 
  // output: 1/0

  // tuple <std::int, std::int> t(u, v);
  int du, dv;
  du = records[u].size(); // deg(u)
  dv = records[v].size(); // deg(v)

  if (du * dv == 0) 
    return 0;

  // if (MM[t] != 0) {
  //   return MM[t];
  // }

  int it;
  if (du < dv) {
    for (auto&x : records[u]) {
      if (records[v].find(u) != records[v].end()) {
        cout << "intersects: " << x << endl;
        // return 1;
      }
    }
  } else {
    for (auto&x : records[v]) {
      if (records[u].find(v) != records[u].end()) {
        cout << "intersects: " << x << endl;
        // return 1;
      }
    }
  }
  return 0;
}


int main() {
  unordered_map<int, int> umap;

  srand(time(NULL));
  timeval starting;
 
  // build datasets
  testdata_map();
  cout << " number of sets : " << records.size() << endl;

  // gettimeofday(&starting, NULL);
  for (int i = 0; i < records.size(); i++) { 
    cout << i << " contains: ";
    unordered_set<int> :: iterator itr;
    for (itr = records[i].begin(); itr != records[i].end(); itr++)
        cout << (*itr) << " ";
    cout << endl;
  }

  // std::cout << access(1, 3) << std::endl;
  // std::cout << access(2, 4) << std::endl;
  access(1, 3);
  access(2, 4);
  // inserts
  // umap[1] = 1;
 
  // traverse map
  // for (auto&x : umap)
  //   std::cout << x.first << " " << x.second << std::endl;
}

