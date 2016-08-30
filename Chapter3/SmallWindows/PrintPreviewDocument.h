namespace SmallWindows {
  class PrintPreviewDocument : Document {
    public:
      PrintPreviewDocument(StandardDocument* parentDocument,
                  int page = 1, Size pageSize = USLetterPortrait);
      bool OnKeyDown(WORD key, bool shiftPressed,
                     bool controlPressed);
      void OnSize(Size clientSize) {/* Empty. */ }
      void OnVerticalScroll(WORD flags, WORD thumbPos = 0);
      void OnPaint(Graphics& graphics) const;

    private:
      void SetHeader();
      int page, totalPages;
  };
};
