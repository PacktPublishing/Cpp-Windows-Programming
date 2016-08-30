extern const int HeaderWidth, HeaderHeight, CellMargin, ColWidth, RowHeight;

#define Rows 10
#define Cols 4

enum Alignment {Left, Center, Right, Justified, Top, Bottom}; 

class Cell {
  public:
    Cell();
    ~Cell();

    Cell(const Cell& cell);
    Cell& operator=(const Cell& cell);

    void Clear();
    void Reset();

    void CharDown(int editIndex, TCHAR tChar,
                  KeyboardMode keyboardMode);
    int MouseToIndex(int x) const;

    String GetText() const {return text;}
    void SetText(String& t) {text = t;}

    bool IsFormula() const {return (cellMode == FormulaMode);}
    vector<Rect> CaretList() const {return caretList;}

    Font CellFont() const {return font;}
    Font& CellFont() {return font;}

    Color BackgroundColor() const {return backgroundColor;}
    Color& BackgroundColor() {return backgroundColor;}

    Alignment HorizontalAlignment() const
                        {return horizontalAlignignment;}
    Alignment& HorizontalAlignment()
                         {return horizontalAlignignment;}
    Alignment VerticalAlignment() const
                      {return verticalAlignignment;}
    Alignment& VerticalAlignment() {return verticalAlignignment;}

    void DrawCell(Graphics& graphics, Reference cellRef,
                  bool inverse) const;
    void GenerateCaretList(Window* windowPtr);

    void DisplayFormula();
    void InterpretCell(set<Reference>& sourceSet);
    void GenerateSourceSet(set<Reference>& sourceSet) const;
    void GenerateSourceSet(Tree<TreeNode>* syntaxNodePtr,
                           set<Reference>& sourceSet) const;
    String TreeToString() const;
    String TreeToString(Tree<TreeNode>* syntaxNodePtr) const;

    void UpdateTree(Reference diffRef, set<Reference>& sourceSet);
    void UpdateTree(Tree<TreeNode>* syntaxNodePtr,
                    Reference diffRef, set<Reference>& sourceSet);

    bool HasValue() const;
    double GetValue() const {return value;}

    void Evaluate(map<Reference,double>& valueMap);
    double Evaluate(Tree<TreeNode>* syntaxNodePtr,
                    map<Reference,double>& valueMap);

    bool WriteCellToStream(ostream& outStream) const;
    bool ReadCellFromStream(istream& inStream);

    void WriteCellToClipboard(InfoList& infoList) const;
    void ReadCellFromClipboard(InfoList& infoList);

  private:
    enum CellMode {TextMode, ValueMode, FormulaMode} cellMode;

    Font font;
    Color backgroundColor;
    Alignment horizontalAlignignment, verticalAlignignment;

    String text;
    vector<Rect> caretList;

    bool hasValue;
    double value;
    Tree<TreeNode>* syntaxTreePtr;
};