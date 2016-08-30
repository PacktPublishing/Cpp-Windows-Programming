class Parser {
  public:
    Parser(String buffer);
    Tree<TreeNode>* Parse();
    void Match(int tokenId);
    Tree<TreeNode>* Expression();
    Tree<TreeNode>* NextExpression(Tree<TreeNode>* leftTermPtr);
    Tree<TreeNode>* Term();
    Tree<TreeNode>* NextTerm(Tree<TreeNode>* leftFactorPtr);
    Tree<TreeNode>* Factor();

  private:
    list<Token> tokenList;
};