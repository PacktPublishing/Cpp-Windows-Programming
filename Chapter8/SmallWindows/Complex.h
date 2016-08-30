namespace SmallWindows {
  class NotaComplexNumber : public exception {
    public:
      NotaComplexNumber() {/* Empty. */}
  };

  extern double Square(double value);

  class Complex {
    public:
      Complex(double x = 0, double y = 0);
      Complex(const Complex &complex);
      Complex operator=(const Complex &complex);
      bool ReadStream(const String& text);
      Complex(const String& text);
      operator String() const;

      bool operator==(const Complex &complex) const;
      bool operator!=(const Complex &complex) const;
      bool operator<(const Complex &complex) const;
      bool operator<=(const Complex &complex) const;
      bool operator>(const Complex &complex) const;
      bool operator>=(const Complex &complex) const;

      Complex operator+=(double x);
      Complex operator+=(Complex &complex);
      friend Complex operator+(double x, const Complex &complex);
      friend Complex operator+(const Complex &complex, double x);
      friend Complex operator+(const Complex &complex1,
                               const Complex &complex2);

      Complex operator-=(double x);
      Complex operator-=(Complex &complex);
      friend Complex operator-(double x, const Complex &complex);
      friend Complex operator-(const Complex &complex, double x);
      friend Complex operator-(const Complex &complex1,
                               const Complex &complex2);

      Complex operator*=(double x);
      Complex operator*=(Complex &complex);
      friend Complex operator*(double x, const Complex &complex);
      friend Complex operator*(const Complex &complex, double x);
      friend Complex operator*(const Complex &complex1,
                               const Complex &complex2);

      Complex operator/=(double x);
      Complex operator/=(Complex &complex);
      friend Complex operator/(double x, const Complex &complex);
      friend Complex operator/(const Complex &complex, double x);
      friend Complex operator/(const Complex &complex1,
                               const Complex &complex2);

      double Abs() const {return sqrt(Square(x) + Square(y));}
      operator double() const {return Abs();}

    private:
      double x, y;
  };
};
