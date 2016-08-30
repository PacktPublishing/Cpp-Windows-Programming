namespace SmallWindows {
  template <class Type>
  class Converter {
    public:
      static bool Check(String& text, int base);
      static Type TextToValue(String& text, int base);
      static String ValueToText(Type& value, int base);
  };

  template <>
  class Converter<int> {
    public:
      static bool Check(String& text, int base);
      static int TextToValue(String& text, int base);
      static String ValueToText(int& value, int base);
  };

  template <>
  class Converter<unsigned int> {
    public:
      static bool Check(String& text, int base);
      static unsigned int TextToValue(String& text, int base);
      static String ValueToText(unsigned int& value, int base);
  };

  template <>
  class Converter<double> {
    public:
      static bool Check(String& text, int /* base */);
      static double TextToValue(String& text, int /* base */);
      static String ValueToText(double& value, int /* base */);
  };

  template <>
  class Converter<String> {
    public:
      static bool Check(String& text, int /* base */)
                       {return true;}
      static String TextToValue(String& text, int /* base */)
                               {return String(text);}
      static String ValueToText(String& value, int /* base */)
                               {return String(value);}
  };

  template <>
  class Converter<Rational> {
    public:
      static bool Check(String& text, int /* base */);
      static Rational TextToValue(String& text, int /* base */);
      static String ValueToText(Rational& value, int /* base */);
  };

  template <>
  class Converter<Complex> {
    public:
      static bool Check(String& text, int /* base */);
      static Complex TextToValue(String& text, int /* base */);
      static String ValueToText(Complex& value, int /* base */);
  };
};