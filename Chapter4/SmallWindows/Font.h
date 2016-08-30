namespace SmallWindows {
  class Font;
  extern const Font SystemFont;
  
  class Font {
    public:
      Font();
      Font(String name, int size,
           bool italic = false, bool bold = false);
      Font(const Font& Font);
      Font& operator=(const Font& font);

      bool operator==(const Font& font) const;
      bool operator!=(const Font& font) const;

      bool WriteFontToStream(ostream& outStream) const;
      bool ReadFontFromStream(istream& inStream);
      void WriteFontToClipboard(InfoList& infoList) const;
      void ReadFontFromClipboard(InfoList& infoList);
      void WriteFontToRegistry(String key);
      void ReadFontFromRegistry(String key,
                                Font defaultFont = SystemFont);

      void PointsToLogical(double zoom = 1.0);

      LOGFONT LogFont() const {return logFont;}
      LOGFONT& LogFont() {return logFont;}

      Color FontColor() const {return color;}
      Color& FontColor() {return color;}

  private:
      LOGFONT logFont;
      Color color;
  };
};