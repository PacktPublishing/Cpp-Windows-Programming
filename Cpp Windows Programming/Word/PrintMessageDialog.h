#ifndef _PRINT_MESSAGE_DIALOG_
#define _PRINT_MESSAGE_DIALOG_

class PrintMessageDialog : public Dialog {
  public:
    PrintMessageDialog(Window* pParent);
    void SetMessage(String stMessage);

    void OnClick(Component* pSource);
    DECLARE_VOID_LISTENER(PrintMessageDialog, OnCancel);

  private:
    Label* m_pMessageLabel;
    Button* m_pCancelButton;
};

#endif
