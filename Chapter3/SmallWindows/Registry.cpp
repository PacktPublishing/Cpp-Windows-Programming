#include "SmallWindows.h"

namespace SmallWindows {
  const String RegistryFileName = TEXT(".\\SmallWindows.ini");

  void Registry::WriteInteger(String key, const int intValue) {
    WriteBuffer(key, &intValue, sizeof intValue);
  }

  void Registry::WriteDouble(String key,
                             const double doubleValue) {
    WriteBuffer(key, &doubleValue, sizeof doubleValue);
  }

  void Registry::WriteBoolean(String key, const bool boolValue) {
    WriteBuffer(key, &boolValue, sizeof boolValue);
  }

  void Registry::WriteString(String key, const String text) {
    ::WritePrivateProfileString
        (Application::ApplicationName().c_str(),
         key.c_str(), text.c_str(), RegistryFileName.c_str());
  }

  void Registry::WriteBuffer(String key, const void* buffer,
                             int size) {
    ::WritePrivateProfileStruct
        (Application::ApplicationName().c_str(),
         key.c_str(), (void*) buffer, size,
         RegistryFileName.c_str());
  }

  int Registry::ReadInteger(String key, const int defaultValue) {
    int intValue;
    ReadBuffer(key, &intValue, sizeof intValue, &defaultValue);
    return intValue;
  }

  double Registry::ReadDouble(String key,
                              const double defaultValue) {
    double doubleValue;
    ReadBuffer(key, &doubleValue, sizeof doubleValue,
               &defaultValue);
    return doubleValue;
  }

  bool Registry::ReadBoolean(String key, const bool defaultValue){
    bool boolValue;
    ReadBuffer(key, &boolValue, sizeof boolValue, &defaultValue);
    return boolValue;
  }

  String Registry::ReadString(String key,
                              const String defaultText) {
    TCHAR text[MAX_PATH];
    int count =
      ::GetPrivateProfileString
          (Application::ApplicationName().c_str(), key.c_str(),
           nullptr, text, MAX_PATH, RegistryFileName.c_str());
    return (count > 0) ? String(text) : defaultText;
  }

  void Registry::ReadBuffer(String key, void* buffer, int size,
                            const void* defaultBuffer) {
    int result =
      ::GetPrivateProfileStruct
          (Application::ApplicationName().c_str(), key.c_str(),
           buffer, size, RegistryFileName.c_str());

    if (result == 0) {
      ::memcpy(buffer, defaultBuffer, size);
    }
  }

  void Registry::Erase(String key) {
    ::WritePrivateProfileString
        (Application::ApplicationName().c_str(),
         key.c_str(),nullptr,RegistryFileName.c_str());
  }
};