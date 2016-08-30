namespace SmallWindows {
  enum Orientation {Portrait, Landscape};

  class PageSetupInfo {
    public:
      PageSetupInfo();
      PageSetupInfo(const PageSetupInfo& pageSetupInfo);
      bool operator==(const PageSetupInfo& pageSetupInfo);
      bool operator!=(const PageSetupInfo& pageSetupInfo);

      void ClearPageSetupInfo();
      bool WritePageSetupInfoToStream(ostream& outStream) const;
      bool ReadPageSetupInfoFromStream(istream& inStream);

      Orientation& GetOrientation() {return orientation;}
      int& LeftMargin() {return leftMargin;}
      int& TopMargin() {return topMargin;}
      int& RightMargin() {return rightMargin;}
      int& BottomMargin() {return bottomMargin;}
      String& HeaderText() {return headerText;}
      String& FooterText() {return footerText;}
      bool& HeaderFirst() {return headerFirst;}
      bool& FooterFirst() {return footerFirst;}
      bool& Frame() {return frame;}
      Font& HeaderFont() {return headerFont;}
      Font& FooterFont() {return footerFont;}

      Orientation GetOrientation() const {return orientation;}
      int LeftMargin() const {return leftMargin;}
      int TopMargin() const {return topMargin;}
      int RightMargin() const {return rightMargin;}
      int BottomMargin() const {return bottomMargin;}
      String HeaderText() const {return headerText;}
      String FooterText() const {return footerText;}
      bool HeaderFirst() const {return headerFirst;}
      bool FooterFirst() const {return footerFirst;}
      bool Frame() const {return frame;}
      Font HeaderFont() const {return headerFont;}
      Font FooterFont() const {return footerFont;}

    private:
      Orientation orientation;
      int leftMargin, topMargin, rightMargin, bottomMargin;
      String headerText, footerText;
      bool headerFirst, footerFirst, frame;
      Font headerFont, footerFont;
 };
};