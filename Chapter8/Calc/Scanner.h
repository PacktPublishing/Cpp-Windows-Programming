class Scanner {
  public:
    Scanner(String buffer);
    list<Token> Scan();
    Token NextToken();
    bool ScanValue(double& value);
    bool ScanReference(Reference& reference);

  private:
    String buffer;
};