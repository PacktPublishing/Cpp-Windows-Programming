#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"
#include "TreeNode.h"
#include "Parser.h"
#include "Cell.h"
#include "CalcDocument.h"

void CalcDocument::ClearDocument() {
  for (int row = 0; row < Rows; ++row) {
    for (int col = 0; col < Cols; ++col) {
      cellMatrix[Reference(row, col)].Clear();
    }
  }

  sourceSetMap.clear();
  targetSetMap.clear();

  calcMode = Mark;
  firstMarkRef.Clear();
  lastMarkRef.Clear();
}

void WriteCellToStream(Cell cell, ostream& outStream) {
  cell.WriteCellToStream(outStream);
}

void ReadCellFromStream(Cell& cell, istream& inStream) {
  cell.ReadCellFromStream(inStream);
}

bool CalcDocument::WriteDocumentToStream(String name,
                                         ostream& outStream)const{
  if (EndsWith(name, TEXT(".clc"))) {
    outStream.write((char*) &calcMode, sizeof calcMode);
    outStream.write((char*) &editIndex, sizeof editIndex);
    editRef.WriteReferenceToStream(outStream);
    firstMarkRef.WriteReferenceToStream(outStream);
    lastMarkRef.WriteReferenceToStream(outStream);
    WriteSetMapToStream(sourceSetMap, outStream);
    WriteSetMapToStream(targetSetMap, outStream);

    for (int row = 0; row < Rows; ++row) {
      for (int col = 0; col < Cols; ++col) {
        cellMatrix[row][col].WriteCellToStream(outStream);
      }
    }
  }
  else if (EndsWith(name, TEXT(".txt"))) {
    for (int row = 0; row < Rows; ++row) {
      if (row > 0) {
        char newline = '\n';
        outStream.write(&newline, sizeof newline);
      }

      for (int col = 0; col < Cols; ++col) {
        if (col > 0) {
          char semicolon = ';';
          outStream.write(&semicolon, sizeof semicolon);
        }

        const Cell& cell = cellMatrix[row][col];
        String text = cell.IsFormula()
                      ? (TEXT("=") + cell.TreeToString())
                      : cell.GetText();

        for (TCHAR tChar : text) {
          char c = (char) tChar;
          outStream.write(&c, sizeof c);
        }
      }
    }
  }

  return ((bool) outStream);
}

bool CalcDocument::ReadDocumentFromStream(String name,
                                          istream& inStream) {
  if (EndsWith(name, TEXT(".clc"))) {
    inStream.read((char*)&calcMode, sizeof calcMode);
    inStream.read((char*) &editIndex, sizeof editIndex);
    editRef.ReadReferenceFromStream(inStream);
    firstMarkRef.ReadReferenceFromStream(inStream);
    lastMarkRef.ReadReferenceFromStream(inStream);
    ReadSetMapFromStream(sourceSetMap, inStream);
    ReadSetMapFromStream(targetSetMap, inStream);
    MakeCellVisible();

    for (int row = 0; row < Rows; ++row) {
      for (int col = 0; col < Cols; ++col) {
        Cell& cell = cellMatrix[Reference(row, col)];
        cell.ReadCellFromStream(inStream);
        cell.GenerateCaretList(this);
      }
    }
  }  
  else if (EndsWith(name, TEXT(".txt"))) {
    String text;
    int row = 0, col = 0;

    while (inStream) {
      char c;
      inStream.read(&c, sizeof c);

      if (inStream) {
        switch (c) {
          case ';':
            cellMatrix[Reference(row, col++)].SetText(text);
            text.clear();
            break;

          case '\n':
            cellMatrix[Reference(row++, col)].SetText(text);
            text.clear();
            col = 0;
            break;

          default:
            text.push_back((TCHAR) c);
            break;
        }
      }
    }
  }

  bool b = (bool) inStream;
  return ((bool) inStream);
}

bool CalcDocument::WriteSetMapToStream(const
                   map<Reference,set<Reference>>& setMap,
                   ostream& outStream) {
  int mapSize = setMap.size();
  outStream.write((char*) &mapSize, sizeof mapSize);

  for (pair<Reference,set<Reference>> entry : setMap) {
    Reference cellRef = entry.first;
    cellRef.WriteReferenceToStream(outStream);

    set<Reference> set = entry.second;
    int setSize = set.size();
    outStream.write((char*) &setSize, sizeof setSize);

    for (Reference ref : set) {
      ref.WriteReferenceToStream(outStream);
    }
  }

  return ((bool) outStream);
}

bool CalcDocument::ReadSetMapFromStream
                   (map<Reference,set<Reference>>& setMap,
                    istream& inStream) {
  int mapSize;
  inStream.read((char*) &mapSize, sizeof mapSize);

  for (int mapIndex = 0; mapIndex < mapSize; ++mapIndex) {
    Reference cellRef;
    cellRef.ReadReferenceFromStream(inStream);

    int setSize;
    inStream.read((char*) &setSize, sizeof setSize);

    set<Reference> set;
    for (int setIndex = 0; setIndex < setSize; ++setIndex) {
      Reference ref;
      ref.ReadReferenceFromStream(inStream);
      set.insert(ref);
    }

    setMap[cellRef] = set;
  }

  return ((bool) inStream);
}

bool CalcDocument::CopyEnable() const {
  return (calcMode == Mark);
}

void CalcDocument::CopyAscii(vector<String>& textList) const {
  CopyUnicode(textList);
}

void CalcDocument::CopyUnicode(vector<String>& textList) const {
  int minMarkRow = min(firstMarkRef.Row(), lastMarkRef.Row()),
      maxMarkRow = max(firstMarkRef.Row(), lastMarkRef.Row()),
      minMarkCol = min(firstMarkRef.Col(), lastMarkRef.Col()),
      maxMarkCol = max(firstMarkRef.Col(), lastMarkRef.Col());

  for (int row = minMarkRow; row <= maxMarkRow; ++row) {
    String text;

    for (int col = minMarkCol; col <= maxMarkCol; ++col) {
      Reference markRef = Reference(row, col);
      const Cell& markCell = cellMatrix[markRef];
      text.append(((col > 0) ? TEXT(";") : TEXT("")) +
                  markCell.TreeToString());
    }

    textList.push_back(text);
  }
}

void CalcDocument::CopyGeneric(int /* format */,
                               InfoList& infoList) const {
  int minRow = min(firstMarkRef.Row(), lastMarkRef.Row()),
      minCol = min(firstMarkRef.Col(), lastMarkRef.Col()),
      copyRows = abs(firstMarkRef.Row() - lastMarkRef.Row()) + 1,
      copyCols = abs(firstMarkRef.Col() - lastMarkRef.Col()) + 1;

  infoList.AddValue<int>(minRow);
  infoList.AddValue<int>(minCol);
  infoList.AddValue<int>(copyRows);
  infoList.AddValue<int>(copyCols);

  for (int row = 0; row < copyRows; ++row) {
    for (int col = 0; col < copyCols; ++col) {
      Reference sourceRef(minRow + row, minCol + col);
      const Cell& cell = cellMatrix[sourceRef];
      cell.WriteCellToClipboard(infoList);
    }
  }
}

bool CalcDocument::IsPasteAsciiReady
                   (const vector<String>& textList) const {
  return IsPasteUnicodeReady(textList);
}

bool CalcDocument::IsPasteUnicodeReady
                   (const vector<String>& textList) const {
  int markedRows = abs(firstMarkRef.Row() - lastMarkRef.Row()) +1,
      markedCols = abs(firstMarkRef.Col() - lastMarkRef.Col()) +1,
      minMarkedRow = min(firstMarkRef.Row(), lastMarkRef.Row()),
      minMarkedCol = min(firstMarkRef.Col(), lastMarkRef.Col());

  if ((markedRows == 1) && (markedCols == 1)) {
    int copyRows = textList.size();
    int maxCopyCols = 0;

    for (String text : textList) {
      maxCopyCols = max(maxCopyCols,
                        ((int) Split(text, ';').size()));
    }

    return ((minMarkedRow + copyRows) < Rows) &&
           ((minMarkedCol + maxCopyCols) < Cols);
  }
  else {
    if (textList.size() != markedRows) {
      return false;
    }

    for (String text : textList) {
      if (((int) Split(text, ';').size()) != markedCols) {
        return false;
      }
    }

    return true;
  }
}

bool CalcDocument::IsPasteGenericReady(int /* format */,
                                       InfoList& infoList) const {
  int markedRows = abs(firstMarkRef.Row() - lastMarkRef.Row()) +1,
      markedCols = abs(firstMarkRef.Col() - lastMarkRef.Col()) +1,
      minMarkedRow = min(firstMarkRef.Row(), lastMarkRef.Row()),
      minMarkedCol = min(firstMarkRef.Col(), lastMarkRef.Col()),
      copyRows, copyCols;

  infoList.PeekValue<int>(copyRows, 0);
  infoList.PeekValue<int>(copyCols, sizeof(int));

  return (((markedRows == copyRows)&&(markedCols == copyCols)) ||
          ((markedRows == 1) && (markedCols == 1))) &&
         ((minMarkedRow + copyRows) <= Rows) &&
         ((minMarkedCol + copyCols) <= Cols);
}

void CalcDocument::PasteAscii(const vector<String>& textList) {
  PasteUnicode(textList);
}

void CalcDocument::PasteUnicode(const vector<String>& textList) {
  Matrix<Rows,Cols,Cell> backupMatrix =
    Matrix<Rows,Cols,Cell>(cellMatrix);
  map<Reference,set<Reference>> backupSourceSetMap = sourceSetMap,
                                backupTargetSetMap = targetSetMap;

  try {
    set<Reference> invalidateSet;
    int row = min(firstMarkRef.Row(), lastMarkRef.Row()),
        minCol = min(firstMarkRef.Col(), lastMarkRef.Col());
    Reference diffRef(row, minCol);

    for (String rowText : textList) {
      int col = minCol;
      vector<String> columnList = Split(rowText, ';');

      for (String colText : columnList) {
        Reference targetRef(row, col++);
        RemoveTargetSetMap(targetRef);
        Cell& targetCell = cellMatrix[targetRef];
        targetCell.Reset();
        targetCell.SetText(colText);
        set<Reference> sourceSet;
        targetCell.InterpretCell(sourceSet);
        targetCell.GenerateCaretList(this);

        if (!diffRef.IsEmpty()) {
          sourceSet.clear();
          targetCell.UpdateTree(diffRef, sourceSet);
        }

        AddTargetSetMap(targetRef, sourceSet);
        sourceSetMap[targetRef] = sourceSet;
        EvaluateRecursive(targetRef, invalidateSet);
        targetCell.GenerateCaretList(this);
      }
    
      ++row;
    }

    for (Reference cellRef : invalidateSet) {
      InvalidateCell(cellRef);
    }
  }
  catch (Error error) {
    cellMatrix = backupMatrix;
    sourceSetMap = backupSourceSetMap;
    targetSetMap = backupTargetSetMap;
    MessageBox(error.ErrorText(), TEXT("Syntax Error"), Ok, Stop);
  }
}

void CalcDocument::PasteGeneric(int /* format */,
                                InfoList& infoList) {
  int minMarkedRow = min(firstMarkRef.Row(), lastMarkRef.Row()),
      minMarkedCol = min(firstMarkRef.Col(), lastMarkRef.Col()),
      copyRows, copyCols, minCopyRow, minCopyCol;

  infoList.GetValue<int>(minCopyRow);
  infoList.GetValue<int>(minCopyCol);
  infoList.GetValue<int>(copyRows);
  infoList.GetValue<int>(copyCols);

  Reference diffRef(minMarkedRow - minCopyRow,
                    minMarkedCol - minCopyCol);
  int maxCopyRow = minCopyRow + copyRows - 1,
      maxCopyCol = minCopyCol + copyCols - 1;

  for (int row = minCopyRow; row <= maxCopyRow; ++row) {
    for (int col = minCopyCol; col <= maxCopyCol; ++col) {
      Cell pastedCell;
      pastedCell.ReadCellFromClipboard(infoList);

      Reference pastedRef(row, col);
      Reference targetRef = pastedRef + diffRef;

      RemoveTargetSetMap(targetRef);
      Cell& targetCell = cellMatrix[targetRef];
      targetCell = pastedCell;

      set<Reference> sourceSet;
      if (diffRef.IsEmpty()) {
        targetCell.GenerateSourceSet(sourceSet);
      }
      else {
        targetCell.UpdateTree(diffRef, sourceSet);
      }

      AddTargetSetMap(targetRef, sourceSet);
      sourceSetMap[targetRef] = sourceSet;

      set<Reference> invalidateSet;
      EvaluateRecursive(targetRef, invalidateSet);

      for (Reference cellRef : invalidateSet) {
        InvalidateCell(cellRef);
      }
    }
  }

  UpdateWindow();
  SetDirty(true);
}

bool CalcDocument::DeleteEnable() const {
  if (calcMode == Edit) {
    const Cell& editCell = cellMatrix[editRef];
    return (editIndex < ((int)editCell.GetText().length()));
  }
  else {
    return true;
  }
}

void CalcDocument::OnDelete() {
  OnDeleteKey();
}

void CalcDocument::OnFont() {
  switch (calcMode) {
    case Edit: {
        Cell& editCell = cellMatrix[editRef];
        Font font = editCell.CellFont();
        Font previousFont = font;

        if (StandardDialog::FontDialog(this, font) &&
            (font != previousFont)) {
          editCell.CellFont() = font;
          editCell.GenerateCaretList(this);
          InvalidateCell(editRef);
          SetDirty(true);
          UpdateCaret();
          UpdateWindow();
        }
      }
      break;

    case Mark: {
        Font font = cellMatrix[lastMarkRef].CellFont();

        if (StandardDialog::FontDialog(this, font)) {
          int minMarkRow = min(firstMarkRef.Row(),
                               lastMarkRef.Row()),
              maxMarkRow = max(firstMarkRef.Row(),
                               lastMarkRef.Row()),
              minMarkCol = min(firstMarkRef.Col(),
                               lastMarkRef.Col()),
              maxMarkCol = max(firstMarkRef.Col(),
                               lastMarkRef.Col());

          for (int row = minMarkRow; row <= maxMarkRow; ++row) {
            for (int col = minMarkCol; col <= maxMarkCol; ++col) {
              Reference markRef = Reference(row, col);
              Cell& markCell = cellMatrix[markRef];

              if (markCell.CellFont() != font) {
                markCell.CellFont() = font;
                markCell.GenerateCaretList(this);
                InvalidateCell(markRef);
                SetDirty(true);
              }
            }
          }

          UpdateWindow();
        }
      }
      break;
  }
}

void CalcDocument::OnBackgroundColor() {
  switch (calcMode) {
    case Edit: {
        Cell& editCell = cellMatrix[editRef];
        Color color = editCell.BackgroundColor();
        Color previousColor = color;

        if (StandardDialog::ColorDialog(this, color) &&
            (color != previousColor)){
          editCell.BackgroundColor() = color;
          InvalidateCell(editRef);
          SetDirty(true);
        }
      }
      break;

    case Mark: {
        Color color = cellMatrix[lastMarkRef].BackgroundColor();

        if (StandardDialog::ColorDialog(this, color)) {
              int minMarkRow = min(firstMarkRef.Row(),
                               lastMarkRef.Row()),
              maxMarkRow = max(firstMarkRef.Row(),
                               lastMarkRef.Row()),
              minMarkCol = min(firstMarkRef.Col(),
                               lastMarkRef.Col()),
              maxMarkCol = max(firstMarkRef.Col(),
                               lastMarkRef.Col());

          for (int row = minMarkRow; row <= maxMarkRow; ++row) {
            for (int col = minMarkCol; col <= maxMarkCol; ++col) {
              Reference markRef = Reference(row, col);
              Cell& markCell = cellMatrix[markRef];

              if (markCell.BackgroundColor() != color) {
                markCell.BackgroundColor() = color;
                InvalidateCell(markRef);
                SetDirty(true);
              }
            }
          }
        }
      }
      break;
  }

  UpdateWindow();
}

bool CalcDocument::HorizontalLeftRadio() const {
  return (IsHorizontalAlignment(Left));
}

bool CalcDocument::HorizontalCenterRadio() const {
  return (IsHorizontalAlignment(Center));
}

bool CalcDocument::HorizontalRightRadio() const {
  return (IsHorizontalAlignment(Right));
}

bool CalcDocument::HorizontalJustifiedRadio() const {
  return (IsHorizontalAlignment(Justified));
}

bool CalcDocument::IsHorizontalAlignment(Alignment alignment)
                                         const {
  switch (calcMode) {
    case Edit:
      return cellMatrix[editRef].HorizontalAlignment() ==
             alignment;

    case Mark: {
        int minMarkRow = min(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            maxMarkRow = max(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            minMarkCol = min(firstMarkRef.Col(),
                             lastMarkRef.Col()),
            maxMarkCol = max(firstMarkRef.Col(),
                             lastMarkRef.Col());

        for (int row = minMarkRow; row <= maxMarkRow; ++row) {
          for (int col = minMarkCol; col <= maxMarkCol; ++col) {
            Reference markRef = Reference(row, col);
            auto x = cellMatrix[markRef];
            if (cellMatrix[markRef].VerticalAlignment()!=
                alignment) {
              return false;
            }
          }
        }
      
        return true;
      }
  }

  return true;
}

void CalcDocument::OnHorizontalLeft() {
  SetHorizontalAlignment(Left);
}

void CalcDocument::OnHorizontalCenter() {
  SetHorizontalAlignment(Center);
}

void CalcDocument::OnHorizontalRight() {
  SetHorizontalAlignment(Right);
}

void CalcDocument::OnHorizontalJustified() {
  SetHorizontalAlignment(Justified);
}

void CalcDocument::SetHorizontalAlignment(Alignment alignment) {
  switch (calcMode) {
    case Edit: {
        Cell& editCell = cellMatrix[editRef];
        editCell.HorizontalAlignment() = alignment;
        editCell.GenerateCaretList(this);
        InvalidateCell(editRef);
        UpdateCaret();
      }
      break;

    case Mark: {
        int minMarkRow = min(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            maxMarkRow = max(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            minMarkCol = min(firstMarkRef.Col(),
                             lastMarkRef.Col()),
            maxMarkCol = max(firstMarkRef.Col(),
                             lastMarkRef.Col());

        for (int row = minMarkRow; row <= maxMarkRow; ++row) {
          for (int col = minMarkCol; col <= maxMarkCol; ++col) {
            Reference markRef = Reference(row, col);
            Cell& markCell = cellMatrix[markRef];

            if (markCell.HorizontalAlignment() != alignment) {
              markCell.HorizontalAlignment() = alignment;
              markCell.GenerateCaretList(this);
              InvalidateCell(markRef);
            }
          }
        }
      }
      break;
  }

  UpdateWindow();
  SetDirty(true);
}

bool CalcDocument::VerticalTopRadio() const {
  return (IsVerticalAlignment(Top));
}

bool CalcDocument::VerticalCenterRadio() const {
  return (IsVerticalAlignment(Center));
}

bool CalcDocument::VerticalBottomRadio() const {
  return (IsVerticalAlignment(Bottom));
}

bool CalcDocument::IsVerticalAlignment(Alignment alignment) const {
  switch (calcMode) {
    case Edit:
      return cellMatrix[editRef].VerticalAlignment() == alignment;

    case Mark: {
        int minMarkRow = min(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            maxMarkRow = max(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            minMarkCol = min(firstMarkRef.Col(),
                             lastMarkRef.Col()),
            maxMarkCol = max(firstMarkRef.Col(),
                             lastMarkRef.Col());

        for (int row = minMarkRow; row <= maxMarkRow; ++row) {
          for (int col = minMarkCol; col <= maxMarkCol; ++col) {
            Reference markRef = Reference(row, col);
            if (cellMatrix[markRef].VerticalAlignment() !=
                alignment){
              return false;
            }
          }
        }
    
        return true;
      }
  }

  return true;
}

void CalcDocument::OnVerticalTop() {
  SetVerticalAlignment(Top);
}

void CalcDocument::OnVerticalCenter() {
  SetVerticalAlignment(Center);
}

void CalcDocument::OnVerticalBottom() {
  SetVerticalAlignment(Bottom);
}

void CalcDocument::SetVerticalAlignment(Alignment alignment) {
  switch (calcMode) {
    case Edit: {
        Cell& editCell = cellMatrix[editRef];
        editCell.VerticalAlignment() = alignment;
        editCell.GenerateCaretList(this);
        InvalidateCell(editRef);
        UpdateCaret();
      }
      break;

    case Mark: {
        int minMarkRow = min(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            maxMarkRow = max(firstMarkRef.Row(),
                             lastMarkRef.Row()),
            minMarkCol = min(firstMarkRef.Col(),
                             lastMarkRef.Col()),
            maxMarkCol = max(firstMarkRef.Col(),
                             lastMarkRef.Col());

        for (int row = minMarkRow; row <= maxMarkRow; ++row) {
          for (int col = minMarkCol; col <= maxMarkCol; ++col) {
            Reference markRef = Reference(row, col);
            Cell& markCell = cellMatrix[markRef];

            if (markCell.VerticalAlignment() != alignment) {
              markCell.VerticalAlignment() = alignment;
              markCell.GenerateCaretList(this);
              InvalidateCell(markRef);
            }
          }
        }
      }
      break;
  }

  UpdateWindow();
  SetDirty(true);
}