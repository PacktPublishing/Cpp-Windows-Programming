namespace SmallWindows {
  class PageSetupDialog : public Dialog {
    public:
      PageSetupDialog(Window* parentPtr, PageSetupInfo* infoPtr);

      DEFINE_VOID_LISTENER(PageSetupDialog, OnHeaderFont);
      DEFINE_VOID_LISTENER(PageSetupDialog, OnFooterFont);      
      DEFINE_VOID_LISTENER(PageSetupDialog, OnOk);
      DEFINE_VOID_LISTENER(PageSetupDialog, OnCancel);

    private:
      PageSetupInfo *infoPtr, backupInfo;
  };
};