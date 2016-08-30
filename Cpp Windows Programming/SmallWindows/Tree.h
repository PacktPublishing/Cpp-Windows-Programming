namespace SmallWindows {
  template <class NodeType>
  class Tree {
    public:
      Tree();
      Tree(NodeType nodeValue,
           initializer_list<Tree<NodeType>*> childList = {});
      Tree(const Tree& tree);
      Tree& operator=(const Tree& tree);
      void Init(const Tree& tree);
      ~Tree();

      bool WriteTreeToStream(ostream& outStream) const;
      bool ReadTreeFromStream(istream& inStream);

      void WriteTreeToClipboard(InfoList& infoList) const;
      void ReadTreeFromClipboard(InfoList& infoList);

      NodeType NodeValue() const {return nodeValue;}
      NodeType& NodeValue() {return nodeValue;}

      const DynamicList<Tree*>& ChildList() const
                                            {return childList;}
      DynamicList<Tree*>& ChildList() {return childList;}

    private:
      NodeType nodeValue;
      DynamicList<Tree*> childList;
  };


  template <class NodeType>
  Tree<NodeType>::Tree() {
    // Empty.
  }

  template <class NodeType>
  Tree<NodeType>::Tree(NodeType nodeValue,
           initializer_list<Tree<NodeType>*> childList /* = {} */)
   :nodeValue(nodeValue) {
    for (Tree<NodeType>* childNodePtr : childList) {
      this->childList.PushBack(childNodePtr);
    }
  }

  template <class NodeType>
  Tree<NodeType>::Tree(const Tree& tree) {
    Init(tree);
  }

  template <class NodeType>
  Tree<NodeType>& Tree<NodeType>::operator=(const Tree& tree) {
    if (this != &tree) {
      Init(tree);
    }

    return *this;
  }

  template <class NodeType>
  void Tree<NodeType>::Init(const Tree& tree) {
    nodeValue = tree.nodeValue;

    for (Tree* childPtr : tree.childList) {
      Tree* childClonePtr = new Tree(*childPtr);
      assert(childClonePtr != nullptr);
      childList.PushBack(childClonePtr);
    }
  }

  template <class NodeType>
  Tree<NodeType>::~Tree() {
    for (Tree* childPtr : childList) {
      delete childPtr;
    }
  }

/*  3; 6    4; 7    = a1 + a2; = b1 + b2 + 1.000000*/

  template <class NodeType>
  bool Tree<NodeType>::WriteTreeToStream(ostream& outStream)const{
    nodeValue.WriteTreeNodeToStream(outStream);

    int childListSize = childList.Size();
    outStream.write((char*) &childListSize, sizeof childListSize);

    for (Tree* childPtr : childList) {
      childPtr->WriteTreeToStream(outStream);
    }

    return ((bool) outStream);
  }

  template <class NodeType>
  bool Tree<NodeType>::ReadTreeFromStream(istream& inStream) {
    nodeValue.ReadTreeNodeFromStream(inStream);

    int childListSize;
    inStream.read((char*) &childListSize, sizeof childListSize);

    for (int count = 0; count < childListSize; ++count) {
      Tree* childPtr = new Tree();
      assert(childPtr != nullptr);
      childPtr->ReadTreeFromStream(inStream);
      childList.PushBack(childPtr);
    }

    return ((bool) inStream);
  }

  template <class NodeType>
  void Tree<NodeType>::WriteTreeToClipboard(InfoList& infoList)
                                            const {
    nodeValue.WriteTreeNodeToClipboard(infoList);
    infoList.AddValue<int>(childList.Size());

    for (Tree* childPtr : childList) {
      childPtr->WriteTreeToClipboard(infoList);
    }
  }

  template <class NodeType>
  void Tree<NodeType>::ReadTreeFromClipboard(InfoList& infoList) {
    nodeValue.ReadTreeNodeFromClipboard(infoList);

    int childListSize;
    infoList.GetValue<int>(childListSize);

    for (int count = 0; count < childListSize; ++count) {
      Tree* childPtr = new Tree();
      assert(childPtr != nullptr);
      childPtr->ReadTreeFromClipboard(infoList);
      childList.PushBack(childPtr);
    }
  }
};
