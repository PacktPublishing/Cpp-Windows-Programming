enum TreeId {EmptyTree, UnaryAdd, UnarySubtract, BinaryAdd, BinarySubtract,
             Multiply, Divide, Parenthesis, RefId, ValueId};

class TreeNode {
  public:
    TreeNode();
    TreeNode(TreeId id);
    TreeNode(Reference reference);
    TreeNode(double value);

    bool WriteTreeNodeToStream(ostream& outStream) const;
    bool ReadTreeNodeFromStream(istream& inStream);

    void WriteTreeNodeToClipboard(InfoList& infoList) const;
    void ReadTreeNodeFromClipboard(InfoList& infoList);

    TreeId Id() const {return id;}
    Reference ReferenceField() const {return reference;}
    Reference& ReferenceField() {return reference;}
    double Value() const {return value;}

  private:
    TreeId id;
    Reference reference;
    double value;
};
