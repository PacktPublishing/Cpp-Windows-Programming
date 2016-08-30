#include "..\\SmallWindows\\SmallWindows.h"
#include "Template.h"
#include "PrintSetupDialog.h"
#include "PrintMessageDialog.h"
#include "PrintPreviewWindow.h"
#include "Position.h"
#include "CharInfo.h"
#include "Paragraph.h"
#include "WordDocument.h"

int WordDocument::BufferSize(Format format) {
  int iSum;

  if (m_psFirstMark.m_iPara == m_psFirstMark.m_iPara) {
    iSum = m_psLastMark.m_iChar - m_psFirstMark.m_iChar + 1;
  }
  else {
    int iSum = m_paraList[m_psFirstMark.m_iPara]->Length() - m_psLastMark.m_iChar;
    iSum += 2; // '\n' '\r';

    for (int iPara = m_psFirstMark.m_iPara + 1; iPara <= m_psLastMark.m_iPara - 1; ++iPara) {
      iSum += m_paraList[iPara]->Length();
      iSum += 2; // '\n' '\r';
    }

    iSum += m_psLastMark.m_iChar + 1;
  }

  ++iSum; // '\0';

  switch (format) {
    case Ascii:
      return iSum * (sizeof (char));

    case Unicode:
      return iSum * (sizeof (wchar_t));

    case Word:
      return iSum * ((sizeof (TCHAR)) + (sizeof (LOGFONT)) + (sizeof (COLORREF)));
  }

  return 0;
}

void* Paragraph::BufferAsciiWrite(void *pBuffer, bool bLast, int iFirstChar, int iLastChar) {
  char* pcBuffer = (char*) pBuffer;
  iLastChar == (iLastChar == -1) ? (m_charList.size() - 2) : iLastChar;

  for (int iChar = iFirstChar; iChar <= iLastChar; ++iChar) {
    CharInfo charInfo = m_charList[iChar];
    char cChar = (char) &charInfo.m_tChar;
    memcpy(pcBuffer, &cChar, sizeof cChar);
    pcBuffer += sizeof cChar;
  }

  if (bLast) {
    char cZero = '\0';
    memcpy(pcBuffer, &cZero, sizeof cZero);
    return NULL;
  }
  else {
    char stReturn[] = { '\n', '\r' };
    memcpy(pcBuffer, stReturn, sizeof stReturn);
    return (pcBuffer + sizeof stReturn);
  }
}

void* Paragraph::BufferUnicodeWrite(void *pBuffer, bool bLast, int iFirstChar, int iLastChar) {
  char* pcBuffer = (char*) pBuffer;
  iLastChar == (iLastChar == -1) ? (m_charList.size() - 2) : iLastChar;

  for (int iChar = iFirstChar; iChar <= iLastChar; ++iChar) {
    CharInfo charInfo = m_charList[iChar];
    wchar_t wChar = (wchar_t)&charInfo.m_tChar;
    memcpy(pcBuffer, &wChar, sizeof wChar);
    pcBuffer += sizeof wChar;
  }

  if (bLast) {
    wchar_t wZero = L'\0';
    memcpy(pcBuffer, &wZero, sizeof wZero);
    return NULL;
  }
  else {
    wchar_t stReturn[] = { L'\n', L'\r' };
    memcpy(pcBuffer, stReturn, sizeof stReturn);
    return (pcBuffer + sizeof stReturn);
  }
}

void* Paragraph::BufferWordWrite(void *pBuffer, bool bLast, int iFirstChar, int iLastChar) {
  char* pcBuffer = (char*) pBuffer;
  iLastChar == (iLastChar == -1) ? (m_charList.size() - 2) : iLastChar;

  for (int iChar = iFirstChar; iChar <= iLastChar; ++iChar) {
    CharInfo charInfo = m_charList[iChar];
    memcpy(pcBuffer, &charInfo.m_tChar, sizeof(TCHAR));
    pcBuffer += sizeof(TCHAR);

    LOGFONT logFont = charInfo.m_font.GetLogFont();
    memcpy(pcBuffer, &logFont, sizeof (LOGFONT));
    pcBuffer += sizeof(LOGFONT);

    COLORREF colorRef = charInfo.m_font.GetColorRGB();
    memcpy(pcBuffer, &colorRef, sizeof (COLORREF));
    pcBuffer += sizeof(COLORREF);
    break;
  }

  if (bLast) {
    TCHAR returnChar = TEXT('\n');
    memcpy(pcBuffer, &returnChar, sizeof(TCHAR));
    pcBuffer += sizeof(TCHAR);

    memset(pcBuffer, 0, sizeof (LOGFONT) + sizeof (COLORREF));
    pcBuffer += sizeof(LOGFONT) + sizeof (COLORREF);

    TCHAR lineFeedChar = TEXT('\r');
    memcpy(pcBuffer, &lineFeedChar, sizeof(TCHAR));
    pcBuffer += sizeof(TCHAR);

    memset(pcBuffer, 0, sizeof (LOGFONT) + sizeof (COLORREF));
    return (pcBuffer + sizeof(LOGFONT) + sizeof (COLORREF));
  }
  else {
    TCHAR zeroChar = TEXT('\0');
    memcpy(pcBuffer, &zeroChar, sizeof(TCHAR));
    pcBuffer += sizeof(TCHAR);

    memset(pcBuffer, 0, sizeof (LOGFONT) + sizeof (COLORREF));
    return NULL;
  }
}

void* Paragraph::BufferWrite(Format format, void *pBuffer, bool bLast, int iFirstChar, int iLastChar) {
  switch (format) {
    case Ascii:
      return BufferAsciiWrite(pBuffer, bLast, iFirstChar, iLastChar);

    case Unicode:
      return BufferUnicodeWrite(pBuffer, bLast, iFirstChar, iLastChar);

    case Word:
      return BufferWordWrite(pBuffer, bLast, iFirstChar, iLastChar);
  }

  return 0;
}

void WordDocument::BufferWrite(Format format, void *pBuffer) {
  char* pcBuffer = (char*) pBuffer;

  if (m_psFirstMark.m_iPara == m_psLastMark.m_iPara) {
    m_paraList[m_psFirstMark.m_iPara]->BufferWrite(format, pcBuffer, true, m_psFirstMark.m_iChar, m_psLastMark.m_iChar);
  }
  else {
    pcBuffer = (char*) m_paraList[m_psFirstMark.m_iPara]->BufferWrite(format, pcBuffer, false, m_psFirstMark.m_iChar);

    for (int iPara = m_psFirstMark.m_iPara + 1; iPara <= m_psLastMark.m_iPara - 1; ++iPara) {
      pcBuffer = (char*) m_paraList[iPara]->BufferWrite(format, pcBuffer, false);
    }

    m_paraList[m_psLastMark.m_iPara]->BufferWrite(format, pcBuffer, true, 0, m_psLastMark.m_iChar);
  }
}

void WordDocument::BufferSave(Format format) {
  HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, BufferSize(format));

  if (hGlobal != NULL) {
    void* pBuffer = GlobalLock(hGlobal);

    if (pBuffer != NULL) {
      BufferWrite(format, pBuffer);
      GlobalUnlock(hGlobal);
      SetClipboardData(format, hGlobal);
    }
  }
}

void* Paragraph::BufferAsciiRead(void *pBuffer) {
  char *pcBuffer = (char*) pBuffer;

  while (true) {
    char cChar;
    memcpy(&cChar, pcBuffer++, sizeof cChar);

    switch (cChar) {
      case '\n':
        return (pcBuffer + 1);

      case '\0':
        m_charList.push_back(CharInfo((TCHAR) '\0'));
        return NULL;

      default:
        m_charList.push_back(CharInfo((TCHAR) cChar));
        break;
    }
  }
}

void* Paragraph::BufferUnicodeRead(void *pBuffer) {
  wchar_t* pwBuffer = (wchar_t*) pBuffer;

  while (true) {
    wchar_t wChar;
    memcpy(&wChar, pwBuffer++, sizeof wChar);

    switch (wChar) {
      case L'\n':
        return (pwBuffer + 1);

      case L'\0':
        m_charList.push_back(CharInfo((TCHAR) L'\0'));
        return NULL;

      default:
        m_charList.push_back(CharInfo((TCHAR) wChar));
        break;
    }
  }
}

void* Paragraph::BufferWordRead(void *pBuffer) {
  char *pcBuffer = (char*) pBuffer;

  while (true) {
    CharInfo charInfo;
    memcpy(pcBuffer, &charInfo.m_tChar, sizeof(TCHAR));
    pcBuffer += sizeof (TCHAR);

    LOGFONT logFont;
    memcpy(&logFont, pcBuffer, sizeof (LOGFONT));
    pcBuffer += sizeof (LOGFONT);

    COLORREF colorRef;
    memcpy(&colorRef, pcBuffer, sizeof (COLORREF));
    pcBuffer += sizeof (COLORREF);
    charInfo.m_font = Font(logFont, colorRef);

    switch (charInfo.m_tChar) {
      case TEXT('\n'):
        return (pcBuffer + (sizeof (TCHAR) + sizeof (LOGFONT) + sizeof (COLORREF)));

      case TEXT('\0'):
        m_charList.push_back(charInfo);
        return NULL;

      default:
        m_charList.push_back(charInfo);
        break;
    }
  }
}

void* Paragraph::BufferRead(Format format, void *pBuffer) {
  switch (format) {
    case Ascii:
      return BufferAsciiRead(pBuffer);

    case Unicode:
      return BufferUnicodeRead(pBuffer);

    case Word:
      return BufferWordRead(pBuffer);
  }

  return 0;
}

void WordDocument::BufferRead(Format format, void *pBuffer, vector<Paragraph*> pasteList) {
  while (pBuffer != NULL) {
    Paragraph* pParagraph = new Paragraph(this);
    pBuffer = pParagraph->BufferRead(format, pBuffer);
    pasteList.push_back(pParagraph);
  }
}

void WordDocument::BufferLoad(Format format, vector<Paragraph*> pasteList) {
  HGLOBAL hGlobal = GetClipboardData(format);

  if (hGlobal != NULL) {
    void* pBuffer = GlobalLock(hGlobal);

    if (pBuffer != NULL) {
      BufferRead(format, pBuffer, pasteList);
      GlobalUnlock(hGlobal);
    }
  }
}

bool WordDocument::CopyEnable() {
  return (m_wordStatus == WordMark);
}

void WordDocument::OnCopy() {
  if (OpenClipboard(m_hWnd) && EmptyClipboard()) {
    BufferSave(Ascii);
    BufferSave(Unicode);
    BufferSave(Word);
    CloseClipboard();
  }
}

bool WordDocument::PasteEnable() {
  bool bResult = false;

  if (OpenClipboard(m_hWnd)) {
    bResult = IsClipboardFormatAvailable(Word) ||
              IsClipboardFormatAvailable(Unicode) ||
              IsClipboardFormatAvailable(Ascii);
    CloseClipboard();
  }

  return bResult;
}

void WordDocument::OnPaste() {
  vector<Paragraph*> pasteList;

  if (OpenClipboard(m_hWnd)) {
    if (IsClipboardFormatAvailable(Word)) {
      BufferLoad(Word, pasteList);
    }
    else if (IsClipboardFormatAvailable(Unicode)) {
      BufferLoad(Unicode, pasteList);
    }
    else if (IsClipboardFormatAvailable(Ascii)) {
      BufferLoad(Ascii, pasteList);
    }
    else if (IsClipboardFormatAvailable(CF_HDROP)) {
      HDROP hDrop = (HDROP) GetClipboardData(CF_HDROP);

      if (hDrop != NULL) {
        OnDropFiles(hDrop);
      }
    }

    CloseClipboard();
  }

  if (!pasteList.empty()) {
    if (pasteList.size() == 1) {      
      m_paraList[m_psEdit.m_iPara]->Insert(m_psEdit.m_iChar, pasteList[0]);
      m_psEdit.m_iChar += pasteList[0]->Length();
    }
    else {
      set<Rect> rectList;
      Paragraph* pLeftPara = m_paraList[m_psEdit.m_iPara];
      pLeftPara->Append(pasteList.front());
      pLeftPara->Calculate(&rectList);
      InvalidateRectSet(rectList);

      Paragraph* pRightPara = pLeftPara->Split(m_psEdit.m_iChar);
      pasteList.back()->Append(pRightPara);
      m_psEdit.m_iChar += pRightPara->Length();

      int iSize = pasteList.size();
      for (int iPara = 1; iPara <= iSize; ++iPara) {
        //m_paraList.insert(++m_psEdit.m_iPara, pasteList[iPara]);
        pasteList[iPara]->Calculate();
      }
    }

    UpdateParagraphAndPageList();
    UpdateCaret();
  }
}

void WordDocument::OnDropFiles(HDROP hDrop) {
  bool bFound = false;
  int iIndex, iCount = DragQueryFile(hDrop, 0xFFFFFFFF,NULL,0);

  for (iIndex = 0; iIndex < iCount; ++iIndex) {
    TCHAR pPath[MAX_PATH];
    DragQueryFile(hDrop, iIndex, pPath, MAX_PATH);
    String stPath = pPath;

    int iLastSlash = stPath.find_last_of(TEXT("\\")),
        iLastDot = stPath.find_last_of(TEXT("."));

    if ((iLastDot != -1) && (iLastDot > iLastSlash)) {
      String stSuffix = stPath.substr(iLastDot + 1);

      if (stSuffix == TEXT("wrd")) {
        if (OnClose()) {
//          FileOpen(m_hWnd, stPath);
        }

        bFound = true;
        break;
      }
    }
  }

  if (!bFound) {
    MessageBox(TEXT("No file with appropriate file suffix (.wrd)."),
               TEXT("File Drop"), Ok, Stop);
  }

  DragFinish(hDrop);
}