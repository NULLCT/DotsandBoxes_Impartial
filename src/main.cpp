#include "dotsandboxes.hpp"

signed main() {
  Wrapper dotsandboxes;

  cout << "縦,横を入力" << endl;
  cin >> dotsandboxes.N >> dotsandboxes.M;

  dotsandboxes.Support();
}
