void OnInitMenu(LPARAM lParam, WPARAM wParam);

void OnNew();
bool OnLoad(istream& instream);
bool OnSave(ostream& outstream);

void CellEmpty(CELL** ppCell, Matrix* pMatrix, int iRow, int iCol, void* pParam);

bool EnableCopy();
bool EnablePaste();

void OnCopy();
void OnPaste();
void OnDelete();

void OnFont();
void OnTextColor();
void OnBackColor();

bool HorizontalLeftRadio();
bool HorizontalCenterRadio();
bool HorizontalRightRadio();
bool HorizontalJustifiedRadio();

void OnHorizontalLeft();
void OnHorizontalCenter();
void OnHorizontalRight();
void OnHorizontalJustified();

bool VerticalTopRadio();
bool VerticalCenterRadio();
bool VerticalBottomRadio();

void OnVerticalTop();
void OnVerticalCenter();
void OnVerticalBottom();

void InvalidateBlock(Reference rfMin, Reference rfMax);

bool IsAlignment(int iDirection, int iAlignment);
void SetAlignment(int iDirection, int iAlignment);