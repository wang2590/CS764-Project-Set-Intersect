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

std::unordered_map<int, unordered_set<int>> records; // database
std::unordered_map<int, std::unordered_map<int, int>> M; // HM

const int NN = 20; 
int S = 0; // storage
int N = 0; // database size

void testdata_map() {
  map<int, unordered_set<int>> temp_map; // set i -> [elements j]
  for(int i = 0 ; i < NN; i++) {
    for(int j = 0 ; j < NN; j++) {
      if (rand() % 4  == 0 && j != i) {
        temp_map[i].insert(j);
        N += 1;
      }
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
void HM() {
  int threshold = floor(N / sqrt(S));  // degree threshold
  std::vector<int> heavyhitter;

  for (int i = 0; i < records.size(); i++) { 
    if (records[i].size() > threshold) {
      heavyhitter.push_back(i); // heavy-hitters
    }
  }

  for (int u : heavyhitter) {
    for (int v : heavyhitter) {
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

  if (M.find(u) != M.end() && M[u].find(v) != M[u].end()) {
    return M[u][v];
  }

  return join(u, v);
}


int main() {
  // build datasets
  testdata_map();
  cout << "number of sets : " << records.size() << endl;
  cout << "database size = " << N << endl;
  
  S = floor(pow(N, 1.5));
  cout << "space usage = " << S << endl;
  cout << "threshold = " << floor(N / sqrt(S)) << endl;

  // print datasets
  for (int i = 0; i < records.size(); i++) { 
    cout << "set " << i << " contains: ";
    unordered_set<int> :: iterator itr;
    for (itr = records[i].begin(); itr != records[i].end(); itr++)
        cout << (*itr) << " ";
    cout << endl;
  }

  // preprocessing phase
  HM();

  for (auto & [i, value]: M) {
    for(auto & [j, v]: value) {
      cout << i << ", " << j << ": " << M[i][j] << endl;
    }
  }

  // online phase
  std::cout << "access request results: " << access(0, 0) << std::endl;
  std::cout << "access request results: " << access(17, 18) << std::endl;
}

