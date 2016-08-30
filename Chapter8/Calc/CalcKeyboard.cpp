#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"
#include "TreeNode.h"
#include "Parser.h"
#include "Cell.h"
#include "CalcDocument.h"

void CalcDocument::OnChar(TCHAR tChar) {
  if (calcMode == Mark) {
    calcMode = Edit;
    editRef = firstMarkRef;
    Remark(editRef, editRef);
    editIndex = 0;
    Cell& editCell = cellMatrix[editRef];
    prevCell = editCell;
    editCell.Reset();
  }

  MakeCellVisible(editRef);
  Cell& cell = cellMatrix[editRef];
  cell.CharDown(editIndex++, tChar, GetKeyboardMode());
  cell.GenerateCaretList(this);
  InvalidateCell(editRef);
  UpdateCaret();
  UpdateWindow();
}

bool CalcDocument::OnKeyDown(WORD key, bool shiftPressed,
                             bool controlPressed) {
  switch (key) {
    case KeyLeft:
      OnLeftArrowKey(shiftPressed);
      break;

    case KeyRight:
      OnRightArrowKey(shiftPressed);
      break;

    case KeyUp:
      OnUpArrowKey(shiftPressed);
      break;

    case KeyDown:
      OnDownArrowKey(shiftPressed);
      break;

    case KeyHome:
      OnHomeKey(shiftPressed, controlPressed);
      break;

    case KeyEnd:
      OnEndKey(shiftPressed, controlPressed);
      break;

    case KeyReturn:
      OnReturnKey();
      break;

    case KeyTabulator:
      OnTabulatorKey(shiftPressed);
      break;

    case KeyEscape:
      OnEscapeKey();
      break;

    case KeyDelete:
      OnDeleteKey();
      break;

    case KeyBackspace:
      OnBackspaceKey();
      break;
  }

  UpdateCaret();
  UpdateWindow();
  return true;
}

void CalcDocument::OnLeftArrowKey(bool shiftPressed) {
  switch (calcMode) {
    case Edit: {
        MakeCellVisible(editRef);
        if (editIndex > 0) {
          --editIndex;
        }
      }
      break;

    case Mark:
      if (lastMarkRef.Col() > 0) {
        if (!shiftPressed) {
          Reference newLastMarkRef(lastMarkRef.Row(),
                                   lastMarkRef.Col() - 1);
          MakeCellVisible(newLastMarkRef);
          Remark(newLastMarkRef, newLastMarkRef);
        }
        else {
          Reference newLastRefMark(lastMarkRef.Row(),
                                   lastMarkRef.Col() - 1);
          MakeCellVisible(newLastRefMark);
          Remark(firstMarkRef, newLastRefMark);
        }
      }
      break;
  }
}

void CalcDocument::OnRightArrowKey(bool shiftPressed) {
  switch (calcMode) {
    case Edit: {
        MakeCellVisible(editRef);

        if (editIndex <
            ((int) cellMatrix[editRef].GetText().length())) {
          ++editIndex;
        }
      }
      break;

    case Mark:
      if (lastMarkRef.Col() < (Cols - 1)) {
        if (!shiftPressed) {
          Reference newLastMarkRef(lastMarkRef.Row(),
                                   lastMarkRef.Col() + 1);
          MakeCellVisible(newLastMarkRef);
          Remark(newLastMarkRef, newLastMarkRef);
        }
        else {
          Reference newLastRefMark(lastMarkRef.Row(),
                                   lastMarkRef.Col() + 1);
          MakeCellVisible(newLastRefMark);
          Remark(firstMarkRef, newLastRefMark);
        }
      }
      break;
  }
}

void CalcDocument::OnUpArrowKey(bool shiftPressed) {
  switch (calcMode) {
    case Edit:
      break;

    case Mark:
      if (lastMarkRef.Row() > 0) {
        if (!shiftPressed) {
          Reference newLastMarkRef(lastMarkRef.Row() - 1,
                                   lastMarkRef.Col());
          MakeCellVisible(newLastMarkRef);
          Remark(newLastMarkRef, newLastMarkRef);
        }
        else {
          Reference newLastRefMark(lastMarkRef.Row() - 1,
                                   lastMarkRef.Col());
          MakeCellVisible(newLastRefMark);
          Remark(firstMarkRef, newLastRefMark);
        }
      }
      break;
  }
}

void CalcDocument::OnDownArrowKey(bool shiftPressed) {
  switch (calcMode) {
    case Edit:
      break;

    case Mark:
      if (lastMarkRef.Row() < (Rows - 1)) {
        if (!shiftPressed) {
          Reference newMarkRef(lastMarkRef.Row() + 1,
                               lastMarkRef.Col());
          MakeCellVisible(newMarkRef);
          Remark(newMarkRef, newMarkRef);
        }
        else {
          Reference newLastRefMark(lastMarkRef.Row() + 1,
                                   lastMarkRef.Col());
          MakeCellVisible(newLastRefMark);
          Remark(firstMarkRef, newLastRefMark);
        }
      }
      break;
  }
}

void CalcDocument::OnHomeKey(bool shiftPressed,
                             bool controlPressed) {
  switch (calcMode) {
    case Edit: {
        MakeCellVisible(editRef);
        editIndex = 0;
        UpdateCaret();
      }
      break;

    case Mark:
      if (!shiftPressed && !controlPressed) {
        Remark(Reference(firstMarkRef.Row(), 0),
               Reference(firstMarkRef.Row(), 0));
        MakeCellVisible(firstMarkRef);
      }
      else if (shiftPressed && !controlPressed) {
        Remark(firstMarkRef, Reference(firstMarkRef.Row(), 0));
        MakeCellVisible(lastMarkRef);
      }
      else if (!shiftPressed && controlPressed) {
        Remark(ZeroReference, ZeroReference);
        MakeCellVisible(lastMarkRef);
      }
      else if (shiftPressed && controlPressed) {
        Remark(firstMarkRef, ZeroReference);
        MakeCellVisible(lastMarkRef);
      }
    break;
  }
}

void CalcDocument::OnEndKey(bool shiftPressed, bool controlPressed) {
  switch (calcMode) {
    case Edit: {
        MakeCellVisible(editRef);
        editIndex = cellMatrix[editRef].GetText().length();
        UpdateCaret();
      }
      break;

    case Mark:
      if (!shiftPressed && !controlPressed) {
        Remark(Reference(firstMarkRef.Row(), Cols - 1),
               Reference(firstMarkRef.Row(), Cols - 1));
        MakeCellVisible(firstMarkRef);
      }
      else if (shiftPressed && !controlPressed) {
        Remark(firstMarkRef,
               Reference(firstMarkRef.Row(), Cols - 1));
        MakeCellVisible(lastMarkRef);
      }
      else if (!shiftPressed && controlPressed) {
        Remark(Reference(Rows - 1, Cols - 1),
               Reference(Rows - 1, Cols - 1));
        MakeCellVisible(lastMarkRef);
      }
      else if (shiftPressed && controlPressed) {
        Remark(firstMarkRef, Reference(Rows - 1, Cols - 1));
        MakeCellVisible(lastMarkRef);
      }
      break;
  }
}

void CalcDocument::OnReturnKey() {
  if ((calcMode == Mark) || ToMarkMode()) {
    Reference newMarkedRef(min(firstMarkRef.Row() + 1, Rows - 1),
                           firstMarkRef.Col());
    Remark(newMarkedRef, newMarkedRef);
    MakeCellVisible(newMarkedRef);
  }
}

void CalcDocument::OnTabulatorKey(bool shiftPressed) {
  if ((calcMode == Mark) || ToMarkMode()) {
    if (shiftPressed && (lastMarkRef.Col() > 0)) {
      Reference firstMarkRef(lastMarkRef.Row(),
                             firstMarkRef.Col() - 1);
      Remark(firstMarkRef, firstMarkRef);
      MakeCellVisible(firstMarkRef);
    }
    
    if (!shiftPressed && (lastMarkRef.Col() < (Cols - 1))) {
      Reference firstMarkRef(firstMarkRef.Row(),
                             firstMarkRef.Col() + 1);
      Remark(firstMarkRef, firstMarkRef);
      MakeCellVisible(firstMarkRef);
    }
  }
}

void CalcDocument::OnEscapeKey() {
  if (calcMode == Edit) {
    Cell& editCell = cellMatrix[editRef];
    editCell = prevCell;
    InvalidateCell(editRef);
    calcMode = Mark;
    firstMarkRef = lastMarkRef = editRef;
  }
}

void CalcDocument::OnDeleteKey() {
  switch (calcMode) {
    case Edit: {
      Cell& editCell = cellMatrix[editRef];
      String& cellText = editCell.GetText();

      if (editIndex < ((int) cellText.length())) {
        String leftPart = cellText.substr(0, editIndex),
               rightPart = cellText.substr(editIndex + 1);
        editCell.SetText(leftPart + rightPart);
        editCell.GenerateCaretList(this);
        InvalidateCell(editRef);
        UpdateWindow();
        SetDirty(true);
      }
    }
    break;

  case Mark: {
      int minMarkRow = min(firstMarkRef.Row(), lastMarkRef.Row()),
          minMarkCol = min(firstMarkRef.Col(), lastMarkRef.Col()),
          maxMarkRow = max(firstMarkRef.Row(), lastMarkRef.Row()),
          maxMarkCol = max(firstMarkRef.Col(), lastMarkRef.Col());

      set<Reference> invalidateSet;
      for (int row = minMarkRow; row <= maxMarkRow; ++row) {
        for (int col = minMarkCol; col <= maxMarkCol; ++col) {
          Reference cellRef = Reference(row, col);
          cellMatrix[cellRef].Reset();
          invalidateSet.insert(cellRef);
          EvaluateRecursive(editRef, invalidateSet);
        }
      }

      for (Reference cellRef : invalidateSet) {
        InvalidateCell(cellRef);
      }

      UpdateWindow();
      SetDirty(true);
    }
    break;
  }
}

void CalcDocument::OnBackspaceKey() {
  switch (calcMode) {
    case Edit:
      if (editIndex > 0) {
        --editIndex;
        OnDeleteKey();
      }
      break;

    case Mark:
      OnDeleteKey();
      break;
  }
}