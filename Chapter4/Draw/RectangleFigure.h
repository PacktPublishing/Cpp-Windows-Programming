class RectangleFigure : public DrawFigure {
  public:
    RectangleFigure(const Window* windowPtr);

    virtual void SetFirstPoint(Point point);
    virtual FigureId GetId() const {return RectangleId;}

    virtual bool IsClick(Point mousePoint);
    virtual bool IsInside(Rect rectangleArea);
    virtual void DoubleClick(Point mousePoint);

    virtual void Modify(Size distanceSize);
    virtual void Move(Size distanceSize);

    virtual Rect Area() const;
    virtual void Draw(Graphics& graphics) const;
    virtual CursorType GetCursor() const;

    bool IsFillable() const {return true;}
    bool IsFilled() const {return filled;}
    void Fill(bool fill) {filled = fill; Invalidate();}

    virtual bool WriteFigureToStream(ostream& outStream) const;
    virtual bool ReadFigureFromStream(istream& inStream);

    virtual void WriteFigureToClipboard(InfoList& infoList) const;
    virtual void ReadFigureFromClipboard(InfoList& infoList);

  private:
    enum {CreateRectangle, TopLeftPoint, TopRightPoint,
          BottomRightPoint, BottomLeftPoint, MoveRectangle}
         rectangleMode;

  protected:
    bool filled = false;
    Point topLeft, bottomRight;
};