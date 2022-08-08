#include <iostream>
using namespace std;

long int fibonacci (int n){
    
    long int fibo = 0;
    if (n <= 1 && n >= 0){
        return 1;
    }
    fibo = fibonacci (n-1) + fibonacci (n-2);
    return fibo;
} 

int main(void) {
    fibonacci (5);
    long int result = fibonacci (5);
    cout << result;
  return 0;
}