#include <iostream>

using namespace std;

//Increment float x by 1
float inc(float &x)
{
  x++;
}

int main()
{
  float x=10.0;
  inc(x);
  cout << x << endl;
}
