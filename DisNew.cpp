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
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

unordered_map<uint64_t, unordered_set<uint64_t>> records; // database
// std::unordered_map<int, std::unordered_map<int, int>> M;    // HM

const uint64_t NN = 50000;
uint64_t S = 0; // storage
uint64_t N = 0; // database size

// bool M[NN][NN];
unordered_map<uint64_t, unordered_map<uint64_t, bool>> M; // HM
unordered_map<uint64_t, bool> MM;

void testdata_map()
{
  map<uint64_t, unordered_set<uint64_t>> temp_map; // set i -> [elements j]
  for (uint64_t i = 0; i < NN; i++)
  {
    for (int j = 0; j < (rand() % NN); j++)
    {
      temp_map[i].insert(j);
      N += 1;
    }
    records[i] = temp_map[i];
  }
}

bool join(int u, int v)
{
  int du, dv;
  du = records[u].size(); // deg(u)
  dv = records[v].size(); // deg(v)
  // if (du * dv == 0)
  //   return false;

  if (du < dv)
  {
    for (auto &x : records[u])
    { // O(deg(u))
      if (records[v].find(x) != records[v].end())
      {
        return true;
      }
    }
  }
  else
  {
    for (auto &x : records[v])
    { // O(deg(v))
      if (records[u].find(x) != records[u].end())
      {
        return true;
      }
    }
  }
  return false;
}

// int join(int u, int v)
// {
//   std::vector<int> intersection;
//   std::set_intersection(records[u].begin(), records[u].end(),
//                         records[v].begin(), records[v].end(),
//                         std::back_inserter(intersection));

//   return intersection.size() > 0 ? 1 : 0;
// }

// preprocessing phase

void HM()
{
  uint64_t threshold = floor(N / sqrt(S)); // degree threshold
  std::vector<uint64_t> heavyhitter;

  for (int i = 0; i < records.size(); i++)
  {
    if (records[i].size() > threshold)
    {
      heavyhitter.push_back(i); // heavy-hitters
    }
  }

  for (uint64_t u : heavyhitter)
  {
    for (uint64_t v : heavyhitter)
    {
      if (u <= v)
      {
        bool result = join(u, v);
        M[u][v] = result;
        M[v][u] = result;

        if (u > v)
        {
          std::exchange(u, v);
        }
        int64_t key = u << 17 + v;
        MM[key] = result;
      }
    }
  }
  return;
}

// online phase
int access(int u, int v)
{
  // input: two sets (u, v)
  // output: 1/0

  int du, dv;
  du = records[u].size(); // deg(u)
  dv = records[v].size(); // deg(v)

  int threshold = floor(N / sqrt(S)); // degree threshold

  // M.find(u) != M.end() && M[u].find(v) != M[u].end()
  if (du > threshold && dv > threshold)
  {
    // cout << "look-up " << endl;
    if (u > v)
    {
      std::exchange(u, v);
    }
    int64_t key = u << 17 + v;
    return MM[key]; // --> on-going work: fix terrible hash table O(1) // results ---> egonets
  }
  else
  {
    return join(u, v);
  }
}

int main(int argc, char *argv[])
{
  // build datasets
  testdata_map();
  cout << "number of sets : " << records.size() << endl;
  cout << "database size = " << N << endl;

  double exponent = 0;
  if (argc >= 2)
  {
    exponent = atof(argv[1]);
  }

  S = floor(pow(N, exponent));
  cout << "exponent = " << exponent << endl;
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
  std::cout << "FINISH MATERIALIZATION" << std::endl;

  // for (auto & [i, value]: M) {
  //   for(auto & [j, v]: value) {
  //     cout << i << ", " << j << ": " << M[i][j] << endl;
  //   }
  // }

  // online phase
  std::cout << "START TEST" << std::endl;
  std::mt19937 rng;
  std::uniform_int_distribution<int> dist(0, records.size() - 1);

  //  # of accesse requests of the map
  //  build access workloads
  int rounds = 100000;
  vector<pair<int, int>> vect;
  for (int i = 0; i < rounds; ++i)
  {
    // generate a random index within the range of the map
    int index1 = dist(rng);
    auto it1 = std::next(records.begin(), index1);
    int index2 = dist(rng);
    auto it2 = std::next(records.begin(), index2);
    vect.push_back(make_pair(it1->first, it2->first));
  }

  std::cout << "START ACCESS" << std::endl;
  auto start = std::chrono::steady_clock::now();
  for (auto &v : vect)
  {
    access(v.first, v.second);
  }
  auto end = std::chrono::steady_clock::now();
  auto elapsed = end - start;

  // print the elapsed time to the standard output
  // std::cout << "# of Access: " << rounds
  //           << " -- time: " << elapsed.count() << " ns" << std::endl;
  std::cout << "# of Access: " << rounds << " -- time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)
                   .count()
            << "ms" << std::endl;
}
