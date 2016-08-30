#include "SmallWindows.h"

namespace SmallWindows {
  StandardDocument::StandardDocument(CoordinateSystem system,
                        Size pageSize,
                        String fileDescriptionsText,
                        Window* parentPtr /* = nullptr */,
                        WindowStyle style/* = OverlappedWindow */,
                        WindowShow windowShow /* = Normal */,
                        initializer_list<unsigned int>
                          copyFormatList /* = {} */,
                        initializer_list<unsigned int>
                          pasteFormatList /* = {}*/,
                        bool acceptDropFiles /* = true */,
                        Size lineSize /* = LineSize */)
   :Document(TEXT("standarddocument"), system, pageSize,
             parentPtr, style, windowShow,
             acceptDropFiles, lineSize),
    copyFormatList(copyFormatList),
    pasteFormatList(pasteFormatList) {
    InitializeFileFilter(fileDescriptionsText);

    SetHorizontalScrollTotalWidth(PageInnerWidth());
    SetVerticalScrollTotalHeight(PageInnerHeight());
  }

  void StandardDocument::InitializeFileFilter(String fileListText)
  {
    OStringStream filterStream;
    vector<String> fileList = Split(fileListText, TEXT(';'));
    assert(fileList.size() > 0);

    for (String fileText : fileList) {
      vector<String> partList = Split(fileText, TEXT(','));
      assert(partList.size() == 2);
      String description = Trim(partList[0]),
        suffix = Trim(partList[1]);
      fileSuffixList.push_back(suffix);
      filterStream << description << TEXT(" (*.") << suffix
        << TEXT(")\n") << TEXT("*.") << suffix
        << TEXT("\n");
    }

    filterStream << TEXT("\n");
    String s = filterStream.str();

    int index = 0;
    for (TCHAR c : filterStream.str()) {
      fileFilter[index++] = (c == TEXT('\n')) ? TEXT('\0') : c;
    }
  }

  Menu StandardDocument::StandardFileMenu(bool print) {
    Menu fileMenu(this, TEXT("&File"));
    fileMenu.AddItem(TEXT("&New\tCtrl+N"), OnNew);
    fileMenu.AddItem(TEXT("&Open\tCtrl+O"), OnOpen);
    fileMenu.AddItem(TEXT("&Save\tCtrl+S"), OnSave, SaveEnable);
    fileMenu.AddItem(TEXT("Save &As\tCtrl+Shift+S"), OnSaveAs);

    if (print) {
      fileMenu.AddSeparator();
      fileMenu.AddItem(TEXT("Page Set&up"), OnPageSetup);
      fileMenu.AddItem(TEXT("Print Pre&view"), OnPrintPreview);
      fileMenu.AddItem(TEXT("&Print\tCtrl+P"), OnPrintItem);
    }

    fileMenu.AddSeparator();
    fileMenu.AddItem(TEXT("E&xit\tAlt+X"), OnExit);
    return fileMenu;
  }

  Menu StandardDocument::StandardEditMenu() {
    Menu editMenu(this, TEXT("&Edit"));
    editMenu.AddItem(TEXT("C&ut\tCtrl+X"), OnCut, CutEnable);
    editMenu.AddItem(TEXT("&Copy\tCtrl+C"), OnCopy, CopyEnable);
    editMenu.AddItem(TEXT("&Paste\tCtrl+V"), OnPaste, PasteEnable);
    editMenu.AddSeparator();
    editMenu.AddItem(TEXT("&Delete\tDelete"),
      OnDelete, DeleteEnable);
    return editMenu;
  }

  Menu StandardDocument::StandardHelpMenu() {
    Menu helpMenu(this, TEXT("&Help"));
    helpMenu.AddItem(TEXT("About ") +
      Application::ApplicationName() +
      TEXT(" ..."), OnAbout);
    return helpMenu;
  }

  bool StandardDocument::TryClose() {
    if (IsDirty()) {
      switch (MessageBox(TEXT("Do you want to save?"),
              TEXT("Unsaved Document"), YesNoCancel)) {
      case Yes:
        OnSave();
        return !IsDirty();

      case No:
        return true;

      case Cancel:
        return false;
      }
    }

    return true;
  }

  void StandardDocument::OnExit() {
    if (TryClose()) {
      delete Application::MainWindowPtr();
    }
  }

  void StandardDocument::OnNew() {
    if (TryClose()) {
      ClearDocument();
      ClearPageSetupInfo();
      SetZoom(1.0);
      SetDirty(false);
      SetName(TEXT(""));
      Invalidate();
      UpdateWindow();
      UpdateCaret();
    }
  }

  void StandardDocument::OnOpen() {
    if (TryClose()) {
      String name = GetName();

      if (StandardDialog::OpenDialog(this, name, fileFilter,
                                     fileSuffixList)) {
        ClearDocument();
        Invalidate();
        UpdateWindow();
        ifstream inStream(name.c_str());

        if (inStream && ReadDocumentFromStream(name, inStream)) {
          SetName(name);
        }
        else {
          MessageBox(TEXT("Could not open \"") +
                     name + TEXT("\"."));
        }
      }
    }

    SetDirty(false);
    SetZoom(1.0);
    Invalidate();
    UpdateWindow();
    UpdateCaret();
  }

  bool StandardDocument::SaveEnable() const {
    return IsDirty();
  }

  void StandardDocument::OnSave() {
    String name = GetName();

    if (!name.empty()) {
      SaveFileWithName(name);
    }
    else {
      SaveFileWithoutName();
    }
  }

  void StandardDocument::OnSaveAs() {
    SaveFileWithoutName();
  }

  void StandardDocument::SaveFileWithoutName() {
    String name = GetName();

    if (StandardDialog::SaveDialog(this, name, fileFilter,
                                   fileSuffixList)) {
      SaveFileWithName(name);
    }
  }

  void StandardDocument::SaveFileWithName(String name) {
    ofstream outStream(name.c_str());

    if (outStream && WriteDocumentToStream(name, outStream)) {
      SetName(name);
      SetDirty(false);
      SetZoom(1.0);
    }
  }

  void StandardDocument::ClearPageSetupInfo() {
    pageSetupInfo.ClearPageSetupInfo();
  }

  bool StandardDocument::ReadPageSetupInfoFromStream
                         (istream &inStream) {
    pageSetupInfo.ReadPageSetupInfoFromStream(inStream);
    bool b = (bool) inStream;
    return ((bool) inStream);
  }

  bool StandardDocument::WritePageSetupInfoToStream
                         (ostream &outStream) const {
    pageSetupInfo.WritePageSetupInfoToStream(outStream);
    return ((bool) outStream);
  }

  void StandardDocument::OnAbout() {
    String applicationName = Application::ApplicationName();
    MessageBox(applicationName + TEXT(", version 1.0"),
               applicationName, Ok, Information);
  }

  bool StandardDocument::CutEnable() const {
    return CopyEnable();
  }

  bool StandardDocument::CopyEnable() const {
    for (unsigned int format : pasteFormatList) {
      switch (format) {
      case AsciiFormat:
        if (IsCopyAsciiReady()) {
          return true;
        }
        break;

      case UnicodeFormat:
        if (IsCopyUnicodeReady()) {
          return true;
        }
        break;

      default:
        if (IsCopyGenericReady(format)) {
          return true;
        }
        break;
      }
    }

    return false;
  }

  bool StandardDocument::PasteEnable() const {
    if (Clipboard::Open(this)) {
      for (unsigned int format : pasteFormatList) {
        if (Clipboard::Available(format)) {
          switch (format) {
            case AsciiFormat: {
                vector<String> asciiList;
                if (Clipboard::ReadText<AsciiFormat,char>
                                       (asciiList) &&
                    IsPasteAsciiReady(asciiList)) {
                  Clipboard::Close();
                  return true;
                }
              }
              break;

            case UnicodeFormat: {
                vector<String> unicodeList;
                if (Clipboard::ReadText<UnicodeFormat,wchar_t>
                                       (unicodeList) &&
                    IsPasteUnicodeReady(unicodeList)) {
                  Clipboard::Close();
                  return true;
                }
              }
              break;

            default: {
                InfoList infoList;
                if (Clipboard::ReadGeneric(format, infoList) &&
                    IsPasteGenericReady(format, infoList)) {
                  Clipboard::Close();
                  return true;
                }
              }
          }
        }
      }

      Clipboard::Close();
    }

    return false;
  }

  bool StandardDocument::DeleteEnable() const {
    return CopyEnable();
  }

  void StandardDocument::OnCut() {
    OnCopy();
    OnDelete();
  }

  void StandardDocument::OnCopy() {
    if (Clipboard::Open(this)) {
      Clipboard::Clear();

      for (unsigned int format : copyFormatList) {
        switch (format) {
          case AsciiFormat:
            if (IsCopyAsciiReady()) {
              vector<String> asciiList;
              CopyAscii(asciiList);
              Clipboard::WriteText<AsciiFormat, char>(asciiList);
            }
            break;

          case UnicodeFormat:
            if (IsCopyUnicodeReady()) {
              vector<String> unicodeList;
              CopyUnicode(unicodeList);
              Clipboard::WriteText<UnicodeFormat,wchar_t>
                                  (unicodeList);
            }
            break;

          default:
            if (IsCopyGenericReady(format)) {
              InfoList infoList;
              CopyGeneric(format, infoList);
              Clipboard::WriteGeneric(format, infoList);
            }
            break;
        }
      }

      Clipboard::Close();
    }
  }

  void StandardDocument::OnPaste() {
    if (Clipboard::Open(this)) {
      for (unsigned int format : pasteFormatList) {
        bool quit = false;

        if (Clipboard::Available(format)) {
          switch (format) {
            case AsciiFormat: {
                vector<String> asciiList;
                if (Clipboard::ReadText<AsciiFormat,char>
                                       (asciiList) &&
                    IsPasteAsciiReady(asciiList)) {
                  PasteAscii(asciiList);
                  quit = true;
                }
              }
              break;

            case UnicodeFormat: {
                vector<String> unicodeList;
                if (Clipboard::ReadText<UnicodeFormat, wchar_t>
                  (unicodeList) &&
                  IsPasteUnicodeReady(unicodeList)) {
                  PasteUnicode(unicodeList);
                  quit = true;
                }
              }
              break;

            default: {
                InfoList infoList;
                if (Clipboard::ReadGeneric(format, infoList) &&
                  IsPasteGenericReady(format, infoList)) {
                  PasteGeneric(format, infoList);
                  quit = true;
                }
              }
              break;
          }

          if (quit) {
            break;
          }
        }
      }

      Clipboard::Close();
    }
  }

  void StandardDocument::OnDelete() {
    // Empty.
  }

  void StandardDocument::OnDropFile(vector<String> pathList) {
    set<String> pathSet;

    for (String path : pathList) {
      for (String suffix : fileSuffixList) {
        if (EndsWith(path, TEXT(".") + suffix)) {
          pathSet.insert(path);
          break;
        }
      }
    }

    if (pathSet.empty()) {
      MessageBox(TEXT("No suitable dropped file."),
        TEXT("Drop File"), Ok, Stop);
    }
    else if (pathSet.size() > 1) {
      MessageBox(TEXT("To many suitable dropped files."),
        TEXT("Drop File"), Ok, Stop);
    }
    else {
      String path = *pathSet.begin();

      if (TryClose()) {
        ClearDocument();
        ReadDocumentFromStream(path, ifstream(path));
        SetName(path);
        SetDirty(false);
        SetZoom(1.0);
        Invalidate();
        UpdateWindow();
        UpdateCaret();
      }
    }
  }

  Size StandardDocument::PageOuterSize() const {
    if (pageSetupInfo.GetOrientation() == Landscape) {
      return Size(pageSize.Height(), pageSize.Width());
    }

    return pageSize;
  }

  Size StandardDocument::PageInnerSize() const {
    Size outerSize = PageOuterSize();

    int innerWidth = outerSize.Width() -
                     (100 * (pageSetupInfo.LeftMargin() +
                     pageSetupInfo.RightMargin())),
        innerHeight = outerSize.Height() -
                      (100 * (pageSetupInfo.TopMargin() +
                      pageSetupInfo.BottomMargin()));

    return Size(innerWidth, innerHeight);
  }

  void StandardDocument::OnPageSetup() {
    PageSetupDialog pageSetupDialog(this, &pageSetupInfo);

    if (pageSetupDialog.DoModal()) {
      OnPageSetup(pageSetupInfo);
    }
  }

  void StandardDocument::OnPrintPreview() {
    new PrintPreviewDocument(this, GetTotalPages());
  }

  void StandardDocument::OnPrintItem() {
    int totalPages = GetTotalPages(), firstPage, lastPage, copies;
    bool sorted;
    Graphics* graphicsPtr =
      StandardDialog::PrintDialog(this, totalPages, firstPage,
                                  lastPage, copies, sorted);

    if (graphicsPtr != nullptr) {
      static DOCINFO docInfo;
      docInfo.cbSize = sizeof docInfo;
      docInfo.lpszDocName = GetName().c_str();

      if (::StartDoc(graphicsPtr->GetDeviceContextHandle(),
        &docInfo) > 0) {
        PrepareDeviceContext
          (graphicsPtr->GetDeviceContextHandle());
        EnableWindow(false);

        if (sorted) {
          bool printOk = true;
          for (int copy = 1; (copy <= copies) && printOk; ++copy){
            for (int page = firstPage;
              (page <= lastPage) && printOk; ++page){
              printOk = PrintPage(graphicsPtr, page,
                copy, totalPages);
            }
          }
        }
        else {
          bool printOk = true;
          for (int page = firstPage;
            (page <= lastPage) && printOk; ++page) {
            for (int copy = 1; (copy <= copies) && printOk;
              ++copy) {
              printOk = PrintPage(graphicsPtr, page,
                copy, totalPages);
            }
          }
        }

        ::EndDoc(graphicsPtr->GetDeviceContextHandle());
      }
    }
  }

  bool StandardDocument::PrintPage(Graphics* graphicsPtr,
                                   int page, int copy, int totalPages){
    if (::StartPage(graphicsPtr->GetDeviceContextHandle()) > 0) {
      OnPrint(*graphicsPtr, page, copy, totalPages);
      return (::EndPage(graphicsPtr->GetDeviceContextHandle())>0);
    }

    return false;
  }

  void StandardDocument::OnPrint(Graphics& graphics, int page,
                                 int copy, int totalPages) const {
    graphics.FillRectangle(Rect(0, 0, PageOuterSize().Width(),
                           PageOuterSize().Height()), White, White);

    int left = 100 * pageSetupInfo.LeftMargin(),
        top = 100 * pageSetupInfo.TopMargin();
    int right = left + PageInnerWidth(),
        bottom = top + PageInnerHeight();

    if (!pageSetupInfo.HeaderText().empty() &&
        !((page == 1) && (!pageSetupInfo.HeaderFirst()))) {
      Rect headerRect(left, 0, right, top);
      String headerText =
        Template(this, pageSetupInfo.HeaderText(),
                 copy, page, totalPages);
      Color textColor = pageSetupInfo.HeaderFont().FontColor();
      Color backColor = textColor.Inverse();
      graphics.DrawText(headerRect, headerText,
                        pageSetupInfo.HeaderFont(), textColor, backColor);
    }

    if (!pageSetupInfo.FooterText().empty() &&
      !((page == 1) && (!pageSetupInfo.HeaderFirst()))) {
      Rect footerRect(left, bottom, right,
                      PageOuterSize().Height());
      String footerText =
        Template(this, pageSetupInfo.FooterText(),
                 copy, page, totalPages);
      Color textColor = pageSetupInfo.FooterFont().FontColor();
      Color backColor = textColor.Inverse();
      graphics.DrawText(footerRect, footerText,
                        pageSetupInfo.FooterFont(), textColor, backColor);
    }

    int save = graphics.Save();
    Point centerPoint(-left,
                      ((page - 1) * PageInnerHeight()) - top);
    graphics.SetOrigin(centerPoint);
    Rect clipRect(0, (page - 1) * PageInnerHeight(),
                  PageInnerWidth(), page * PageInnerHeight());
    graphics.IntersectClip(clipRect);
    OnDraw(graphics, Print);
    graphics.Restore(save);

    if (pageSetupInfo.Frame()) {
      graphics.DrawRectangle(Rect(left, top, right, bottom),
                             Black);
    }
  }
};