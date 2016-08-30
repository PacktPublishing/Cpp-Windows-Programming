namespace SmallWindows {
  class StandardDocument : public Document {
    public:
      StandardDocument(CoordinateSystem system, Size pageSize,
                       String fileDescriptionsText,
                       Window* parentPtr=nullptr,
                       WindowStyle style = OverlappedWindow,
                       WindowShow windowShow = Normal,
                       initializer_list<unsigned int>
                       copyFormatList = {},
                       initializer_list<unsigned int>
                       pasteFormatList = {},
                       bool acceptDropFiles = true,
                       Size lineSize = LineSize);

    private:
      void InitializeFileFilter(String fileDescription);

    protected:
      Menu StandardFileMenu(bool print);
      Menu StandardEditMenu();
      Menu StandardHelpMenu();

    private:
      DEFINE_VOID_LISTENER(StandardDocument, OnNew);
      DEFINE_VOID_LISTENER(StandardDocument, OnOpen);
      DEFINE_BOOL_LISTENER(StandardDocument, SaveEnable);
      DEFINE_VOID_LISTENER(StandardDocument, OnSave);
      DEFINE_VOID_LISTENER(StandardDocument, OnSaveAs);

   private:
      void SaveFileWithName(String name);
      void SaveFileWithoutName();

    protected:
      void ClearPageSetupInfo();
      bool ReadPageSetupInfoFromStream(istream &inStream);
      bool WritePageSetupInfoToStream(ostream &outStream) const;

      virtual void ClearDocument() {/* Empty. */}
      virtual bool WriteDocumentToStream(String name,
                        ostream& outStream) const {return true;}
      virtual bool ReadDocumentFromStream(String name,
                       istream& inStream) {return true;}

      DEFINE_VOID_LISTENER(StandardDocument, OnCut);
      DEFINE_VOID_LISTENER(StandardDocument, OnCopy);
      DEFINE_VOID_LISTENER(StandardDocument, OnPaste);
      DEFINE_VOID_LISTENER(StandardDocument, OnDelete);

      DEFINE_BOOL_LISTENER(StandardDocument, CutEnable);
      DEFINE_BOOL_LISTENER(StandardDocument, CopyEnable);
      DEFINE_BOOL_LISTENER(StandardDocument, PasteEnable);
      DEFINE_BOOL_LISTENER(StandardDocument, DeleteEnable);

      virtual bool IsCopyAsciiReady() const {return false;}
      virtual bool IsCopyUnicodeReady() const {return false;}
      virtual bool IsCopyGenericReady(int format)
                                      const {return false;}

      virtual void CopyAscii(vector<String>& textList) const
                            {/* Empty. */}
      virtual void CopyUnicode(vector<String>& textList) const
                              {/* Empty. */}
      virtual void CopyGeneric(int format, InfoList& infoList)
                               const {/* Empty. */}

      virtual void PasteAscii(const vector<String>& textList)
                             {/* Empty. */}
      virtual bool IsPasteAsciiReady
                   (const vector<String>& textList) const {return true;}

      virtual void PasteUnicode(const vector<String>& textList)
                               {/* Empty. */}
      virtual bool IsPasteUnicodeReady
                   (const vector<String>& textList) const {return true;}


      virtual void PasteGeneric(int format, InfoList& infoList)
                               {/* Empty. */}
      virtual bool IsPasteGenericReady(int format,
                                       InfoList& infoList) const {return true;}

      void OnDropFile(vector<String> pathList);

    private:
      Size PageOuterSize() const;
      Size PageInnerSize() const;

    protected:
      int PageInnerWidth() const{return PageInnerSize().Width();}
      int PageInnerHeight()const{return PageInnerSize().Height();}

    public:
      DEFINE_VOID_LISTENER(StandardDocument, OnPageSetup);
      DEFINE_VOID_LISTENER(StandardDocument, OnPrintPreview);
      DEFINE_VOID_LISTENER(StandardDocument, OnPrintItem);

      bool PrintPage(Graphics* graphicsPtr, int page,
                     int copy, int totalPages);

      virtual void OnPageSetup(PageSetupInfo info) {/* Empty. */}

      virtual int GetTotalPages() const {return 1;}

      virtual void OnPrint(Graphics& graphics, int page,
                           int copy, int totalPages) const;

      DEFINE_VOID_LISTENER(StandardDocument, OnExit);
      virtual bool TryClose();

      DEFINE_VOID_LISTENER(StandardDocument, OnAbout);

    private:
      TCHAR fileFilter[MAX_PATH];
      vector<String> fileSuffixList;
      PageSetupInfo pageSetupInfo;
      list<unsigned int> copyFormatList, pasteFormatList;
  };
};