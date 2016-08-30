class Circle {
  public:
    Circle();
    Circle(Point center, int radius, Color color);

    bool WriteCircle(ostream& outStream) const;
    bool ReadCircle(istream& inStream);

    bool IsClick(Point point) const;
    Rect Area() const;
    void Draw(Graphics& graphics) const;

    Point Center() const {return center;}
    Point& Center() {return center;}
    Color GetColor() {return color;}

  private:
    Point center;
    int radius;
    Color color;
};
