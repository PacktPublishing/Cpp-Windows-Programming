void OnCharDown(WPARAM wParam, LPARAM lParam);
void OnKeyDown(WPARAM wParam, LPARAM lParam);
void OnControlKey(int iKey);
void OnShiftKey(int iKey);
void EnsureMarkStatus();
void OnRegularKey(int iKey);
void EnsureEditStatus();
void SetNextFont(FONT_INFO* pNextFontInfo);
void OnNeutralKey(int iKey);

void OnLeftArrowKey();
void OnRightArrowKey();
void OnUpArrowKey();
void OnDownArrowKey();
void OnPageUpKey();
void OnPageDownKey();
void OnHomeKey();
void OnEndKey();

void OnShiftLeftArrowKey();
void OnShiftRightArrowKey();
void OnShiftUpArrowKey();
void OnShiftDownArrowKey();
void OnShiftPageUpKey();
void OnShiftPageDownKey();
void OnShiftHomeKey();
void OnShiftControlHomeKey();
void OnShiftEndKey();
void OnShiftControlEndKey();
void UpdateMarkedArea(Position psFirstMark,
                      Position psOldLastMark, Position psNewLastMark);

void OnReturnKey();
void OnInsertKey(void);
void OnDeleteKey();
void OnBackspaceKey();

Position PointToChar(POINT ptMouse);
void MakeVisible();
void MakeAreaVisible(RECT rcArea, bool bSpace);
void GetRepaintSet(set<Rect>* pRepaintSet, Position psFirst,
                   Position psLast);
