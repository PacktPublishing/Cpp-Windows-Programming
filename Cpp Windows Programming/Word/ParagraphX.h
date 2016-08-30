#if 0
class WordDocument;

class ParagraphX {
  public:
    ParagraphX(WordDocument* wordDocumentPtr, String text = ZeroString,
              Alignment alignment = Left, Font font = SystemFont,
              Color color = SystemColor);
    String ExtractText() const;

    void WriteParagraphToCharList(list<char>& charList) const;
    void ReadParagraphFromBuffer(char** bufferPtr);

    bool WriteParagraphToStream(ostream& outStream) const;
    bool ReadParagraphFromStream(istream& inStream);

    void Draw(Graphics& graphics, bool marked,
              int firstMarked = 0, int lastMarked = -1);

    DynamicList<CharInfo>& GetCharList() {return m_charList;}

    void AddChar(int charIndex, TCHAR newChar, Font newFont, Color newColor);
    void OnDelete(int firstIndex = 0, int lastIndex = -1);

    Rect GaretRect(int charIndex, KeyboardState keyboardState);
    Font GetFont(int charIndex);
    Color GetColor(int charIndex);

    bool GetWord(int charIndex, int& firstIndex, int &lastIndex);
    int HomeChar(int charIndex);
    int EndChar(int charIndex);

    void InsertParagraph(int charIndex, ParagraphX* paragraphPtr);
    void AppendParagraph(ParagraphX* paragraphPtr);
    void InsertText(int charIndex, String text);
    void AppendText(String text);




    ParagraphX* Split(int charIndex);
    ParagraphX* Extract(int firstIndex, int lastIndex);



    void SwitchPageBreak();
    CharInfo GetCharInfo(int charIndex) const {return m_charList[charIndex];}

    int MouseDown(Point mousePoint);


    Rect CaretArea(int charIndex);
//    CharInfo& GetCharInfo(int index) {return m_charList[index];}

    pair<int,int> LineArea(int charIndex);

    void CalculateParagraph(Font nextFont = SystemFont, int editIndex = 0);

  private:
    void GenerateEmptyList(Font nextFont);
    void GenerateSizeAndAscentList(vector<Size>& sizeList, vector<int>&
                                   ascentList, Font nextFont, int editIndex);
    void GenerateLineList(vector<Size>& sizeList, vector<int>& ascentList);
    void GenerateLineRectList(LineInfo* lineInfoPtr, vector<Size> sizeList,
                              vector<int> ascentList);
    void GenerateRepaintSet(DynamicList<CharInfo>& oldCharList);

  public:
    void InvalidateBlock(int firstIndex, int lastIndex = -1);

    void SetFont(Font font, int firstIndex = 0, int lastIndex = -1);
    void SetColor(Color color, int firstIndex = 0, int lastIndex = -1);

    Alignment GetAlignment() const {return m_alignment;}
    void SetAlignment(Alignment alignment) {m_alignment = alignment;
                      CalculateParagraph();}

    int StartPos() {return m_yStartPos;}
    void SetStartPos(int yStartPos) {m_yStartPos = yStartPos;}
    int Height() {return m_height;}
    int Length() {return m_charList.size();}
    bool PageBreak() const {return m_pageBreak;}

    int PageWidth() const;
    int PageHeight() const;

private:
    WordDocument* m_wordDocumentPtr;
    Alignment m_alignment;

    DynamicList<CharInfo> m_charList;
    vector<LineInfo> m_lineList;

    Font m_emptyFont;
    Color m_emptyColor;
    Rect m_emptyRect;

    int m_yStartPos, m_height;
    bool m_pageBreak = false;
};
#endif