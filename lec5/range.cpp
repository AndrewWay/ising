#include <iostream>

using namespace std;

int main()
{
  float num=0;

  cout << "Please enter a number between 0 and 1" << endl;

  cin >> num;

  if(num >= 0){
    if(num <= 1){
      cout << "Acceptable" << endl;
    }
    else{
      cout << "Too high" << endl;
    }
  }
  else{
    cout << "Too low" << endl;
  }

}
