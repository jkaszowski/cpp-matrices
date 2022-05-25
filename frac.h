#ifndef _FRAC_H_
#define _FRAC_H_

#include <iostream>
#include <numeric>
#include <string>

class Frac
{
  // overload cout and cin operators
  friend std::ostream &operator<<(std::ostream &os, const Frac &entity);
  friend std::istream &operator>>(std::istream &is, Frac &entity);

  //both arguments are const because they should not be modified, as reference not to create copy
  friend Frac operator+(const Frac &left, const Frac &right);
  friend Frac operator-(const Frac &left, const Frac &right);
  friend Frac operator*(const Frac &left, const Frac &right);
  friend Frac operator/(const Frac &left, const Frac &right);

  // negation
  friend Frac& operator-(Frac &entity);

  //both arguments as reference, but left is not const since it should be modified
  friend Frac& operator+=(Frac &left, const Frac &right);
  friend Frac& operator-=(Frac &left, const Frac &right);
  friend Frac& operator*=(Frac &left, const Frac &right);
  friend Frac& operator/=(Frac &left, const Frac &right);

  // both arguments const and references since they are read only original values
  friend bool operator<(const Frac& left, const Frac& right);
  friend bool operator>(const Frac& left, const Frac& right);
  friend bool operator<=(const Frac& left, const Frac& right);
  friend bool operator>=(const Frac& left, const Frac& right);
  friend bool operator==(const Frac& left, const Frac& right);
  friend bool operator!=(const Frac& left, const Frac& right);

public:
  Frac() = default;
  Frac(int a, int b); //basic constructor
  Frac(int a); // implicit conversion from integer to fraction
  Frac(const Frac &entity); // copy constructor
  
  // reduce fraction to its simplest form
  void reduce();
  // provide inverse
  Frac inverse(const Frac& entity);

  // if denominator is a negative number, mulitply both numerator and denominator by -1
  void inline simplifySigns();

private:
  int numerator;
  int denominator;
  bool isWholeNumber;
};

#endif