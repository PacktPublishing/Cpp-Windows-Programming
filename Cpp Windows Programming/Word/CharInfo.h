class LineInfo;
class Paragraph;

class CharInfo {
  public:
    CharInfo(Paragraph* paragraphPtr = nullptr,
             TCHAR tChar = TEXT('\0'),
             Font font = SystemFont, Rect rect = ZeroRect);

    CharInfo(const CharInfo& charInfo);
    CharInfo& operator=(const CharInfo& charInfo);

    bool WriteCharInfoToStream(ostream& outStream) const;
    bool ReadCharInfoFromStream(istream& inStream);

    void WriteCharInfoToClipboard(InfoList& infoList) const;
    void ReadCharInfoFromClipboard(InfoList& infoList);

    TCHAR Char() const {return tChar;}
    Font CharFont() const {return charFont;}
    Rect CharRect() const {return charRect;}
    LineInfo* LineInfoPtr() const {return lineInfoPtr;}
    Paragraph* ParagraphPtr() const {return paragraphPtr;}

    TCHAR& Char() {return tChar;}
    Font& CharFont() {return charFont;}
    Rect& CharRect() {return charRect;}
    LineInfo*& LineInfoPtr() {return lineInfoPtr;}
    Paragraph*& ParagraphPtr() {return paragraphPtr;}

  private:
    TCHAR tChar;
    Font charFont;
    Rect charRect;
    Paragraph* paragraphPtr;
    LineInfo* lineInfoPtr;
};