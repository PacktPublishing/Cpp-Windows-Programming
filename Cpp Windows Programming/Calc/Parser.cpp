#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"
#include "TreeNode.h"
#include "Parser.h"

Parser::Parser(String buffer) {
  Scanner scanner(buffer);
  tokenList = scanner.Scan();
}

Tree<TreeNode>* Parser::Parse() {
  Tree<TreeNode>* resultTreePtr = Expression();
  Match(EndOfLine);
  return resultTreePtr;
}

void Parser::Match(int tokenId) {
  if (tokenList.empty() || (tokenList.front().Id() != tokenId)) {
    throw Error(SyntaxError);
  }

  tokenList.pop_front();
}

Tree<TreeNode>* Parser::Expression() {
  Tree<TreeNode>* termTreePtr = Term();
  return NextExpression(termTreePtr);
}

Tree<TreeNode>* Parser::NextExpression(Tree<TreeNode>*
                                       leftTermTreePtr) {
  Token token = tokenList.front();

  switch (token.Id()) {
    case Plus: {
        Match(Plus);
        Tree<TreeNode>* rightTermTreePtr = Term();
        Tree<TreeNode>* sumTreePtr =
          new Tree<TreeNode>(TreeNode(BinaryAdd),
                             {leftTermTreePtr, rightTermTreePtr});
        assert(sumTreePtr != nullptr);
        return NextExpression(sumTreePtr);
      }

    case Minus: {
        Match(Minus);
        Tree<TreeNode>* rightTermTreePtr = Term();
        Tree<TreeNode>* diffTreePtr =
          new Tree<TreeNode>(TreeNode(BinarySubtract),
                             {leftTermTreePtr, rightTermTreePtr});
        assert(diffTreePtr != nullptr);
        return NextExpression(diffTreePtr);
      }

    default:
      return leftTermTreePtr;
  }
}

Tree<TreeNode>* Parser::Term() {
  Tree<TreeNode>* pFactorTree = Factor();
  return NextTerm(pFactorTree);
}

Tree<TreeNode>* Parser::NextTerm(Tree<TreeNode>*leftFactorTreePtr) {
  Token token = tokenList.front();

  switch (token.Id()) {
    case Star: {
        Match(Star);
        Tree<TreeNode>* rightFactorTreePtr = Factor();
        Tree<TreeNode>* productTreePtr =
          new Tree<TreeNode>(TreeNode(Multiply),
                         {leftFactorTreePtr, rightFactorTreePtr});
        assert(productTreePtr != nullptr);
        return NextExpression(productTreePtr);
      }

    case Slash: {
        Match(Slash);
        Tree<TreeNode>* rightFactorTreePtr = Factor();
        Tree<TreeNode>* quotientTreePtr =
          new Tree<TreeNode>(TreeNode(Divide),
                         {leftFactorTreePtr, rightFactorTreePtr});
        assert(quotientTreePtr != nullptr);
        return NextExpression(quotientTreePtr);
      }
  
    default:
      return leftFactorTreePtr;
  }
}

Tree<TreeNode>* Parser::Factor() {
  Token token = tokenList.front();

  switch (token.Id()) {
    case Plus: {
        Match(Plus);
        Tree<TreeNode>* nextExprTreePtr = Expression();
        Tree<TreeNode>* plusTreePtr =
          new Tree<TreeNode>(TreeNode(UnaryAdd),
                             {nextExprTreePtr});
        assert(plusTreePtr!= nullptr);
        return plusTreePtr;
      }

    case Minus: {
        Match(Minus);
        Tree<TreeNode>* nextExprTreePtr = Expression();
        Tree<TreeNode>* minusTreePtr =
          new Tree<TreeNode>(TreeNode(UnaryAdd),
                             {nextExprTreePtr});
        assert(minusTreePtr!= nullptr);
        return minusTreePtr;
      }

    case LeftParenthesis: {
        Match(LeftParenthesis);
        Tree<TreeNode>* innerExprTreePtr = Expression();
        Match(RightParenthesis);
        Tree<TreeNode>* resultTreePtr = 
          new Tree<TreeNode>(TreeNode(Parenthesis),
                             {innerExprTreePtr});
        assert(resultTreePtr != nullptr);
        return resultTreePtr;
    }

    case RefToken: {
        Match(RefToken);
        Tree<TreeNode>* resultTreePtr =
          new Tree<TreeNode>(TreeNode(token.ReferenceField()));
        assert(resultTreePtr != nullptr);
        return resultTreePtr;
    }

    case Number: {
        Match(Number);
        Tree<TreeNode>* resultTreePtr =
          new Tree<TreeNode>(TreeNode(token.Value()));
        assert(resultTreePtr != nullptr);
        return resultTreePtr;
    }

    default:
      throw Error(SyntaxError);
  }
}