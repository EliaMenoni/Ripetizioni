#include <iostream>
using namespace std;

// fib_i = fib_i-1 + fib_i-2
// 0 1 1 2 3 5 8 13 ...
int fibo(int num){
    if(num == 1) return 1;
    if(num < 1) return 0;
    
    return fibo(num - 1) + fibo(num - 2);
}

int main(void) {
  int res = fibo(5);
  cout << res << endl;
  return 0;
}