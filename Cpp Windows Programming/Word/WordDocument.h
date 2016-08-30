class WordDocument : public StandardDocument {
  public:
    WordDocument(WindowShow windowShow);
    ~WordDocument();
    void InitDocument();

    void OnKeyboardMode(KeyboardMode keyboardMode);
    void UpdateCaret();

    void OnMouseDown(MouseButton mouseButtons, Point mousePoint,
                     bool shiftPressed,
                     bool controlPressed);
    void OnMouseMove(MouseButton mouseButtons, Point mousePoint,
                     bool shiftPressed,
                     bool controlPressed);
    void OnMouseUp(MouseButton mouseButtons, Point mousePoint,
                   bool shiftPressed,
                   bool controlPressed);
    int MousePointToIndex(Point mousePoint) const;
    int MousePointToParagraphIndex(Paragraph* paragraphPtr,
                                   Point mousePoint) const;
    void InvalidateBlock(int firstIndex, int lastIndex);

    void OnDoubleClick(MouseButton mouseButtons, Point mousePoint,
                       bool shiftPressed, bool controlPressed);
    int GetFirstWordIndex(int charIndex) const;
    int GetLastWordIndex(int charIndex) const;

    void OnTouchDown(vector<Point> pointList);
    void OnTouchMove(vector<Point> pointList);

    void OnPageSetup(PageSetupInfo pageSetupInfo);
    void CalculateDocument();

    int GetTotalPages() const {return totalPages;}
    void OnPaint(Graphics& graphics) const;
    void OnDraw(Graphics& graphics, DrawMode drawMode) const;

    void ClearDocument();
    bool WriteDocumentToStream(String name, ostream& outStream)
                               const;
    bool ReadDocumentFromStream(String name, istream& inStream);

    bool CopyEnable() const;
    bool IsCopyAsciiReady() const;
    bool IsCopyUnicodeReady() const;
    bool IsCopyGenericReady(int format) const;

    void CopyAscii(vector<String>& textList) const;
    void CopyUnicode(vector<String>& textList) const;
    void CopyGeneric(int format, InfoList& infoList) const;

    void PasteAscii(const vector<String>& textList);
    void PasteUnicode(const vector<String>& textList);
    void PasteGeneric(int format, InfoList& infoList);

    bool DeleteEnable() const;
    void OnDelete();
    void Delete(int firstIndex, int lastIndex);

    DEFINE_BOOL_LISTENER(WordDocument, PageBreakEnable)
    DEFINE_VOID_LISTENER(WordDocument, OnPageBreak)
    DEFINE_VOID_LISTENER(WordDocument, OnFont)

    DEFINE_BOOL_LISTENER(WordDocument, LeftRadio)
    DEFINE_VOID_LISTENER(WordDocument, OnLeft)
    DEFINE_BOOL_LISTENER(WordDocument, CenterRadio)
    DEFINE_VOID_LISTENER(WordDocument, OnCenter)
    DEFINE_BOOL_LISTENER(WordDocument, RightRadio)
    DEFINE_VOID_LISTENER(WordDocument, OnRight)
    DEFINE_BOOL_LISTENER(WordDocument, JustifiedRadio)
    DEFINE_VOID_LISTENER(WordDocument, OnJustified)

    bool IsAlignment(Alignment alignment) const;
    void SetAlignment(Alignment alignment);

    void OnChar(TCHAR tChar);
    void OnInsertChar(TCHAR tChar, Paragraph* paragraphPtr);
    void OnOverwriteChar(TCHAR tChar, Paragraph* paragraphPtr);
    void ClearNextFont();

    bool OnKeyDown(WORD key, bool shiftPressed,
                   bool controlPressed);
    void OnRegularKey(WORD key);
    void EnsureEditStatus();
    void OnLeftArrowKey();
    void OnRightArrowKey();
    void OnUpArrowKey();
    void OnDownArrowKey();
    int MousePointToIndexDown(Point mousePoint) const;
    void OnPageUpKey();
    void OnPageDownKey();
    void OnHomeKey();
    void OnEndKey();

    void OnShiftKey(WORD key);
    void EnsureMarkStatus();
    void OnShiftLeftArrowKey();
    void OnShiftRightArrowKey();
    void OnShiftUpArrowKey();
    void OnShiftDownArrowKey();
    void OnShiftPageUpKey();
    void OnShiftPageDownKey();
    void OnShiftHomeKey();
    void OnShiftEndKey();

    DEFINE_VOID_LISTENER(WordDocument, OnControlHomeKey);
    DEFINE_VOID_LISTENER(WordDocument, OnControlEndKey);
    DEFINE_VOID_LISTENER(WordDocument, OnShiftControlHomeKey);
    DEFINE_VOID_LISTENER(WordDocument, OnShiftControlEndKey);

    void OnNeutralKey(WORD key);
    void OnReturnKey();
    void OnBackspaceKey();

    bool MakeVisible();

    void GenerateParagraph(Paragraph* paragraphPtr);
    void GenerateSizeAndAscentList(Paragraph* paragraphPtr,
                                   DynamicList<Size>& sizeList,
                                   DynamicList<int>& ascentList);
    void GenerateLineList(Paragraph* paragraphPtr,
                          DynamicList<Size>& sizeList,
                          DynamicList<int>& ascentList);
    void GenerateRegularLineRectList(Paragraph* paragraphPtr,
                                     LineInfo* lineInfoPtr,
                                     DynamicList<Size>& sizeList,
                                     DynamicList<int>&ascentList);
    void GenerateJustifiedLineRectList(Paragraph* paragraphPtr,
                                  LineInfo* lineInfoPtr,
                                  DynamicList<Size>& sizeList,
                                  DynamicList<int>& ascentList);
    void InvalidateRepaintSet(Paragraph* paragraphPtr,
                              DynamicList<CharInfo>&prevRectList);

    DynamicList<CharInfo>& CharList() {return charList;}

  private:
    enum {WordEdit, WordMark} wordMode;

    int totalPages;
    DynamicList<Paragraph*> paragraphList;

    int editIndex = 0, firstMarkIndex = 0, lastMarkIndex = 0;
    Font nextFont;

    double initZoom, initDistance;

    static const unsigned int WordFormat = 1002;
    DynamicList<CharInfo> charList;
};
