#include <sys/time.h>
#include <time.h>

#include <algorithm>
#include <bitset>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
//
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

std::unordered_map<int, unordered_set<int>> records;      // database
std::unordered_map<int, std::unordered_map<int, int>> M;  // HM

const int NN = 20;
int S = 0;  // storage
int N = 0;  // database size

void testdata_map() {
  map<int, unordered_set<int>> temp_map;  // set i -> [elements j]
  for (int i = 0; i < NN; i++) {
    for (int j = 0; j < NN; j++) {
      if (rand() % 4 == 0 && j != i) {
        temp_map[i].insert(j);
        N += 1;
      }
    }
    records[i] = temp_map[i];
  }
}

int join(int u, int v) {
  int du, dv;
  du = records[u].size();  // deg(u)
  dv = records[v].size();  // deg(v)
  if (du * dv == 0) return 0;

  if (du < dv) {
    for (auto& x : records[u]) {  // O(deg(u))
      if (records[v].find(x) != records[v].end()) {
        return 1;
      }
    }
  } else {
    for (auto& x : records[v]) {  // O(deg(v))
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
      heavyhitter.push_back(i);  // heavy-hitters
    }
  }

  for (int u : heavyhitter) {
    for (int v : heavyhitter) {
      int result = join(u, v);
      M[u][v] = result;
      M[v][u] = result;
    }
  }
  return;
}

// online phase
int access(int u, int v) {
  // input: two sets (u, v)
  // output: 1/0

  int du, dv;
  du = records[u].size();  // deg(u)
  dv = records[v].size();  // deg(v)

  if (du * dv == 0) return 0;

  if (M.find(u) != M.end() && M[u].find(v) != M[u].end()) {
    return M[u][v];
  }

  return join(u, v);
}

// prep-datset
int dataset() {
  // Create an unordered_map to store the parsed data
  std::unordered_map<int, std::unordered_set<int>> data;

  // Set the directory path
  std::filesystem::path directory_path("/users/shutao/dataset");

  // Iterate over all files in the directory
  for (const auto& entry :
       std::filesystem::directory_iterator(directory_path)) {
    // Check if the entry is a regular file with the ".txt" extension
    if (entry.is_regular_file() && entry.path().extension() == ".egonet") {
      // Open the file for reading
      std::ifstream input_file(entry.path());
      if (!input_file.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
      }

      // Read each line of the input file
      std::string line;
      while (std::getline(input_file, line)) {
        // Split the line into two parts: the key and the value
        std::istringstream iss(line);
        int key;
        iss >> key;
        std::string delimiter;
        iss >> delimiter;

        // Parse the value string into a set of integers
        std::unordered_set<int> value;
        int val;
        while (iss >> val) {
          value.insert(val);
        }

        // Add the key-value pair to the unordered_map
        data[key] = value;
      }

      // // Print the contents of the unordered_map
      // for (const auto& [key, value] : data) {
      //   std::cout << key << ":";
      //   for (int val : value) {
      //     std::cout << " " << val;
      //   }
      //   std::cout << std::endl;
      // }

      // assign to global 'records' database
      records = data;
      N = data.size();
    }
  }
  return 0;
}

int main() {
  // build datasets
  dataset();
  // testdata_map();
  cout << "number of sets : " << records.size() << endl;
  cout << "database size = " << N << endl;

  S = floor(pow(N, 1.3));
  cout << "space usage = " << S << endl;
  cout << "threshold = " << floor(N / sqrt(S)) << endl;

  // print datasets
  // for (int i = 0; i < records.size(); i++) {
  //   cout << "set " << i << " contains: ";
  //   unordered_set<int> :: iterator itr;
  //   for (itr = records[i].begin(); itr != records[i].end(); itr++)
  //       cout << (*itr) << " ";
  //   cout << endl;
  // }

  // preprocessing phase
  HM();

  // print M
  // for (auto& [i, value] : M) {
  //   for (auto& [j, v] : value) {
  //     cout << i << ", " << j << ": " << M[i][j] << endl;
  //   }
  // }

  // // online phase
  // std::cout << "access request results: " << access(0, 0) << std::endl;
  // std::cout << "access request results: " << access(17, 18) << std::endl;

  // // quick test
  std::vector<int> vec = {10, 100, 500, 1000};
  for (int element : vec) {
    // Create a uniform random number generator
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist(0, records.size() - 1);

    // Record the start time
    auto start = std::chrono::steady_clock::now();

    // Access # of random elements of the map
    for (int i = 0; i < element; ++i) {
      // Generate a random index within the range of the map
      int index1 = dist(rng);
      auto it1 = std::next(records.begin(), index1);
      int index2 = dist(rng);
      auto it2 = std::next(records.begin(), index2);
      access(it1->first, it2->first);
    }
    // Record the end time
    auto end = std::chrono::steady_clock::now();
    // Compute the elapsed time
    auto elapsed = end - start;
    // Print the elapsed time to the standard output
    std::cout << "Number of Access: " << element
              << " Elapsed time: " << elapsed.count() << " ns" << std::endl;
    std::cout << "Number of Access: " << element << " Elapsed time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
                     .count()
              << "ms" << std::endl;
  }

  return 0;
}
