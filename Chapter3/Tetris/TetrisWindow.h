class TetrisWindow : public Window {
  public:
    TetrisWindow(WindowShow windowShow);
    ~TetrisWindow();

    bool OnKeyDown(WORD key, bool shiftPressed,
                   bool controlPressed);
    void OnDraw(Graphics& graphics, DrawMode drawMode) const;

    void OnGainFocus();
    void OnLoseFocus();

    void OnTimer(int timerId);
    void EndOfFigure();
    void GameOver();
    void NewGame();

    void DeleteFullRows();
    bool IsRowFull(int row);
    void FlashRow(int row);
    void DeleteRow(int markedRow);

    bool TryClose();

  private:
    GameGrid gameGrid;
    TetrisFigure fallingFigure, nextFigure;

    int currScore = 0;
    bool timerActive = true, inverse = false;

    static const int TimerId = 1000;
    vector<TetrisFigure> figureList;
};