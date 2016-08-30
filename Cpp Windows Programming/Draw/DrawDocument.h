class DrawDocument : public StandardDocument {
  public:
    DrawDocument(WindowShow windowShow);
    ~DrawDocument();

    void OnMouseDown(MouseButton mouseButtons, Point mousePoint,
           bool shiftPressed,bool controlPressed);
    void OnMouseMove(MouseButton mouseButtons, Point mousePoint,
           bool shiftPressed,bool controlPressed);
    void OnDoubleClick(MouseButton mouseButtons, Point mousePoint,
                       bool shiftPressed, bool controlPressed);
    void OnMouseUp(MouseButton mouseButtons, Point mousePoint,
                   bool shiftPressed, bool controlPressed);

    void OnDraw(Graphics& graphics, DrawMode drawMode) const;

    void ClearDocument();
    bool WriteDocumentToStream(String name, ostream& outStream)
                               const;
    bool ReadDocumentFromStream(String name, istream& inStream);
    DrawFigure* CreateFigure(FigureId figureId) const;

    bool CopyEnable() const;
    void OnDelete();

    bool IsCopyGenericReady(int /* format */) const;
    void CopyGeneric(int format, InfoList& infoList) const;
    void PasteGeneric(int format, InfoList& infoList);

    DEFINE_BOOL_LISTENER(DrawDocument, ModifyEnable)
    DEFINE_BOOL_LISTENER(DrawDocument, ModifyRadio)
    DEFINE_VOID_LISTENER(DrawDocument, OnModify)

    DEFINE_BOOL_LISTENER(DrawDocument, ColorEnable)
    DEFINE_VOID_LISTENER(DrawDocument, OnColor)

    DEFINE_BOOL_LISTENER(DrawDocument, FillEnable)
    DEFINE_BOOL_LISTENER(DrawDocument, FillCheck)
    DEFINE_VOID_LISTENER(DrawDocument, OnFill)

    DEFINE_BOOL_LISTENER(DrawDocument, LineEnable);
    DEFINE_BOOL_LISTENER(DrawDocument, ArrowEnable);
    DEFINE_BOOL_LISTENER(DrawDocument, RectangleEnable);
    DEFINE_BOOL_LISTENER(DrawDocument, EllipseEnable);

    DEFINE_BOOL_LISTENER(DrawDocument, LineRadio);
    DEFINE_BOOL_LISTENER(DrawDocument, ArrowRadio);
    DEFINE_BOOL_LISTENER(DrawDocument, RectangleRadio);
    DEFINE_BOOL_LISTENER(DrawDocument, EllipseRadio);

    DEFINE_VOID_LISTENER(DrawDocument, OnLine);
    DEFINE_VOID_LISTENER(DrawDocument, OnArrow);
    DEFINE_VOID_LISTENER(DrawDocument, OnRectangle);
    DEFINE_VOID_LISTENER(DrawDocument, OnEllipse);

    void UpdateCursor();

private:
    enum ApplicationMode {Idle, ModifySingle, MoveMultiple,
                          ModifyRectangle};
    ApplicationMode applicationMode = Idle;

    enum ActionMode {Modify, Add};
    ActionMode actionMode = Add;
    FigureId addFigureId = LineId;

    Color nextColor;
    bool nextFill;
    Point prevMousePoint;

    DynamicList<DrawFigure*> figurePtrList;
    Rect insideRectangle;

    static const unsigned int DrawFormat = 1000;
};