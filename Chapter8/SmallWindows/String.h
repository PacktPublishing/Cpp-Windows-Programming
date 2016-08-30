namespace SmallWindows {
  extern String CharPtrToGenericString(char* text);
  extern vector<String> Split(String text, TCHAR c = TEXT(' '));
  extern bool IsNumeric(String text);
  extern String Trim(String text);
  void ReplaceAll(String& text, String from, String to);
  extern bool WriteStringToStream(const String& text,
                                  ostream& outStream);
  extern bool ReadStringFromStream(String& text,
                                   istream& inStream);
  extern bool StartsWith(String text, String part);
  extern bool EndsWith(String text, String part);
};
