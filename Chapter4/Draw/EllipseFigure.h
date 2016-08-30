class EllipseFigure : public RectangleFigure {
  public:
    EllipseFigure(const Window* windowPtr);
    FigureId GetId() const {return EllipseId;}

    bool IsClick(Point mousePoint);
    void Modify(Size distanceSize);
    void Draw(Graphics& graphics) const;
    CursorType GetCursor() const;

  private:
    enum {CreateEllipse, LeftPoint, TopPoint, RightPoint,
          BottomPoint, MoveEllipse} ellipseMode;
};