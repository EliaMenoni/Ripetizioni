#include <iostream>
using namespace std;

// n! = 1 * 2 * ... * n
// n! = n * (n-1)! 
int fact(int num){
    if(num == 1) return 1;
    
    return num * fact(num-1);
}

/*

fact(10)
return 10 * fact(9)

fact(9)
return 9 * fact(8)

...

fact(1)
return 1

*/

int main(void) {
  int res = fact(3);
  cout << res << endl;
  return 0;
}