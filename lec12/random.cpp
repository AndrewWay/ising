#include <iostream>
#include <random> 
#include <ctime>
#include <iomanip>

using namespace std;


const int MAX_DISP_ROWS = 10;
const int MAX_DISP_COLS = 10;
const int SEED = 0;//Set to 0 seed based on system time.
const int DISP_DECIMALS = 2;


void deallocate(double **array,int rows){
  cout << "Deallocating memory..." << endl;
  for(int i = 0;i<rows;i++){
    delete[] array[i];
  }
  delete[] array;
  cout << "Memory deallocated" << endl;
}

void randomize_array(double **array,int rows, int columns){
  mt19937 engine;
  uniform_real_distribution<>dist(0,1);
  
  if(SEED == 0){
    engine.seed(time(nullptr));
  }
  else{
    engine.seed(SEED);
  }
  
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      array[i][j]=dist(engine);
    }
  }
}

void print_array(double **array,int rows, int columns){
  std :: cout << std::fixed;
  std :: cout << std::setprecision(DISP_DECIMALS);
  
  if(rows <= MAX_DISP_ROWS && columns <= MAX_DISP_COLS){
    for(int i=0;i<rows;i++){
      for(int j=0;j<columns;j++){
        std::cout << array[i][j] << " ";
      }
      std::cout << endl;
    }
  }
  else{
    cout << "Array too big. Printing cancelled." << endl;
  }
}

int main()
{
  double **array;

  int n,m;
  cout << "Please enter number of rows" << endl;
  cin >> n;
  cout << "Please enter number of columns" << endl;
  cin >> m;

  array = new double*[n];
  for(int i = 0; i < n; i++){
    array[i] = new double[m];
  }

  
  randomize_array(array,n,m);
  print_array(array,n,m);
  deallocate(array,n);

}
