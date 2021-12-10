#pragma once

//       _   _     _       _            _                                   //
//   ___| |_| | __| | __ _| |_ __ _  __| |_   _ _ __ ___  _ __   ___ _ __   //
//  / __| __| |/ _` |/ _` | __/ _` |/ _` | | | | '_ ` _ \| '_ \ / _ \ '__|  //
//  \__ \ |_| | (_| | (_| | || (_| | (_| | |_| | | | | | | |_) |  __/ |     //
//  |___/\__|_|\__,_|\__,_|\__\__,_|\__,_|\__,_|_| |_| |_| .__/ \___|_|     //
//                                                       |_|                //

#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifdef DEBUG
#define D(var) cout << "(\e[34m" << #var << "\e[0m \e[2m@" << __LINE__ << "\e[0m): \e[36m\e[1m" << var << "\e[0m" << endl;
#else
#define D(var) ;
#endif

using namespace std;

template <class T>
ostream &operator<<(ostream &_ostr, const vector<T> &_v);
template <class T>
ostream &operator<<(ostream &_ostr, const deque<T> &_v);
template <class T>
ostream &operator<<(ostream &_ostr, const list<T> &_v);
template <class T, class Y>
ostream &operator<<(ostream &_ostr, const pair<T, Y> &_v);
template <class... Ts>
ostream &operator<<(ostream &_ostr, const tuple<Ts...> &t);
template <class T, class Y>
ostream &operator<<(ostream &_ostr, const map<T, Y> &_v);
template <class T>
ostream &operator<<(ostream &_ostr, const set<T> &_v);
template <class T, class Y>
ostream &operator<<(ostream &_ostr, const unordered_map<T, Y> &_v);
template <class T>
ostream &operator<<(ostream &_ostr, const unordered_set<T> &_v);

template <class T>
void dumpFromRangeList(ostream &_ostr, const T &_v) {
  _ostr << "{";
  for (auto itr = _v.begin(); itr != _v.end(); itr++) {
    _ostr << *itr;
    if (next(itr) != _v.end())
      _ostr << ",";
  }
  _ostr << "}";
}
template <class T>
ostream &operator<<(ostream &_ostr, const vector<T> &_v) {
  _ostr << "v";
  dumpFromRangeList(_ostr, _v);
  return _ostr;
}
template <class T>
ostream &operator<<(ostream &_ostr, const deque<T> &_v) {
  _ostr << "d";
  dumpFromRangeList(_ostr, _v);
  return _ostr;
}
template <class T>
ostream &operator<<(ostream &_ostr, const list<T> &_v) {
  _ostr << "l";
  dumpFromRangeList(_ostr, _v);
  return _ostr;
}
template <class T, class Y>
ostream &operator<<(ostream &_ostr, const pair<T, Y> &_v) {
  _ostr << "p{" << _v.first << "," << _v.second << "}";
  return _ostr;
}
template <class... Ts>
ostream &operator<<(ostream &_ostr, const tuple<Ts...> &_v) {
  _ostr << "t{";
  bool first = true;
  apply([&_ostr, &first](auto &&...args) {
    auto print = [&](auto &&val) {
      if (!first)
        _ostr << ",";
      (_ostr << val);
      first = false;
    };
    (print(args), ...);
  },
        _v);
  _ostr << "}";
  return _ostr;
}
template <class T, class Y>
ostream &operator<<(ostream &_ostr, const map<T, Y> &_v) {
  _ostr << "m";
  dumpFromRangeList(_ostr, _v);
  return _ostr;
}
template <class T, class Y>
ostream &operator<<(ostream &_ostr, const unordered_map<T, Y> &_v) {
  _ostr << "um";
  dumpFromRangeList(_ostr, _v);
  return _ostr;
}
template <class T>
ostream &operator<<(ostream &_ostr, const set<T> &_v) {
  _ostr << "s";
  dumpFromRangeList(_ostr, _v);
  return _ostr;
}
template <class T>
ostream &operator<<(ostream &_ostr, const unordered_set<T> &_v) {
  _ostr << "us";
  dumpFromRangeList(_ostr, _v);
  return _ostr;
}
