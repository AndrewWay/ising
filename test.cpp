#include <iostream>

using namespace std;


int mod(int a, int b)
{ 
return (a%b+b)%b; 
}

void change(int &v){
v = 2;
}

int main(){
int vn4 = -4;
int vn1 = -1 ;
int v1 = 1;
int v3 = 3;
int v4 = 6;

int N=4;
cout << mod(vn4,N) << endl;
cout << mod(vn1,N) << endl;
cout << mod(v1,N) << endl;
cout << mod(v3,N) << endl;
cout << mod(v4,N) << endl;


}
