#include "frac.h"
#include "frac_math.h"

std::ostream &operator<<(std::ostream &os, const Frac &entity)
{
  if (entity.denominator == 1)
    os << entity.numerator;
  else if (entity.isWholeNumber)
    os << entity.numerator / entity.denominator;
  else
    os << entity.numerator << "/" << entity.denominator;
  return os;
}

std::istream &operator>>(std::istream& is, Frac &entity){
  // temporary variables
  int a, b = 1;
  char c;
  // skip whitespaces
  is >> std::ws;
  // check if line is a comment 
  while(is.peek() == ';') 
  {
    // if so, then skip this line
    std::string tmp;
    getline(is,tmp,'\n');
    c = is.peek(); // check if input is integer or fraction
    if(is.eof()) return is;
  }

  is >> a; // get numerator
  is >> std::ws;
  c = is.peek(); // check if input is integer or fraction
  if(c=='/'){
    is >> c >> std::ws >> b >> std::ws; // get denominator if fraction
    c = is.peek();
  }
  if(c == ',') is >> c; // skip comma
  entity = Frac(a,b);
  return is;
}

Frac operator+(const Frac &left, const Frac &right)
{
  int common_denominator = frac_math::lcm(left.denominator, right.denominator);
  int left_mulitplier = common_denominator / left.denominator;
  int right_multiplier = common_denominator / right.denominator;
  int numerator = left_mulitplier * left.numerator + right_multiplier * right.numerator;
  Frac result(numerator, common_denominator);
  result.reduce();
  result.simplifySigns();
  return result;
}

Frac operator-(const Frac &left, const Frac &right)
{
  int common_denominator = frac_math::lcm(left.denominator, right.denominator);
  int left_mulitplier = common_denominator / left.denominator;
  int right_multiplier = common_denominator / right.denominator;
  int numerator = left_mulitplier * left.numerator - right_multiplier * right.numerator;
  Frac result(numerator, common_denominator);
  result.reduce();
  result.simplifySigns();
  return result;
}

Frac& operator-(Frac& entity){
  entity.numerator *= -1;
  return entity;
}

Frac operator*(const Frac &left, const Frac &right)
{
  Frac result(left.numerator * right.numerator, left.denominator * right.denominator);
  result.reduce();
  result.simplifySigns();
  return result;
}

Frac operator/(const Frac &left, const Frac &right)
{
  Frac result(left.numerator * right.denominator, left.denominator * right.numerator);
  result.reduce();
  result.simplifySigns();
  return result;
}

Frac& operator+=(Frac &left, const Frac &right)
{
  left = left + right;
  left.simplifySigns();
  return left;
}

Frac& operator-=(Frac &left, const Frac &right)
{
  left = left - right;
  left.simplifySigns();
  return left;
}

Frac& operator*=(Frac &left, const Frac &right)
{
  left = left * right;
  left.simplifySigns();
  return left;
}

Frac& operator/=(Frac &left, const Frac &right)
{
  left = left / right;
  left.simplifySigns();
  return left;
}

bool operator<(const Frac& left, const Frac& right){
  int a, b, comm_denominator;
  comm_denominator = frac_math::lcm(left.denominator, right.denominator);
  a = comm_denominator/left.denominator;
  b = comm_denominator/right.denominator;
  return (left.numerator * a) < (right.numerator * b);
}

bool operator>(const Frac& left, const Frac& right){return right < left;}

bool operator<=(const Frac& left, const Frac& right){return !(right > left);}

bool operator>=(const Frac& left, const Frac& right){return !(right < left);}

bool operator==(const Frac& left, const Frac& right){
  return (left.numerator == right.numerator) && (left.denominator == right.denominator);
}

bool operator!=(const Frac& left, const Frac& right){
  return !(left == right);
}

void Frac::reduce()
{
  // assume the greatest divisor of numerator and denominator to be equal 1 
  int gcd = 1;
  do
  {
    // find greatest divisor of numerator and denominator
    gcd = frac_math::gcd(numerator, denominator);
    // divide numerator and denominator by greatest divisor
    numerator /= gcd;
    denominator /= gcd;
  } while (gcd != 1);
  // check if we got an integer as result of reduction
  if (denominator == 1)
    isWholeNumber = true;
  else
    isWholeNumber = false;
}

Frac Frac::inverse(const Frac& entity){
  return Frac(entity.denominator,entity.numerator);
}
Frac::Frac(int a, int b) : numerator(a), denominator(b)
{
  if (denominator == 0)
  {
    std::cerr << "Denominator cannot be equal to zero!!!";
    denominator = 1;
  }
  else
  {
    reduce();
    simplifySigns();
  }
}

// if denominator is a negative number, mulitply both numerator and denominator by -1
void inline Frac::simplifySigns(){
  if(this->denominator == -1){
    this->numerator *= -1;
    this->denominator *= -1;
  }
}

// implicit conversion from integer to fraction
Frac::Frac(int a) : numerator(a), denominator(1) { isWholeNumber = true; };

// copy constructor
Frac::Frac(const Frac &entity)
{
  numerator = entity.numerator;
  denominator = entity.denominator;
  isWholeNumber = entity.isWholeNumber;
}
