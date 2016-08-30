#include "SmallWindows.h"

namespace SmallWindows {
  Rational::Rational(int numerator /* = 0 */,
                     int denominator /* = 1 */)
   :numerator(numerator),
    denominator(denominator) {
    if (denominator == 0) {
      throw NotaRationalNumber();
    }

    Normalize();
  }

  Rational::Rational(const String& text)
   :numerator(0),
    denominator(1) {
    String trimText(Trim(text));

    { IStringStream totalStream(trimText);
      TCHAR slash;
      totalStream >> numerator >> setiosflags(ios::skipws)
                  >> slash >> denominator;
      if (totalStream.eof() && (denominator != 0) &&
          (slash == TEXT('/'))) {
        Normalize();
        return;
      }
    }

    { IStringStream numeratorStream(trimText);
      numeratorStream >> numerator;
      if (numeratorStream.eof()) {
        return;
      }
    }

    throw NotaRationalNumber();
  }

  Rational::Rational(const Rational &rational)
   :numerator(rational.numerator),
    denominator(rational.denominator) {
    // Empty.
  }

  Rational Rational::operator=(const Rational &rational) {
    numerator = rational.numerator;
    denominator = rational.denominator;
    return *this;
  }

  Rational::operator String() const {
    OStringStream outStream;

    if (denominator == 1) {
      outStream << numerator;
    }
    else {
      outStream << numerator << TEXT("/") << denominator;
    }

    return outStream.str();
  }

  bool Rational::operator==(const Rational &rational) const {
    return (numerator == rational.numerator) &&
           (denominator == rational.denominator);
  }

  bool Rational::operator!=(const Rational &rational) const {
    return !(*this == rational);
  }

  bool Rational::operator<(const Rational &rational) const {
    return ((numerator * rational.denominator) <
            (rational.numerator * denominator));
  }

  bool Rational::operator<=(const Rational &rational) const {
    return ((*this < rational) || (*this == rational));
  }

  bool Rational::operator>(const Rational &rational) const {
    return !(*this <= rational);
  }

  bool Rational::operator>=(const Rational &rational) const {
    return !(*this < rational);
  }

  Rational Rational::operator+(const Rational &rational) const {
    Rational result((numerator * rational.denominator) +
                    (rational.numerator * denominator),
                    denominator * rational.denominator);
    result.Normalize();
    return result;
  }

  Rational Rational::operator-(const Rational &rational) const {
    Rational result((numerator * rational.denominator) -
                    (rational.numerator * denominator),
                    denominator * rational.denominator);
    result.Normalize();
    return result;
  }

  Rational Rational::operator*(const Rational &rational) const {
    Rational result(numerator * rational.numerator,
                    denominator * rational.denominator);
    result.Normalize();
    return result;
  }

  Rational Rational::operator/(const Rational &rational) const {
    assert(rational.numerator != 0);
    Rational result(numerator * rational.denominator,
                    denominator * rational.numerator);
    result.Normalize();
    return result;
  }

  void Rational::Normalize() {
    if (numerator == 0) {
      denominator = 1;
      return;
    }

    if (denominator < 0) {
      numerator = -numerator;
      denominator = -denominator;
    }

    int gcd = GCD(abs(numerator), denominator);
    numerator /= gcd;
    denominator /= gcd;
  }

  int Rational::GCD(int number1, int number2) {
    if (number1 > number2) {
      return GCD(number1 - number2, number2);
    }
    else if (number1 < number2) {
      return GCD(number1, number2 - number1);
    }
    else {
      return number1;
    }
  }
};