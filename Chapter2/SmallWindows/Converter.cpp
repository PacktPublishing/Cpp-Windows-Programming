#include "SmallWindows.h"

namespace SmallWindows {
  bool Converter<int>::Check(String& text, int base) {
    IStringStream stringStream(Trim(text));
    int value;
    stringStream >> setbase(base) >> value;
    return stringStream.eof();
  }

  int Converter<int>::TextToValue(String& text, int base) {
    IStringStream stringStream(Trim(text));
    int value;
    stringStream >> setbase(base) >> value;
    return value;
  }

  String Converter<int>::ValueToText(int& value, int base) {
    OStringStream outputStream;
    outputStream << setbase(base) << value;
    return outputStream.str();
  }

  bool Converter<unsigned int>::Check(String& text, int base) {
    IStringStream stringStream(Trim(text));
    unsigned int value;
    stringStream >> setbase(base) >> value;
    return stringStream.eof() && (text.find(TEXT("-")) == -1);
  }

  unsigned int Converter<unsigned int>::TextToValue(String& text,
                                                     int base){
    IStringStream stringStream(Trim(text));
    unsigned int value;
    stringStream >> setbase(base) >> value;
    return value;
  }

  String Converter<unsigned int>::ValueToText(unsigned int&value,
                                               int base){
    OStringStream outputStream;
    outputStream << setbase(base) << value;
    return outputStream.str();
  }

  bool Converter<double>::Check(String& text, int /* base */) {
    IStringStream stringStream(Trim(text));
    double value;
    stringStream >> value;
    return stringStream.eof();
  }

  double Converter<double>::TextToValue(String& text,
                                         int /* base */) {
    IStringStream stringStream(Trim(text));
    double value;
    stringStream >> value;
    return value;
  }

  String Converter<double>::ValueToText(double& value,
                                         int /* base */) {
    OStringStream outputStream;
    outputStream << value;
    return outputStream.str();
  }

  bool Converter<Rational>::Check(String& text, int /* base */) {
    try {
      Rational value(text);
      return true;
    }
    catch (NotaRationalNumber) {
      return false;
    }
  }

  Rational Converter<Rational>::TextToValue(String& text,
                                             int /* base */) {
    return Rational(text);
  }

  String Converter<Rational>::ValueToText(Rational& value,
                                           int /* base */) {
    return ((String) value);
  }

  bool Converter<Complex>::Check(String& text, int /* base */) {
    try {
      Complex value(text);
      return true;
    }
    catch (NotaComplexNumber) {
      return false;
    }
  }

  Complex Converter<Complex>::TextToValue(String& text,
                                           int /* base */) {
    return Complex(text);
  }

  String Converter<Complex>::ValueToText(Complex& value,
                                          int /* base */) {
    return ((String) value);
  }
};