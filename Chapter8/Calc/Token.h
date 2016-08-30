enum TokenId {Plus, Minus, Star, Slash, LeftParenthesis,
              RightParenthesis, RefToken, Number, EndOfLine};

class Token{
  public:
    Token(TokenId tokenId);
    Token(double value);
    Token(Reference reference);

    TokenId Id() const {return tokenId;}
    double Value() const {return value;}
    Reference ReferenceField() const {return reference;}

  private:
    TokenId tokenId;
    double value;
    Reference reference;
};