#if 0
struct Position {
  Position() :m_paragraph(0), m_char(0) { /* Empty. */ }
  Position(int paragraph, int charIndex)
    :m_paragraph(paragraph), m_char(charIndex) { /* Empty. */ }
  void Clear() { m_paragraph = 0; m_char = 0; }

  friend bool operator==(Position psLeft, Position psRight);
  friend bool operator!=(Position psLeft, Position psRight);
  friend bool operator<(Position psLeft, Position psRight);
  friend bool operator<=(Position psLeft, Position psRight);
  friend bool operator>(Position psLeft, Position psRight);
  friend bool operator>=(Position psLeft, Position psRight);

  int m_paragraph, m_char;F
};

extern Position Min(Position psLeft, Position psRight);
extern Position Max(Position psLeft, Position psRight);
#endif