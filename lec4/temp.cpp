#include <iostream>

using namespace std;

int main()
{
double tK;

cout << " Enter the temperature in Kelvin: "<<endl;
cin >> tK;

double tF = 9*(tK - 273.15)/5 + 32;

cout << tF << endl;

}
