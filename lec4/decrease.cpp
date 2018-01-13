#include <iostream>

using namespace std;

int main()
{
  int i = 10;
  int neg1 = -1;
  cout << i << endl;
  //Statement 1
  i--;
  cout << i << endl;
  //Statement 2
  i = i - 1;
  cout << i << endl;
  //Statement 3
  i = i + neg1;
  cout << i << endl;
  //Statement 4
  i-= 1;
  cout << i << endl;
}
