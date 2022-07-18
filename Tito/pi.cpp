#include <iostream>
#include <ostream>
using namespace std;

float pi(unsigned int nMaxCicli) {

  float pigreco = 0;
  float menouno = 1;

  for (int i = 1; i < 2 * nMaxCicli; i = i + 2) {
    pigreco = (menouno)*4 / i + pigreco;

    menouno = (-menouno);
  }
  return pigreco;
}

int main() {
  int nMaxCicli;
  cin >> nMaxCicli;
  float res = pi(nMaxCicli);
  cout << res << endl;
  return 0;
}