namespace SmallWindows {
  class Application {
    public:
      static void RegisterWindowClasses(HINSTANCE instanceHandle);
      static int RunMessageLoop();

      static HINSTANCE& InstanceHandle() {return instanceHandle;}
      static String& ApplicationName() {return applicationName;}
      static Window*& MainWindowPtr() {return mainWindowPtr;}

    private:
      static HINSTANCE instanceHandle;
      static String applicationName;
      static Window* mainWindowPtr;
  };
};