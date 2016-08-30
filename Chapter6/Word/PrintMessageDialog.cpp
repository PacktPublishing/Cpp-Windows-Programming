#include "..\\SmallWindows\\SmallWindows.h"
#include "PrintMessageDialog.h"

PrintMessageDialog::PrintMessageDialog(Window* pParent)
 :Dialog(pParent, TEXT("Printing Progress"), Size(70, 50)) {
  SetWindowSize(Size(300, 200));

  m_pMessageLabel = new Label(this, Point(10, 10), Size(50, 10), ZeroString);
//  m_pCancelButton = new Button(this, Point(30, 30) , Size(30, 10), TEXT("Cancel"), OnCancel);

  ShowWindow(true);
}

void PrintMessageDialog::SetMessage(String stMessage) {
  m_pMessageLabel->SetText(stMessage);
}

void PrintMessageDialog::OnClick(Component* pSource) {
  if (pSource == m_pCancelButton) {
    OnCancel();
  }
}

void PrintMessageDialog::OnCancel() {
  Dialog::OnCancel();
}