class CircleDocument : public StandardDocument {
  public:
    CircleDocument(WindowShow windowShow);
    ~CircleDocument();

    void OnMouseDown(MouseButton mouseButtons,
                     Point mousePoint,
                     bool shiftPressed,
                     bool controlPressed);
    void OnMouseUp(MouseButton mouseButtons,
                   Point mousePoint,
                   bool shiftPressed,
                   bool controlPressed);
    void OnMouseMove(MouseButton mouseButtons,
                     Point mousePoint,
                     bool shiftPressed,
                     bool controlPressed);

    void OnDraw(Graphics& graphics, DrawMode drawMode) const;
    bool ReadDocumentFromStream(String name,
                                istream& inStream);
    bool WriteDocumentToStream(String name,
                           ostream& outStream) const;
    void ClearDocument();

    DEFINE_VOID_LISTENER(CircleDocument, OnRed);
    DEFINE_VOID_LISTENER(CircleDocument, OnGreen);
    DEFINE_VOID_LISTENER(CircleDocument, OnBlue);
    DEFINE_VOID_LISTENER(CircleDocument, OnColorDialog);

    DEFINE_BOOL_LISTENER(CircleDocument, RedRadio);
    DEFINE_BOOL_LISTENER(CircleDocument, GreenRadio);
    DEFINE_BOOL_LISTENER(CircleDocument, BlueRadio);

    static const int CircleRadius = 500;

  private:
    vector<Circle> circleList;
    Color nextColor;
    int moveIndex = -1;
    Point movePoint;
};