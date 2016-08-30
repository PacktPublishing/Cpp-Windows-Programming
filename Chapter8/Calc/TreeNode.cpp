#include "..\\SmallWindows\\SmallWindows.h"
#include "TreeNode.h"

TreeNode::TreeNode()
 :id(EmptyTree),
  value(0) {
  // Empty.
}

TreeNode::TreeNode(TreeId id)
 :id(id),
  value(0) {
    // Empty.  
}

TreeNode::TreeNode(Reference reference)
: id(RefId),
  value(0),
  reference(reference) {
  // Empty.
}

TreeNode::TreeNode(double value)
 :id(ValueId),
  value(value) {
  // Empty.
}

bool TreeNode::WriteTreeNodeToStream(ostream& outStream) const {
  outStream.write((char*) &id, sizeof id);
  outStream.write((char*) &value, sizeof value);
  reference.WriteReferenceToStream(outStream);
  return ((bool) outStream);
}

bool TreeNode::ReadTreeNodeFromStream(istream& inStream) {
  inStream.read((char*) &id, sizeof id);
  inStream.read((char*) &value, sizeof value);
  reference.ReadReferenceFromStream(inStream);
  return ((bool) inStream);
}

void TreeNode::WriteTreeNodeToClipboard(InfoList& infoList) const {
  infoList.AddValue<TreeId>(id);
  infoList.AddValue<double>(value);
  reference.WriteReferenceToClipboard(infoList);
}

void TreeNode::ReadTreeNodeFromClipboard(InfoList& infoList) {
  infoList.GetValue<TreeId>(id);
  infoList.GetValue<double>(value);
  reference.ReadReferenceFromClipboard(infoList); 
}
