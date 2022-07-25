#include <iostream>
using namespace std;

int main() {
  int x;
  cin >> x;
  for (int i = 0; i < 10; i++) {
    cout << x * (i + 1) << endl;
  }
  return 0;
}