class HelloWindow : public Window {
  public:
    HelloWindow(WindowShow windowShow);
    void OnDraw(Graphics& graphics, DrawMode drawMode) const;
};
