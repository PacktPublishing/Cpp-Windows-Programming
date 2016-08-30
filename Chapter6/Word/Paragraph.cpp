#include "..\\SmallWindows\\SmallWindows.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

Paragraph::Paragraph() {
  // Empty.
}

Paragraph::Paragraph(int first, int last,
                     Alignment alignment, int index)
 :top(-1),
  first(first),
  last(last),
  index(index),
  pageBreak(false),
  alignment(alignment) {
  // Empty.
}

bool Paragraph::WriteParagraphToStream(ostream& outStream) const {
  outStream.write((char*) &first, sizeof first);
  outStream.write((char*) &last, sizeof last);
  outStream.write((char*) &top, sizeof top);
  outStream.write((char*) &height, sizeof height);
  outStream.write((char*) &index, sizeof index);
  outStream.write((char*) &pageBreak, sizeof pageBreak);
  outStream.write((char*) &alignment, sizeof alignment);

  { int linePtrListSize = linePtrList.Size();
    outStream.write((char*) &linePtrListSize,
                    sizeof linePtrListSize);

    for (const LineInfo* lineInfoPtr : linePtrList) {
      lineInfoPtr->WriteLineInfoToStream(outStream);
    }
  }

  return ((bool) outStream);
}

bool Paragraph::ReadParagraphFromStream
                (WordDocument*wordDocumentPtr, istream& inStream){
  inStream.read((char*) &first, sizeof first);
  inStream.read((char*) &last, sizeof last);
  inStream.read((char*) &top, sizeof top);
  inStream.read((char*) &height, sizeof height);
  inStream.read((char*) &index, sizeof index);
  inStream.read((char*) &pageBreak, sizeof pageBreak);
  inStream.read((char*) &alignment, sizeof alignment);

  for (int index = first; index <= last; ++index) {
    wordDocumentPtr->CharList()[index].ParagraphPtr() = this;
  }

  { int linePtrListSize = linePtrList.Size();
    inStream.read((char*) &linePtrListSize,
                  sizeof linePtrListSize);

    for (int count = 0; count < linePtrListSize; ++count) {
      LineInfo* lineInfoPtr = new LineInfo();
      assert(lineInfoPtr != nullptr);
      lineInfoPtr->ReadLineInfoFromStream(inStream);
      linePtrList.PushBack(lineInfoPtr);

      for (int index = lineInfoPtr->First();
           index <= lineInfoPtr->Last(); ++index) {
        wordDocumentPtr->CharList()[first + index].
          LineInfoPtr() = lineInfoPtr;
      }
    }
  }

  return ((bool) inStream);
}

void Paragraph::WriteParagraphToClipboard(InfoList& infoList)
                                          const {
  infoList.AddValue<int>(first);
  infoList.AddValue<int>(last);
  infoList.AddValue<int>(top);
  infoList.AddValue<int>(index);
  infoList.AddValue<bool>(pageBreak);
  infoList.AddValue<Alignment>(alignment);
}

void Paragraph::ReadParagraphFromClipboard(InfoList& infoList) {
  infoList.GetValue<int>(first);
  infoList.GetValue<int>(last);
  infoList.GetValue<int>(top);
  infoList.GetValue<int>(index);
  infoList.GetValue<bool>(pageBreak);
  infoList.GetValue<Alignment>(alignment);
}