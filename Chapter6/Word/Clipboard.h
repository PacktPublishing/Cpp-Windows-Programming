void OnEditCopy();

int CharAsciiByteSize(CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphAsciiSize(Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
int CharAsciiByteSave(char* pcBuffer, CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphAsciiSave(void* pBuffer, Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
void SaveAscii(HWND LIST* pCopyList);

int CharAsciiByteLoad(char* pcBuffer, CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphAsciiLoad(void* pBuffer, Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
void LoadAscii(HWND LIST* pCopyList);

int CharUnicodeByteSize(CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphUnicodeSize(Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
int CharUnicodeByteSave(char* pcBuffer, CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphUnicodeSave(void* pBuffer, Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
void SaveUnicode(HWND LIST* pCopyList);

int CharUnicodeByteLoad(char* pcBuffer, CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphUnicodeLoad(void* pBuffer, Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
void LoadUnicode(HWND LIST* pCopyList);

int CharInfoByteSize(CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphByteSize(Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
int CharInfoByteSave(char* pcBuffer, CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int InternalByteSave(void* pBuffer, Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
void SaveInternal(HWND LIST* pCopyList);

int CharInfoByteLoad(char* pcBuffer, CharInfo* pCharInfo, LIST* pList, int iIndex, void* pParam);
int ParagraphByteLoad(void* pBuffer, Paragraph** ppParagraph, LIST* pList, int iIndex, void* pParam);
void LoadInternal(HWND LIST* pCopyList);
void OnEditPaste();

void DropFilesLoad();
void OnDropFiles(HWND HDROP hDrop);
