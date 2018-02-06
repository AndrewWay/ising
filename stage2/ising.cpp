#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <random>

using namespace std;

#define J 1             // Magnetic Dipole Spin

// CMSC 6920 replace the random seed with your student number
#define SEED 201316338 // Seed for random number generator

/* Global parameters */

int N, NSPINS, PMCS, EQMCS;
double T, dT, TMAX;
double NORM;

random_device rd;
mt19937 engine;
uniform_real_distribution<double> realDist;
uniform_int_distribution<int> intDist;

// create lookup table of exp(-DeltaE / T)
double exp_table[9];

const string UP   = "\u21BF";   // unicode up arrow (half barb)
const string DOWN = "\u21C2";   // unicode down arrow (half barb)

/// Read the parameters from the input file
///
/// Read N, T, dT, PMCS, EQMCS, and TMAX from input file
///
///  @param file name of the input file as a string
///  @result exit code, 0 on success

int read_input(string filename) 
{
  ifstream ifile(filename.c_str());
  
  if (!ifile) 
    {
      cerr << "Could not open input file" << endl;
      return(1);
    }
    
  // Parse input
  
  while (ifile.good()) 
    {
      string s;
      ifile >> s;
      if (s.compare("lsize") == 0) 
	{
	  ifile >> N;                         // Lattice size
	}
      else if (s.compare("eqsteps") == 0) 
	{
	  ifile >> EQMCS;                       // Monte Carlo steps
	} 
      
      else if (s.compare("steps") == 0) 
	{
	  ifile >> PMCS;                       // Monte Carlo steps
	} 
      else if (s.compare("temp") == 0) 
	{
	  ifile >> T;                         // Minimum temperature
	  ifile >> TMAX;                      // Maximum temperature
	  ifile >> dT;                        // Temperature increment
	} 
      else if (s.compare(" ")) 
	{
	  continue; // skip emtpy lines
	} 
      else 
	{
	  cerr << "Unrecognized parameter in input file" << endl;
	  return(1);
	}
    }
  
  ifile.close();
  
  /* Check that all parameters were initialized */
  if (!N || !T || !dT || !TMAX || !PMCS || !EQMCS) 
    {
      cerr << "Not all parameters specified in input file" << endl;
      return(1);
    }
  
  /* Calculate additional parameters */
  NSPINS = N*N;                   // Number of spins
  
  return(0);
}

/// Print out the lattice using arrows to indicate spin
///
/// The Ising lattice is printed as a series of up and down arrows
///
///  @param lat is a 2D array of integers representing the lattice of spins
///  @result output is printed to the console, void return

void printSpins(int **lat) 
{
  for (int x = 0; x < N; x++) 
    {
      for (int y = 0; y < N; y++) 
	{
	  string s;
	  switch (lat[x][y]) 
	    {
	    case 1:
	      s = UP;
	      break;
	    case -1:
	      s = DOWN;
	      break;
	    default:
	      s = "ERR";
            }
	  cout << s << " ";
        }
      cout << endl;
    }
}


/// Return the bond energy at point (x,y)
///
/// The bond energy between site i and neighbor j is defined as
///  -J * s_i * s_j
///  For all neighbors:
///  H_i = -J * s_i * sum(s_j)
///
///  @param lat is a 2D array of integers representing the lattice of spins
///  @param x is the x coordinate of the point in the lattice
///  @param y is the y coordinate of the point in the lattice
///  @result the bond energy of point (x,y) in the lattice
 
int bondEnergy(int **lat, int x, int y) 
{
  /* Bond energy between site i and neighbor j is
   *          -J * s_i * s_j
   *
   * For all neighbors:
   *          H_i = -J * s_i * sum(s_j)
   *
   * Neighbor Key
   *
   *      o a o
   *      d i b
   *      o c o
   */
  int a = y + 1;
  int c = y - 1;
  int d = x - 1;
  int b = x + 1;
  
  /* Adjust for periodic boundaries */
  if (y == N-1) { a = 0;   }
  if (y == 0)   { c = N-1; }
  if (x == N-1) { b = 0;   }
  if (x == 0)   { d = N-1; }
  
  // s_i 
  int atom = lat[x][y];
  
  // sum(s_j) 
  int neighbors = lat[d][y] + lat[b][y] + lat[x][a] + lat[x][c];
  
  // -J * s_i * sum(s_j) 
  return -J * atom * neighbors;
}

/// Calculate and return the total energy of the lattice
///
/// The total bond energy for the lattice is sum(H_i)
/// The bond energy between site i and neighbor j is defined as
///  -J * s_i * s_j
///  For all neighbors:
///  H_i = -J * s_i * sum(s_j)
///
///  @param lat is a 2D array of integers representing the lattice of spins
///  @param x is the first point to calculate the bond energy between
///  @result the total energy of the lattice

int energy(int **lat) 
{
  /* Total bond energy for lattice is
   *          sum(H_i)
   */
  int total = 0;
  
  for (int x = 0; x < N; x++)
    {
      for (int y = 0; y < N; y++)
	{
	  total += bondEnergy(lat, x, y);
	}
    }
  
  return(total/2); // Divide by 2 to remove duplicate spins
}

/// Calculate and return the total magnetization of a lattice
///
/// The total magnetization of a lattice is the sum of all lattice points
///
/// @param lat is a 2D array of integers representing the lattice of spins
/// @result is the net magnetization of the lattice

int magnetization(int **lat) 
{
  int tot = 0;
  
  for (int x = 0; x < N; x++) {
    for (int y = 0; y < N; y++) {
      tot += lat[x][y];
    }
  }
  return tot;
}

/// Calculate whether or not to flip the spin of a lattice position
///
/// Return true if the spin at (x,y) of the lattice lat should be
/// flipped based on the Metropolis criterion at temperature temp
/// The energy change of the spin flip is
/// dE = 2 * -H_i = J * s_i * sum(s_j)
/// you can use the BondEnergy function to calculate the change in spin
/// will be flipped
/// Note: total change is twice the bond energy
///
/// @param lat is a 2D array of integers representing the lattice of spins
/// @param x is the x position in the lattice of the spin to be flipped
/// @param y is the y position in the lattice of the spin to be flipped
/// @param dE is the change in energy corresponding to the spin flip
///        it is passed by reference and should be updated for the main
///        function
/// @param temp is the temperature
/// @result true if the spin at (x,y) should be flipped and otherwise false

bool testFlip(int **lat, int x, int y, int &dE, double temp) 
{
  // CDMSC6D920: calculate the change in energy corresponding to the given
  // spin flip and apply the Metropolis criterion to it to determine if
  // the spin should be flipped
  dE = -2 * bondEnergy(lat, x, y);
  
  if (dE <= 0)
    {
      return true;
    }
  else
    {
      double xi = realDist(engine);
      if (xi < exp_table[dE])
	{
	  return true;
	}
    }
  return(false);
}

void initExpLookup(double T)
{
  for(int i=0;i<9;++i)
    {
      exp_table[i]=exp(-i/T);
    }
}

//Allocate a rowsXcolumns array
void allocate_array(int ** &array,int rows, int columns){
  array = new int*[rows];
  
  //Create new rows
  for(int i = 0; i < rows; i++){
    array[i] = new int[columns];
  }
}

//Deallocate a rowsXcolumns array
void deallocate(int **array,int rows){
  cout << "Deallocating memory..." << endl;
  //Delete each row
  for(int i = 0;i<rows;i++){
    delete[] array[i];
  }
  delete[] array;//Delete the main array
  cout << "Memory deallocated" << endl;
}

///Given a rowsXcolumns array, randomly assign to each element +/-J
void randomize_array(int ** &array,int rows, int columns){
  mt19937 engine;
  uniform_real_distribution<>dist(0,1);
  double rando=0;
  
  if(SEED == 0){
    engine.seed(time(nullptr));
  }
  else{
    engine.seed(SEED);
  }
  
  //Loop through all spin sites
  for(int i=0;i<rows;i++){
    for(int j=0;j<columns;j++){
      rando = dist(engine);
      if(rando >= 0.5){
        array[i][j] = J;
      }
      else{
        array[i][j] = -J;
      }
    }
  }
}

// Run a simulation

int main(int argc, char *argv[]) 
{
  // Initialize global parameter variables from input file Exit with
  // error code 1 if input file has errors
  
  if (!argv[1]) 
    {
      cerr << "Specify an input file" << endl;
      return(1);
    }
  
  if (read_input(argv[1])) 
    {
      return(1);
    }
  
  if (!argv[2]) 
    {
      cerr << "Specify an output file" << endl;
      return 1;
    }
  
  ofstream ofile(argv[2]);
  
  if (!ofile) 
    {
      cerr << "Could not open output file! " << argv[2] << endl;
      return(1);
    }
  
  ofile << "#N " << N << endl;
  ofile << "#EQMCS " << EQMCS << endl;
  ofile << "#PMCS " << PMCS << endl;
  ofile << "#T " << T << endl;
  ofile << "#dT " << dT << endl;
  ofile << "#TMAX " << TMAX << endl;
  
  // Keep record of seed 
  ofile << "#SEED " << SEED << endl;
  ofile << "#" << endl;
  
  engine=mt19937(SEED);
  
  uniform_int_distribution<> intLattice(0, N-1);
  uniform_real_distribution<> realDist(0, 1);
  uniform_int_distribution<> intDistLattice(0, 1);
  
  NSPINS = N*N;                   // Number of spins
  NORM = 1.0 / (PMCS * NSPINS);    // Normalization constant
  
  int **lattice;
  
  //  CMSC 6920 allocate NxN 2D array in the variable lattice to store lattice
  allocate_array(lattice,N,N);
  
  //  CMSC 6920 Initialize NxN lattice and assign each point a random spin of +/- J
  randomize_array(lattice,N,N);
  /* Observables 
   * M - magnetization
   * E - energy */
  
  printSpins(lattice);
  
  int M=0;
  double E=0;        // for a given state
  double M_avg, E_avg;    // ensemble averages
  double M_tot, E_tot;    // sum for all states at a given temperature 
  int x,y,dE;
  bool verdict;
  // Column headers for data file 
  char s[68];
  sprintf(s, "%-7s%12s%12s%12s\n", "#TEMP", "<E>", "<E>^2", "|<M>|");
  ofile << s;
  
  // loop over temperatures from T to TMAX
  // and compute E_ave and M_ave at each temperature
  // by performing a Monte Carlo simulation at each 
  // temperature
  
  for(double temp=T;temp<TMAX;temp+=dT) 
    {
      /* Energy and magnetization of initial state */
      E = energy(lattice);
      M = magnetization(lattice);
      initExpLookup(temp);
      
      // perform EQMCS steps of Monte Carlo Sampling to equilibriate the lattice at
      // the new temperature. The energies and magnetizations from these steps should
      // not be used to compute the totals/averages
      // each cycle is comprised of NSPINS spin-flip attempts
      
      // perform PMCS steps of Monte Carlo Sampling 
      
      // CMSC6920: Complete this loop to perform PMCS Metropolis Monte
      // Carlo iteration. These are the production/sampling cycles. The 
      // Each Monte Carlo iteration is comprised of  NSPINS spip-flip attempts
      for(int i=0;i<PMCS;i++){
        for(int n=0;n<NSPINS;n++){
        
          x = intLattice(engine);
          y = intLattice(engine);
        
          verdict = testFlip(lattice, x, y, dE, temp);
          
          if(verdict == true){
            lattice[x][y] *= -1;
          }
        }
      }
      
      // Reset sums before start of production simulation at temperature T 
      E_tot = 0;
      M_tot = 0;
      
      // You should not recalculate E and M for each Monte Carlo cycle
      // These values can be calculated more efficiently by determining
      // the difference compared to the previous configuration and updating
      // E and M with that difference
      // each Monte Carlo step should involve N^2 attempts to flip the
      // spin of a random lattice point
      for(int i=0;i<EQMCS;i++){
        for(int n=0;n<NSPINS;n++){
        
          x = intLattice(engine);
          y = intLattice(engine);
        
          verdict = testFlip(lattice, x, y, dE, temp);
          
          if(verdict == true){
            lattice[x][y] *= -1;
            E_tot+=dE;
            M_tot += lattice[x][y];
          }
        }
      }
      
      // after each Monte Carlo step calculate
      // the sum of the energy and magnetization of new lattice
      // configuration
      
      // sums of E_tot and M_tot (averages will be computed later)
      // at the end of the Monte Carlo cycles
           
      // Ensemble averages (per spin) for current temperature 
      E_avg = E_tot / (PMCS * NSPINS);
      M_avg = M_tot / (PMCS * NSPINS);
      sprintf(s, "%-7.2f%12.3f%12.3f%12.3f\n", temp,
	      E_avg, E_avg*E_avg, fabs(M_avg) );
      ofile << s;
      printSpins(lattice);
    } // END Temperature Loop
  
  // CMSC 6920: free memory used for lattice
  deallocate(lattice,N);
  return(0);
}

