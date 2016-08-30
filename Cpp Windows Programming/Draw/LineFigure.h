class LineFigure : public DrawFigure {
  public:
    LineFigure(const Window* windowPtr);
    virtual FigureId GetId() const {return LineId;}
    virtual void SetFirstPoint(Point point);

    virtual bool IsClick(Point mousePoint);
    virtual bool IsInside(Rect rectangleArea);
    virtual void DoubleClick(Point mousePoint) {/* Empty. */}

    virtual void Modify(Size distanceSize);
    virtual void Move(Size distanceSize);

    virtual Rect Area() const;
    virtual void Draw(Graphics& graphics) const;
    virtual CursorType GetCursor() const;

    virtual bool WriteFigureToStream(ostream& outStream) const;
    virtual bool ReadFigureFromStream(istream& inStream);

    virtual void WriteFigureToClipboard(InfoList& infoList) const;
    virtual void ReadFigureFromClipboard(InfoList& infoList);

  protected:
    enum {CreateLine, FirstPoint, LastPoint, MoveLine} lineMode;
    Point firstPoint, lastPoint;
    static bool IsPointInLine(Point firstPoint, Point lastPoint,
                              Point point);
};