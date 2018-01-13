#include<iostream>

using namespace std;

int maxmul(int a, int b, int c)
{
  int product=0;
  cout << "a: " << a << " b: " << b << " c: " << c << endl;
  if(a>=b){
    if(b>=c){
      product=a*b;
    }
    else{
      product=a*c;
    }
  }
  else{
    if(a>=c){
      product=b*a;
    }
    else{
      product=b*c; 
    }
  }
  
  return product;
}

int main()
{
    cout << maxmul(1,2,3) << endl;
    cout << maxmul(3,1,2) << endl;
    cout << maxmul(3,2,1) << endl;
} 
