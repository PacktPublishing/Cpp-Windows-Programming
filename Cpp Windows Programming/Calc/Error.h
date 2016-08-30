enum ErrorId {SyntaxError, CircularReference, ReferenceOutOfRange,
              DivisionByZero, MissingValue};

class Error : public exception {
  public:
    Error(ErrorId errorId);
    String ErrorText() const;

  private:
    ErrorId errorId;
};