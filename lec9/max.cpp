#include <iostream>

using namespace std;

void maxentry(int a, int b, int *m, int *r)
{
  if(a>b){
    *m=a;
    *r=1;
  }
  else{
    *m=b;
    *r=2;
  }
}

int main(){
  int max, rank, n1, n2;
  cout << "First number: ";
  cin >> n1;
  cout << "Second number";
  cin >> n2;
  maxentry(n1,n2,&max,&rank);
  cout << max << endl;
  cout << rank << endl;

}
