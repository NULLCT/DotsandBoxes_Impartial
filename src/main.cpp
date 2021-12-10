#include "dotsandboxes.hpp"

int main() {
  cin.tie(nullptr);
  ios_base::sync_with_stdio(false);
  cout << "縦,横を入力" << endl;
  cin >> N >> M;
  Support();

  /*
  cout << "今引かれている線の本数を入力" << endl;
  int x;
  cin >> x;
  llint Game = 0;
  if (x != 0) {
          cout << "線が引かれている箇所を入力" << endl;
          rep(i, x) {
                  int y;
                  cin >> y;
                  Game += (llint)1 << y;
          }
  }
  cout << Count(Game) << endl;
  */
  return 0;
}
