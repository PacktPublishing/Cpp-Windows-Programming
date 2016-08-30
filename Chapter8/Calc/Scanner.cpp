#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"

Scanner::Scanner(String buffer)
 :buffer(buffer) {
 // Empty.
}

list<Token> Scanner::Scan() {
  list<Token> tokenList;

  while (true) {
    Token token = NextToken();
    tokenList.push_back(token);

    if (token.Id() == EndOfLine) {
      break;
    }
  }

  return tokenList;
}

Token Scanner::NextToken() {
  while (buffer[0] == TEXT(' ')) {
    buffer.erase(0, 1);
  }

  switch (buffer[0]) {
    case TEXT('\0'):
      return Token(EndOfLine);

    case TEXT('+'):
      buffer.erase(0, 1);
      return Token(Plus);

    case TEXT('-'):
      buffer.erase(0, 1);
      return Token(Minus);
    
    case TEXT('*'):
      buffer.erase(0, 1);
      return Token(Star);

    case TEXT('/'):
      buffer.erase(0, 1);
      return Token(Slash);

    case TEXT('('):
      buffer.erase(0, 1);
      return Token(LeftParenthesis);

    case TEXT(')'):
      buffer.erase(0, 1);
      return Token(RightParenthesis);

    default: {
        double value;
        Reference reference;

        if (ScanValue(value)) {
          return Token(value);
        }
        else if (ScanReference(reference)) {
          return Token(reference);
        }
        else {
          throw Error(SyntaxError);
        }
      }
      break;
  }
}

bool Scanner::ScanValue(double& value) {
  int charCount;
  int fieldCount = _stscanf_s(buffer.c_str(), TEXT("%lf%n"),
                              &value, &charCount);

  if (fieldCount > 0) {
    buffer.erase(0, charCount);
    return true;
  }

  return false;
}

bool Scanner::ScanReference(Reference& reference) {
  if (isalpha(buffer[0]) && (isdigit(buffer[1]))) {
    reference.Col() = tolower(buffer[0]) - TEXT('a');
    buffer.erase(0, 1);

    int row;
    int charCount;
    _stscanf_s(buffer.c_str(), TEXT("%d%n"), &row, &charCount);
    reference.Row() = row - 1;
    buffer.erase(0, charCount);
    return true;
  }

  return false;
}