namespace SmallWindows {
  class Registry {
    public:
      static void WriteInteger(String key, const int value);
      static void WriteDouble(String key, const double value);
      static void WriteBoolean(String key, const bool value);
      static void WriteString(String key, const String text);
      static void WriteBuffer(String key, const void* buffer,
                              int size);

      static int ReadInteger(String key, const int defaultValue);
      static double ReadDouble(String key,
                               const double defaultValue);
      static bool ReadBoolean(String key,
                              const bool defaultValue);
      static String ReadString(String key,
                               const String defaultText);
      static void ReadBuffer(String key, void* buffer, int size,
                             const void* defaultBuffer);

      static void Erase(String key);
  };
};
