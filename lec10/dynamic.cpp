#include <iostream>

using namespace std;

int main(){
double **array;

int n,m;
cout << "Please enter n" << endl;
cin >> n;
cout << "Please enter m" << endl;
cin >> m;

array = new double*[n];

for(int i = 0; i < n; i++){
  array[i] = new double[m];
}

for(int i=0;i<n;i++){
  for(int j=0;j<m;j++){
    array[i][j]=0;
  }
}

for(int i=0;i<n;i++){
  for(int j=0;j<m;j++){
    cout << array[i][j] << " ";
  }
  cout << endl;
}

for(int i = 0;i<n;i++){
  delete[] array[i];
}
delete[] array;
}
