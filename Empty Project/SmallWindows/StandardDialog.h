namespace SmallWindows {
  class Window;
  class Graphics;

  class StandardDialog {
    public:
      enum SaveFlags {NoSaveFlag = 0,
                      PromptBeforeOverwrite = OFN_OVERWRITEPROMPT,
                      PathMustExist = OFN_PATHMUSTEXIST,
                      NormalSaveFlags = OFN_OVERWRITEPROMPT |
                                        OFN_PATHMUSTEXIST};

      static bool SaveDialog(Window* windowPtr, String& path,
                             const TCHAR* filter,
                             const vector<String> fileSuffixList,
                             StandardDialog::SaveFlags saveFlags =
                               NormalSaveFlags); 

      enum OpenFlags {NoOpenFlag = 0,
                      PromptBeforeCreate = OFN_CREATEPROMPT,
                      FileMustExist = OFN_FILEMUSTEXIST,
                      HideReadOnly = OFN_HIDEREADONLY,
                      NormalOpenFlags = OFN_CREATEPROMPT |
                                        OFN_FILEMUSTEXIST |
                                        OFN_HIDEREADONLY};

      static bool OpenDialog(Window* windowPtr, String& path,
                             const TCHAR* filter,
                             const vector<String> fileSuffixList,
                             StandardDialog::OpenFlags openFlags =
                               NormalOpenFlags); 

      static COLORREF customColorArray[];
      static bool ColorDialog(Window* windowPtr, Color& color);

      static bool FontDialog(Window* windowPtr, Font& font);

      static Graphics* PrintDialog(Window* parentPtr,
                                   int totalPages,
                                   int& firstPage, int& lastPage,
                                   int& copies, bool& sorted);
  };
};