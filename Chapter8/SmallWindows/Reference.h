namespace SmallWindows {
  class Reference;
  extern const Reference ZeroReference;

  class Reference {
    public:
      Reference();
      Reference(int row, int col);
      Reference(const Reference& ref);
      Reference& operator=(const Reference& ref);

      friend bool operator==(const Reference& ref1,
                             const Reference& ref2);
      friend bool operator!=(const Reference& ref1,
                             const Reference& ref2);
      friend bool operator<(const Reference& ref1,
                            const Reference& ref2);
      friend bool operator<=(const Reference& ref1,
                             const Reference& ref2);
      friend bool operator>(const Reference& ref1,
                            const Reference& ref2);
      friend bool operator>=(const Reference& ref1,
                             const Reference& ref2);

      Reference& operator+=(const Reference& ref);
      Reference& operator-=(const Reference& ref);
      friend Reference operator+(const Reference& ref1,
                                 const Reference& ref2); 
      friend Reference operator-(const Reference& ref1,
                                 const Reference& ref2);

      void Clear() {row = 0; col = 0;}
      bool IsEmpty() const {return ((row == 0) && (col == 0));}
      String ToString() const;
      bool Inside(Reference minRef, Reference maxRef);

      bool WriteReferenceToStream(ostream& outStream) const;
      bool ReadReferenceFromStream(istream& inStream);
      void WriteReferenceToClipboard(InfoList& infoList) const;
      void ReadReferenceFromClipboard(InfoList& infoList);
      void WriteReferenceToRegistry(String key) const;
      void ReadReferenceFromRegistry(String key,
                            Reference defaultRef = ZeroReference);

      int Row() const {return row;}
      int Col() const {return col;}
      int& Row() {return row;}
      int& Col() {return col;}

    private:
      int row, col;
  };
};