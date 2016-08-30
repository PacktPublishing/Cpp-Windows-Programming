enum Alignment {Left, Center, Right, Justified};
class WordDocument;

class Paragraph {
  public:
    Paragraph();
    Paragraph(int first, int last,
              Alignment alignment, int index);

    bool WriteParagraphToStream(ostream& outStream) const;
    bool ReadParagraphFromStream(WordDocument* wordDocumentPtr,
                                 istream& inStream);

    void WriteParagraphToClipboard(InfoList& infoList) const;
    void ReadParagraphFromClipboard(InfoList& infoList);

    int& First() {return first;}
    int& Last() {return last;}
    int& Top() {return top;}
    int& Index() {return index;}
    int& Height() {return height;}
    bool& PageBreak() {return pageBreak;}
    Alignment& AlignmentField() {return alignment;}
    DynamicList<LineInfo*>& LinePtrList() {return linePtrList;}

  private:
    int first, last, top, height, index;
    bool pageBreak;
    Alignment alignment;
    DynamicList<LineInfo*> linePtrList;
};