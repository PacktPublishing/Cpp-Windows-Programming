#include "SmallWindows.h"

namespace SmallWindows {
  String Template(const Document* documentPtr,String templateText,
                  int copy /* = 0 */, int page /* = 0 */,
                  int totalPages /* = 0 */) {
    ReplaceAll(templateText, TEXT("%c"), to_String(copy));
    ReplaceAll(templateText, TEXT("%n"), to_String(page));
    ReplaceAll(templateText, TEXT("%N"), to_String(totalPages));

    String pathWithSuffix = documentPtr->GetName();
    ReplaceAll(templateText, TEXT("%P"), pathWithSuffix);

    int lastPathDot = pathWithSuffix.find_last_of(TEXT('.'));
    String pathWithoutSuffix =
      pathWithSuffix.substr(0, lastPathDot);
    ReplaceAll(templateText, TEXT("%p"), pathWithoutSuffix);

    int lastBackslash = pathWithSuffix.find_last_of(TEXT('\\'));
    String fileWithSuffix =
      pathWithSuffix.substr(lastBackslash + 1);
    ReplaceAll(templateText, TEXT("%F"), fileWithSuffix);

    int lastFileDot = fileWithSuffix.find_last_of(TEXT('.'));
    String fileWithoutSuffix =
      fileWithSuffix.substr(0, lastFileDot);
    ReplaceAll(templateText, TEXT("%f"), fileWithoutSuffix);

    time_t t = ::time(nullptr);
    struct tm time;
    ::localtime_s(&time, &t);

    { OStringStream timeWithoutSeconds;
      timeWithoutSeconds << std::setw(2) << setw(2)
                         << setiosflags(ios::right) 
                         << setfill(TEXT('0')) << time.tm_hour
                         << TEXT(":") << setiosflags(ios::right)
                         << setw(2) << setfill(TEXT('0'))
                         << time.tm_min;

      ReplaceAll(templateText, TEXT("%t"),
                 timeWithoutSeconds.str());

      OStringStream timeWithSeconds;
      timeWithSeconds << timeWithoutSeconds.str() << TEXT(":")
                      << setiosflags(ios::right) << setw(2)
                      << setfill(TEXT('0')) << time.tm_sec;
      ReplaceAll(templateText, TEXT("%T"), timeWithSeconds.str());
    }

    { static const String longMonths[] =
        {TEXT("January"), TEXT("February"), TEXT("March"),
         TEXT("April"), TEXT("May"), TEXT("June"), TEXT("July"),
         TEXT("August"), TEXT("September"), TEXT("October"),
         TEXT("November"), TEXT("December")};
      OStringStream dateFullMonth;
      dateFullMonth << longMonths[time.tm_mon] << TEXT(" ")
                    << time.tm_mday << TEXT(", ")
                    << (1900 + time.tm_year);
      ReplaceAll(templateText, TEXT("%D"), dateFullMonth.str());
    }

    { static const String shortMonths[] =
        {TEXT("Jan"), TEXT("Feb"), TEXT("Mar"), TEXT("Apr"),
         TEXT("May"), TEXT("Jun"), TEXT("Jul"), TEXT("Aug"),
         TEXT("Sep"), TEXT("Oct"), TEXT("Nov"), TEXT("Dec")};
      OStringStream dateShortMonth;
      dateShortMonth << shortMonths[time.tm_mon] << TEXT(" ")
                     << time.tm_mday << TEXT(", ")
                     << (1900 + time.tm_year);
      ReplaceAll(templateText, TEXT("%d"), dateShortMonth.str());
    }

    ReplaceAll(templateText, TEXT("%%"), TEXT("%"));
    return templateText;
  }
};

// <%P> <%p> <%F> <%f> <%N> <%n>
// <%c> <%D> <%d> <%T> <%t> <%%>

#if 0

int size = templateText.length();
for (int index = (size - 2); index >= 0; --index) {
  TCHAR currChar = templateText[index],
    nextChar = templateText[index + 1];

  if (currChar == TEXT('%')) {
    templateText.erase(index, index + 1);

    switch (nextChar) {
    case TEXT('P'):
      templateText.insert(index, pathWithSuffix);
      break;

    case TEXT('p'):
      templateText.insert(index, pathWithoutSuffix);
      break;

    case TEXT('F'):
      templateText.insert(index, fileWithSuffix);
      break;

    case TEXT('f'):
      templateText.insert(index, fileWithoutSuffix);
      break;

    case TEXT('N'):
      templateText.insert(index, totalPagesText);
      break;

    case TEXT('n'):
      templateText.insert(index, pageText);
      break;

    case TEXT('c'):
      templateText.insert(index, copyText);
      break;

    case TEXT('D'):
      templateText.insert(index, dateFullMonth.str());
      break;

    case TEXT('d'):
      templateText.insert(index, dateShortMonth.str());
      break;

    case TEXT('T'):
      templateText.insert(index, timeWithSeconds.str());
      break;

    case TEXT('t'):
      templateText.insert(index, timeWithoutSeconds.str());
      break;

    case TEXT('%'):
      templateText.insert(index, TEXT("%"));
      break;
    }
  }
}
#endif

/*
  void Template::Load(Window* windowPtr, int pageNum) {
    numberOfPagesText = to_String(pageNum);

    int lastSlash = pathWithSuffix.find_last_of(TEXT('\\')),
        lastDot = pathWithSuffix.find_last_of(TEXT('.'));

    pathWithSuffix = windowPtr->GetName();
    stFileSuffix = pathWithSuffix.substr(lastSlash + 1);

    if (lastDot != -1) {
      pathWithoutSuffix = pathWithSuffix.substr(0, lastDot);
      stFileNoSuffix = pathWithSuffix.substr(lastSlash,
                                               lastDot - (lastSlash + 1));
    }
    else {
      pathWithoutSuffix = pathWithSuffix;
      stFileNoSuffix = stFileSuffix;
    }

    String fullMonths[] ={TEXT("January"), TEXT("February"), TEXT("March"),
                          TEXT("April"), TEXT("May"), TEXT("June"),
                          TEXT("July"), TEXT("August"), TEXT("September"),
                          TEXT("October"),TEXT("November"),TEXT("December")};

    String abbrevMonths[] = {TEXT("Jan"), TEXT("Feb"), TEXT("Mar"),
                             TEXT("Apr"), TEXT("May"), TEXT("Jun"),
                             TEXT("Jul"), TEXT("Aug"), TEXT("Sep"),
                             TEXT("Oct"), TEXT("Nov"),TEXT("Dec")};

    time_t t = time(nullptr);
    struct tm time;
    localtime_s(&time, &t);

    TCHAR buffer[MAX_PATH];
    _stprintf_s(buffer, MAX_PATH, TEXT("%02d:%02d:%02d"),
                time.tm_hour, time.tm_min, time.tm_sec);
    stTimeSeconds = buffer;

    _stprintf_s(buffer, MAX_PATH, TEXT("%02d:%02d"),
                time.tm_hour, time.tm_min);
    stTimeNoSeconds = buffer;

    _stprintf_s(buffer, MAX_PATH, TEXT("%s %d, %d"),
                fullMonths[time.tm_mon],
                time.tm_mday, 1900 + time.tm_year);
    stDateFullMonth = buffer;

    _stprintf_s(buffer, MAX_PATH, TEXT("%s %d, %d"),
                abbrevMonths[time.tm_mon],
                time.tm_mday, 1900 + time.tm_year);
    stDateAbbrevMonth = buffer;
  }

  String Template::Fill(String templateText, int page) {
    String page = to_String(page);
    String resultText = templateText;
    int size = templateText.length();

    for (int index = (size - 2); index >= 0; --index) {
      TCHAR cCurrChar = templateText[index],
            cNextChar = templateText[index + 1];

      if (cCurrChar == TEXT('%')) {
        resultText.erase(index, index + 1);

        switch (cNextChar) {
          case TEXT('P'):
            resultText.insert(index, pathWithSuffix);
            break;

          case TEXT('p'):
            resultText.insert(index, pathWithoutSuffix);
            break;

          case TEXT('F'):
            resultText.insert(index, stFileSuffix);
            break;

          case TEXT('f'):
            resultText.insert(index, stFileNoSuffix);
            break;

          case TEXT('N'):
            resultText.insert(index, numberOfPagesText);
            break;

          case TEXT('n'):
            resultText.insert(index, stPage);
            break;

          case TEXT('D'):
            resultText.insert(index, stDateFullMonth);
            break;

          case TEXT('d'):
            resultText.insert(index, stDateAbbrevMonth);
            break;

          case TEXT('T'):
            resultText.insert(index, stTimeSeconds);
            break;

          case TEXT('t'):
            resultText.insert(index, stTimeNoSeconds);
            break;

          case TEXT('%'):
            resultText.insert(index, TEXT("%"));
            break;
        }
      }
    }

    return resultText;
  }
*/