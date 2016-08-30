#include <Windows.h>
#include <StdIO.h>
#include <TChar.h>
#include <WChar.h>
#include <String.h>
#include <Assert.h>
#include <Time.h>
#include "Resource.h"
#include "..\\List.h"
#include "..\\Set.h"
#include "..\\Unit.h"
#include "..\\UnitScroll.h"
#include "UnitScrollZoom.h"
#include "Position.h"
#include "Paragraph.h"
#include "Calculate.h"
#include "Resource.h"
#include "Word.h"
#include "Command.h"
#include "Keyboard.h"
#include "Info.h"
#include "Print.h"

int CharComp(void* pRecord1, void* pRecord2)
{
  TCHAR* pChar1 = (TCHAR*) pRecord1;
  TCHAR* pChar2 = (TCHAR*) pRecord2;
  return (*pChar1 < *pChar2) ? -1 : ((*pChar1 > *pChar2) ? 1 : 0);
}

void InfoLoad(INFO* pInfo, int iPageNum)
{
  { TCHAR stPage[STRING_SIZE];
    _stprintf_s(stPage, STRING_SIZE, TEXT("%d"), iPageNum);
    pInfo->numberOfPages = ListEmpty(sizeof (TCHAR));
    ListFromString(&pInfo->numberOfPages, stPage);
  }

  pInfo->pathWithSuffix = ListEmpty(sizeof (TCHAR));
  pInfo->pathWithoutSuffix = ListEmpty(sizeof (TCHAR));
  pInfo->fileWithSuffix = ListEmpty(sizeof (TCHAR));
  pInfo->fileWithoutSuffix = ListEmpty(sizeof (TCHAR));

  ListFromString(&pInfo->pathWithSuffix, g_stPath);
  ListFromString(&pInfo->pathWithoutSuffix, g_stPath);
  ListFromString(&pInfo->fileWithSuffix, g_stPath);
  ListFromString(&pInfo->fileWithoutSuffix, g_stPath);

  { TCHAR cSlash = TEXT('\\');
    int iLastSlash = ListLastIndexOf(&pInfo->pathWithSuffix, &cSlash, &CharComp);

    if (iLastSlash != -1) {
      ListRemoveBlock(&pInfo->fileWithSuffix, 0, iLastSlash);
      ListRemoveBlock(&pInfo->fileWithoutSuffix, 0, iLastSlash);
    }
  }

  { TCHAR cDot = TEXT('.');
    int iLastDot = ListLastIndexOf(&pInfo->pathWithSuffix, &cDot, &CharComp);

    if (iLastDot != -1) {
      ListRemoveBlock(&pInfo->pathWithoutSuffix, iLastDot, -1);
      ListRemoveBlock(&pInfo->fileWithoutSuffix, iLastDot, -1);
    }
  }

  { time_t t = time(NULL);
    struct tm timeInfo;
    
    TCHAR stTimeWithSeconds[STRING_SIZE],
          stTimeWithoutSeconds[STRING_SIZE],
          stDateWithFullMonth[STRING_SIZE],
          stDateWithAbbrevatedMonth[STRING_SIZE];

    TCHAR *fullMonths[] = {TEXT("January"), TEXT("February"), TEXT("March"), TEXT("April"), TEXT("May"), TEXT("June"),
                           TEXT("July"), TEXT("August"), TEXT("September"), TEXT("October"), TEXT("November"), TEXT("December")};
    TCHAR *abbrevatedMonths[] = {TEXT("Jan"), TEXT("Feb"), TEXT("Mar"), TEXT("Apr"), TEXT("May"), TEXT("Jun"),
                                 TEXT("Jul"), TEXT("Aug"), TEXT("Sep"), TEXT("Oct"), TEXT("Nov"), TEXT("Dec")};

    pInfo->dateWithFullMonth = ListEmpty(sizeof (TCHAR));
    pInfo->dateWithAbbrevatedMonth = ListEmpty(sizeof (TCHAR));
    pInfo->timeWithSeconds = ListEmpty(sizeof (TCHAR));
    pInfo->timeWithoutSeconds = ListEmpty(sizeof (TCHAR));

    localtime_s(&timeInfo, &t);
    _stprintf_s(stTimeWithSeconds, STRING_SIZE, TEXT("%02d:%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    _stprintf_s(stTimeWithoutSeconds, STRING_SIZE, TEXT("%02d:%02d"), timeInfo.tm_hour, timeInfo.tm_min);
    _stprintf_s(stDateWithFullMonth, STRING_SIZE, TEXT("%s %d, %d"), fullMonths[timeInfo.tm_mon], timeInfo.tm_mday, 1900 + timeInfo.tm_year);
    _stprintf_s(stDateWithAbbrevatedMonth, STRING_SIZE, TEXT("%s %d, %d"), abbrevatedMonths[timeInfo.tm_mon], timeInfo.tm_mday, 1900 + timeInfo.tm_year);

    ListFromString(&pInfo->timeWithSeconds, stTimeWithSeconds);
    ListFromString(&pInfo->timeWithoutSeconds, stTimeWithoutSeconds);
    ListFromString(&pInfo->dateWithFullMonth, stDateWithFullMonth);
    ListFromString(&pInfo->dateWithAbbrevatedMonth, stDateWithAbbrevatedMonth);
  }
}

void InfoFill(TCHAR stTemplate[], int iPage, INFO* pInfo, TCHAR stResult[])
{
  LIST list = ListEmpty(sizeof (TCHAR));
  ListFromString(&list, stTemplate);

  { int iIndex, iSize = ListSize(&list);
    TCHAR stPage[STRING_SIZE];
    LIST pageList = ListEmpty(sizeof (TCHAR));
    _stprintf_s(stPage, STRING_SIZE, TEXT("%d"), iPage);
    ListFromString(&pageList, stPage);

    for (iIndex = (iSize - 2); iIndex >= 0; --iIndex) {
      TCHAR cCurrChar, cNextChar, cPercent = TEXT('%');

      ListGet(&list, iIndex, &cCurrChar);
      ListGet(&list, iIndex + 1, &cNextChar);

      if (cCurrChar == TEXT('%'))
      {
        ListRemoveBlock(&list, iIndex, iIndex + 1);

        switch (cNextChar) {
          case TEXT('P'):
            ListInsertBlock(&list, iIndex, &pInfo->pathWithSuffix);
            break;

          case TEXT('p'):
            ListInsertBlock(&list, iIndex, &pInfo->pathWithoutSuffix);
            break;

          case TEXT('F'):
            ListInsertBlock(&list, iIndex, &pInfo->pathWithSuffix);
            break;

          case TEXT('f'):
            ListInsertBlock(&list, iIndex, &pInfo->fileWithoutSuffix);
            break;

          case TEXT('N'):
            ListInsertBlock(&list, iIndex, &pInfo->numberOfPages);
            break;

          case TEXT('n'):
            ListInsertBlock(&list, iIndex, &pageList);
            break;

          case TEXT('D'):
            ListInsertBlock(&list, iIndex, &pInfo->dateWithFullMonth);
            break;

          case TEXT('d'):
            ListInsertBlock(&list, iIndex, &pInfo->dateWithAbbrevatedMonth);
            break;

          case TEXT('T'):
            ListInsertBlock(&list, iIndex, &pInfo->timeWithSeconds);
            break;

          case TEXT('t'):
            ListInsertBlock(&list, iIndex, &pInfo->timeWithoutSeconds);
            break;

          case TEXT('%'):
            ListInsert(&list, iIndex, &cPercent);
            break;
        }
      }
    }
  }

  ListToString(&list, stResult);
  ListFree(&list);
}

void InfoFree(INFO* pInfo)
{
  ListFree(&pInfo->numberOfPages);  
  ListFree(&pInfo->pathWithSuffix);
  ListFree(&pInfo->pathWithoutSuffix);
  ListFree(&pInfo->fileWithSuffix);
  ListFree(&pInfo->fileWithoutSuffix);
  ListFree(&pInfo->dateWithFullMonth);
  ListFree(&pInfo->dateWithAbbrevatedMonth);
  ListFree(&pInfo->timeWithSeconds);
  ListFree(&pInfo->timeWithoutSeconds);
}
