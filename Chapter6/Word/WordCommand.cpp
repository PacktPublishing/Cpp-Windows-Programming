#include "..\\SmallWindows\\SmallWindows.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "PageInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

void DeleteParagraph(Paragraph* paragraphPtr) {
  for (LineInfo* lineInfoPtr : paragraphPtr->LinePtrList()) {
    delete lineInfoPtr;
  }

  delete paragraphPtr;
}
  
void WordDocument::ClearDocument() {
  nextFont = SystemFont;

  for (Paragraph* paragraphPtr : paragraphList) {
    DeleteParagraph(paragraphPtr);
  }

  charList.Clear();
  paragraphList.Clear();
  InitDocument();
}

bool WordDocument::WriteDocumentToStream(String name,
                                         ostream& outStream)const{
  if (EndsWith(name, TEXT(".wrd")) &&
      WritePageSetupInfoToStream(outStream)){
    outStream.write((char*) &wordMode, sizeof wordMode);
    outStream.write((char*) &editIndex, sizeof editIndex);
    outStream.write((char*) &firstMarkIndex,
                    sizeof firstMarkIndex);
    outStream.write((char*) &lastMarkIndex, sizeof lastMarkIndex);
    outStream.write((char*) &totalPages, sizeof totalPages);
    nextFont.WriteFontToStream(outStream);

    { int charInfoListSize = charList.Size();
      outStream.write((char*) &charInfoListSize,
                      sizeof charInfoListSize);
      for (CharInfo charInfo : charList) {
        charInfo.WriteCharInfoToStream(outStream);
      }
    }

    { int paragraphListSize = paragraphList.Size();
      outStream.write((char*) &paragraphListSize,
                      sizeof paragraphListSize);

      for (const Paragraph* paragraphPtr : paragraphList) {
        paragraphPtr->WriteParagraphToStream(outStream);
      }
    }
  }
  else if (EndsWith(name, TEXT(".txt"))) {
    for (CharInfo charInfo : charList) {
      char c = (char) charInfo.Char();
      outStream.write(&c, sizeof c);
    }
  }

  return ((bool) outStream);
}

bool WordDocument::ReadDocumentFromStream(String name,
                                          istream& inStream) {
  if (EndsWith(name, TEXT(".wrd")) &&
      ReadPageSetupInfoFromStream(inStream)){
    inStream.read((char*) &wordMode, sizeof wordMode);
    inStream.read((char*) &editIndex, sizeof editIndex);
    inStream.read((char*) &firstMarkIndex, sizeof firstMarkIndex);
    inStream.read((char*) &lastMarkIndex, sizeof lastMarkIndex);
    inStream.read((char*) &totalPages, sizeof totalPages);
    nextFont.ReadFontFromStream(inStream);

    { charList.Clear();
      int charInfoListSize;
      inStream.read((char*) &charInfoListSize,
                    sizeof charInfoListSize);

      for (int count = 0; count < charInfoListSize; ++count) {
        CharInfo charInfo;
        charInfo.ReadCharInfoFromStream(inStream);
        charList.PushBack(charInfo);
      }
    }

    { paragraphList.Clear();
      int paragraphListSize;
      inStream.read((char*) &paragraphListSize,
                    sizeof paragraphListSize);

      for (int count = 0; count < paragraphListSize; ++count) {
        Paragraph* paragraphPtr = new Paragraph();
        assert(paragraphPtr != nullptr);
        paragraphPtr->ReadParagraphFromStream(this, inStream);
        paragraphList.PushBack(paragraphPtr);
      }
    }
  }  
  else if (EndsWith(name, TEXT(".txt"))) {
    wordMode = WordEdit;
    editIndex = 0;
    firstMarkIndex = 0;
    lastMarkIndex = 0;
    totalPages = 0;
    nextFont = SystemFont;

    Paragraph* paragraphPtr = new Paragraph(0, 0, Left, 0);
    int charIndex = 0, paragraphIndex = 0;
    char c;

    while (inStream >> c) {
      CharInfo charInfo(paragraphPtr, (TCHAR) c,
                        SystemFont, ZeroRect);
      charList.PushBack(charInfo);

      if (c == '\n') {
        paragraphPtr->Last() = charIndex;

        for (int index = paragraphPtr->First();
             index <= paragraphPtr->Last(); ++index) {
          charList[index].ParagraphPtr() = paragraphPtr;
        }

        GenerateParagraph(paragraphPtr);
        paragraphList.PushBack(paragraphPtr);

        Paragraph* paragraphPtr =
          new Paragraph(charIndex + 1, 0, Left, ++paragraphIndex);
      }

      ++charIndex;
    }

    paragraphPtr->Last() = charIndex;
    for (int index = paragraphPtr->First();
         index <= paragraphPtr->Last(); ++index) {
      charList[index].ParagraphPtr() = paragraphPtr;
    }

    GenerateParagraph(paragraphPtr);
    paragraphList.PushBack(paragraphPtr);
    CalculateDocument();
  }

  MakeVisible();
  return ((bool) inStream);
}

bool WordDocument::CopyEnable() const {
  return (wordMode == WordMark);
}

bool WordDocument::IsCopyAsciiReady() const {
  return true;
}

bool WordDocument::IsCopyUnicodeReady() const {
  return true;
}

bool WordDocument::IsCopyGenericReady(int /* format */) const {
  return true;
}

void WordDocument::CopyAscii(vector<String>& textList) const {
  CopyUnicode(textList);
}

void WordDocument::CopyUnicode(vector<String>& textList) const {
  int minCharIndex = min(firstMarkIndex, lastMarkIndex),
      maxCharIndex = max(firstMarkIndex, lastMarkIndex);

  String text;
  for (int charIndex = minCharIndex; charIndex < maxCharIndex;
       ++charIndex) {
    CharInfo charInfo = charList[charIndex];
    text.push_back(charInfo.Char());

    if (charInfo.Char() == NewLine) {
      textList.push_back(text);
      text.clear();
    }
  }

  textList.push_back(text);
}

void WordDocument::CopyGeneric(int /* format */,
                               InfoList& infoList) const {
  int minCharIndex = min(firstMarkIndex, lastMarkIndex),
      maxCharIndex = max(firstMarkIndex, lastMarkIndex);
  int copySize = maxCharIndex - minCharIndex;
  infoList.AddValue<int>(copySize);

  for (int charIndex = minCharIndex; charIndex < maxCharIndex;
       ++charIndex) {
    CharInfo charInfo = charList[charIndex];
    charInfo.WriteCharInfoToClipboard(infoList);
  }
}

void WordDocument::PasteAscii(const vector<String>& textList) {
  PasteUnicode(textList);
}

void WordDocument::PasteUnicode(const vector<String>& textList) {
  if (wordMode == WordMark) {
    Delete(firstMarkIndex, lastMarkIndex);
    EnsureEditStatus();
  }
  else {
    ClearNextFont();
  }

  Paragraph* paragraphPtr = charList[editIndex].ParagraphPtr();
  paragraphList.Erase(paragraphPtr->Index());

  Alignment alignment = paragraphPtr->AlignmentField();
  Font font = charList[editIndex].CharFont();

  int restChars = paragraphPtr->Last() - editIndex,
      prevEditIndex = editIndex, textListSize = textList.size();

  for (int textIndex = 0; textIndex < textListSize; ++textIndex) {
    for (TCHAR tChar : textList[textIndex]) {
      charList.Insert(editIndex++,
                      CharInfo(paragraphPtr, tChar, font));
    }

    if (textIndex < (textListSize - 1)) {
      charList.Insert(editIndex++,
                      CharInfo(paragraphPtr, NewLine));
      paragraphPtr->Last() = editIndex - 1;
      for (int index = paragraphPtr->First();
           index <= paragraphPtr->Last(); ++index) {
        charList[index].ParagraphPtr() = paragraphPtr;
      }
      GenerateParagraph(paragraphPtr);

      paragraphList.Insert(paragraphPtr->Index(), paragraphPtr);
      paragraphPtr = new Paragraph(editIndex, 0, alignment,
                                   paragraphPtr->Index() + 1);
    }
    else {
      paragraphPtr->Last() = editIndex + restChars;
      for (int index = paragraphPtr->First();
           index <= paragraphPtr->Last(); ++index) {
        charList[index].ParagraphPtr() = paragraphPtr;
      }
      
      GenerateParagraph(paragraphPtr);
      paragraphList.Insert(paragraphPtr->Index(), paragraphPtr);
    }
  }

  int totalAddedChars = editIndex - prevEditIndex;
  for (int parIndex = paragraphPtr->Index() + 1;
       parIndex < paragraphList.Size(); ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];
    paragraphPtr->Index() = parIndex;
    paragraphPtr->First() += totalAddedChars;
    paragraphPtr->Last() += totalAddedChars;
  }

  CalculateDocument();
  UpdateCaret();
  UpdateWindow();
}

void WordDocument::PasteGeneric(int /* format */,
                                InfoList& infoList) {
  if (wordMode == WordMark) {
    Delete(firstMarkIndex, lastMarkIndex);
    EnsureEditStatus();
  }
  else {
    ClearNextFont();
  }

  Paragraph* paragraphPtr = charList[editIndex].ParagraphPtr();
  paragraphList.Erase(paragraphPtr->Index());
  Alignment alignment = paragraphPtr->AlignmentField();

  int pasteSize, restChars = paragraphPtr->Last() - editIndex;
  infoList.GetValue<int>(pasteSize);

  for (int pasteCount = 0; pasteCount < pasteSize; ++pasteCount) {
    CharInfo charInfo(paragraphPtr);
    charInfo.ReadCharInfoFromClipboard(infoList);
    charList.Insert(editIndex++, charInfo);

    if (charInfo.Char() == NewLine) {
      paragraphPtr->Last() = editIndex - 1;
      GenerateParagraph(paragraphPtr);
      paragraphList.Insert(paragraphPtr->Index(), paragraphPtr);
      paragraphPtr = new Paragraph(editIndex, 0, alignment,
                                   paragraphPtr->Index() + 1);
      assert(paragraphPtr != nullptr);
    }
  }

  paragraphPtr->Last() = editIndex + restChars;
  for (int charIndex = editIndex;
       charIndex <= paragraphPtr->Last(); ++charIndex) {
    charList[charIndex].ParagraphPtr() = paragraphPtr;
  }
  GenerateParagraph(paragraphPtr);
  paragraphList.Insert(paragraphPtr->Index(), paragraphPtr);

  for (int parIndex = paragraphPtr->Index() + 1;
       parIndex < paragraphList.Size(); ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];
    paragraphPtr->Index() = parIndex;
    paragraphPtr->First() += pasteSize;
    paragraphPtr->Last() += pasteSize;
  }

  CalculateDocument();
  UpdateCaret();
  UpdateWindow();
}

bool WordDocument::DeleteEnable() const {
  switch (wordMode) {
    case WordEdit:
      return (editIndex < (charList.Size() - 1));

    case WordMark:
      return true;
  }

  return false;
}

void WordDocument::OnDelete() {
  switch (wordMode) {
    case WordEdit:
      ClearNextFont();
      Delete(editIndex, editIndex + 1);
      break;

    case WordMark:
      Delete(firstMarkIndex, lastMarkIndex);
      editIndex = min(firstMarkIndex, lastMarkIndex);
      wordMode = WordEdit;
      break;
  }

  SetDirty(true);
  CalculateDocument();
  UpdateCaret();
  UpdateWindow();
}

void WordDocument::Delete(int firstIndex, int lastIndex) {
  int minCharIndex = min(firstIndex, lastIndex),
      maxCharIndex = max(firstIndex, lastIndex);

  Paragraph* minParagraphPtr =
    charList[minCharIndex].ParagraphPtr();
  Paragraph* maxParagraphPtr =
    charList[maxCharIndex].ParagraphPtr();

  if (minParagraphPtr != maxParagraphPtr) {
    for (int charIndex = maxParagraphPtr->First();
         charIndex <= maxParagraphPtr->Last(); ++charIndex) {
      CharInfo& charInfo = charList[charIndex];
      charInfo.ParagraphPtr() = minParagraphPtr;
      charInfo.CharRect() = ZeroRect;
    }
  }

  int deleteChars = maxCharIndex - minCharIndex;
  minParagraphPtr->Last() = maxParagraphPtr->Last() - deleteChars;
  charList.Remove(minCharIndex, maxCharIndex - 1);
  GenerateParagraph(minParagraphPtr);

  int minParIndex = minParagraphPtr->Index(),
      maxParIndex = maxParagraphPtr->Index();

  if (minParIndex < maxParIndex) {
    for (int parIndex = minParIndex + 1;
         parIndex <= maxParIndex; ++parIndex) {
      DeleteParagraph(paragraphList[parIndex]);
    }

    paragraphList.Remove(minParIndex + 1, maxParIndex);
  }

  int deleteParagraphs = maxParIndex - minParIndex;
  for (int parIndex = minParIndex + 1;
       parIndex < paragraphList.Size(); ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];
    paragraphPtr->Index() -= deleteParagraphs;
    paragraphPtr->First() -= deleteChars;
    paragraphPtr->Last() -= deleteChars;
  }

  if (charList.Empty()) {
    int i = 1;
  }

  wordMode = WordEdit;
  editIndex = minCharIndex;
}

#if 0
void WordDocument::Delete(int firstIndex, int lastIndex) {
  int minCharIndex = min(firstIndex, lastIndex),
    maxCharIndex = max(firstIndex, lastIndex);

  Paragraph* minParagraphPtr =
    charList[minCharIndex].ParagraphPtr();
  Paragraph* maxParagraphPtr =
    charList[maxCharIndex].ParagraphPtr();

  int deleteChars = maxCharIndex - minCharIndex;
  charList.Remove(minCharIndex, maxCharIndex - 1);
  minParagraphPtr->Last() = maxParagraphPtr->Last() - deleteChars;

  if (minParagraphPtr->Index() < maxParagraphPtr->Index()) {
    for (int charIndex = editIndex;
      charIndex <= maxParagraphPtr->Last(); ++charIndex) {
      CharInfo& charInfo = charList[charIndex];
      charInfo.ParagraphPtr() = minParagraphPtr;
      charInfo.CharRect() = ZeroRect;
    }
  }

  GenerateParagraph(minParagraphPtr);

  if (minParagraphPtr->Index() < maxParagraphPtr->Index()) {
    int maxParIndex = maxParagraphPtr->Index();

    for (int parIndex = minParagraphPtr->Index() + 1;
      parIndex <= maxParagraphPtr->Index(); ++parIndex) {
      DeleteParagraph(paragraphList[parIndex]);
    }

    paragraphList.Remove(minParagraphPtr->Index() + 1,
      maxParIndex);

    for (int parIndex = minParagraphPtr->Index() + 1;
      parIndex < paragraphList.Size(); ++parIndex) {
      paragraphList[parIndex]->Index() = parIndex;
    }
  }

  for (int parIndex = minParagraphPtr->Index() + 1;
    parIndex < paragraphList.Size(); ++parIndex) {
    Paragraph* paragraphPtr = paragraphList[parIndex];
    paragraphPtr->First() -= deleteChars;
    paragraphPtr->Last() -= deleteChars;
  }

  wordMode = WordEdit;
  editIndex = minCharIndex;

  if (charList.Empty()) {
    InitDocument();
  }
}
#endif

bool WordDocument::PageBreakEnable() const {
  return (wordMode == WordEdit);
}

void WordDocument::OnPageBreak() {
  Paragraph* paragraphPtr = charList[editIndex].ParagraphPtr();
  paragraphPtr->PageBreak() = !paragraphPtr->PageBreak();
  CalculateDocument();
  UpdateCaret();
}

void WordDocument::OnFont() {
  switch (wordMode) {
    case WordEdit: {
        Font font;

        if (nextFont != SystemFont) {
          font = nextFont;
        }
        else if (editIndex ==
                 charList[editIndex].ParagraphPtr()->First()) {
          font = charList[editIndex].CharFont();
        }
        else {
          font = charList[editIndex - 1].CharFont();
        }

        if (StandardDialog::FontDialog(this, font)) {
          nextFont = font;
          Paragraph* paragraphPtr =
            charList[editIndex].ParagraphPtr();
          GenerateParagraph(paragraphPtr);
          SetDirty(true);
          CalculateDocument();
          UpdateCaret();
          UpdateWindow();
        }
      }
      break;

    case WordMark: {
        int minCharIndex = min(firstMarkIndex, lastMarkIndex),
            maxCharIndex = max(firstMarkIndex, lastMarkIndex);
        Font font = charList[minCharIndex].CharFont();

        if (StandardDialog::FontDialog(this, font)) {
          for (int charIndex = minCharIndex;
               charIndex < maxCharIndex; ++charIndex) {
            charList[charIndex].CharFont() = font;
          }
          
          int minParIndex =
                charList[minCharIndex].ParagraphPtr()->Index(),
              maxParIndex =
                charList[maxCharIndex].ParagraphPtr()->Index();

          for (int parIndex = minParIndex;
               parIndex <= maxParIndex; ++parIndex) {
            Paragraph* paragraphPtr = paragraphList[parIndex];
            GenerateParagraph(paragraphPtr);
          }

          SetDirty(true);
          CalculateDocument();
          UpdateCaret();
          UpdateWindow();
        }
      }
      break;
  }
}

bool WordDocument::LeftRadio() const {
  return IsAlignment(Left);
}

void WordDocument::OnLeft() {
  SetAlignment(Left);
}

bool WordDocument::CenterRadio() const {
  return IsAlignment(Center);
}

void WordDocument::OnCenter() {
  SetAlignment(Center);
}

bool WordDocument::RightRadio() const {
  return IsAlignment(Right);
}

void WordDocument::OnRight() {
  SetAlignment(Right);
}

bool WordDocument::JustifiedRadio() const {
  return IsAlignment(Justified);
}

void WordDocument::OnJustified() {
  SetAlignment(Justified);
}

bool WordDocument::IsAlignment(Alignment alignment) const {
  switch (wordMode) {
    case WordEdit: {
        Alignment editAlignment =
          charList[editIndex].ParagraphPtr()->AlignmentField();
        return (editAlignment == alignment);
      }
      
    case WordMark: {
        int minCharIndex = min(firstMarkIndex, lastMarkIndex),
            maxCharIndex = max(firstMarkIndex, lastMarkIndex);

        int minParIndex =
              charList[minCharIndex].ParagraphPtr()->Index(),
            maxParIndex =
              charList[maxCharIndex].ParagraphPtr()->Index();

        for (int parIndex = minParIndex; parIndex < maxParIndex;
             ++parIndex) {
          Alignment markAlignment =
            paragraphList[parIndex]->AlignmentField();

          if (markAlignment != alignment) {
            return false;
          }
        }

        return true;
      }
  }

  assert(false);
  return false;
}

void WordDocument::SetAlignment(Alignment alignment) {
  switch (wordMode) {
    case WordEdit: {
        Paragraph* paragraphPtr =
          charList[editIndex].ParagraphPtr();
        paragraphPtr->AlignmentField() = alignment;
        GenerateParagraph(paragraphPtr);
        UpdateCaret();
      }
      break;

    case WordMark: {
        int minCharIndex = min(firstMarkIndex, lastMarkIndex),
            maxCharIndex = max(firstMarkIndex, lastMarkIndex);

        int minParIndex =
              charList[minCharIndex].ParagraphPtr()->Index(),
            maxParIndex =
              charList[maxCharIndex].ParagraphPtr()->Index();

        for (int parIndex = minParIndex; parIndex < maxParIndex;
             ++parIndex) {
          Paragraph* paragraphPtr = paragraphList[parIndex];
          paragraphPtr->AlignmentField() = alignment;
          GenerateParagraph(paragraphPtr);
        }
      }
      break;
  }

  UpdateWindow();
}