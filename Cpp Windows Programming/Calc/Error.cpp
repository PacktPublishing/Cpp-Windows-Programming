#include "..\\SmallWindows\\SmallWindows.h"
#include "Error.h"

Error::Error(ErrorId errorId)
 :errorId(errorId) {
  // Empty.
}

String Error::ErrorText() const {
  switch (errorId) {
    case SyntaxError:
      return TEXT("Syntax Error.");

    case CircularReference:
      return TEXT("#Circular reference#");

    case DivisionByZero:
      return TEXT("#Division by Zero#");

    case MissingValue:
      return TEXT("#Missing Value#");

    case ReferenceOutOfRange:
      return TEXT("#Reference out of Range.#");
  }

  return TEXT("");
}