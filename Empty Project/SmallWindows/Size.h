namespace SmallWindows {
  class Size;
  extern const Size ZeroSize;

  class Size {
    public:
      Size();
      Size(int width, int height);
      Size(const Size& size);
      Size& operator=(const Size& size);

      Size(const SIZE& size);
      Size& operator=(const SIZE& size);
      operator SIZE() const;

      bool operator==(const Size& size) const;
      bool operator!=(const Size& size) const;
      bool operator<(const Size& size) const;
      bool operator<=(const Size& size) const;
      bool operator>(const Size& size) const;
      bool operator>=(const Size& size) const;

      friend Size Min(const Size& left, const Size& right);
      friend Size Max(const Size& left, const Size& right);

      bool IsZero() const {return ((width == 0) && (height == 0));}

      Size operator*=(double factor);
      friend Size operator*(const Size& size, double factor);
      friend Size operator*(double factor, const Size& size);

      Size operator*=(pair<double,double> factorPair);
      friend Size operator*(const Size& size,
                            pair<double,double> factorPair);
      friend Size operator*(pair<double,double> factorPair,
                            const Size& size);

      Size operator+=(int distance);
      Size operator-=(int distance);
      friend Size operator+(const Size& size, int distance);
      friend Size operator-(const Size& size, int distance);

      Size operator+=(const Size& size);
      Size operator-=(const Size& size);
      friend Size operator+(const Size& left, const Size& right);
      friend Size operator-(const Size& left, const Size& right);

      bool WriteSizeToStream(ostream& outStream) const;
      bool ReadSizeFromStream(istream& inStream);
      void WriteSizeToClipboard(InfoList& infoList) const;
      void ReadSizeFromClipboard(InfoList& infoList);
      void WriteSizeToRegistry(String key) const;
      void ReadSizeFromRegistry(String key,
                                Size defaultSize = ZeroSize);

      int Width() const {return width;}
      int Height() const {return height;}
      int& Width() {return width;}
      int& Height() {return height;}

    private:
      int width, height;
  };
};