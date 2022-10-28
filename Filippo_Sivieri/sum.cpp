#include <iostream>
using namespace std;

int main(){
    int num, sum = 0, penultimo = 0;
    int back = 0;
    do{
        do{
            cin >> num;  // 1
        }while(num <= 0 || num >= 20);

        if(sum % num != 0 || sum == 0) {
            back = sum;
            sum += num;
            penultimo = num;
        }

    }while(back % num == 0 || sum == 0);

    cout << sum << endl << num << endl << penultimo - num;
    return 0;
}