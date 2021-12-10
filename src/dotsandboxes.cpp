#include "dotsandboxes.hpp"

// Search2の解析結果をもとに、プレイヤーのサポートをする
//盤面が渡されると、それが先手必勝か後手必勝かを返し、
//もし先手必勝盤面なら最善手を返す
//バグあり直せ6/10→直した6/17
void Wrapper::Support() {
  vector<int> Grundy(1 << (N * (M + 1) + (N + 1) * M));
  {
    fstream fs("Grundy/" + to_string(N) + "x" + to_string(M) + ".txt");
    if (!fs) {
      cout << "ファイルが開けませんでした" << endl;
      assert(false);
    }

    drep(Game, 1 << (N * (M + 1) + (N + 1) * M)) {
      string buf;
      getline(fs, buf);
      if (fs.eof())
        break;
      if (buf[0] == '-')
        Grundy[Game] = -1;
      else {
        Grundy[Game] = 0;
        rep(i, buf.size()) {
          Grundy[Game] *= 10;
          Grundy[Game] += (int)(buf[i] - '0');
        }
      }
    }
  }

  //番号をシャッフルして戦略にランダム性を持たせる(12/5)
  srand(time(NULL));
  vint Number(N * (M + 1) + (N + 1) * M);
  rep(i, N * (M + 1) + (N + 1) * M) Number[i] = i;
  rep(i, 20000) {
    int x = rand() % (N * (M + 1) + (N + 1) * M),
        y = rand() % (N * (M + 1) + (N + 1) * M);
    swap(Number[x], Number[y]);
  }

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
  while (1) {
    if (Grundy[Game] == -1) {
      cout << "探索対象外の盤面です" << endl;
      break;
    } else if (Grundy[Game]) {
      cout << "先手必勝盤面です" << endl;
      if (Grundy[Game] == 2) {
        cout << "正方形を取りまくってください、勝てます" << endl;
        break;
      } else {
        for (int i : Number) {
          llint k = (llint)1 << i;
          if (!(Game & k) && !Grundy[Game | k]) {
            cout << i << "に線を引いてください" << endl;
            Game |= k;
            break;
          }
        }
        while (1) {
          cout << "相手が線を引いた箇所を入力(-1で終了)" << endl;
          int y;
          cin >> y;
          if (y == -1)
            break;
          Game |= (llint)1 << y;
        }
      }
    } else {
      cout << "後手必勝盤面です\n諦めてください" << endl;
      break;
    }
  }
  return;
}

// N*Mの盤面を解析し、盤面(draw)、Grundy数の順番でtest.txtに書き出す
//-1:探索対象外、0:後手必勝、100:その手番で先手必勝、その他:先手必勝
void Wrapper::Search() {

  string fileName = "Grundy_";
  fileName += ('0' + N);
  fileName += 'x';
  fileName += ('0' + M);
  fileName += ".txt";
  ofstream ofs(fileName, ofstream::app);
  if (!ofs) {
    cout << "ファイルが開けませんでした。" << endl;
    return;
  }

  //その盤面でスタートした時に
  // 0なら後手必勝、1なら先手必勝、100ならその手番で勝ち
  vint Grundy((llint)1 << (N * (M + 1) + (N + 1) * M), -1);
  drep(Game, 1 << (N * (M + 1) + (N + 1) * M)) {
    if (Square_Search(Game)) {
      Grundy[Game] = -1;
    } else if (Count(Game) > N * M / 2) {
      Grundy[Game] = 100;
    } else {
      set<int> s;
      rep(i, N * (M + 1) + (N + 1) * M) {
        if (!(Game & (1 << i))) {
          s.insert(Grundy[Game | (1 << i)]);
        }
      }
      int mex = 0;
      if (s.size() == 0) {
        Grundy[Game] = 1;
      } else {
        while (s.find(mex) != s.end()) {
          mex++;
        }
        Grundy[Game] = mex;
      }
    }
    if (Grundy[Game] != -1) {
      draw(Game, fileName);
      ofs << Grundy[Game] << endl << endl;
    }
  }
  cout << Grundy[0] << endl;
  return;
}

// N*Mの盤面を解析し、ナンバリング逆順でGrundy数をtest.txtに書き出す
//-1:探索対象外、0:後手必勝、1:先手必勝、2:その手番で先手必勝
void Wrapper::Search2() {

  string fileName = "Grundy\\";
  fileName += ('0' + N);
  fileName += 'x';
  fileName += ('0' + M);
  fileName += ".txt";
  ofstream ofs(fileName, ofstream::app);
  if (!ofs) {
    cout << "ファイルが開けませんでした。" << endl;
    return;
  }

  //その盤面でスタートした時に
  // 0なら後手必勝、1なら先手必勝、2ならその手番で勝ち
  vint Grundy((llint)1 << (N * (M + 1) + (N + 1) * M), -1);
  drep(Game, 1 << (N * (M + 1) + (N + 1) * M)) {
    if (Square_Search(Game)) {
      Grundy[Game] = -1;
    } else if (Count(Game) > N * M / 2) {
      Grundy[Game] = 2;
    } else {
      //ここで、正方形取ったときの連取を考慮して移動する必要あり(12/4)
      //移動先に0があるかどうか(あれば勝ち、Grundyは1になる)
      bool isOK = false;
      set<int> visited;
      queue<int> que;
      que.push(Game);
      while (!isOK && !que.empty()) {
        int G = que.front();
        que.pop();
        if (visited.find(G) != visited.end())
          continue;
        visited.insert(G);
        rep(i, N) rep(j, M) {
          int udlr[4];
          refer(i, j, udlr);
          int cnt = 0, d = -1;
          rep(k, 4) {
            if (G & (1 << udlr[k]))
              cnt++;
            else
              d = 1 << udlr[k];
          }
          if (cnt == 4)
            continue;
          else if (cnt == 3) {
            if (visited.find(G | d) == visited.end())
              que.push(G | d);
          } else {
            rep(k, 4) {
              int t = 1 << udlr[k];
              if (!(G & t)) {
                //探索対象外に出たら勝ちと認識させる→対象外に出られる盤面には移動したくなくなる(12/4)
                if (Grundy[G | t] <= 0) {
                  isOK = true;
                  i = N;
                  j = M;
                  break;
                }
              }
            }
          }
        }
      }
      if (isOK)
        Grundy[Game] = 1;
      else
        Grundy[Game] = 0;
    }
    ofs << Grundy[Game] << endl;
  }
  cout << Grundy[0] << endl;
  return;
}

// Gameを描画する
void Wrapper::draw(int Game, string fileName) {
  ofstream ofs(fileName, ofstream::app);
  rep(i, 2 * N + 1) {
    rep(j, 2 * M + 1) {
      if (i % 2 == 0) {
        if (j % 2 == 0)
          ofs << " ";
        else {
          if (Game & (1 << (i / 2 * M + j / 2)))
            ofs << "-";
          else
            ofs << " ";
        }
      } else {
        if (j % 2 == 0) {
          if (Game & (1 << (M * (N + 1) + j / 2 * N + i / 2)))
            ofs << "|";
          else
            ofs << " ";
        } else
          ofs << " ";
      }
    }
    ofs << endl;
  }
  return;
}

//(x,y)の正方形の4辺の番号を求める
// O(1)
void Wrapper::refer(int x, int y, int num[4]) {
  num[0] = x * M + y;
  num[1] = (x + 1) * M + y;
  num[2] = M * (N + 1) + y * N + x;
  num[3] = M * (N + 1) + (y + 1) * N + x;
}

//(x,y)の正方形のどこかから塗るときの最大点数
//→場所指定なしで、BFS使ってそこから1手で何個取れるかを調べる(10/14)
// O(NM)
int Wrapper::chain(int &Game, vbool &visited, bool &CanDC) {
  struct tmp {
    int x, y, d, nx, ny;
  };
  //取れる正方形をBFS
  //取った辺は塗る
  queue<tmp> que;

  //まず最初に取れる正方形をqueにぶち込む
  rep(i, N) {
    rep(j, M) {
      int udlr[4];
      refer(i, j, udlr);
      int cnt = 0;
      int d = -1;
      rep(k, 4) {
        if (Game & (1 << udlr[k]))
          cnt++;
        else {
          d = k;
        }
      }
      if (cnt == 3) {
        int nx = i, ny = j;
        if (d == 0)
          nx = i - 1;
        if (d == 1)
          nx = i + 1;
        if (d == 2)
          ny = j - 1;
        if (d == 3)
          ny = j + 1;
        que.push({i, j, udlr[d], nx, ny});
      }
    }
  }

  //次にBFSをぐるぐる回す
  int ans = 0;
  while (!que.empty()) {
    auto [i, j, t, nx, ny] = que.front();
    que.pop();
    if (i < 0 || i == N || j < 0 || j == M)
      continue;
    Game |= 1 << t;
    if (visited[i * M + j])
      continue;
    visited[i * M + j] = true;
    ans++;
    int udlr[4];
    refer(nx, ny, udlr);
    int cnt = 0, d = -1;
    rep(k, 4) {
      if (Game & (1 << udlr[k]))
        cnt++;
      else {
        d = k;
      }
    }
    if (cnt == 3) {
      CanDC = true;
      int nnx = nx, nny = ny;
      if (d == 0)
        nnx = nx - 1;
      if (d == 1)
        nnx = nx + 1;
      if (d == 2)
        nny = ny - 1;
      if (d == 3)
        nny = ny + 1;
      que.push({nx, ny, udlr[d], nnx, nny});
    } else if (cnt == 4) {
      ans++;
    }
  }
  return ans;
}

//ダブルクロス戦略も考慮したい(7/15)
//相手も取ることになってしまうのは困る
// Chainをうまいこと利用する形でダブルクロス戦略を実現するという案(10/14)
int Wrapper::Count(int Game) {
  vbool visited(N * M, false);
  int G = Game;
  bool CanDC = false;
  int ans = chain(G, visited, CanDC);
  //飽和しているかどうかを調べる
  //飽和していなければansをそのまま出力して終わり
  {
    bool tmp = false;
    rep(i, N) rep(j, M) {
      int cnt = 0;
      if (Game & (1 << (i * M + j)))
        cnt++;
      if (Game & (1 << ((i + 1) * M + j)))
        cnt++;
      if (Game & (1 << (M * (N + 1) + j * N + i)))
        cnt++;
      if (Game & (1 << (M * (N + 1) + (j + 1) * N + i)))
        cnt++;
      if (cnt < 2) {
        tmp = true;
        break;
      }
    }
    if (tmp)
      return ans;
  }
  //ここから2辺が塗られている各マスについて、
  //仮にもう1本増えたときにどれくらいとれるかを調べる
  // DC:ダブルクロス出来るチェーン、
  // other：ダブルクロスできないチェーン
  //大きさが2以下は必ずother、3以上は必ずDCになる
  vint DC, other;
  int Sum = 0;
  rep(i, N) {
    rep(j, M) {
      if (visited[i * M + j])
        continue;
      int udlr[4];
      refer(i, j, udlr);
      int cnt = 0;
      int d = -1, d2 = -1;
      rep(k, 4) {
        if (Game & (1 << udlr[k]))
          cnt++;
        else {
          if (d == -1)
            d = k;
          else
            d2 = k;
        }
      }
      if (cnt == 2) {
        int tG = G | (1 << udlr[d]);
        bool candc = false;
        int tmp = chain(tG, visited, candc);
        if (tmp > 2)
          DC.push_back(tmp);
        else
          other.push_back(tmp);
        Sum += tmp;
      }
    }
  }

  //数列AとBの2つが渡される(ここでは、DCがA、otherがB)
  //プレイヤーは交互にその要素を取っていき、総和が大きい方が勝ち
  //ただし、Aを取る場合は「自分にA_i-2を足して、相手に2足す」ことで、
  //もう一度連続で自分のターンにすることができる。
  //後手は最大で何点取れるか？
  //ただし、Bの要素は2以下、Aの要素は3以上
  //という問題を解けばよい

  //先手の最善手は、
  //まずAの要素で4以上のものをすべて取り、
  //そこからA、Bの残りすべてを交互にとる
  //先手の最大点数を計算する
  int fpoint = 0;
  int now = 0;
  rep(i, DC.size()) {
    if (DC[i] >= 4)
      fpoint += DC[i] - 2;
    else {
      if (now == 0) {
        fpoint += 3;
      }
      now++;
      now %= 2;
    }
  }
  sort(all(other));
  reverse(all(other));
  rep(i, other.size()) {
    if (now == 0)
      fpoint += other[i];
    now++;
    now %= 2;
  }

  if (CanDC) {
    return ans + max(fpoint, Sum - fpoint);
  } else {
    return ans + (Sum - fpoint);
  }
}

//盤面に既に正方形だらけかどうかを返す
//半数以上が既に取られた正方形ならばtrue、そうでなければfalse(7/15)
// O(NM)
bool Wrapper::Square_Search(int Game) {
  int cnt = 0;
  rep(i, N) {
    rep(j, M) {
      int udlr[4];
      refer(i, j, udlr);
      if ((Game & (1 << udlr[0])) && (Game & (1 << udlr[1])) &&
          (Game & (1 << udlr[2])) && (Game & (1 << udlr[3]))) {
        cnt++;
      }
    }
  }
  return cnt >= (N * M + 1) / 2;
}
