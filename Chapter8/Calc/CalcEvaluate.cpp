#include "..\\SmallWindows\\SmallWindows.h"
#include "Token.h"
#include "Error.h"
#include "Scanner.h"
#include "TreeNode.h"
#include "Parser.h"
#include "Cell.h"
#include "CalcDocument.h"

bool CalcDocument::InterpretEditCell() {
  try {
    Cell& editCell = cellMatrix[editRef];
    set<Reference> sourceSet;
    editCell.InterpretCell(sourceSet);
    RemoveTargetSetMap(editRef);
    AddTargetSetMap(editRef, sourceSet);
    sourceSetMap[editRef] = sourceSet;

    set<Reference> invalidateSet;
    EvaluateRecursive(editRef, invalidateSet);
    editCell.GenerateCaretList(this);

    for (Reference cellRef : invalidateSet) {
      InvalidateCell(cellRef);
    }

    SetDirty(true);
    return true;
  }
  catch (Error error) {
    MessageBox(error.ErrorText(), TEXT("Syntax Error"), Ok, Stop);
    return false;
  }
}

void CalcDocument::InvalidateCell(Reference cellRef) {
  Point topLeft(HeaderWidth + (cellRef.Col() * ColWidth),
                HeaderHeight + (cellRef.Row() * RowHeight));
  Size cellSize(ColWidth, RowHeight);
  Rect cellRect(topLeft, cellSize);
  Invalidate(cellRect);
}

void CalcDocument::EvaluateRecursive(Reference cellRef,
                           set<Reference>& invalidateSet) {
  set<Reference> targetSet, evaluatedSet;
  targetSet.insert(cellRef);

  while (!targetSet.empty()) {
    Reference targetRef = *targetSet.begin();
    targetSet.erase(targetRef);

    if (evaluatedSet.count(targetRef) == 0) {
      EvaluateCell(targetRef);
      evaluatedSet.insert(targetRef);
      invalidateSet.insert(targetRef);
      set<Reference> nextTargetSet = targetSetMap[targetRef];
      targetSet.insert(nextTargetSet.begin(),
                       nextTargetSet.end());
    }
  }
}

void CalcDocument::EvaluateCell(Reference cellRef) {
  Cell& cell = cellMatrix[cellRef];

  if (IsCircular(cellRef, targetSetMap[cellRef])) {
    cell.SetText(Error(CircularReference).ErrorText());
  }
  else {
    set<Reference> sourceSet = sourceSetMap[cellRef];
    map<Reference,double> valueMap;

    for (Reference sourceRef : sourceSet) {
      if ((sourceRef.Row() >= 0) && (sourceRef.Row() < Rows) &&
          (sourceRef.Col() >= 0) && (sourceRef.Col() < Cols)) {
        Cell& sourceCell = cellMatrix[sourceRef];

        if (sourceCell.HasValue()) {
          valueMap[sourceRef] = sourceCell.GetValue();
        }
      }
    }

    cell.Evaluate(valueMap);
  }

  cell.GenerateCaretList(this);
}

bool CalcDocument::IsCircular(Reference cellRef,
                              set<Reference>& targetSet){
  for (Reference targetRef : targetSet) {
    if ((cellRef == targetRef) ||
        IsCircular(cellRef, targetSetMap[targetRef])) {
      return true;
    }
  }

  return false;
}

void CalcDocument::RemoveTargetSetMap(Reference cellRef) {
  for (Reference sourceRef : sourceSetMap[cellRef]) {
    int row = sourceRef.Row(), col = sourceRef.Col();
    if ((row >= 0) && (row < Rows) && (col >= 0) && (col < Cols)){
      targetSetMap[sourceRef].erase(cellRef);
    }
  }
}

void CalcDocument::AddTargetSetMap(Reference cellRef,
                                   set<Reference>& sourceSet) {
  for (Reference sourceRef : sourceSet) {
    int row = sourceRef.Row(), col = sourceRef.Col();
    if ((row >= 0) && (row < Rows) && (col >= 0) && (col < Cols)){
      targetSetMap[sourceRef].insert(cellRef);
    }
  }
}
