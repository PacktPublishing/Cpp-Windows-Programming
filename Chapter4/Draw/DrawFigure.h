enum FigureId {LineId, ArrowId, RectangleId, EllipseId};

class DrawDocument;

class DrawFigure {
  public:
    DrawFigure(const Window* windowPtr);

    virtual FigureId GetId() const = 0;
    virtual void SetFirstPoint(Point point) = 0;

    virtual bool IsClick(Point mousePoint) = 0;
    virtual bool IsInside(Rect area) = 0;
    virtual void DoubleClick(Point mousePoint) = 0;

    virtual void Modify(Size distanceSize) = 0;
    virtual void Move(Size distanceSize) = 0;

    virtual void Invalidate() const {windowPtr->Invalidate(Area());}
    virtual Rect Area() const = 0;
    virtual void Draw(Graphics& graphics) const = 0;

    virtual bool WriteFigureToStream(ostream& outStream) const;
    virtual bool ReadFigureFromStream(istream& inStream);

    virtual void WriteFigureToClipboard(InfoList& infoList) const;
    virtual void ReadFigureFromClipboard(InfoList& infoList);

    bool IsMarked() const {return marked;}
    void Mark(bool mark);

    Color GetColor() const {return color;}
    void SetColor(Color color);

    virtual bool IsFillable() const {return false;}
    virtual bool IsFilled() const {return false;}
    virtual void Fill(bool fill) {/* Empty. */ }

    virtual CursorType GetCursor() const = 0;

    static const Size MarkRadius;

  private:
    const Window* windowPtr;
    Color color;
    bool marked = false;
};