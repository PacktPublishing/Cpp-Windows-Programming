#include "SmallWindows.h"

namespace SmallWindows {
  double Square(double value) {
    return value * value;
  }

  Complex::Complex(double x, double y)
   :x(x), y(y) {
    // Empty.
  }

  Complex::Complex(const Complex &complex)
   :x(complex.x),
    y(complex.y) {
    // Empty.
  }

  Complex Complex::operator=(const Complex &complex) {
    x = complex.x;
    y = complex.y;
    return *this;
  }

  void ReadWhiteSpaces(IStringStream& inStream) {
    while (true) {
      TCHAR tChar = inStream.peek();

      if ((tChar >= 0) && (tChar <= 255) && isspace(tChar)) {
        inStream.get();
      }
      else {
        break;
      }
    }
  }

  TCHAR Peek(IStringStream& inStream) {
    ReadWhiteSpaces(inStream);

    if (inStream.eof()) {
      return TEXT('\0');
    }
    else {
      return (TCHAR) inStream.peek();
    }
  }

  bool ReadI(IStringStream& inStream) {
    if (tolower(Peek(inStream)) == TEXT('i')) {
      inStream.get();
      return true;
    }

    return false;
  }

  bool ReadSign(IStringStream& inStream, TCHAR& sign) {
    TCHAR tChar = Peek(inStream);
    
    switch (tChar) {
      case TEXT('+'):
        inStream.get();
        sign = TEXT('+');
        return true;

      case TEXT('-'):
        inStream.get();
        sign = TEXT('-');
        return true;

      default:
        return false;
    }
  }

  bool ReadValue(IStringStream& inStream, double& value) {
    TCHAR tChar = Peek(inStream);

    if ((tChar == TEXT('+')) || (tChar == TEXT('-'))) {      
      inStream.get();
      tChar = Peek(inStream);
      inStream.unget();

      if (isdigit(tChar) || (tChar == TEXT('.'))) {        
        inStream >> value;
        return true;
      }
    }
    else if (isdigit(tChar) || (tChar == TEXT('.'))) {
      inStream >> value;
      return true;
    }

    return false;
  }

  bool EndOfLine(IStringStream& inStream) {
    return Peek(inStream) == TEXT('\0');
  }

  bool Complex::ReadStream(const String& text) {
    IStringStream inStream(Trim(text));
    double value1, value2;
    TCHAR sign1, sign2;

    if (ReadValue(inStream, value1)) {
      if (ReadSign(inStream, sign1)) {
        if (ReadValue(inStream, value2) && ReadI(inStream) &&
            EndOfLine(inStream)) { // 1. x + yi
          x = value1;
          y = (sign1 == TEXT('-')) ? -value2 : value2;
          return true;
        }
        else if (ReadSign(inStream, sign2)) {
          if (ReadI(inStream) && EndOfLine(inStream)) { // 2. x + -i
               x = value1;
            y = (sign1 == TEXT('-')) ? -1 : 1;
            y = (sign2 == TEXT('-')) ? -y : y;
            return true;
          }
        }
        else if (ReadI(inStream) && EndOfLine(inStream)) { // 3. x + i
          x = value1;
          y = (sign1 == TEXT('-')) ? -1 : 1;
          return true;
        }
      }
      else if (ReadI(inStream)) {
        if (ReadSign(inStream, sign1)) {
          if (ReadValue(inStream, value2) && EndOfLine(inStream)){ //4.yi + x
            y = value1;
            x = (sign1 == TEXT('-')) ? -value2 : value2;
            return true;
          }
        }
        else if(EndOfLine(inStream)) { // 7. yi
          y = value1;
          x = 0;
          return true;
        }
      }
      else if (EndOfLine(inStream)) { // 10. x
        x = value1;
        y = 0;
        return true;
      }
    }
    else if (ReadSign(inStream, sign1)) {
      if (ReadI(inStream)) {
        if (ReadSign(inStream, sign2)) {
          if (ReadValue(inStream, value2) && EndOfLine(inStream)){ //5.-i + x
            y = (sign1 == TEXT('-')) ? -1 : 1;
            x = (sign2 == TEXT('-')) ? -value2 : value2;
            return true;
          }
        }
        else if (EndOfLine(inStream)) { // 8. -i
          y = (sign1 == TEXT('-')) ? -1 : 1;
          x = 0;
          return true;
        }
      }
    }
    else if (ReadI(inStream)) {
      if (ReadSign(inStream, sign2)) {
        if (ReadValue(inStream, value2) && EndOfLine(inStream)) { // 6. i + x
          y = 1;
          x = (sign2 == TEXT('-')) ? -value2 : value2;
          return true;
        }
      }
      else if (EndOfLine(inStream)) { // 9. i
        y = 1;
        x = 0;
        return true;
      }
    }

    return false;
  }

  Complex::Complex(const String& text) {
    if (!ReadStream(text)) {
      throw NotaComplexNumber();
    }
  }

  Complex::operator String() const {
    OStringStream outStream;

    if (x != 0) {
      if (y == 1) {
        outStream << x << TEXT("+i");
      }
      else if (y == -1) {
        outStream << x << TEXT("-i");
      }
      else if (y != 0) {
        outStream << x << setiosflags(ios::showpos)
                  << y << TEXT("i");
      }
      else {
        outStream << x;
      }
    }
    else {
      if (y == 1) {
        outStream << TEXT("i");
      }
      else if (y == -1) {
        outStream << TEXT("-i");
      }
      else if (y != 0) {
        outStream << y << TEXT("i");
      }
      else {
        outStream << TEXT("0");
      }
    }

    return outStream.str();
  }

  bool Complex::operator==(const Complex &complex) const {
    return ((x == complex.x) && (y == complex.y));
  }

  bool Complex::operator!=(const Complex &complex) const {
    return !(*this == complex);
  }

  bool Complex::operator<(const Complex &complex) const {
    return (Abs() < complex.Abs());
  }

  bool Complex::operator<=(const Complex &complex) const {
    return ((*this < complex) || (*this == complex));
  }

  bool Complex::operator>(const Complex &complex) const {
    return !(*this <= complex);
  }

  bool Complex::operator>=(const Complex &complex) const {
    return !(*this < complex);
  }

  Complex Complex::operator+=(double x) {
    *this = (*this + Complex(x));
    return *this;
  }

  Complex Complex::operator+=(Complex &complex) {
    *this = (*this + complex);
    return *this;
  }

  Complex operator+(double x, const Complex &complex) {
    return (Complex(x) + complex);
  }

  Complex operator+(const Complex &complex, double x) {
    return (complex + Complex(x));
  }

  Complex operator+(const Complex &complex1,
                    const Complex &complex2) {
    return Complex(complex1.x + complex2.x,
                   complex1.y + complex2.y);
  }

  Complex Complex::operator-=(double x) {
    return (*this - Complex(x));
  }

  Complex Complex::operator-=(Complex &complex) {
    return (*this - complex);
  }

  Complex operator-(double x, const Complex &complex) {
    return (Complex(x) - complex);
  }

  Complex operator-(const Complex &complex, double x) {
    return (complex - Complex(x));
  }

  Complex operator-(const Complex &complex1,
                    const Complex &complex2) {
    return Complex(complex1.x - complex2.x,
                   complex1.y - complex2.y);
  }

  Complex Complex::operator*=(double x) {
    *this = (*this * Complex(x));
    return *this;
  }

  Complex Complex::operator*=(Complex &complex) {
    *this = (*this * complex);
    return *this;
  }

  Complex operator*(double x, const Complex &complex) {
    return (Complex(x) * complex);
  }

  Complex operator*(const Complex &complex, double x) {
    return (complex * Complex(x));
  }

  Complex operator*(const Complex &complex1,
                    const Complex &complex2) {
    return Complex((complex1.x * complex2.x) -
                   (complex1.y * complex2.y),
                   (complex1.x * complex2.y) +
                   (complex2.x * complex1.y));
  }

  Complex Complex::operator/=(double x) {
    *this = (*this / Complex(x));
    return *this;
  }

  Complex Complex::operator/=(Complex &complex) {
    *this = (*this / complex);
    return *this;
  }

  Complex operator/(double x, const Complex &complex) {
    return (Complex(x) / complex);
  }

  Complex operator/(const Complex &complex, double x) {
    return (complex / Complex(x));
  }

  Complex operator/(const Complex &complex1,
                    const Complex &complex2) {
    double sum = Square(complex2.x) + Square(complex2.y);
    double x = ((complex1.x * complex2.x) +
                (complex1.y * complex2.y)) / sum,   
           y = ((complex2.x * complex1.y) +
                (complex1.x * complex2.y)) / sum;
    return Complex(x, y);
  }
};