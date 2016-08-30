#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"
#include "TreeNode.h"
#include "Parser.h"
#include "Cell.h"

const int CellMargin = 100,
          ColWidth = 4000, RowHeight = 1000,
          HeaderWidth = 1000, HeaderHeight = 700;

Cell::Cell()
 :cellMode(TextMode),
  font(SystemFont),
  backgroundColor(White),
  horizontalAlignignment(Center),
  verticalAlignignment(Center),
  hasValue(false),
  value(0),
  syntaxTreePtr(nullptr) {
  // Empty.
}

Cell::Cell(const Cell& cell)
 :cellMode(cell.cellMode),
  font(cell.font),
  backgroundColor(cell.backgroundColor),
  horizontalAlignignment(cell.horizontalAlignignment),
  verticalAlignignment(cell.verticalAlignignment),
  text(cell.text),
  caretList(cell.caretList),
  hasValue(cell.hasValue),
  value(cell.value) {
  if (cell.syntaxTreePtr != nullptr) {
    syntaxTreePtr = new Tree<TreeNode>(*cell.syntaxTreePtr);
    assert(syntaxTreePtr != nullptr);
  }
  else {
    syntaxTreePtr = nullptr;
  }
}

Cell& Cell::operator=(const Cell& cell)  {
  if (this != &cell) {

    cellMode = cell.cellMode;
    font = cell.font;
    backgroundColor = cell.backgroundColor;
    horizontalAlignignment = cell.horizontalAlignignment;
    verticalAlignignment = cell.verticalAlignignment;
    text = cell.text;
    caretList = cell.caretList;
    hasValue = cell.hasValue;
    value = cell.value;
    delete syntaxTreePtr;

    if (cell.syntaxTreePtr != nullptr) {
      syntaxTreePtr = new Tree<TreeNode>(*cell.syntaxTreePtr);
      assert(syntaxTreePtr != nullptr);
    }
    else {
      syntaxTreePtr = nullptr;
    }
  }

  return *this;
}

Cell::~Cell() {
  delete syntaxTreePtr;
}

void Cell::Clear() {
  font = SystemFont;
  backgroundColor = White;
  horizontalAlignignment = Center;
  verticalAlignignment = Center;
  Reset();
}

void Cell::Reset() {
  cellMode = TextMode;
  text.clear();
  caretList.clear();
  delete syntaxTreePtr;
  syntaxTreePtr = nullptr;
}

void Cell::CharDown(int editIndex, TCHAR tChar,
                    KeyboardMode keyboardMode) {
  if (editIndex == text.length()) {
    text.append(1, tChar);
  }
  else {
    switch (keyboardMode) {
      case InsertKeyboard:
        text.insert(editIndex, 1, tChar);
        break;

      case OverwriteKeyboard:
        text[editIndex] = tChar;
        break;            
    }
  }
}

int Cell::MouseToIndex(int x) const {
  x -= CellMargin;

  if (x < caretList[0].Left()) {
    return 0;
  }

  int size = text.length();
  for (int index = 0; index < size; ++index) {
    if (x < caretList[index].Right()) {
      return index;
    }
  }

  return size;
}

void Cell::DrawCell(Graphics& graphics, Reference cellRef,
                    bool inverse) const {
  Point topLeft(HeaderWidth + cellRef.Col() * ColWidth,
                HeaderHeight + cellRef.Row() * RowHeight);
  Size cellSize(ColWidth, RowHeight);
  Rect cellRect(topLeft, cellSize);

  Color textColor = font.FontColor(),
        backColor = backgroundColor, borderColor = Black;

  if (inverse) {
    textColor = textColor.Inverse();
    backColor = backColor.Inverse();
    borderColor = borderColor.Inverse();
  }

  graphics.FillRectangle(cellRect, borderColor, backColor);
  Size marginSize(CellMargin, CellMargin);
  int size = text.length();

  for (int index = 0; index < size; ++index) {
    TCHAR tChar = text[index];
    Rect caretRect = caretList[index];

    Rect charRect = (topLeft + marginSize) + caretRect;
    TCHAR text[] = {tChar, TEXT('\0')};
    graphics.DrawText(charRect, text, font, textColor, backColor);
  }
}

void Cell::GenerateCaretList(Window* windowPtr) {
  vector<int> widthList;
  int textWidth = 0, spaceCount = 0, noSpaceWidth = 0;

  for (const TCHAR tChar : text) {
    int charWidth = windowPtr->GetCharacterWidth(font, tChar);
    widthList.push_back(charWidth);
    textWidth += charWidth;

    if (horizontalAlignignment == Justified) {
      if (tChar == TEXT(' ')) {
        ++spaceCount;
      }
      else {
        noSpaceWidth += charWidth;
      }
    }
  }

  int startPos = 0, spaceWidth,
      cellWidth = ColWidth - (3 * CellMargin);

  switch (horizontalAlignignment) {
    case Left:
      startPos = CellMargin;
      break;

    case Justified: {
        if (spaceCount > 0) {
          spaceWidth = max(0,(cellWidth-noSpaceWidth)/spaceCount);
        }
        else {
          startPos = CellMargin;
        }
      }
      break;

    case Right:
      startPos = CellMargin + max(0, cellWidth - textWidth);
      break;

    case Center:
      startPos = CellMargin + max(0, (cellWidth - textWidth) / 2);
      break;
  }

  int topPos = 0,
      textHeight = windowPtr->GetCharacterHeight(font),
      cellHeight = RowHeight - (3 * CellMargin);

  switch (verticalAlignignment) {
    case Top:
      topPos = CellMargin;
      break;

    case Bottom:
      topPos = CellMargin + max(0, cellHeight - textHeight);
      break;


    case Center:
      topPos = CellMargin + max(0, (cellHeight - textHeight) / 2);
      break;
  }

  caretList.clear();
  int size = text.size();
  for (int index = 0; index < size; ++index) {
    int charWidth = widthList[index];

    if ((horizontalAlignignment == Justified) &&
        (text[index] == TEXT(' '))) {
      charWidth = spaceWidth;
    }

    Point topLeft(startPos, topPos);
    Size charSize(charWidth, textHeight);
    caretList.push_back(Rect(topLeft, charSize));
    startPos += charWidth;
  }

  Point topLeft(startPos, topPos);
  int averageWidth = windowPtr->GetCharacterAverageWidth(font);
  Size charSize(averageWidth, textHeight);
  caretList.push_back(Rect(topLeft, charSize));
}

void Cell::DisplayFormula() {
  switch (cellMode) {
    case TextMode:
    case ValueMode:
      break;

    case FormulaMode:
      text = TEXT("=") + TreeToString(syntaxTreePtr);
      break;
  }
}

void Cell::InterpretCell(set<Reference>& sourceSet) {
  String trimText = Trim(text);

 if (IsNumeric(trimText)) {
    cellMode = ValueMode;
    text = trimText;
    value = stod(text);
  }
  else if (!trimText.empty() && (trimText[0] == TEXT('='))) {
    cellMode = FormulaMode;
    Parser parser(trimText.substr(1));
    syntaxTreePtr = parser.Parse();
    GenerateSourceSet(syntaxTreePtr, sourceSet);
  }
  else {
    cellMode = TextMode;
  }
}

void Cell::GenerateSourceSet(set<Reference>& sourceSet) const {
  if (cellMode == FormulaMode) {
    GenerateSourceSet(syntaxTreePtr, sourceSet);
  }
}

void Cell::GenerateSourceSet(Tree<TreeNode>* syntaxNodePtr,
                             set<Reference>& sourceSet) const {
  DynamicList<Tree<TreeNode>*> childList =
    syntaxNodePtr->ChildList();

  switch (syntaxNodePtr->NodeValue().Id()) {
    case UnaryAdd:
    case UnarySubtract:
    case Parenthesis:
      GenerateSourceSet(childList[0], sourceSet);
      break;

    case BinaryAdd:
    case BinarySubtract:
    case Multiply:
    case Divide:
      GenerateSourceSet(childList[0], sourceSet),
      GenerateSourceSet(childList[1], sourceSet);
      break;

    case RefId: {
        Reference sourceRef =
          syntaxNodePtr->NodeValue().ReferenceField();

        if ((sourceRef.Row() >= 0) && (sourceRef.Row() < Rows) &&
            (sourceRef.Col() >= 0) && (sourceRef.Col() < Cols)) {
          sourceSet.insert(sourceRef);
        }
      }
      break;

    case ValueId:
      break;
  }
}

String Cell::TreeToString() const {
  if (cellMode == FormulaMode) {
    return TEXT("=") + TreeToString(syntaxTreePtr);
  }
  else {
    return text;
  }
}

String Cell::TreeToString(Tree<TreeNode>* syntaxNodePtr) const {
  DynamicList<Tree<TreeNode>*> childList =
    syntaxNodePtr->ChildList();

  switch (syntaxNodePtr->NodeValue().Id()) {
    case UnaryAdd:
      return TEXT("+") + TreeToString(childList[0]);

    case UnarySubtract:
      return TEXT("-") + TreeToString(childList[0]);
      break;

    case BinaryAdd:
      return TreeToString(childList[0]) + TEXT("+") +
             TreeToString(childList[1]);

    case BinarySubtract:
      return TreeToString(childList[0]) + TEXT("-") +
             TreeToString(childList[1]);

    case Multiply:
      return TreeToString(childList[0]) + TEXT("*") +
             TreeToString(childList[1]);

    case Divide:
      return TreeToString(childList[0]) + TEXT("/") +
             TreeToString(childList[1]);

    case Parenthesis:
      return TEXT("(") + TreeToString(childList[0]) + TEXT(")");

    case RefId:
      return syntaxNodePtr->
        NodeValue().ReferenceField().ToString();

    case ValueId:
      return to_String(syntaxNodePtr->NodeValue().Value());
  }

  assert(false);
  return TEXT("");
}

void Cell::UpdateTree(Reference diffRef, set<Reference>&sourceSet){
  if (cellMode == FormulaMode) {
    UpdateTree(syntaxTreePtr, diffRef, sourceSet);
  }
}

void Cell::UpdateTree(Tree<TreeNode>* syntaxNodePtr,
                 Reference diffRef, set<Reference>& sourceSet) {
  if (syntaxNodePtr->NodeValue().Id() == RefId) {
    syntaxNodePtr->NodeValue().ReferenceField() += diffRef;
    sourceSet.insert(syntaxNodePtr->NodeValue().ReferenceField());
  }
  else {
    for (Tree<TreeNode>* childNodePtr :
         syntaxNodePtr->ChildList()) {
      UpdateTree(childNodePtr, diffRef, sourceSet);
    }
  }
}

bool Cell::HasValue() const {
  switch (cellMode) {
    case TextMode:
      return false;

    case ValueMode:
      return true;

    case FormulaMode:
      return hasValue;
  }

  assert(false);
  return false;
}

void Cell::Evaluate(map<Reference,double>& valueMap) {
  if (cellMode == FormulaMode) {
    try {
      value = Evaluate(syntaxTreePtr, valueMap);
      text = to_String(value);
      hasValue = true;
    }
    catch (Error error) {
      text = error.ErrorText();
      hasValue = false;
    }
  }
}

double Cell::Evaluate(Tree<TreeNode>* syntaxNodePtr,
                      map<Reference,double>& valueMap) {
  DynamicList<Tree<TreeNode>*> childList =
    syntaxNodePtr->ChildList();

  switch (syntaxNodePtr->NodeValue().Id()) {
    case UnaryAdd:
      return Evaluate(childList[0], valueMap);

    case UnarySubtract:
      return -Evaluate(childList[0], valueMap);

    case BinaryAdd:
      return Evaluate(childList[0], valueMap) +
             Evaluate(childList[1], valueMap);

    case BinarySubtract:
      return Evaluate(childList[0], valueMap) -
             Evaluate(childList[1], valueMap);

    case Multiply:
      return Evaluate(childList[0], valueMap) *
             Evaluate(childList[1], valueMap);

    case Divide: {
        double remainder = Evaluate(childList[1], valueMap);
      
        if (remainder != 0) {
          return Evaluate(childList[0], valueMap) / remainder;
        }
        else {
          throw Error(DivisionByZero);
        }
      }
      break;

    case Parenthesis:
      return Evaluate(childList[0], valueMap);

    case RefId: {
        Reference sourceRef =
          syntaxNodePtr->NodeValue().ReferenceField();

        if ((sourceRef.Row() >= 0) && (sourceRef.Row() < Rows) &&
            (sourceRef.Col() >= 0) && (sourceRef.Col() < Cols)) {

          if (valueMap.find(sourceRef) != valueMap.end()) {
            return valueMap[sourceRef];
          }
          else {
            throw Error(MissingValue);
          }
        }
        else {
          throw Error(ReferenceOutOfRange);
        }
      }
      break;

    case ValueId:
      return syntaxNodePtr->NodeValue().Value();
  }

  assert(false);
  return 0;
}

bool Cell::WriteCellToStream(ostream& outStream) const {
  outStream.write((char*) &cellMode, sizeof cellMode);
  outStream.write((char*) &horizontalAlignignment,
                  sizeof horizontalAlignignment);
  outStream.write((char*) &verticalAlignignment,
                  sizeof verticalAlignignment);
  outStream.write((char*) &hasValue, sizeof hasValue);
  outStream.write((char*) &value, sizeof value);

  backgroundColor.WriteColorToStream(outStream);
  font.WriteFontToStream(outStream);

  if (caretList.size() != (text.size() + 1)) {
    int i = 1;
  }

  int charListSize = text.size();
  outStream.write((char*) &charListSize, sizeof charListSize);

  for (const TCHAR tChar : text) {
    outStream.write((char*) &tChar, sizeof tChar);
  }

/*  for (const Rect caretRect : caretList) {
    caretRect.WriteRectToStream(outStream);
  }*/

  if (cellMode == FormulaMode) {
    syntaxTreePtr->WriteTreeToStream(outStream);
  }

  return ((bool) outStream);
}

bool Cell::ReadCellFromStream(istream& inStream) {
  inStream.read((char*) &cellMode, sizeof cellMode);
  inStream.read((char*) &horizontalAlignignment,
                sizeof horizontalAlignignment);
  inStream.read((char*) &verticalAlignignment,
                sizeof verticalAlignignment);
  inStream.read((char*) &hasValue, sizeof hasValue);
  inStream.read((char*) &value, sizeof value);

  backgroundColor.ReadColorFromStream(inStream);
  font.ReadFontFromStream(inStream);

  int charListSize;
  inStream.read((char*) &charListSize, sizeof charListSize);

  for (int count = 0; count < charListSize; ++count) {
    TCHAR tChar;
    inStream.read((char*) &tChar, sizeof tChar);
    text.append(1, tChar);
  }

/*  int caretListSize = charListSize + 1;
  for (int count = 0; count < caretListSize; ++count) {
    Rect caretRect;
    caretRect.ReadRectFromStream(inStream);
    caretList.push_back(caretRect);
  }*/

  if (cellMode == FormulaMode) {
    syntaxTreePtr = new Tree<TreeNode>();
    assert(syntaxTreePtr != nullptr);
    syntaxTreePtr->ReadTreeFromStream(inStream);
  }
  else {
    syntaxTreePtr = nullptr;
  }

  return ((bool) inStream);
}

void Cell::WriteCellToClipboard(InfoList& infoList) const {
  infoList.AddValue<CellMode>(cellMode);
  infoList.AddValue<Alignment>(horizontalAlignignment);
  infoList.AddValue<Alignment>(verticalAlignignment);
  infoList.AddValue<double>(value);
  infoList.AddValue<bool>(hasValue);

  font.WriteFontToClipboard(infoList);
  backgroundColor.WriteColorToClipboard(infoList);

  infoList.AddValue<int>(text.size());
  for (const TCHAR tChar : text) {
    infoList.AddValue<TCHAR>(tChar);
  }

  for (const Rect caretRect : caretList) {
    caretRect.WriteRectToClipboard(infoList);
  }

  if (cellMode == FormulaMode) {
    syntaxTreePtr->WriteTreeToClipboard(infoList);
  }
}

void Cell::ReadCellFromClipboard(InfoList& infoList) {
  infoList.GetValue<CellMode>(cellMode);
  infoList.GetValue<Alignment>(horizontalAlignignment);
  infoList.GetValue<Alignment>(verticalAlignignment);
  infoList.GetValue<double>(value);
  infoList.GetValue<bool>(hasValue);

  font.ReadFontFromClipboard(infoList);
  backgroundColor.ReadColorFromClipboard(infoList);

  int listSize;
  infoList.GetValue<int>(listSize);

  for (int count = 0; count < listSize; ++count) {
    TCHAR tChar;
    infoList.GetValue<TCHAR>(tChar);
    text.append(1, tChar);
  }

  for (int count = 0; count < (listSize + 1); ++count) {
    Rect caretRect;
    caretRect.ReadRectFromClipboard(infoList);
    caretList.push_back(caretRect);
  }

  if (cellMode == FormulaMode) {
    syntaxTreePtr = new Tree<TreeNode>();
    assert(syntaxTreePtr != nullptr);
    syntaxTreePtr->ReadTreeFromClipboard(infoList);
  }
  else {
    syntaxTreePtr = nullptr;
  }
}
