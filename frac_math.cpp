// This file include implementation of custom 
// math functions that are needed for Frac class to work

#include "frac_math.h"

using namespace frac_math;

int frac_math::gcd(int a, int b){
    if(a < 0) a *= -1;
    if(b < 0) b *= -1;
   
    if (a == 0) // If a becomes zero  
       return b; // b is the GCD   
    if (b == 0)// If b becomes zero  
       return a;// a is the GCD   
    
     
    if (a == b) // The case of equal numbers   
        return a; // return any one of them   
    
   // Apply case of substraction   
    if (a > b) // if a is greater subtract b   
        return gcd(a-b, b);  
    return gcd(a, b-a); //otherwise subtract a   
}

// just derive Least Common Multiplier from Greatest Common Divisor, that's the easiest way
int frac_math::lcm(int a, int b){
    return (a/gcd(a,b)) * b;
}