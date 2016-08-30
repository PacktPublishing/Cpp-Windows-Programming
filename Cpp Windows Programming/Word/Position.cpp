#if 0
#include "..\\SmallWindows\\SmallWindows.h"
#include "Position.h"

bool operator==(Position psLeft, Position psRight) {
  return (psLeft.m_paragraph == psRight.m_paragraph) &&
         (psLeft.m_char == psRight.m_char);
}

bool operator!=(Position psLeft, Position psRight) {
  return !(psLeft == psRight);
}

bool operator<(Position psLeft, Position psRight) {
  return (psLeft.m_paragraph < psRight.m_paragraph) ||
         ((psLeft.m_paragraph == psRight.m_paragraph) &&
          (psLeft.m_char < psRight.m_char));
}

bool operator<=(Position psLeft, Position psRight) {
  return (psLeft < psRight) || (psLeft == psRight);
}

bool operator>(Position psLeft, Position psRight) {
  return !(psLeft <= psRight);
}

bool operator>=(Position psLeft, Position psRight) {
  return !(psLeft < psRight);
}

Position Min(Position psLeft, Position psRight) {
  return (psLeft < psRight) ? psLeft : psRight;  
}

Position Max(Position psLeft, Position psRight) {
  return (psLeft > psRight) ? psLeft : psRight;
}
#endif