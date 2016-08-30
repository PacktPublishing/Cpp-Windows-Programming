void ParagraphCalculate(HWND Paragraph* pParagraph,
                        LOGFONT* pNextFontInfo, int iFont, set<Rect>* pRepaintSet);
void ParagraphGenerateEmptyList(Paragraph* pParagraph, LOGFONT* plfFont, HDC hDC);
void ParagraphGenerateSizeAscentList(Paragraph* pParagraph,
                               LIST* pSizeList, LIST* pAscentList, HDC hDC);
void ParagraphGenerateLineList(Paragraph* pParagraph, LOGFONT* pFont, int iFont,
                               LIST* pSizeList, LIST* pAscentList, HDC hDC);
void ParagraphGenerateRectList(Paragraph* pParagraph,
                               LIST* pSizeList, LIST* pAscentList);
void ParagraphLeftPositionSpaceWidth(Paragraph* pParagraph,
                               LineInfo lineInfo, LIST* pSizeList,
                               LIST* pAsscentList,
                               int* pxLeftPos, int *piSpaceWidth);
void ParagraphGenerateLineRectList(Paragraph* pParagraph, LineInfo lineInfo,
                                   LIST* pSizeList, LIST* pAscentList,
                                   int xLeftPos, int iSpaceWidth);
void ParagraphGetRectSet(Paragraph* pParagraph, LIST* pOldCharacList, set<Rect>* pRepaintSet);
//void ParagraphGetWhiteSet(Paragraph* pParagraph, set<Rect>* pRepaintSet);
//void OverlapBlocks(Paragraph* pParagraph, set<Rect>* pOldBlockSet, set<Rect>* pRepaintSet);
//void ExtendRectangleSet(set<Rect>* pRectSet);
