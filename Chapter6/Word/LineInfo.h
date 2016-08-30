class LineInfo {
  public:
    LineInfo();
    LineInfo(int first, int last, int top,
             int height, int ascent);

    bool WriteLineInfoToStream(ostream& outStream) const;
    bool ReadLineInfoFromStream(istream& inStream);

    int First() const {return first;}
    int Last() const {return last;}
    int Top() const {return top;}
    int Height() const {return height;}
    int Ascent() const {return ascent;}

    int& First() {return first;}
    int& Last() {return last;}
    int& Top() {return top;}
    int& Height() {return height;}
    int& Ascent() {return ascent;}

  private:
    int first, last, top, height, ascent;
};