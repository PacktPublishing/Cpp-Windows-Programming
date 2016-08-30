#include <Windows.h>
#include <StdIO.h>
#include <TChar.h>
#include <WChar.h>
#include <String.h>
#include <Assert.h>
#include "Resource.h"
#include "..\\List.h"
#include "..\\Set.h"
#include "..\\Unit.h"
#include "..\\UnitScroll.h"
#include "UnitScrollZoom.h"
#include "Position.h"
#include "Paragraph.h"
#include "Calculate.h"
#include "Resource.h"
#include "Word.h"
#include "Command.h"
#include "Keyboard.h"
#include "Clipboard.h"

int AsciiCharInfoSize(CHAR_DESCR* pCharInfo, LIST* pList, int iIndex, void* pParam)
{
  return sizeof(char);
}

int AsciiParagraphSize(PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  return ListByteSize(&pParagraph->m_charInfoList, FALSE, &AsciiCharInfoSize, NULL) + 2 * sizeof(char);
}

int AsciiCharInfoSave(char* pcBuffer, CHAR_DESCR* pCharInfo, LIST* pList, int iIndex, void* pParam)
{
  memcpy(pcBuffer, &pCharInfo->m_cChar, sizeof(char));
  return sizeof(char);
}

int AsciiParagraphSave(void* pBuffer, PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  int iSize = ListByteSave(&pParagraph->m_charInfoList, pBuffer, FALSE, &AsciiCharInfoSave, NULL);
  char* pcBuffer = (char*) pBuffer;
  char cEndChar = (iIndex < (ListSize(pList) - 1)) ? '\n' : '\0';
  memcpy(pcBuffer + iSize, &cEndChar, 2 * sizeof(char));
  return iSize + sizeof(char);
}

void AsciiSave(HWND hWnd, LIST* pCopyList)
{
  if (OpenClipboard(hWnd) && EmptyClipboard()) {
    int iBufferSize = ListByteSize(pCopyList, FALSE, &AsciiParagraphSize, NULL);
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, iBufferSize + sizeof(char));
    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);
      if (pBuffer != NULL) {
        ListByteSave(pCopyList, pBuffer, FALSE, &AsciiParagraphSave, NULL);
        GlobalUnlock(hGlobal);
        SetClipboardData(CF_TEXT, hGlobal);
      }
    }
    CloseClipboard();
  }
}

int AsciiCharInfoLoad(char* pcBuffer, CHAR_DESCR* pCharInfo, BOOL* pbContinue, LIST* pList, int iIndex, FONT_INFO* pFontInfo)
{
  pCharInfo->m_cChar = pcBuffer[0];
  pCharInfo->m_fontInfo = *pFontInfo;

  if (pcBuffer[1] == '\n') {
    *pbContinue = FALSE;
    return 3 * sizeof(char);
  }
  else {
    *pbContinue = (pcBuffer[1] != '\0');
    return sizeof(char);
  }
}

int AsciiParagraphLoad(char* pcBuffer, PARAGRAPH** ppParagraph, BOOL* pbContinue, LIST* pList, int iIndex, FONT_INFO* pFontInfo)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  int iSize = ListByteLoadUntil(&pParagraph->m_charInfoList, pcBuffer, NULL, NULL, &AsciiCharInfoLoad, pFontInfo);
  *pbContinue = (pcBuffer[iSize] != '\0');
  return iSize;
}

void AsciiLoad(HWND hWnd, LIST* pPasteList, FONT_INFO* pFontInfo)
{
  if (OpenClipboard(hWnd)) {
    HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);
      if (pBuffer != NULL) {
        ListByteLoadUntil(pPasteList, pBuffer, NULL, NULL, &AsciiParagraphLoad, pFontInfo);
        GlobalUnlock(hGlobal);
      }
    }
    CloseClipboard(); 
  }
}

#if 0
BOOL AsciiParagraphLoad(void* pBuffer, int* piBufferIndex, PARAGRAPH* pParagraph, FONT_INFO* pFontInfo)
{
  char* pcBuffer = (char*) pBuffer;

  while (TRUE) {
    char cChar = pcBuffer[(*piBufferIndex)++];

    switch (cChar) {
      case '\n':
        ++(*piBufferIndex); // '\r'
        return TRUE;

      case '\0':
        return FALSE;

      default: {
          CHAR_DESCR charInfo;
          charInfo.m_cChar = (TCHAR) cChar;
          charInfo.m_fontInfo = *pFontInfo;
          ListAdd(&pParagraph->m_charInfoList, &charInfo);
        }
        break;
    }
  }
}

void AsciiLoad(HWND hWnd, LIST* pPasteList, FONT_INFO* pFontInfo)
{
  if (OpenClipboard(hWnd)) {
    HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);
      if (pBuffer != NULL) {
        int iBufferIndex = 0;

        while (TRUE) {
          PARAGRAPH* pParagraph = ParagraphEmpty(hWnd, NULL);
          BOOL bContinue = AsciiParagraphLoad(pBuffer, &iBufferIndex, pParagraph, pFontInfo);
          ListAdd(pPasteList, &pParagraph);
          if (!bContinue) {
            break;
          }
        }
        
        GlobalUnlock(hGlobal);
        SetClipboardData(CF_TEXT, hGlobal);
      }
    }
    CloseClipboard(); 
  }
}
#endif

int UnicodeCharInfoSize(CHAR_DESCR* pCharInfo, LIST* pList, int iIndex, void* pParam)
{
  return sizeof(TCHAR);
}

int UnicodeParagraphSize(PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  return ListByteSize(&pParagraph->m_charInfoList, FALSE, &UnicodeCharInfoSize, NULL);
}

int UnicodeCharInfoSave(char* pcBuffer, CHAR_DESCR* pCharInfo, LIST* pList, int iIndex, void* pParam)
{
  memcpy(pcBuffer, &pCharInfo->m_cChar, sizeof(TCHAR));
  return sizeof(TCHAR);
}

int UnicodeParagraphSave(void* pBuffer, PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  int iSize = ListByteSave(&pParagraph->m_charInfoList, pBuffer, FALSE, &UnicodeCharInfoSave, NULL);
  char* pcBuffer = (char*) pBuffer;
  TCHAR cEndChar = (iIndex < (ListSize(pList) - 1)) ? TEXT('\n') : TEXT('\0');
  memcpy(pcBuffer + iSize, &cEndChar, 2 * sizeof(TCHAR));
  return iSize + sizeof(TCHAR);
}

void UnicodeSave(HWND hWnd, LIST* pCopyList)
{
  if (OpenClipboard(hWnd) && EmptyClipboard()) {
    int iBufferSize = ListByteSize(pCopyList, FALSE, &UnicodeParagraphSize, NULL);
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, iBufferSize + sizeof(TCHAR));
    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);
      if (pBuffer != NULL) {
        ListByteSave(pCopyList, pBuffer, FALSE, &UnicodeParagraphSave, NULL);
        GlobalUnlock(hGlobal);
        SetClipboardData(CF_UNICODETEXT, hGlobal);
      }
    }
    CloseClipboard();
  }
}

int UnicodeCharInfoLoad(TCHAR* pcBuffer, CHAR_DESCR* pCharInfo, BOOL* pbContinue, LIST* pList, int iIndex, FONT_INFO* pFontInfo)
{
  pCharInfo->m_cChar = pcBuffer[0];
  pCharInfo->m_fontInfo = *pFontInfo;

  if (pcBuffer[1] == TEXT('\n')) {
    *pbContinue = FALSE;
    return 3 * sizeof(TCHAR);
  }
  else {
    *pbContinue = (pcBuffer[1] != TEXT('\0'));
    return sizeof(TCHAR);
  }
}

int UnicodeParagraphLoad(TCHAR* pcBuffer, PARAGRAPH** ppParagraph, BOOL* pbContinue, LIST* pList, int iIndex, FONT_INFO* pFontInfo)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  int iSize = ListByteLoadUntil(&pParagraph->m_charInfoList, pcBuffer, NULL, NULL, &UnicodeCharInfoLoad, pFontInfo);
  *pbContinue = (pcBuffer[iSize] != TEXT('\0'));
  return iSize;
}

void UnicodeLoad(HWND hWnd, LIST* pPasteList, FONT_INFO* pFontInfo)
{
  if (OpenClipboard(hWnd)) {
    HGLOBAL hGlobal = GetClipboardData(CF_UNICODETEXT);
    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);
      if (pBuffer != NULL) {
        ListByteLoadUntil(pPasteList, pBuffer, NULL, NULL, &UnicodeParagraphLoad, pFontInfo);
        GlobalUnlock(hGlobal);
      }
    }
    CloseClipboard(); 
  }
}

#if 0
BOOL UnicodeParagraphLoad(void* pBuffer, int* piBufferIndex, PARAGRAPH* pParagraph, FONT_INFO* pFontInfo)
{
  TCHAR* pcBuffer = (TCHAR*) pBuffer;

  while (TRUE) {
    TCHAR cChar = pcBuffer[(*piBufferIndex)++];

    switch (cChar) {
      case TEXT('\n'):
        ++(*piBufferIndex); // TEXT('\r')
        return TRUE;

      case TEXT('\0'):
        return FALSE;

      default: {
          CHAR_DESCR charInfo;
          charInfo.m_cChar = cChar;
          charInfo.m_fontInfo = *pFontInfo;
          ListAdd(&pParagraph->m_charInfoList, &charInfo);
        }
        break;
    }
  }
}

void UnicodeLoad(HWND hWnd, LIST* pPasteList, FONT_INFO* pFontInfo)
{
  if (OpenClipboard(hWnd)) {
    HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);
      if (pBuffer != NULL) {
        int iBufferIndex = 0;

        while (TRUE) {
          PARAGRAPH* pParagraph = ParagraphEmpty(hWnd, NULL);
          BOOL bContinue = UnicodeParagraphLoad(pBuffer, &iBufferIndex, pParagraph, pFontInfo);
          ListAdd(pPasteList, &pParagraph);
          if (!bContinue) {
            break;
          }
        }
        
        GlobalUnlock(hGlobal);
        SetClipboardData(CF_TEXT, hGlobal);
      }
    }
    CloseClipboard(); 
  }
}
#endif

int InternalCharInfoSize(CHAR_DESCR* pCharInfo, LIST* pList, int iIndex, void* pParam)
{
  return sizeof(TCHAR) + sizeof(FONT_INFO);
}

int InternalParagraphSize(PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  return ListByteSize(&pParagraph->m_charInfoList, TRUE, &InternalCharInfoSize, NULL);
}

int InternalCharInfoSave(char* pcBuffer, CHAR_DESCR* pCharInfo, LIST* pList, int iIndex, void* pParam)
{
  memcpy(pcBuffer, &pCharInfo->m_cChar, sizeof(TCHAR));
  memcpy(pcBuffer + sizeof(TCHAR), &pCharInfo->m_fontInfo, sizeof(FONT_INFO));
  return sizeof(TCHAR) + sizeof(FONT_INFO);
}

int InternalParagraphSave(void* pBuffer, PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  return ListByteSave(&pParagraph->m_charInfoList, pBuffer, TRUE, &InternalCharInfoSave, NULL);
}

void InternalSave(HWND hWnd, LIST* pCopyList)
{
  if (OpenClipboard(hWnd) && EmptyClipboard()) {
    int iBufferSize = ListByteSize(pCopyList, TRUE, &InternalParagraphSize, NULL);
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, iBufferSize);

    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);

      if (pBuffer != NULL) {
        ListByteSave(pCopyList, pBuffer, TRUE, &InternalParagraphSave, NULL);
        GlobalUnlock(hGlobal);
        SetClipboardData(g_uiInternalFormat, hGlobal);
      }
    }
    CloseClipboard();
  }
}

int InternalCharLoad(char* pcBuffer, CHAR_DESCR* pCharInfo, LIST* pList, int iIndex, void* pParam)
{
  memcpy(&pCharInfo->m_cChar, pcBuffer, sizeof(TCHAR));
  memcpy(&pCharInfo->m_fontInfo, pcBuffer + sizeof(TCHAR), sizeof(FONT_INFO));
  return sizeof(TCHAR) + sizeof(FONT_INFO);
}

int InternalParagraphLoad(void* pBuffer, PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  PARAGRAPH* pParagraph = *ppParagraph;
  return ListByteLoadSize(&pParagraph->m_charInfoList, pBuffer, NULL, NULL, &InternalCharLoad, NULL);
}

void InternalParagraphInit(PARAGRAPH** ppParagraph, LIST* pList, int iIndex, void* pParam)
{
  *ppParagraph = ParagraphEmpty((HWND) pParam, NULL);
}

void InternalLoad(HWND hWnd, LIST* pasteList)
{
  if (OpenClipboard(hWnd)) {
    HGLOBAL hGlobal = GetClipboardData(g_uiInternalFormat);
    if (hGlobal != NULL) {
      void* pBuffer = GlobalLock(hGlobal);

      if (pBuffer != NULL) {
        ListByteLoadSize(pasteList, pBuffer, &InternalParagraphInit, hWnd, &InternalParagraphLoad, NULL);
        GlobalUnlock(hGlobal);
      }
    }
    CloseClipboard();
  }
}

void OnEditCopy(HWND hWnd)
{
/* The first (${g_psFirstMark}) and last (${g_psLastMark}) may be reversed. Therefore, the ${psMin} and ${psMax} variables are set to the first and last positions in of the marked area. */
  POSITION psMin = PositionMin(g_psFirstMark, g_psLastMark);
  POSITION psMax = PositionMax(g_psFirstMark, g_psLastMark);
/* We clear the copy array holding the previously copied paragraphs. */
  LIST copyList = ListEmpty(sizeof(PARAGRAPH));
/* In only one paragraph is marked, we simple extract its marked portion and add the resulting paragraph to the copy list. */
  if (psMin.m_iParagraph == psMax.m_iParagraph) {
    PARAGRAPH *pSourceParagraph, *pTargetParagraph;
    ListGet(&g_paragraphList, psMin.m_iParagraph,
            &pSourceParagraph);
    pTargetParagraph =
           ParagraphExtractText(hWnd, pSourceParagraph,
                                psMin.m_iChar, psMax.m_iChar);
    ListAdd(&copyList, &pTargetParagraph);
  }
/* If at least two paragraphs are marked, we extract the marked portions of the first and last ones. We also add the paragraphs in between, in any. */
  else {
    PARAGRAPH* pParagraph;
    ListGet(&g_paragraphList, psMin.m_iParagraph,
            &pParagraph);

    { int iParagraph;
      PARAGRAPH* pCopyParagraph =
        ParagraphExtractText(hWnd, pParagraph,psMin.m_iChar, -1);
      ListAdd(&copyList, &pCopyParagraph);

      for (iParagraph = psMin.m_iParagraph + 1;
           iParagraph < psMax.m_iParagraph; ++iParagraph) {
        PARAGRAPH *pParagraph;
        ListGet(&g_paragraphList, iParagraph, &pParagraph);
        ListAdd(&copyList, &pParagraph);
      }

      ListGet(&g_paragraphList, psMax.m_iParagraph,
              &pParagraph);
      pCopyParagraph =
      ParagraphExtractText(hWnd, pParagraph, 0, psMax.m_iChar);
      ListAdd(&copyList, &pCopyParagraph);
    }
  }

//  InternalSave(hWnd, &copyList);
//  UnicodeSave(hWnd, &copyList);
  AsciiSave(hWnd, &copyList);

  ListFree(&copyList);
}

void OnEditPaste(HWND hWnd)
{
  FONT_INFO fontInfo;
  LIST pasteList = LIST_EMPTY(sizeof(PARAGRAPH));

  { PARAGRAPH* pParagraph;
    ListGet(&g_paragraphList, g_psEdit.m_iParagraph, &pParagraph);
    ParagraphGetFont(pParagraph, g_psEdit.m_iChar, &fontInfo);
  }

  if (IsClipboardFormatAvailable(g_uiInternalFormat)) {
    InternalLoad(hWnd, &pasteList);
  }
  else if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {
    UnicodeLoad(hWnd, &pasteList, &fontInfo);
  }
  else if (IsClipboardFormatAvailable(CF_TEXT)) {
    AsciiLoad(hWnd, &pasteList, &fontInfo);
  }

  if (ListSize(&pasteList) == 0) {
    return;
  }

  { PARAGRAPH* pEditParagraph;
    PARAGRAPH* pNextParagraph;
    int iPasteSize = ListSize(&pasteList);

    ListGet(&g_paragraphList, g_psEdit.m_iParagraph, &pEditParagraph);
    pNextParagraph = ParagraphSplit(hWnd, pEditParagraph, g_psEdit.m_iChar);

    if (iPasteSize == 1) {
      PARAGRAPH* pPasteParagraph;
      ListGet(&pasteList, 0, &pPasteParagraph);
      g_psEdit.m_iChar += ParagraphSize(pPasteParagraph);
      ParagraphAppend(pEditParagraph, pPasteParagraph);
      ParagraphAppend(pEditParagraph, pNextParagraph);
      ParagraphCalculate(hWnd, pEditParagraph, NULL, 0, NULL);
      pEditParagraph->m_yStartPos = -1;
      ParagraphFree(&pPasteParagraph, NULL, 0, NULL);
      ParagraphFree(&pNextParagraph, NULL, 0, NULL);
    }

    else {
      int iParagraph;
      PARAGRAPH* pFirstParagraph;
      PARAGRAPH* pLastParagraph;

      ListGet(&pasteList, 0, &pFirstParagraph);
      ParagraphAppend(pEditParagraph, pFirstParagraph);
      ParagraphCalculate(hWnd, pEditParagraph, NULL, 0, NULL);
      ParagraphFree(&pFirstParagraph, NULL, 0, NULL);
      pEditParagraph->m_yStartPos = -1;

      for (iParagraph = 0; iParagraph < iPasteSize; ++iParagraph) {
        PARAGRAPH* pParagraph;
        ListGet(&pasteList, iParagraph, &pParagraph);
        ParagraphCalculate(hWnd, pParagraph, NULL, 0, NULL);
        pParagraph->m_yStartPos = -1;
        ListInsert(&g_paragraphList, g_psEdit.m_iParagraph + iParagraph, pParagraph);
      }

      ListGet(&pasteList, iPasteSize - 1, &pLastParagraph);
      g_psEdit.m_iChar = ParagraphSize(pLastParagraph);

      ParagraphAppend(pLastParagraph, pNextParagraph);
      ParagraphCalculate(hWnd, pLastParagraph, NULL, 0, NULL);
      ListSet(&g_paragraphList, g_psEdit.m_iParagraph + iPasteSize - 1, pLastParagraph);
      ParagraphFree(&pNextParagraph, NULL, 0, NULL);
      pLastParagraph->m_yStartPos = -1;
      g_psEdit.m_iParagraph += (iPasteSize - 1);
    }

    UpdateParagraphAndPageList(hWnd);
    UpdateCaret(hWnd);
  }

  ListFree(&pasteList);
}
