#include "SmallWindows.h"

void InfoList::FromBuffer(const void* voidBuffer, int size) {
  const char* charBuffer = (const char*)voidBuffer;

  for (int count = 0; count < size; ++count) {
    list.PushBack(*(charBuffer++));
  }
}

void InfoList::ToBuffer(void* voidBuffer) {
  char* charBuffer = (char*)voidBuffer;

  for (char c : list) {
    *(charBuffer++) = c;
  }
}