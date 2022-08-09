#include <iostream>
using namespace std;

int conta(char str[]) {
  int conteggio = 0;
  while (str[conteggio]) {
    conteggio++;
  }
  return conteggio;
}

void trasferimento(char str[], char str2[], int indice1, int indice2) {
  int j = 0;
  for (int i = indice1; i <= indice2; i++) {
    str2[j] = str[i];
    j++;
  }
  str2[j] = '\0';
}

bool palindroma(char str[]) {
  if (str[0] == '*' && conta(str) == 1) {
    return true;
  }
  if (str[0] == str[conta(str) - 1]) {
    char str2[conta(str) - 1];
    trasferimento(str, str2, 1, conta(str) - 2);
    return palindroma(str2);
  }
  return false;
}

int main(void) {
  char str[] = "abc*cba";
  cout << palindroma(str) << endl;
  char str2[] = "abc*bba";
  cout << palindroma(str2) << endl;
  char str3[] = "abcd*cba";
  cout << palindroma(str3) << endl;

  return 0;
}