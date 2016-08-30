#include "SmallWindows.h"

namespace SmallWindows {
/*
  bool FontDialog(Window* windowPtr, Font& font, Color& color) {
    LOGFONT logFont = font.LogFont();

    CHOOSEFONT chooseFont;
    memset(&chooseFont, 0, sizeof chooseFont);

    chooseFont.lStructSize = sizeof(CHOOSEFONT);
    chooseFont.hInstance = Application::InstanceHandle();
    chooseFont.hwndOwner = windowPtr->WindowHandle();
    chooseFont.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
    chooseFont.lpLogFont = &logFont;
    chooseFont.rgbColors = color.ColorRef();

    if (ChooseFont(&chooseFont) != 0) {
      font.LogFont() = logFont;
      color.ColorRef() = chooseFont.rgbColors;
      return true;
    }

    return false;
  }
*/

  bool FontDialog(Window* windowPtr, Font& font) {
    LOGFONT logFont = font.LogFont();

    CHOOSEFONT chooseFont;
    memset(&chooseFont, 0, sizeof chooseFont);

    chooseFont.lStructSize = sizeof(CHOOSEFONT);
    chooseFont.hInstance = Application::InstanceHandle();
    chooseFont.hwndOwner = windowPtr->WindowHandle();
    chooseFont.Flags = CF_INITTOLOGFONTSTRUCT |
                       CF_SCREENFONTS | CF_EFFECTS;
    chooseFont.lpLogFont = &logFont;
    chooseFont.rgbColors = font.FontColor().ColorRef();

    if (::ChooseFont(&chooseFont) != 0) {
      font.LogFont() = logFont;
      font.FontColor() = Color(chooseFont.rgbColors);
      return true;
    }

    return false;
  }
};

/*  bool FontDialog(Window* windowPtr, Font& font) {
    LOGFONT logFont = font.LogFont();

    CHOOSEFONT chooseFont;
    memset(&chooseFont, 0, sizeof chooseFont);

    chooseFont.lStructSize = sizeof(CHOOSEFONT);
    chooseFont.hInstance = Application::InstanceHandle();
    chooseFont.hwndOwner = windowPtr->WindowHandle();
    chooseFont.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
    chooseFont.lpLogFont = &logFont;
    chooseFont.rgbColors = SystemColor.ColorRef();

    if (ChooseFont(&chooseFont) != 0) {
      font.SetLogFont(logFont);
      return true;
    }

    return false;
  }
  
  FontDialog::FontDialog(Window* windowPtr, Font font) {
    memset(&chooseFont, 0, sizeof chooseFont);

    chooseFont.lStructSize = sizeof(CHOOSEFONT);
    chooseFont.hInstance = InstanceHandle;
    chooseFont.hwndOwner = windowPtr->WindowHandle();
    chooseFont.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
    chooseFont.lpLogFont = &font.LogFont();
    chooseFont.rgbColors = font.GetColorRGB();
  }

  FontDialog::FontDialog(Window* windowPtr, Font font, Color color) {
    memset(&chooseFont, 0, sizeof chooseFont);

    chooseFont.lStructSize = sizeof(CHOOSEFONT);
    chooseFont.hInstance = InstanceHandle;
    chooseFont.hwndOwner = windowPtr->WindowHandle();
    chooseFont.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
    chooseFont.lpLogFont = &font.LogFont();
    chooseFont.rgbColors = color;
  }

  bool FontDialog::Display() {
    return (ChooseFont(&chooseFont) != 0);
  }

  Font FontDialog::GetFont() {
    return Font(*chooseFont.lpLogFont, chooseFont.rgbColors);
  }

  Color FontDialog::FontColor() {
    return Color(chooseFont.rgbColors);
  }*/