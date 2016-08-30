class CalcDocument : public StandardDocument {
  public:
    CalcDocument(WindowShow windowShow);

    void OnMouseDown(MouseButton mouseButtons, Point mousePoint,
                     bool shiftPressed, bool controlPressed);
    void OnMouseMove(MouseButton mouseButtons, Point mousePoint,
                     bool shiftPressed, bool controlPressed);
    void OnDoubleClick(MouseButton mouseButtons, Point mousePoint,
                       bool shiftPressed,bool controlPressed);

    virtual void OnVerticalScroll(WORD flags, WORD y);
    virtual void OnHorizontalScroll(WORD flags, WORD x);

    enum ClickArea {ClickAll, ClickRow, ClickColumn, ClickCell};
    ClickArea GetMouseLocation(Point mousePoint,
                               Reference& cellRef) const;
    void MarkBlock(ClickArea clickArea, Reference newFirstMarkRef,
                   Reference newLastMarkRef);

    void OnDraw(Graphics& graphics, DrawMode drawMode) const;
    void UpdateCaret();

    bool IsCellVisible(Reference cellRef) const;
    void MakeCellVisible();
    void MakeCellVisible(Reference cellRect);

    bool ToMarkMode();
    void Remark(Reference newFirstRef, Reference newLastRef);

    void OnChar(TCHAR tChar);
    bool OnKeyDown(WORD key, bool shiftPressed,
                   bool controlPressed);
    void OnLeftArrowKey(bool shiftPressed);
    void OnRightArrowKey(bool shiftPressed);
    void OnUpArrowKey(bool shiftPressed);
    void OnDownArrowKey(bool shiftPressed);
    void OnHomeKey(bool shiftPressed, bool controlPressed);
    void OnEndKey(bool shiftPressed, bool controlPressed);
    void OnReturnKey();
    void OnTabulatorKey(bool shiftPressed);
    void OnEscapeKey();
    void OnDeleteKey();
    void OnBackspaceKey();

    void ClearDocument();
    bool ReadDocumentFromStream(String name, istream& inStream);
    bool WriteDocumentToStream(String name, ostream& outStream)
                               const;

    static bool WriteSetMapToStream(const map<Reference,
                     set<Reference>>& setMap, ostream& outStream);
    static bool ReadSetMapFromStream(map<Reference,set<Reference>>
                                     &setMap, istream& inStream);

    bool CopyEnable() const;
    bool IsCopyAsciiReady() const {return true;}
    bool IsCopyUnicodeReady() const {return true;}
    bool IsCopyGenericReady(int format) const {return true;}

    void CopyAscii(vector<String>& textList) const;
    void CopyUnicode(vector<String>& textList) const;
    void CopyGeneric(int format, InfoList& infoList) const;

    bool IsPasteAsciiReady(const vector<String>& textList) const;
    bool IsPasteUnicodeReady(const vector<String>& textList)const;
    bool IsPasteGenericReady(int format, InfoList& infoList)const;

    void PasteAscii(const vector<String>& textList);
    void PasteUnicode(const vector<String>& textList);
    void PasteGeneric(int format, InfoList& infoList);

    bool DeleteEnable() const;
    void OnDelete();

    DEFINE_VOID_LISTENER(CalcDocument, OnFont);
    DEFINE_VOID_LISTENER(CalcDocument, OnBackgroundColor);

    DEFINE_BOOL_LISTENER(CalcDocument, HorizontalLeftRadio);
    DEFINE_BOOL_LISTENER(CalcDocument, HorizontalCenterRadio);
    DEFINE_BOOL_LISTENER(CalcDocument, HorizontalRightRadio);
    DEFINE_BOOL_LISTENER(CalcDocument, HorizontalJustifiedRadio);
    bool IsHorizontalAlignment(Alignment alignment) const;

    DEFINE_VOID_LISTENER(CalcDocument, OnHorizontalLeft);
    DEFINE_VOID_LISTENER(CalcDocument, OnHorizontalCenter);
    DEFINE_VOID_LISTENER(CalcDocument, OnHorizontalRight);
    DEFINE_VOID_LISTENER(CalcDocument, OnHorizontalJustified);
    void SetHorizontalAlignment(Alignment alignment);

    DEFINE_BOOL_LISTENER(CalcDocument, VerticalTopRadio);
    DEFINE_BOOL_LISTENER(CalcDocument, VerticalCenterRadio);
    DEFINE_BOOL_LISTENER(CalcDocument, VerticalBottomRadio);
    bool IsVerticalAlignment(Alignment alignment) const;

    DEFINE_VOID_LISTENER(CalcDocument, OnVerticalTop);
    DEFINE_VOID_LISTENER(CalcDocument, OnVerticalCenter);
    DEFINE_VOID_LISTENER(CalcDocument, OnVerticalBottom);    
    void SetVerticalAlignment(Alignment alignment);

    bool InterpretEditCell();
    bool IsCircular(Reference cellRef, set<Reference>& targetSet);
    void RemoveTargetSetMap(Reference cellRef);
    void AddTargetSetMap(Reference cellRef,
                         set<Reference>& newSourceSet);
    void InvalidateCell(Reference cellRef);
    void EvaluateRecursive(Reference cellRef,
                           set<Reference>& invalidateSet);
    void EvaluateCell(Reference cellRef);

  private:
    enum CalcMode {Edit, Mark} calcMode = Mark;
    bool markOk;

    Reference firstMarkRef, lastMarkRef, editRef;
    int editIndex;

    Matrix<Rows, Cols, Cell> cellMatrix;
    Cell cellMatrixX[Rows][Cols];
    map<Reference, set<Reference>> sourceSetMap, targetSetMap;

    static const unsigned int CalcFormat = 1003;

    Cell prevCell;
};