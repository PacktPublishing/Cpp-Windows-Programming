namespace SmallWindows {
  typedef LPCTSTR CursorType;

  class Cursor {
    public:
      static const CursorType Normal;
      static const CursorType Arrow;
      static const CursorType ArrowHourGlass;
      static const CursorType Crosshair;
      static const CursorType Hand;
      static const CursorType ArrowQuestionMark;
      static const CursorType IBeam;
      static const CursorType SlashedCircle;
      static const CursorType SizeAll;
      static const CursorType SizeNorthEastSouthWest;
      static const CursorType SizeNorthSouth;
      static const CursorType SizeNorthWestSouthEast;
      static const CursorType SizeWestEast;
      static const CursorType VerticalArrow;
      static const CursorType HourGlass;

      static void Set(CursorType cursor);
   };
};