namespace SmallWindows {
  typedef void (*VoidListener)(void* sourceCell);
  typedef bool (*BoolListener)(void* sourceCell);

  class Command {
    public:
      Command();
      Command(VoidListener selection, BoolListener enable,
              BoolListener check, BoolListener radio);

      VoidListener Selection() const {return selection;}
      BoolListener Enable() const {return enable;}
      BoolListener Check() const {return check;}
      BoolListener Radio() const {return radio;}

    private:
      VoidListener selection;
      BoolListener enable, check, radio;
  };
};
