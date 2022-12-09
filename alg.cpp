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
#include <cmath>
using namespace std;

std::unordered_map<int, unordered_set<int>> records;
std::unordered_map<int, std::unordered_map<int, int>> M; // materialization
const int S = 1; // storage
const int N = 20; // database size

void testdata_map() {
  map<int, unordered_set<int>> temp_map; // set i -> [elements j]
  for(int i = 0 ; i < N; i++) {
    for(int j = 0 ; j < N; j++) {
      if (rand() % 4  == 0)
        temp_map[i].insert(j);
    }
    records[i] = temp_map[i];
  }
}

int join(int u, int v) {
  int du, dv;
  du = records[u].size(); // deg(u)
  dv = records[v].size(); // deg(v)
  if (du * dv == 0) 
    return 0;
  
  if (du < dv) {
    for (auto&x : records[u]) {  // O(deg(u))
      if (records[v].find(x) != records[v].end()) {
        return 1;
      }
    }
  } else {
    for (auto&x : records[v]) {  // O(deg(v))
      if (records[u].find(x) != records[u].end()) {
        return 1;
      }
    }
  }
  return 0;
}

// preprocessing phase
void materialization() {
  int threshold = floor(records.size() / sqrt(S));  // degree threshold
  std::vector<int> heavyhitter;

  for (int i = 0; i < records.size(); i++) { 
    if (records[i].size() > threshold) {
      heavyhitter.push_back(i); // heavy-hitters
    }
  }

  int H = heavyhitter.size();
  // k-Set Disjointness, k = 2, 4, 8
  for (int i = 0; i < H; ++i) {
        for (int j = 0; j < H; ++j) { // k = 2
          int u = heavyhitter[i];
          int v = heavyhitter[j];

          int result = join(u, v);
          M[u][v] =  result;
          M[v][u] =  result;
        } 
  }
  return;
}

// online phase
int access(int u, int v) { 
  // input: two sets (u, v) 
  // output: 1/0

  int du, dv;
  du = records[u].size(); // deg(u)
  dv = records[v].size(); // deg(v)

  if (du * dv == 0) 
    return 0;

  if (M.find(u) != M.end()) {
    if (M[u].find(v) != M[u].end()) {
      cout << "stored" << endl;
      return M[u][v];
    }
  }
  return join(u, v);
}


int main() {
  // srand(time(NULL));
  // timeval starting;
 
  // build datasets
  testdata_map();
  cout << " number of sets : " << records.size() << endl;

  // gettimeofday(&starting, NULL);
  // print datasets
  for (int i = 0; i < records.size(); i++) { 
    cout << "the set " << i << " contains: ";
    unordered_set<int> :: iterator itr;
    for (itr = records[i].begin(); itr != records[i].end(); itr++)
        cout << (*itr) << " ";
    cout << endl;
  }

  // preprocessing phase
  materialization();

  for (int i = 0; i < M.size(); i++) {
    for(int j = 0; j < M[i].size(); j++) {
      cout << i << ", " << j << ": " << M[i][j] << endl;
    }
  }

  // online phase
  std::cout << "access request results: " << access(0, 1) << std::endl;

  // inserts
  // umap[1] = 1;
 
  // traverse map
  // for (auto&x : umap)
  //   std::cout << x.first << " " << x.second << std::endl;
}

