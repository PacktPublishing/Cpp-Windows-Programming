#include <Windows.h>
#include <StdIO.h>
#include <TChar.h>
#include <WChar.h>
#include <String.h>
#include <Assert.h>
#include "..\\List.h"
#include "..\\Set.h"
#include "..\\Unit.h"
#include "..\\UnitScroll.h"
#include "Position.h"
#include "Paragraph.h"
#include "Calculate.h"
#include "Resource.h"
#include "Word.h"
#include "Command.h"
#include "Keyboard.h"

void GetTextZoomMetrics(HDC hDC, int iZoom, LPTEXTMETRIC pTextMetric)
{
  double dZoom = ((double) iZoom) / 100;
  GetTextMetrics(hDC, pTextMetric);

  pTextMetric->tmHeight = (LONG) (dZoom * pTextMetric->tmHeight);
  pTextMetric->tmAscent = (LONG) (dZoom * pTextMetric->tmAscent);
  pTextMetric->tmDescent = (LONG) (dZoom * pTextMetric->tmDescent);
  pTextMetric->tmInternalLeading = (LONG) (dZoom * pTextMetric->tmInternalLeading);
  pTextMetric->tmExternalLeading = (LONG) (dZoom * pTextMetric->tmExternalLeading);
  pTextMetric->tmAveCharWidth = (LONG) (dZoom * pTextMetric->tmAveCharWidth);
  pTextMetric->tmMaxCharWidth = (LONG) (dZoom * pTextMetric->tmMaxCharWidth);
}

BOOL GetTextExtentZoomPoint(HDC hDC, int iZoom, LPCTSTR lpString, int cbString, LPSIZE lpSize)
{
  double dZoom = ((double) iZoom) / 100;
  BOOL bResult = GetTextExtentPoint(hDC, lpString, cbString, lpSize);
  lpSize->cx = (LONG) (dZoom * lpSize->cx);
  lpSize->cy = (LONG) (dZoom * lpSize->cy);
  return bResult;
}

/* ${Recalculate} is called in order to recalculate the rectangle (${pParagraph->m_rectList}) and lineInfo (${pParagraph->m_lineInfoList}) lists every time one or several character have be added or removed, or when the font or the alignment has been changed. */
void ParagraphCalculate(HWND hWnd, PARAGRAPH* pParagraph,
                        LOGFONT* plfFont, int iFont,
                        SET* pRepaintSet)
{
  HDC hDC = GetPreparedScrollDC(hWnd);
  LIST oldCharInfoList = LIST_EMPTY(sizeof (CHAR_DESCR));
  ListCopyAll(&oldCharInfoList, &pParagraph->m_charInfoList);
  pParagraph->m_iHeight = 0;
/* First we clear the lineInfo and rectangle list. */
  ListClear(&pParagraph->m_lineInfoList);

  if (ListSize(&pParagraph->m_charInfoList) == 0) {
    ParagraphGenerateEmptyList(pParagraph, plfFont, hDC);
  }
/* If the paragraph is not empty, we generate lists of size and ascent lines for every character as well as the lineInfo and rectangle lists by calling ${GenerateSizeList}, ${GenerateAscentList}, ${GenerateLineList}, and ${GenerateRectList}; ${sizeList} and ${ascentList} are temporary lists needed to calculate the final rectangle and lineInfo lists. */
  else {
    int iSize = ListSize(&pParagraph->m_charInfoList);
    LIST sizeList = ListCreate(sizeof (SIZE), iSize),
         ascentList = ListCreate(sizeof (int), iSize);

    ParagraphGenerateSizeAscentList(pParagraph, &sizeList,
                                    &ascentList, hDC);
    ParagraphGenerateLineList(pParagraph, plfFont, iFont,
                              &sizeList, &ascentList, hDC);
    ParagraphGenerateRectList(pParagraph, &sizeList,
                              &ascentList);
  }
/* Finally, if the pointer to the invalidate set is not null, we also call ${GetRepaintSet} in order to obtain the set of rectangles for the characters that need to be redrawn. */
  if (pRepaintSet != NULL) {
    ParagraphGetRectSet(pParagraph, &oldCharInfoList,
                        pRepaintSet);
  }

  ReleaseDC(hWnd, hDC);
}
/* If the paragraph is empty, we use the empty font (${m_emptyFontInfo.lfFont}) to find the height (${m_iHeight}) and average width (${m_iEmptyAverage}) of a character. We need the averagen width for the caret. */
void ParagraphGenerateEmptyList(PARAGRAPH* pParagraph,
                                LOGFONT* plfFont, HDC hDC)
{
  LOGFONT lfFont = (plfFont != NULL) ? (*plfFont)
                 : pParagraph->m_emptyFontInfo.lfFont;
  LOGFONT lfMetricFont = PointsToMeters(lfFont);
  HFONT hFont = CreateFontIndirect(&lfMetricFont);
  HFONT hOldFont = SelectObject(hDC, hFont);

  LINE_INFO lineInfo = {0, 0, 0};
  TEXTMETRIC textMetric;

  GetTextZoomMetrics(hDC, g_iZoom, &textMetric);
  DeleteObject(hFont);
  SelectObject(hDC, hOldFont);

  pParagraph->m_iHeight = textMetric.tmHeight;
  pParagraph->m_iEmptyAverage =
    textMetric.tmAveCharWidth;

  lineInfo.m_iHeight = pParagraph->m_iHeight;
  ListAdd(&pParagraph->m_lineInfoList, &lineInfo);
}
/* ${GenerateSizeList} fills the given list with the size (width and height) of every character in the paragraph (in logical units). For every character, we load the device context with the character's font. Note that we have to translate the font from typographical points at hundredth of millimeters by calling ${PointToMeters}. */
void ParagraphGenerateSizeAscentList(PARAGRAPH* pParagraph,
                  LIST* pSizeList, LIST* pAscentList, HDC hDC)
{
  int iIndex, iTextSize=ListSize(&pParagraph->m_charInfoList);

  for (iIndex = 0; iIndex < iTextSize; ++iIndex) {
    CHAR_DESCR c;
    SIZE szChar;
    TEXTMETRIC textMetric;
    HFONT hFont, hOldFont;
    LOGFONT lfMetricFont;
    /* The font size of the character is given in typographical points (1 point is 1/72 inch or approximataly 0.339 mm). However, we want the size of the character in logical units (hundredth of millimeters) why we transform the font size by calling ${PointsToMeters}. */
    ListGet(&pParagraph->m_charInfoList, iIndex, &c);
    lfMetricFont = PointsToMeters(c.m_fontInfo.lfFont);

    hFont = CreateFontIndirect(&lfMetricFont);
    hOldFont = SelectObject(hDC, hFont);
/* The ${GetTextExtentZoomPoint} Win32 API function returns size of the given character in the ${szChar} parameter. */
    GetTextExtentZoomPoint(hDC, g_iZoom, &c.m_cChar, 1, &szChar);
/* CharInfoters written in italic style tend to request slightly more space, so we increase the size with twenty percent. As a matter of fact, also plain text tend to need a little bit more space than specified by the call to ${GetTextExtentZoomPoint} above; therefore, we increase the size with ten percent. */
/* When we have obtain the size of the character, we add it to the size list (${pSizeList}). */
    ListSet(pSizeList, iIndex, &szChar);

    GetTextZoomMetrics(hDC, g_iZoom, &textMetric);
    ListSet(pAscentList, iIndex, &textMetric.tmAscent);

    c.m_iAverage = textMetric.tmAveCharWidth;
    ListSet(&pParagraph->m_charInfoList, iIndex, &c);

    DeleteObject(hFont);
    SelectObject(hDC, hOldFont);
  }
}
/* ${GenerateLineList} generates the lineInfo list. We have to decide how many words each lineInfo can hold. We traverse the text, calculate the size of each word and when the next word does not fit on the lineInfo, we start a new lineInfo and save the index of the first and last character on the lineInfo as well as the height of the lineInfo (the height of the highest character on the lineInfo). */
void ParagraphGenerateLineList(PARAGRAPH* pParagraph,
              LOGFONT* pFont, int iFont, LIST* pSizeList,
              LIST* pAscentList, HDC hDC)
{
  BOOL bSpace = FALSE;
  int iIndex = 0,
      iTextSize = ListSize(&pParagraph->m_charInfoList),
      iSpaceIndex = 0, iStartIndex = 0,
      iLineWidth = 0,
      iLineAscent = 0, iSpaceLineAscent = 0,
      iLineHeight = 0, iSpaceLineHeight = 0;
  ListClear(&pParagraph->m_lineInfoList);

  while (iIndex < iTextSize) {
    SIZE szChar;
    int iAscent;
    CHAR_DESCR charInfo;

    ListGet(pSizeList, iIndex, &szChar);
    ListGet(pAscentList, iIndex, &iAscent);
    ListGet(&pParagraph->m_charInfoList, iIndex, &charInfo);

    iLineAscent = max(iLineAscent, iAscent);
    iLineHeight = max(iLineHeight, szChar.cy);
    iLineWidth += szChar.cx;
/* The latest space is a suitable point to break the lineInfo at. */
    if (charInfo.m_cChar == TEXT(' ')) {
      bSpace = TRUE;
      iSpaceIndex = iIndex;

      iSpaceLineAscent = max(iSpaceLineAscent, iLineAscent);
      iSpaceLineHeight = max(iSpaceLineHeight, iLineHeight);
      
      iLineAscent = 0;
      iLineHeight = 0;
    }
/* When no more characters fit on the lineInfo, we break it and look up the latest space. That is, we use the saved start index togheter with the current index and the height of the higest character to creeate a new lineInfo structure. Note that we have in fact three differnt cases: the lineInfo is constituted by at least one complete word (${bSpace} is true), the lineInfo may hold one word so long that it does not fit on the lineInfo, and the lineInfo might (unlikely) be made up by one single character so width that it does not fit on the lineInfo. */
    if (iLineWidth > PAGE_WIDTH) {
      LINE_INFO lineInfo;
/* If the lineInfo is constituted by at least one complete word followed by a space, we discard the latest space and start the new lineInfo at the next character. */
      if (bSpace) {
        lineInfo.m_iFirstChar = iStartIndex;
        lineInfo.m_iLastChar = iSpaceIndex - 1;
        lineInfo.m_iAscent = iSpaceLineAscent;
        lineInfo.m_iHeight = iSpaceLineHeight;
        iStartIndex = iSpaceIndex + 1;
      }
/* If the line is constituted by one single word so width that it does not fit on the line, we define the line to hold the word including the last fitting character, and we start the new line with the following character. */
      else {
        if (iIndex > iStartIndex) {
          lineInfo.m_iFirstChar = iStartIndex;
          lineInfo.m_iLastChar = iIndex - 1;
          iStartIndex = iIndex;
        }
/* If one single character is wider than the line, we just let that character constitute the whole line and let the next index be the start index. */
        else {
          lineInfo.m_iFirstChar = iIndex;
          lineInfo.m_iLastChar = iIndex;
          iStartIndex = iIndex + 1;
        }

        lineInfo.m_iAscent = iLineAscent;
        lineInfo.m_iHeight = iLineHeight;

        iLineAscent = 0;
        iLineHeight = 0;
      }
/* When the line is defined, we add it to the line list. We also set the width of the next line to zero and set the next character index. */
      ListAdd(&pParagraph->m_lineInfoList, &lineInfo);
      bSpace = FALSE;

      iLineWidth = 0;
      iIndex = iStartIndex;

      iSpaceLineAscent = 0;
      iSpaceLineHeight = 0;
    }
/* If the character fits on the line, we go on to the next one. */
    else {
      ++iIndex;
    }
  }
/* If there are characters on the last line after the last line break, we add them too. */
  if (iStartIndex < iTextSize) {
    LINE_INFO lineInfo = {iStartIndex, iTextSize - 1,
                 max(iLineAscent, iSpaceLineAscent),
                 max(iLineHeight, iSpaceLineHeight)};

    ListAdd(&pParagraph->m_lineInfoList, &lineInfo);
 }

  if (pFont != NULL) {
    int iLineIndex,
        iSize = ListSize(&pParagraph->m_charInfoList);
    for (iLineIndex = 0;
         iLineIndex < ListSize(&pParagraph->m_lineInfoList);
         ++iLineIndex) {
      LINE_INFO lineInfo;
      BOOL bInside, bSpace = FALSE;

      ListGet(&pParagraph->m_lineInfoList, iLineIndex,
              &lineInfo);
      bInside = ((iFont >= lineInfo.m_iFirstChar) &&
                 (iFont <= lineInfo.m_iLastChar));

      if ((iFont == (lineInfo.m_iLastChar + 1)) &&
          (iFont < iSize)) {
        CHAR_DESCR charInfo;
        ListGet(&pParagraph->m_charInfoList, iFont,&charInfo);
        bSpace = (charInfo.m_cChar == TEXT(' '));
      }

      if (bInside || bSpace || (iFont == iSize)) {
        LOGFONT lfMetricFont = PointsToMeters(*pFont);
        HFONT hFont = CreateFontIndirect(&lfMetricFont);
        HFONT hOldFont = SelectObject(hDC, hFont);

        TEXTMETRIC textMetric;
        GetTextZoomMetrics(hDC, g_iZoom, &textMetric);
        DeleteObject(hFont);
        SelectObject(hDC, hOldFont);

        lineInfo.m_iAscent = max(lineInfo.m_iAscent,
                               textMetric.tmAscent);
        lineInfo.m_iHeight = max(lineInfo.m_iHeight,
                               textMetric.tmHeight);

        ListSet(&pParagraph->m_lineInfoList, iLineIndex,
                &lineInfo);
        break;
      }
    }
  }
}
/* ${GenerateRectList} generates the rectangle list. Given the size, ascent, and line lists, we calculate the rectangle of each character. */
void ParagraphGenerateRectList(PARAGRAPH* pParagraph,
                          LIST* pSizeList, LIST* pAscentList)
{
  int iLineIndex,
      iTextSize = ListSize(&pParagraph->m_charInfoList);
/* First, we traverse the lines of the paragraph. */
  for (iLineIndex = 0;
       iLineIndex < ListSize(&pParagraph->m_lineInfoList);
       ++iLineIndex) {
    int iWidth = 0, yTopPos = 0, iHeight = 0;
/* We find the first and last character of each lineInfo. */
    LINE_INFO lineInfo;
    int xLeftPos = 0, iSpaceWidth = 0;
    ListGet(&pParagraph->m_lineInfoList,iLineIndex,&lineInfo);
    ParagraphLeftPositionSpaceWidth(pParagraph, lineInfo,
             pSizeList, pAscentList, &xLeftPos, &iSpaceWidth);
    ParagraphGenerateLineRectList(pParagraph, lineInfo,
             pSizeList, pAscentList, xLeftPos, iSpaceWidth);
/* The height of the paragraph is increased with the height of this line. */
    pParagraph->m_iHeight += lineInfo.m_iHeight;
  }
}

void ParagraphLeftPositionSpaceWidth(PARAGRAPH* pParagraph,
                          LINE_INFO lineInfo, LIST* pSizeList,
                          LIST* pAsscentList, int* pxLeftPos,
                          int *piSpaceWidth)
{
  int iIndex, iLineWidth = 0, iSpaces = 0;
  for (iIndex = lineInfo.m_iFirstChar;
       iIndex <= lineInfo.m_iLastChar; ++iIndex) {
    SIZE szChar;
    CHAR_DESCR charInfo;
/* If the paragraph has justified alignment, we count the number of spaces on the line. */
    ListGet(&pParagraph->m_charInfoList, iIndex, &charInfo);
    ListGet(pSizeList, iIndex, &szChar);

    if ((pParagraph->m_iAlignment == ID_ALIGN_JUSTIFIED) &&
        (charInfo.m_cChar == TEXT(' '))) {
      ++iSpaces;
    }
    else {
      iLineWidth += szChar.cx;
    }
  }
/* The width of the lineInfo is the sum of the width of all characters. */
/* We find the start position of the lineInfo   }
by considering the alignment of the paragraph and the width of the lineInfo. */
  switch (pParagraph->m_iAlignment) {
/* If left alignment, the lineInfo start at the left side. */
    case ID_ALIGN_LEFT:
      *pxLeftPos = 0;
      break;
/* If center and right alignment, we compute the start position by comparing width of the lineInfo with the width of the page. */
    case ID_ALIGN_CENTER:
      *pxLeftPos = (PAGE_WIDTH - iLineWidth) / 2;
      break;

    case ID_ALIGN_RIGHT:
      *pxLeftPos = PAGE_WIDTH - iLineWidth;
      break;
/* If justified alignment, we need to find the number of spaces on the lineInfo and calculate the width of each space in order for the lineInfo to completely fill the width of the page. */
    case ID_ALIGN_JUSTIFIED: {
        *pxLeftPos = 0;
        *piSpaceWidth = (iSpaces > 0) ?
                    ((PAGE_WIDTH - iLineWidth) / iSpaces) : 0;
      }
      break;
  }
}
/* Finally, we calculate the rectangle for each character. We traverse the line and with the sizes of the characters and the height and ascent line of the line we find each rectangle. We start by the start position we found above and increase the position for each character on the line. */
void ParagraphGenerateLineRectList(PARAGRAPH* pParagraph,
                LINE_INFO lineInfo, LIST* pSizeList,
                LIST*pAscentList,int xLeftPos,int iSpaceWidth)
{
  int iIndex;

  for (iIndex = lineInfo.m_iFirstChar;
       iIndex <= lineInfo.m_iLastChar; ++iIndex) {
    CHAR_DESCR c;
    SIZE szChar;
    int iWidth, iAscent;

    ListGet(pSizeList, iIndex, &szChar);
    ListGet(pAscentList, iIndex, &iAscent);
    ListGet(&pParagraph->m_charInfoList, iIndex, &c);
/* If the paragraph has justified alignment and the character is a space, we use the space width calculated by ${ParagraphLeftPositionSpaceWidth} above; otherwise, we use the size of the character. */
    iWidth = ((c.m_cChar == TEXT(' ')) &&
             (pParagraph->m_iAlignment == ID_ALIGN_JUSTIFIED))
             ? iSpaceWidth : szChar.cx;

    { int yTopPos = pParagraph->m_iHeight +
                    lineInfo.m_iAscent -iAscent;
      RECT rcChar = {xLeftPos, yTopPos, xLeftPos + iWidth,
                     yTopPos + szChar.cy};
      c.m_rcRect = rcChar;
      ListSet(&pParagraph->m_charInfoList, iIndex, &c);
    }

    xLeftPos += iWidth;
  }
/* If the last character on this line is not the last one of the paragraph and is followed by a space, we add a rectangle for that space. We use the ${yTopPos}, ${xLeftPos}, and ${iHeight} variables from thew loop above, they are set by the last iteration of the loop. As each line holds at least one character, they have been properly set. */
  if ((lineInfo.m_iLastChar + 1) <
      ListSize(&pParagraph->m_charInfoList))
  {
    CHAR_DESCR c;
    ListGet(&pParagraph->m_charInfoList,
            lineInfo.m_iLastChar + 1, &c);

    if (c.m_cChar == TEXT(' ')) {
      SIZE szChar;
      int iAscent;
      ListGet(pSizeList, lineInfo.m_iLastChar + 1, &szChar);
      ListGet(pAscentList, iIndex, &iAscent);

      { int yTopPos = pParagraph->m_iHeight +
                      lineInfo.m_iAscent - iAscent;
        RECT rcChar =
          {xLeftPos, yTopPos, xLeftPos + szChar.cx,
           yTopPos + szChar.cy};
        c.m_rcRect = rcChar;
        ListSet(&pParagraph->m_charInfoList,
                lineInfo.m_iLastChar + 1, &c);
      }
    }
  }
}
/* When a paragraph has been altered, we have to repaint the altered area of the client area. However, we do not want to repaint the whole paragraph, just the characters that need to be updated. GetRepaintSet compares the original rectangle list with the newly generated one and fills the invalidate set with every rectangle that has been altered. */
void ParagraphGetRectSet(PARAGRAPH* pParagraph,
                     LIST* pOldCharInfoList, SET* pRepaintSet)
{
/* Remember that the position of each character is relative its own paragraph; therefore, we start by defining the top left corner of the paragraph relative the document. */
/* We traverse the characters and add those that has been given new dimensions. */
  int iIndex, iSize = ListSize(pOldCharInfoList);

  for (iIndex = 0; iIndex < iSize; ++iIndex) {
    CHAR_DESCR o, n;
    ListGet(pOldCharInfoList, iIndex, &o);
    ListGet(&pParagraph->m_charInfoList, iIndex, &n);

    if (RectComp(&o.m_rcRect, &n.m_rcRect) != 0) {
      if (!IsRectEmpty(&o.m_rcRect)) {
        RECT rcAbsOldChar = o.m_rcRect;
        rcAbsOldChar.top += pParagraph->m_yStartPos;
        rcAbsOldChar.bottom += pParagraph->m_yStartPos;
        SetAdd(pRepaintSet, &rcAbsOldChar);
      }

      if (!IsRectEmpty(&n.m_rcRect)) {
        RECT rcAbsNewChar = n.m_rcRect;
        rcAbsNewChar.top += pParagraph->m_yStartPos;
        rcAbsNewChar.bottom += pParagraph->m_yStartPos;
        SetAdd(pRepaintSet, &rcAbsNewChar);
      }
    }
  }
}
#if 0
/* ${ExtendRectangleSet} extands each rectangle in the set due to updating errors. */
void ExtendRectangleSet(SET* pRectSet)
{
  RECT* prcArea;
  ITERATOR iterator = IteratorCreate(pRectSet);

  while ((prcArea = IteratorNextPtr(&iterator)) != NULL) {
    int cxWidth = (int) (EXTEND_FACTOR *
                         (prcArea->right - prcArea->left)),
        cyHeight = (int) (EXTEND_FACTOR *
                          (prcArea->bottom - prcArea->top));
/*
    prcArea->left -= cxWidth;
    prcArea->right += cxWidth;
    prcArea->top -= cyHeight;
    prcArea->bottom += cyHeight;*/
  }
}
#endif