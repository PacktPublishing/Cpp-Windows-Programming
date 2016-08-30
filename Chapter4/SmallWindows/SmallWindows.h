#include <windows.h>

#include "Windows.h"
using namespace std;

#include <map>
#include <set>
#include <list>
#include <array>
#include <cmath>
#include <ctime>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <tchar.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <initializer_list>
using namespace std;

#ifndef UNICODE
  typedef std::string String;
#define to_String std::to_string
#else
  typedef std::wstring String;
  #define to_String std::to_wstring
#endif

typedef basic_istringstream<TCHAR> IStringStream;
typedef basic_ostringstream<TCHAR> OStringStream;

#define Space TEXT(' ')
#define NewLine TEXT('\n')

#include "DynamicList.h"
#include "InfoList.h"

#include "Size.h"
#include "Point.h"
#include "Rect.h"

#include "Color.h"
#include "Font.h"

#include "Cursor.h"
#include "Registry.h"

#include "Reference.h"
#include "String.h"
#include "Matrix.h"
#include "Tree.h"

#include "StandardDialog.h"
#include "Graphics.h"
#include "Accelerator.h"
#include "Command.h"
#include "Menu.h"

#include "Template.h"
#include "PageSetupInfo.h"

#include "Window.h"
#include "Document.h"
#include "StandardDocument.h"
#include "PrintPreviewDocument.h"

#include "Rational.h"
#include "Complex.h"
#include "Converter.h"

#include "Controls\\Control.h"
#include "Controls\\Label.h"
#include "Controls\\TextField.h"
#include "Controls\\ComboBox.h"
#include "Controls\\ListControl.h"
#include "Controls\\ListBox.h"
#include "Controls\\MultipleListBox.h"
#include "Controls\\ButtonControl.h"
#include "Controls\\GroupBox.h"
#include "Controls\\CheckBox.h"
#include "Controls\\RadioButton.h"
#include "Controls\\PushButton.h"
#include "Dialog.h"
#include "PageSetupDialog.h"

#include "Clipboard.h"
#include "Application.h"
#include "MainWindow.h"

using namespace SmallWindows;
