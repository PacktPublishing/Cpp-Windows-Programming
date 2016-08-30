#include "SmallWindows.h"

namespace SmallWindows {

  PageSetupDialog::PageSetupDialog(Window* parentPtr,
                                   PageSetupInfo* infoPtr)
   :Dialog(TEXT("Page Setup"), Point(0, 0), parentPtr),
    infoPtr(infoPtr),
    backupInfo(*infoPtr) {

    new GroupBox(this, Point(10, 10),
                 Size(330, 50), TEXT("Margins"));
    new Label(this, Point(20, 20), Size(50, 10),
              TEXT("&Top Margin:"));

    new TextField<int>(this, Point(70, 20), Size(100, 12),
                       &infoPtr->TopMargin());
    new Label(this, Point(180, 20), Size(50, 10),
              TEXT("&Bottom Margin:"));
    new TextField<int>(this, Point(230, 20), Size(100, 12),
                       &infoPtr->BottomMargin());
    new Label(this, Point(20, 40), Size(50, 10),
              TEXT("&Left Margin:"));
    new TextField<int>(this, Point(70, 40), Size(100, 12),
                       &infoPtr->LeftMargin());
    new Label(this, Point(180, 40), Size(50, 10),
              TEXT("&Right Margin:"));
    new TextField<int>(this, Point(230, 40), Size(100, 12),
                       &infoPtr->RightMargin());

    new GroupBox(this, Point(10, 70),
                 Size(330, 50), TEXT("Header"));
    new Label(this, Point(20, 80), Size(50, 10),
              TEXT("&Header Text:"));
    new TextField<>(this, Point(70, 80), Size(260, 12),
                    &infoPtr->HeaderText());

    new CheckBox(this, Point(70, 100), Size(100, 10),
                 TEXT("H&eader at First Page"),
                 &infoPtr->HeaderFirst());

    new PushButton(this, Point(270, 98), Size(60, 15),
                   TEXT("He&ader Font"), OnHeaderFont);

    new GroupBox(this, Point(10, 130),
                 Size(330, 50), TEXT("Footer"));
    new Label(this, Point(20, 140), Size(50, 10),
              TEXT("&Footer Text:"));
    new TextField<>(this, Point(70, 140), Size(260, 12),
                    &infoPtr->FooterText());
    new CheckBox(this, Point(70, 160), Size(100, 10),
                 TEXT("F&ooter at First Page"),
                 &infoPtr->FooterFirst());
    new PushButton(this, Point(270, 158), Size(60, 15),
                   TEXT("Footer Fo&nt"), OnFooterFont);

    new Label(this, Point(20, 190), Size(40, 10),
              TEXT("&Orientation:"));
    new ComboBox(this, Point(65, 190), Size(70, 30),
                 {TEXT("Portrait"), TEXT("Landscape")},
                 (int*) &infoPtr->GetOrientation());
    new CheckBox(this, Point(20, 205), Size(100, 10),
                 TEXT("Page &Surrounded by Frame"),
                 &infoPtr->Frame());
    new PushButton(this, Point(200, 200),
                   Size(60, 15), TEXT("Ok"), OnOk);
    new PushButton(this, Point(270, 200), Size(60, 15),
                   TEXT("Cancel"), OnCancel);
  }

#if 0
    new PushButton(this, Point(270, 200), Size(60, 15),
                   TEXT("Cancel"), OnCancel);
    new PushButton(this, Point(200, 200),
                   Size(60, 15), TEXT("Ok"), OnOk);
    new CheckBox(this, Point(20, 205), Size(100, 10),
                 TEXT("Page &Surrounded by Frame"),
                 &infoPtr->Frame());
    new ComboBox(this, Point(65, 190), Size(70, 30),
                 {TEXT("Portrait"), TEXT("Landscape")},
                 (int*) &infoPtr->GetOrientation());
    new Label(this, Point(20, 190), Size(40, 10),
              TEXT("&Orientation:"));
    new PushButton(this, Point(270, 158), Size(60, 15),
                   TEXT("Footer Fo&nt"), OnFooterFont);
    new CheckBox(this, Point(70, 160), Size(100, 10),
                 TEXT("F&ooter at First Page"),
                 &infoPtr->FooterFirst());
    new TextField<>(this, Point(70, 140), Size(260, 12),
                    &infoPtr->FooterText());
    new Label(this, Point(20, 140), Size(50, 10),
              TEXT("&Footer Text:"));
    new GroupBox(this, Point(10, 130),
                 Size(330, 50), TEXT("Footer"));
    new PushButton(this, Point(270, 98), Size(60, 15),
                   TEXT("He&ader Font"), OnHeaderFont);
    new CheckBox(this, Point(70, 100), Size(100, 10),
                 TEXT("H&eader at First Page"),
                 &infoPtr->HeaderFirst());
    new TextField<>(this, Point(70, 80), Size(260, 12),
                    &infoPtr->HeaderText());
    new Label(this, Point(20, 80), Size(50, 10),
              TEXT("&Header Text:"));
    new GroupBox(this, Point(10, 70),
                 Size(330, 50), TEXT("Header"));
    new TextField<int>(this, Point(230, 40), Size(100, 12),
                       &infoPtr->RightMargin());
    new Label(this, Point(180, 40), Size(50, 10),
              TEXT("&Right Margin:"));
    new TextField<int>(this, Point(70, 40), Size(100, 12),
                       &infoPtr->LeftMargin());
    new Label(this, Point(20, 40), Size(50, 10),
              TEXT("&Left Margin:"));
    new TextField<int>(this, Point(230, 20), Size(100, 12),
                       &infoPtr->BottomMargin());
    new Label(this, Point(180, 20), Size(50, 10),
              TEXT("&Bottom Margin:"));
    new TextField<int>(this, Point(70, 20), Size(100, 12),
                       &infoPtr->TopMargin());
    new Label(this, Point(20, 20), Size(50, 10),
              TEXT("&Top Margin:"));
    new GroupBox(this, Point(10, 10),
                 Size(330, 50), TEXT("Margins"));
  }
#endif

  void PageSetupDialog::OnHeaderFont() {
    StandardDialog::FontDialog(this, infoPtr->HeaderFont()); 
  }

  void PageSetupDialog::OnFooterFont() {
    StandardDialog::FontDialog(this, infoPtr->FooterFont()); 
  }

  void PageSetupDialog::OnOk() {
    Dialog::OnReturn();
  }

  void PageSetupDialog::OnCancel() {
    *infoPtr = backupInfo;
    Dialog::OnEscape();
  }
};