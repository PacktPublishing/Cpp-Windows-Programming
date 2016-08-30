#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"

Token::Token(TokenId tokenId)
 :tokenId(tokenId) {
  // Empty.
}

Token::Token(double value)
 :tokenId(Number),
  value(value) {
  // Empty.
}

Token::Token(Reference reference)
: tokenId(RefToken),
  reference(reference) {
  // Empty.
}

