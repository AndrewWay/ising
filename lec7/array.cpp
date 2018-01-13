#include <iostream>

using namespace std;


double print_array(double d[], int n){

  for(int i = 0; i < n; i++){
    cout << d[i] << " ";
  }
  cout << endl;

}

double average_array(double d[], int n)
{
  double average = 0;
  for(int i = 0; i < n; i++){
    average+=d[i];
  }
  average=average/n;
  return average;
}

double subtract_average(double d[], int n){
  double average = average_array(d,n);
  for(int i = 0; i < n; i++){
    d[i]-=average;
  }
}

int main(){
  int size = 5;
  double data[5] = {1.0,2.0,-3.0,12.124,0.1};
  
  cout << "BEFORE SHIFTING" << endl;
  print_array(data,size);

  double average = average_array(data,size);
  cout << "AVERAGE: " << average << endl;


  subtract_average(data,size);
  cout << "AFTER SHIFTING" << endl;
  print_array(data,size);
}
