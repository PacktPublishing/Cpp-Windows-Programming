namespace SmallWindows {
  enum {AsciiFormat = CF_TEXT, UnicodeFormat = CF_UNICODETEXT};

  class Clipboard {
    public: 
      static bool Open(const Window* windowPtr);
      static bool Close();
      static bool Clear();
      static bool Available(unsigned int format);

      static bool WriteGeneric(unsigned int format,
                               InfoList& infoList);
      static bool ReadGeneric(unsigned int format,
                              InfoList& infoList);

      template<int Format, class CharType>
      static bool WriteText(vector<String>& lineList);
      template<int Format, class CharType>
      static bool ReadText(vector<String>& lineList);
  };

  template<int Format,class CharType>
  bool Clipboard::WriteText(vector<String>& lineList) {
    int bufferSize = 0;
 
    for (String line : lineList) {
      bufferSize += line.size();
    }

    int listSize = lineList.size();
    bufferSize += listSize;

    HGLOBAL globalHandle =
      ::GlobalAlloc(GMEM_MOVEABLE, bufferSize*sizeof(CharType));

    if (globalHandle != nullptr) {
      CharType* buffer = (CharType*) ::GlobalLock(globalHandle);

      if (buffer != nullptr) {
        int bufferIndex = 0;

        for (int listIndex = 0; listIndex < listSize;++listIndex){
          for (TCHAR tChar : lineList[listIndex]) {
            buffer[bufferIndex++] = (CharType) tChar;
          }

          if (listIndex < (listSize - 1)) {
            buffer[bufferIndex++] = (CharType) '\r';
          }
        }

        buffer[bufferIndex] = (CharType) '\0';
        ::GlobalUnlock(globalHandle);
        ::SetClipboardData(Format, globalHandle);
        return true;
      }
    }

    return false;
  }

  template<int Format,class CharType>
  bool Clipboard::ReadText(vector<String>& lineList) {
    HGLOBAL globalHandle = ::GetClipboardData(Format);

    if (globalHandle != nullptr) {
      CharType* buffer = (CharType*) ::GlobalLock(globalHandle);

      if (buffer != nullptr) {
        String currentLine;

        int charCount =
          ::GlobalSize(globalHandle) / (sizeof(CharType));

        for (int count = 0; count < charCount; ++count) {
          CharType cChar = (*buffer++);

          switch (cChar) {
            case ((CharType) '\r') :
              lineList.push_back(currentLine);
              currentLine.clear();
              break;

            case ((CharType) '\0') :
              lineList.push_back(currentLine);
              break;

            default:
              currentLine += (TCHAR) cChar;
              break;
          }
        }

        ::GlobalUnlock(globalHandle);
        return true;
      }
    }

    return false;
  }
};