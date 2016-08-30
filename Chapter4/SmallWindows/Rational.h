namespace SmallWindows {
  class NotaRationalNumber : public exception {
    public:
      NotaRationalNumber() {/* Empty. */}
  };

  class Rational {
    public:
      Rational(int numerator = 0, int denominator = 1);
      Rational(const String& text);
      Rational(const Rational &rational);
      Rational operator=(const Rational &complex);
      operator String() const;

      bool operator==(const Rational &rational) const;
      bool operator!=(const Rational &rational) const;
      bool operator< (const Rational &rational) const;
      bool operator<=(const Rational &rational) const;
      bool operator> (const Rational &rational) const;
      bool operator>=(const Rational &rational) const;

      Rational operator+(const Rational &rational) const;
      Rational operator-(const Rational &rational) const;
      Rational operator*(const Rational &rational) const;
      Rational operator/(const Rational &rational) const;

    private:
      void Normalize();
      int GCD(int iNum1, int iNum2);
      int numerator, denominator;
  };
};