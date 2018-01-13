#include <iostream>

using namespace std;


const int rows=100;
const int columns=100;

double average_array(int array[rows][columns])
{
  double average = 0;
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < columns; j++){
      average+=array[i][j];
    }
  }
  int n = rows*columns;
  average=average/n;
  return average;
}

void populate_array(int array[rows][columns]){
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      array[i][j]=i+j;
    }
  }
}

int main(){
  int bigarray[rows][columns];
  populate_array(bigarray);
  double average = average_array(bigarray);
  cout << "The average of the array is: " << average << endl;
}
