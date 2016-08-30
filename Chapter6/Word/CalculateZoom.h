void GetTextZoomMetrics(HDC hDC, int iZoom, LPTEXTMETRIC pTextMetric);
BOOL GetTextExtentZoomPoint(HDC hDC, int iZoom, LPCTSTR lpString, int cbString, LPSIZE lpSize);

void ParagraphCalculate(HWND hWnd, PARAGRAPH* pParagraph,
                        LOGFONT* pNextFontInfo, int iFont, SET* pRepaintSet);
void ParagraphGenerateEmptyList(PARAGRAPH* pParagraph, LOGFONT* plfFont, HDC hDC);
void ParagraphGenerateSizeAscentList(PARAGRAPH* pParagraph,
                               LIST* pSizeList, LIST* pAscentList, HDC hDC);
void ParagraphGenerateLineList(PARAGRAPH* pParagraph, LOGFONT* pFont, int iFont,
                               LIST* pSizeList, LIST* pAscentList, HDC hDC);
void ParagraphGenerateRectList(PARAGRAPH* pParagraph,
                               LIST* pSizeList, LIST* pAscentList);
void ParagraphLeftPositionSpaceWidth(PARAGRAPH* pParagraph,
                               LINE_INFO lineInfo, LIST* pSizeList,
                               LIST* pAsscentList,
                               int* pxLeftPos, int *piSpaceWidth);
void ParagraphGenerateLineRectList(PARAGRAPH* pParagraph, LINE_INFO lineInfo,
                                   LIST* pSizeList, LIST* pAscentList,
                                   int xLeftPos, int iSpaceWidth);
void ParagraphGetRectSet(PARAGRAPH* pParagraph, LIST* pOldCharacList, SET* pRepaintSet);
//void ParagraphGetWhiteSet(PARAGRAPH* pParagraph, SET* pRepaintSet);
//void OverlapBlocks(PARAGRAPH* pParagraph, SET* pOldBlockSet, SET* pRepaintSet);
//void ExtendRectangleSet(SET* pRectSet);
