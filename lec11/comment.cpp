#include<cmath>

///Evaluate the function exp(-x^2)
///
///This function evaluates the function exp(-x^2) at point x
///
///@param x - The value at which exp(-x^2) is evaluated
///@retun The value of exp(-x^2) evaluated at x
float exp_mx2(int x)
{
    return(exp(-x*x));
}

///Numerically integrate exp(-x^2)
///
///This function numerically integrates exp(-x^2) in the
///interval x=[a,b]. 
///
///@param a - the left endpoint of the interval
///@param b - the right endpoint of the interval
///@param n - the number of points to evaluate the function at
///@return The numerically integration value

float trap(float a, float b, int n)
{
    float h = (b-a)/(n-1); 
    float value = 0.5*( f(a)+f(b)); 
    
    for(int k=2; k < n; k++)
    {
        value+=f( a + h*(k-1) );
    } 
    
    value*=h;
    
    return(value); 
}
