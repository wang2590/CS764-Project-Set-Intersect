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
std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> M; // LM
std::vector<int> heavies;

const int NN = 25; 
int S = 0; // storage
int N = 0; // database size

void testdata_map() {
  map<int, unordered_set<int>> temp_map; // set i -> [elements j]
  for(int i = 0 ; i < NN; i++) {
    for(int j = 0 ; j < NN; j++) {
      if (rand() % 2  == 0 && j != i) {
        temp_map[i].insert(j);
        N += 1;
      }
    }
    records[i] = temp_map[i];
  }
}


std::vector<int> join(int u, int v) {
  std::vector<int> intersection;
  std::set_intersection(records[u].begin(), records[u].end(),
                        records[v].begin(), records[v].end(),
                        std::back_inserter(intersection));

  return intersection;
}

void LM() {
  // populate the inverted records
  std::unordered_map<int, std::unordered_set<int>> inverted_records; // k, sets that contain k

  for (int i = 0; i < records.size(); i++) { 
    unordered_set<int> :: iterator itr;
    for (itr = records[i].begin(); itr != records[i].end(); itr++)
        inverted_records[*itr].insert(i); // (*itr) is in set i
  }

  int threshold = ceil(S / N);  // degree threshold
  for (const auto& [k, set] : inverted_records) {
    if (set.size() < threshold) {
      for (const auto& x : set) {
        for (const auto& y : set) {
          // store [(x, y), {k}] into M)
          // cout << x << ", " << y << ": " << k << endl;
          M[x][y].insert(k);
        }
      }
    } else {
      // store heavy ones
      heavies.push_back(k);
    }
  }
}


// online phase
std::vector<int> access(int u, int v) { 
  // input: two sets (u, v) 
  // output: 1/0

  int du, dv;
  du = records[u].size(); // deg(u)
  dv = records[v].size(); // deg(v)

  vector<int> results;
  vector<int> resultsL;
  vector<int> resultsH;
  if (du * dv == 0) 
    return results;

  unordered_set<int> :: iterator itr;
  for (itr = M[u][v].begin(); itr != M[u][v].end(); itr++) {
    resultsL.push_back(*itr);
  }

  for (int i : heavies) {
    if (records[u].find(i) != records[u].end() && records[v].find(i) != records[v].end()) {
      resultsH.push_back(i);
    }
  }
  
  std::set_union(resultsL.begin(), resultsL.end(),
               resultsH.begin(), resultsH.end(),
               std::back_inserter(results));

  return results;
}


int main() {
  // build datasets
  testdata_map();
  cout << "number of sets : " << records.size() << endl;
  cout << "database size = " << N << endl;
  S = floor(pow(N, 1.45));
  cout << "space usage = " << S << endl;
  cout << "threshold = " << ceil(S / N) << endl;

  // print datasets
  for (int i = 0; i < records.size(); i++) { 
    cout << "the set " << i << " contains: ";
    unordered_set<int> :: iterator itr;
    for (itr = records[i].begin(); itr != records[i].end(); itr++)
        cout << (*itr) << " ";
    cout << endl;
  }

  // preprocessing phase
  LM();

  for (auto& i : M) {
    for(auto& j : M[i.first]) {
      unordered_set<int> :: iterator itr;
      cout << i.first << ", " << j.first << ": ";
      for (itr = M[i.first][j.first].begin(); itr != M[i.first][j.first].end(); itr++) {
        cout << (*itr) << " ";
      }
      cout << endl;
    }
  }

  // online phase
  for(int j : access(0, 1)) {
    cout << "j = " << j << endl;
  }

  // inserts
  // umap[1] = 1;
 
  // traverse map
  // for (auto&x : umap)
  //   std::cout << x.first << " " << x.second << std::endl;
}

