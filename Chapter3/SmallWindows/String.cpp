#include "SmallWindows.h"

namespace SmallWindows {
  String CharPtrToGenericString(char* text) {
    String result;

    for (int index = 0; text[index] != '\0'; ++index) {
      result += (TCHAR) text[index];
    }

    return result;
  }

  vector<String> Split(String text, TCHAR c /* = TEXT(' ') */) {
    vector<String> list;
    int spaceIndex = -1, size = text.size();

    for (int index = 0; index < size; ++index) {
      if (text[index] == c) {
        String word =
          text.substr(spaceIndex + 1, index - spaceIndex - 1);
        list.push_back(word);
        spaceIndex = index;
      }
    }

    String lastWord = text.substr(spaceIndex + 1);
    list.push_back(lastWord);
    return list;
  }
  
  bool IsNumeric(String text) {
    IStringStream stringStream(Trim(text));
    double value;
    stringStream >> value;
    return stringStream.eof();
  }

  String Trim(String text) {
    while (!text.empty() && isspace(text[0])) {
      text.erase(0, 1);
    }

    while (!text.empty() && isspace(text[text.length() - 1])) {
      text.erase(text.length() - 1, 1);
    }

    return text;
  }

  void ReplaceAll(String& text, String from, String to) {
    int index, fromSize = from.size();

    while ((index = text.find(from)) != -1) {
      text.erase(index, fromSize);
      text.insert(index, to);
    }
  }

  bool WriteStringToStream(const String& text,ostream& outStream){
    int size = text.size();
    outStream.write((char*) &size, sizeof size);

    for (TCHAR tChar : text) {
      outStream.write((char*) &tChar, sizeof tChar);
    }

    return ((bool) outStream);
  }

  bool ReadStringFromStream(String& text, istream& inStream) {
    int size;
    inStream.read((char*) &size, sizeof size);

    for (int count = 0; count < size; ++count) {
      TCHAR tChar;
      inStream.read((char*) &tChar, sizeof tChar);
      text.push_back(tChar);
    }

    return ((bool) inStream);
  }

  bool StartsWith(String text, String part) {
    return (text.find(part) == 0);
  }

  bool EndsWith(String text, String part) {
    int index = text.rfind(part),
        difference = text.length() - part.length();
    return ((index != -1) && (index == difference));
  }
};
