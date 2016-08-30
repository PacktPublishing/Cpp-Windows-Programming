#include "..\\SmallWindows\\SmallWindows.h"
#include "DrawFigure.h"
#include "LineFigure.h"
#include "ArrowFigure.h"
#include "RectangleFigure.h"
#include "EllipseFigure.h"
#include "DrawDocument.h"

DrawDocument::DrawDocument(WindowShow windowShow)
 :StandardDocument(LogicalWithScroll, USLetterPortrait,
                   TEXT("Draw Files, drw"), nullptr,
                   OverlappedWindow, windowShow,
                   {DrawFormat}, {DrawFormat}) {
  Menu menuBar(this);
  menuBar.AddMenu(StandardFileMenu(false));
  menuBar.AddMenu(StandardEditMenu());

  Menu formatMenu(this, TEXT("F&ormat"));
  formatMenu.AddItem(TEXT("&Modify\tCtrl+M"), OnModify,
                     ModifyEnable, nullptr, ModifyRadio);
  formatMenu.AddItem(TEXT("&Color\tAlt+C"), OnColor, ColorEnable);
  formatMenu.AddItem(TEXT("F&ill\tCtrl+I"), OnFill, FillEnable,
                     FillCheck, nullptr);
  menuBar.AddMenu(formatMenu);

  Menu addMenu(this, TEXT("&Add"));
  addMenu.AddItem(TEXT("&Line\tCtrl+L"), OnLine,
                  LineEnable, nullptr, LineRadio);
  addMenu.AddItem(TEXT("&Arrow\tAlt+A"), OnArrow,
                  ArrowEnable, nullptr, ArrowRadio);
  addMenu.AddItem(TEXT("&Rectangle\tCtrl+R"), OnRectangle,
                  RectangleEnable, nullptr, RectangleRadio);
  addMenu.AddItem(TEXT("&Ellipse\tCtrl+E"), OnEllipse,
                  EllipseEnable, nullptr, EllipseRadio);
  menuBar.AddMenu(addMenu);

  menuBar.AddMenu(StandardHelpMenu());
  SetMenuBar(menuBar);

/*  Menu formatMenu(this);
  formatMenu.AddItem(TEXT("&Modify\tCtrl+M"), OnModify,
                     ModifyEnable, nullptr, ModifyRadio);
  formatMenu.AddItem(TEXT("F&ill\tCtrl+I"), OnFill,
                     FillEnable, nullptr, FillCheck);
  formatMenu.AddItem(TEXT("F&ill\tCtrl+I"), OnFill, FillEnable);
  AddToStandardMenuBar(formatMenu, TEXT("F&ormat"));

  Menu addMenu(this);
  addMenu.AddItem(TEXT("&Line\tCtrl+L"), OnLine,
                  LineEnable, nullptr, LineRadio);
  addMenu.AddItem(TEXT("&Arrow\tAlt+A"), OnArrow,
                  ArrowEnable, nullptr, ArrowRadio);
  addMenu.AddItem(TEXT("&Rectangle\tCtrl+R"), OnRectangle,
                  RectangleEnable, nullptr, RectangleRadio);
  addMenu.AddItem(TEXT("&Ellipse\tCtrl+E"), OnEllipse,
                  EllipseEnable, nullptr, EllipseRadio);
  AddToStandardMenuBar(addMenu, TEXT("&Add"));
*/
  actionMode = (ActionMode)
              Registry::ReadInteger(TEXT("actionMode"), Modify);
  addFigureId = (FigureId)
              Registry::ReadInteger(TEXT("addFigureId"), LineId);
  nextColor.ReadColorFromRegistry(TEXT("nextColor"));
  nextFill = Registry::ReadBoolean(TEXT("nextFill"), false);
}

DrawDocument::~DrawDocument() {
  Registry::WriteInteger(TEXT("actionMode"), actionMode);
  Registry::WriteInteger(TEXT("addFigureId "), addFigureId);
  nextColor.WriteColorToRegistry(TEXT("nextColor"));
  Registry::WriteBoolean(TEXT("nextFill"), nextFill);
}

auto IsFigureMarked = [](DrawFigure* const& figurePtr,
                         void* /* voidPtr */) {
  return figurePtr->IsMarked();
};

bool IsFigureClicked(DrawFigure* const& figurePtr, void* voidPtr) {
  Point* mousePointPtr = (Point*) voidPtr;
  return figurePtr->IsClick(*mousePointPtr);
}

void UnmarkFigure(DrawFigure*& figurePtr, void* /* voidPtr */) {
  if (figurePtr->IsMarked()) {
    figurePtr->Mark(false);
  }
}

void DrawDocument::OnMouseDown(MouseButton mouseButtons,
                     Point mousePoint, bool shiftPressed,
                     bool controlPressed) {
  if (mouseButtons == LeftButton) {
    prevMousePoint = mousePoint;

    switch (actionMode) {
      case Modify: {
          DrawFigure* topClickedFigurePtr;

          if (figurePtrList.FirstOf(IsFigureClicked,
                               topClickedFigurePtr, &mousePoint)) {
            if (!controlPressed) {
              applicationMode = ModifySingle;

              figurePtrList.ApplyIf(IsFigureMarked, UnmarkFigure);
              int topFigureIndex =
                figurePtrList.IndexOf(topClickedFigurePtr);
              figurePtrList.Erase(topFigureIndex);
              figurePtrList.PushFront(topClickedFigurePtr);
              topClickedFigurePtr->Mark(true);
            }
            else {
              if (topClickedFigurePtr->IsMarked()) {
                applicationMode = Idle;
                topClickedFigurePtr->Mark(false);
              }
              else {
                applicationMode = MoveMultiple;
                topClickedFigurePtr->Mark(true);
              }
            }
          }
          else {
            figurePtrList.ApplyIf(IsFigureMarked, UnmarkFigure);
            insideRectangle = Rect(mousePoint, mousePoint);
            applicationMode = ModifyRectangle;
          }
        }
        break;

      case Add: {
          DrawFigure* newFigurePtr = CreateFigure(addFigureId);
          newFigurePtr->SetColor(nextColor);
          newFigurePtr->Fill(nextFill);
          newFigurePtr->SetFirstPoint(mousePoint);
          SetDirty(true);
          figurePtrList.PushFront(newFigurePtr);
          applicationMode = ModifySingle;
        }
        break;
    }

    UpdateWindow();
    UpdateCursor();
  }
}

void MoveMarkedFigure(DrawFigure*& figurePtr, void* voidPtr) {
  if (figurePtr->IsMarked()) {
    figurePtr->Invalidate();
    Size* distanzeSizePtr = (Size*)voidPtr;
    figurePtr->Move(*distanzeSizePtr);
    figurePtr->Invalidate();
  }
}

void DrawDocument::OnMouseMove(MouseButton mouseButtons,
                               Point mousePoint,bool shiftPressed,
                               bool controlPressed) {
  if (mouseButtons == LeftButton) {
    Size distanceSize = mousePoint - prevMousePoint;
    prevMousePoint = mousePoint;

    switch (applicationMode) {
      case ModifySingle:
        figurePtrList[0]->Modify(distanceSize);
        SetDirty(true);
        break;

      case MoveMultiple:
        figurePtrList.Apply(MoveMarkedFigure, &distanceSize);
        SetDirty(true);
        break;

      case ModifyRectangle:
        Invalidate(insideRectangle);
        insideRectangle.SetBottomRight(mousePoint);
        Invalidate(insideRectangle);
        UpdateWindow();
        break;
    }

    UpdateWindow();
    UpdateCursor();
  }
}

bool IsFigureInside(DrawFigure* const& figurePtr, void* voidPtr) {
  Rect* insideRectanglePtr = (Rect*) voidPtr;
  bool b = figurePtr->IsInside(*insideRectanglePtr);
  return b;
}

void MarkFigure(DrawFigure*& figurePtr, void* /* voidPtr */) {
  figurePtr->Mark(true);
}

void DrawDocument::OnMouseUp(MouseButton mouseButtons,
                             Point mousePoint, bool shiftPressed,
                             bool controlPressed) {
  if (mouseButtons == LeftButton) {
    switch (applicationMode) {
      case ModifyRectangle: {
          insideRectangle.Normalize();
          DynamicList<DrawFigure*> insideList;
          figurePtrList.CopyIf(IsFigureInside, insideList,
                                 &insideRectangle);
          figurePtrList.RemoveIf(IsFigureInside,
                                 &insideRectangle);
          figurePtrList.PushFront(insideList);
          insideList.Apply(MarkFigure);
          Invalidate(insideRectangle);
          insideRectangle.Clear();
          UpdateWindow();
        }
        break;
    }

    applicationMode = Idle;
  }
}

void DrawDocument::OnDoubleClick(MouseButton mouseButtons,
                           Point mousePoint, bool shiftPressed,
                           bool controlPressed) {
  if ((mouseButtons == LeftButton) && !controlPressed) {
    DrawFigure* topClickedFigurePtr;

    if (figurePtrList.FirstOf(IsFigureClicked,topClickedFigurePtr, 
                              &mousePoint)) {
      topClickedFigurePtr->DoubleClick(mousePoint);
    }
  }
}

void DrawDocument::OnDraw(Graphics& graphics,
                          DrawMode /* drawMode */) const {
  int size = figurePtrList.Size();
  for (int index = (size - 1); index >= 0; --index) {
    DrawFigure* figurePtr = figurePtrList[index];
    figurePtr->Draw(graphics);
  }

  if (applicationMode == ModifyRectangle) {
    graphics.DrawRectangle(insideRectangle, Gray);
  }
}

void DrawDocument::ClearDocument() {
  for (DrawFigure* figurePtr : figurePtrList) {
    delete figurePtr;
  }

  figurePtrList.Clear();
}

bool DrawDocument::WriteDocumentToStream(String name,
                                         ostream& outStream)const{
  int listSize = figurePtrList.Size();
  outStream.write((char*) &listSize, sizeof listSize);

  for (DrawFigure* figurePtr : figurePtrList) {
    FigureId figureId = figurePtr->GetId();
    outStream.write((char*) &figureId, sizeof figureId);
    figurePtr->WriteFigureToStream(outStream);
  }

  return ((bool) outStream);
}

bool DrawDocument::ReadDocumentFromStream(String name,
                                          istream& inStream) {
  int listSize;
  inStream.read((char*) &listSize, sizeof listSize);

  for (int index = 0; index < listSize; ++index) {
    FigureId figureId;
    inStream.read((char*) &figureId, sizeof figureId);

    DrawFigure* figurePtr = CreateFigure(figureId);
    figurePtr->ReadFigureFromStream(inStream);

    figurePtrList.PushBack(figurePtr);
  }

  return ((bool) inStream);
}

DrawFigure* DrawDocument::CreateFigure(FigureId figureId) const {
  switch (figureId) {
    case LineId:
      return (new LineFigure(this));

    case ArrowId:
      return (new ArrowFigure(this));

    case RectangleId:
      return (new RectangleFigure(this));

    case EllipseId:
      return (new EllipseFigure(this));
  }

  return nullptr;
}

bool DrawDocument::CopyEnable() const {
  if (applicationMode == Idle) {
    switch (actionMode) {
      case Modify:
        return figurePtrList.AnyOf(IsFigureMarked);

      case Add:
        return false;
    }
  }

  return false;
}

bool DrawDocument::IsCopyGenericReady(int /* format */) const {
  return true;
}

void DrawDocument::CopyGeneric(int format, InfoList& infoList)
                               const {
  DynamicList<DrawFigure*> markedList;
  figurePtrList.CopyIf(IsFigureMarked, markedList);
  infoList.AddValue<int>(markedList.Size());

  for (DrawFigure* figurePtr : markedList) {
    infoList.AddValue<FigureId>(figurePtr->GetId());
    figurePtr->WriteFigureToClipboard(infoList);
  }
}

void DrawDocument::PasteGeneric(int format, InfoList& infoList) {
  figurePtrList.ApplyIf(IsFigureMarked, UnmarkFigure);

  int pasteSize;
  infoList.GetValue<int>(pasteSize);

  for (int count = 0; count < pasteSize; ++count) {
    FigureId figureId;
    infoList.GetValue<FigureId>(figureId);

    DrawFigure* figurePtr = CreateFigure(figureId);
    figurePtr->ReadFigureFromClipboard(infoList);
    figurePtr->Move(Size(1000, 1000));
    figurePtrList.PushBack(figurePtr);

    figurePtr->Mark(true);
  }

  UpdateWindow();
}

void DeleteFigure(DrawFigure*& figurePtr, void* /* voidPtr */) {
  figurePtr->Invalidate();
  delete figurePtr;
}

void DrawDocument::OnDelete() {
  figurePtrList.ApplyRemoveIf(IsFigureMarked, DeleteFigure,
                              nullptr, this);
  UpdateWindow();
  SetDirty(true);
}

bool DrawDocument::ModifyEnable() const {
  return (applicationMode == Idle);
}

bool DrawDocument::ModifyRadio() const {
  return ((applicationMode == Idle) && (actionMode == Modify));
}

void DrawDocument::OnModify() {
  actionMode = Modify;
}

bool DrawDocument::ColorEnable() const {
  if (applicationMode == Idle) {
    switch (actionMode) {
      case Modify:
        return figurePtrList.AnyOf(IsFigureMarked);

      case Add:
        return true;
    }
  }

  return false;
}

void SetFigureColor(DrawFigure*& figurePtr, void* voidPtr) {
  Color* colorPtr = (Color*) voidPtr;

  if (figurePtr->IsMarked() &&
      (figurePtr->GetColor() != *colorPtr)) {
    figurePtr->SetColor(*colorPtr);
  }
}

void DrawDocument::OnColor() {
  switch (actionMode) {
    case Modify: {
        DynamicList<DrawFigure*> markedList;
        figurePtrList.CopyIf(IsFigureMarked, markedList);
        DrawFigure* topFigurePtr = markedList[0];
        Color topColor = topFigurePtr->GetColor();

        if (StandardDialog::ColorDialog(this, topColor)) {
          nextColor = topColor;
          figurePtrList.ApplyIf(IsFigureMarked, SetFigureColor,
                                nullptr, &topColor);
          UpdateWindow();
          SetDirty(true);
        }
      }
      break;

    case Add:
      StandardDialog::ColorDialog(this, nextColor);
      break;
  }
}

bool IsFigureMarkedAndFilled(DrawFigure* const& figurePtr,
                             void* /* voidPtr */) {
  return (figurePtr->IsMarked() && figurePtr->IsFilled());
}

bool DrawDocument::FillCheck() const {
  if (applicationMode == Idle) {
    switch (actionMode) {
      case Modify:
        return figurePtrList.AnyOf(IsFigureMarkedAndFilled);

      case Add:
        return nextFill;
    }
  }

  return false;
}

bool IsFigureMarkedAndFillable(DrawFigure* const& figurePtr,
                               void* /* voidPtr */){
  return (figurePtr->IsMarked() && figurePtr->IsFillable());
}

bool DrawDocument::FillEnable() const {
  if (applicationMode == Idle) {
    switch (actionMode) {
      case Modify:
        return figurePtrList.AnyOf(IsFigureMarkedAndFillable);

      case Add: {
          DrawFigure* addFigurePtr = CreateFigure(addFigureId);
          bool fillable = addFigurePtr->IsFillable();
          delete addFigurePtr;
          return fillable;
        }
    }
  }

  return false;
}

void InverseFill(DrawFigure*& figurePtr, void* /* voidPtr */) {
  if (figurePtr->IsMarked()) {
    figurePtr->Fill(!figurePtr->IsFilled());
  }
}

void DrawDocument::OnFill() {
  switch (actionMode) {
    case Modify:
      figurePtrList.ApplyIf(IsFigureMarked, InverseFill);
      UpdateWindow();
      break;

    case Add:
      nextFill = !nextFill;
      break;
  }
}

bool DrawDocument::LineEnable() const {
  return (applicationMode == Idle);
}

bool DrawDocument::ArrowEnable() const {
  return (applicationMode == Idle);
}

bool DrawDocument::RectangleEnable() const {
  return (applicationMode == Idle);
}

bool DrawDocument::EllipseEnable() const {
  return (applicationMode == Idle);
}

bool DrawDocument::LineRadio() const {
  return ((actionMode == Add) && (addFigureId == LineId));
}

bool DrawDocument::ArrowRadio() const {
  return ((actionMode == Add) && (addFigureId == ArrowId));
}

bool DrawDocument::RectangleRadio() const {
  return ((actionMode == Add) && (addFigureId == RectangleId));
}

bool DrawDocument::EllipseRadio() const {
  return ((actionMode == Add) && (addFigureId == EllipseId));
}

void DrawDocument::OnLine() {
  actionMode = Add;
  addFigureId = LineId;
}

void DrawDocument::OnArrow() {
  actionMode = Add;
  addFigureId = ArrowId;
}

void DrawDocument::OnRectangle() {
  actionMode = Add;
  addFigureId = RectangleId;
}

void DrawDocument::OnEllipse() {
  actionMode = Add;
  addFigureId = EllipseId;
}

void DrawDocument::UpdateCursor() {
  switch (applicationMode) {
    case Idle:
      Cursor::Set(Cursor::Arrow);
      break;

    case ModifyRectangle:
      Cursor::Set(Cursor::Crosshair);
      break;

    case MoveMultiple:
      Cursor::Set(Cursor::SizeAll);
      break;

    case ModifySingle:
      Cursor::Set(figurePtrList[0]->GetCursor());
      break;
  }
}
