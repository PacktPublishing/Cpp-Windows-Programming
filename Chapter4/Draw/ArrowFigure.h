class ArrowFigure : public LineFigure {
  public:
    ArrowFigure(const Window* windowPtr);
    FigureId GetId() const {return ArrowId;};

    bool IsClick(Point mousePoint);
    bool IsInside(Rect rectangleArea);
    void DoubleClick(Point mousePoint);

    void Modify(Size distanceSize);
    void Move(Size distanceSize);

    Rect Area() const;
    void Draw(Graphics& graphics) const;

    bool WriteFigureToStream(ostream& outStream) const;
    bool ReadFigureFromStream(istream& inStream);

    void WriteFigureToClipboard(InfoList& infoList) const;
    void ReadFigureFromClipboard(InfoList& infoList);

  private:
    static const int ArrowLength = 500;
    Point leftPoint, rightPoint;
    void CalculateArrowHead();
};