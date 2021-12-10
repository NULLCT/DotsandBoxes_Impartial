#pragma once

#include "stldatadumper.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

//ごめんなさい
#define int long long

using namespace std;

typedef int llint;
typedef vector<int> vint;
typedef vector<vint> vvint;
typedef vector<bool> vbool;
typedef pair<int, int> pint;

#define rep(i, n) for (int i = 0; i < n; i++)
#define drep(i, n) for (int i = (n)-1; 0 <= i; i--)
#define all(a) a.begin(), a.end()

//複数インスタンスに対応
class Wrapper {
public:
  //縦x横
  int N, M;

  void draw(int, string);
  void refer(int, int, int[4]);
  int chain(int &, vbool &, bool &);
  bool Square_Search(int);
  int Count(int);
  void Search();
  void Search2();
  void Support();

private:

};
