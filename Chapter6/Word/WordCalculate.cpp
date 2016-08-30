#include "..\\SmallWindows\\SmallWindows.h"
#include "CharInfo.h"
#include "LineInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

void WordDocument::GenerateParagraph(Paragraph* paragraphPtr) {
  DynamicList<Size> sizeList;
  DynamicList<int> ascentList;
  DynamicList<CharInfo> prevCharList;
  charList.Copy(prevCharList, paragraphPtr->First(),
                paragraphPtr->Last());

  GenerateSizeAndAscentList(paragraphPtr, sizeList, ascentList);
  GenerateLineList(paragraphPtr, sizeList, ascentList);

  for (LineInfo* lineInfoPtr : paragraphPtr->LinePtrList()) {
    if (paragraphPtr->AlignmentField() == Justified) {
      GenerateJustifiedLineRectList(paragraphPtr, lineInfoPtr,
                                    sizeList, ascentList);
    }
    else {
      GenerateRegularLineRectList(paragraphPtr, lineInfoPtr,
                                  sizeList, ascentList);
    }
  }

  InvalidateRepaintSet(paragraphPtr, prevCharList);
}

void WordDocument::GenerateSizeAndAscentList
         (Paragraph* paragraphPtr, DynamicList<Size>& sizeList,
          DynamicList<int>& ascentList) {
  int index = 0;

  for (int charIndex = paragraphPtr->First();
       charIndex <= paragraphPtr->Last(); ++charIndex) {
    CharInfo charInfo = charList[charIndex];
    TCHAR tChar = (charInfo.Char() == NewLine)
                  ? Space : charInfo.Char();

    int width = GetCharacterWidth(charInfo.CharFont(), tChar),
        height = GetCharacterHeight(charInfo.CharFont()),
        ascent = GetCharacterAscent(charInfo.CharFont());

    sizeList.PushBack(Size(width, height));
    ascentList.PushBack(ascent);
  }
}

void WordDocument::GenerateLineList(Paragraph* paragraphPtr,
                                    DynamicList<Size>& sizeList,
                                    DynamicList<int>& ascentList){
  int maxHeight = 0, maxAscent = 0, lineWidth = 0,
      spaceLineHeight = 0, spaceLineAscent = 0,
      startIndex = paragraphPtr->First(), spaceIndex = -1;

  for (LineInfo* lineInfoPtr : paragraphPtr->LinePtrList()) {
    delete lineInfoPtr;
  }

  paragraphPtr->Height() = 0;
  paragraphPtr->LinePtrList().Clear();
  int lineTop = 0;

  for (int charIndex = paragraphPtr->First();
       charIndex <= paragraphPtr->Last(); ++charIndex) {
    CharInfo charInfo = charList[charIndex];

    if (charInfo.Char() != NewLine) {
      lineWidth +=
        sizeList[charIndex - paragraphPtr->First()].Width();
    }

    if ((wordMode == WordEdit) && (charIndex == editIndex) &&
        (nextFont != SystemFont)) {
      maxHeight = max(maxHeight, GetCharacterHeight(nextFont));
      maxAscent = max(maxAscent, GetCharacterAscent(nextFont));
    }
    else {
      maxHeight = max(maxHeight,
        sizeList[charIndex - paragraphPtr->First()].Height());
      maxAscent = max(maxAscent,
        ascentList[charIndex - paragraphPtr->First()]);
    }

    if (charInfo.Char() == Space) {
      spaceIndex = charIndex;

      spaceLineHeight = max(spaceLineHeight, maxHeight);
      spaceLineAscent = max(spaceLineAscent, maxAscent);

      maxHeight = 0;
      maxAscent = 0;
    }

    if (charInfo.Char() == NewLine) {
      spaceLineHeight = max(spaceLineHeight, maxHeight);
      spaceLineAscent = max(spaceLineAscent, maxAscent);

      LineInfo* lineInfoPtr =
        new LineInfo(startIndex - paragraphPtr->First(),
                     charIndex - paragraphPtr->First(),
                     lineTop, spaceLineHeight, spaceLineAscent);
      assert(lineInfoPtr != nullptr);

      for (int index = lineInfoPtr->First();
           index <= lineInfoPtr->Last(); ++index) {
        charList[paragraphPtr->First() + index].LineInfoPtr() =
          lineInfoPtr;
      }

      paragraphPtr->Height() += spaceLineHeight;
      paragraphPtr->LinePtrList().PushBack(lineInfoPtr);
      break;
    }

    if (lineWidth > PageInnerWidth()) {
      spaceLineHeight = max(spaceLineHeight, maxHeight);
      spaceLineAscent = max(spaceLineAscent, maxAscent);

      LineInfo* lineInfoPtr = new LineInfo();
      assert(lineInfoPtr != nullptr);
      lineInfoPtr->Top() = lineTop;
      lineTop += spaceLineHeight;

      if (spaceIndex != -1) {
        lineInfoPtr->First() = startIndex - paragraphPtr->First();
        lineInfoPtr->Last() = spaceIndex - paragraphPtr->First();
        lineInfoPtr->Ascent() = spaceLineAscent;
        lineInfoPtr->Height() = spaceLineHeight;
        startIndex = spaceIndex + 1;
      }
      else {
        if (charIndex > startIndex) {
          lineInfoPtr->First() =
            startIndex - paragraphPtr->First();
          lineInfoPtr->Last() =
            charIndex - paragraphPtr->First() - 1;
          startIndex = charIndex;
        }
        else {
          lineInfoPtr->First() = charIndex - paragraphPtr->First();
          lineInfoPtr->Last() = charIndex - paragraphPtr->First();
          startIndex = charIndex + 1;
        }

        lineInfoPtr->Height() = spaceLineHeight;
        lineInfoPtr->Ascent() = spaceLineAscent;
      }

      for (int index = lineInfoPtr->First();
           index <= lineInfoPtr->Last(); ++index) {
        charList[paragraphPtr->First() + index].LineInfoPtr() =
          lineInfoPtr;
      }

      paragraphPtr->Height() += spaceLineHeight;
      paragraphPtr->LinePtrList().PushBack(lineInfoPtr);

      lineWidth = 0;
      maxAscent = 0;
      maxHeight = 0;
      spaceLineHeight = 0;
      spaceLineAscent = 0;

      charIndex = startIndex;
      spaceIndex = -1;
    }
  }
}

void WordDocument::GenerateRegularLineRectList
                   (Paragraph* paragraphPtr,LineInfo* lineInfoPtr,
                    DynamicList<Size>& sizeList,
                    DynamicList<int>& ascentList) {
  int lineWidth = 0;

  for (int charIndex = lineInfoPtr->First();
       charIndex < lineInfoPtr->Last(); ++charIndex) {
    if (charList[paragraphPtr->First() + charIndex].Char() !=
        NewLine) {
      lineWidth +=
        sizeList[charIndex - lineInfoPtr->First()].Width();
    }
  }

  if ((charList[paragraphPtr->First()+lineInfoPtr->Last()].Char()
      != Space) &&
      (charList[paragraphPtr->First()+lineInfoPtr->Last()].Char()
      !=NewLine)) {
    lineWidth +=
      sizeList[lineInfoPtr->Last()-lineInfoPtr->First()].Width();
  }

  int leftPos;

  switch (paragraphPtr->AlignmentField()) {
    case Left:
      leftPos = 0;
      break;

    case Center:
      leftPos = (PageInnerWidth() - lineWidth) / 2;
      break;

    case Right:
      leftPos = PageInnerWidth() - lineWidth;
      break;
  }

  for (int charIndex = lineInfoPtr->First();
       charIndex <= lineInfoPtr->Last(); ++charIndex) {
    Size charSize = sizeList[charIndex];
    int ascent = ascentList[charIndex];
    int topPos = lineInfoPtr->Top() +
                 lineInfoPtr->Ascent() - ascent;

    Rect charRect(leftPos, topPos, leftPos + charSize.Width(),
                  topPos + charSize.Height());
    int parIndex = paragraphPtr->First() + charIndex;
    charList[parIndex].CharRect() = charRect;
    leftPos += charSize.Width();
  }
}

void WordDocument::GenerateJustifiedLineRectList
     (Paragraph* paragraphPtr, LineInfo* lineInfoPtr,
      DynamicList<Size>& sizeList, DynamicList<int>& ascentList) {
  int spaceCount = 0, lineWidth = 0;

  for (int charIndex = lineInfoPtr->First();
       charIndex <= lineInfoPtr->Last(); ++charIndex) {
    CharInfo charInfo =
      charList[paragraphPtr->First() + charIndex];

    if (charInfo.Char() == Space) {
      ++spaceCount;
    }
    else if (charInfo.Char() != NewLine) {
      lineWidth += sizeList[charIndex].Width();
    }
  }

  if ((charList[paragraphPtr->First()+lineInfoPtr->Last()].Char()
      != Space) &&
      (charList[paragraphPtr->First()+lineInfoPtr->Last()].Char() 
      !=NewLine)) {
    lineWidth += sizeList[lineInfoPtr->Last()].Width();
  }

  int leftPos = 0, spaceWidth;
  if (spaceCount > 0) {
    spaceWidth = (PageInnerWidth() - lineWidth) / spaceCount;
  }

  for (int charIndex = lineInfoPtr->First();
       charIndex <= lineInfoPtr->Last(); ++charIndex) {
    Size charSize = sizeList[charIndex];
    int ascent = ascentList[charIndex], charWidth;
    
    if (charList[paragraphPtr->First() + charIndex].Char() ==
        Space) {
      charWidth = spaceWidth;
    }
    else {
      charWidth = charSize.Width();
    }

    int topPos =
      lineInfoPtr->Top() + lineInfoPtr->Ascent() - ascent;
    charList[paragraphPtr->First() + charIndex].CharRect() =
      Rect(leftPos, topPos, leftPos + charWidth,
           topPos + charSize.Height());
    leftPos += charWidth;
  }
}

void WordDocument::InvalidateRepaintSet(Paragraph* paragraphPtr,
                             DynamicList<CharInfo>& prevCharList){
  Point topLeft(0, paragraphPtr->Top());

  for (int charIndex = paragraphPtr->First();
       charIndex <= paragraphPtr->Last(); ++ charIndex) {
    Rect prevRect =
      prevCharList[charIndex - paragraphPtr->First()].CharRect(),
         currRect = charList[charIndex].CharRect();

    if (prevRect != currRect) {
      Invalidate(topLeft + prevRect);
      Invalidate(topLeft + currRect);
    }
  }

  int pageWidth = PageInnerWidth();

  for (LineInfo* lineInfoPtr : paragraphPtr->LinePtrList()) {
    Rect firstRect = charList[paragraphPtr->First() +
                              lineInfoPtr->First()].CharRect();

    if (firstRect.Left() > 0) {
      Rect leftRect(0, lineInfoPtr->Top(), firstRect.Left(),
                    lineInfoPtr->Top() + lineInfoPtr->Height());
      Invalidate(topLeft + leftRect);
    }

    Rect lastRect = charList[paragraphPtr->First() +
                             lineInfoPtr->Last()].CharRect();

    if (lastRect.Right() < pageWidth) {
      Rect rightRect(lastRect.Right(), lineInfoPtr->Top(),
             pageWidth, lineInfoPtr->Top()+lineInfoPtr->Height());
      Invalidate(topLeft + rightRect);
    }

    int firstChar = paragraphPtr->First(),
        lineTop = lineInfoPtr->Top(),
        lineHeight = lineInfoPtr->Height();

    for (int charIndex = lineInfoPtr->First();
         charIndex <= lineInfoPtr->Last(); ++charIndex) {
      Rect charRect = charList[firstChar + charIndex].CharRect();
      Rect upperRect(charRect.Left(), topLeft.Y() + lineTop,
                     charRect.Right(),topLeft.Y()+charRect.Top());
      Rect lowerRect(charRect.Left(),
                     topLeft.Y() + charRect.Bottom(),
                     charRect.Right(),
                     topLeft.Y() + lineTop + lineHeight);
      if (upperRect != ZeroRect) {
        Invalidate(upperRect);
      }

      if (lowerRect != ZeroRect) {
        Invalidate(lowerRect);
      }
    }
  }
}