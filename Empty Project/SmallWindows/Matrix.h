/*
m_
s_

Cell* => Cell&

cellPtr-> => cell. X
CellPtr-> => Cell. X

cellPtr => cell
CellPtr => Cell

.GetPtr(row, col) => [row][col]
.GetPtr(markRef) => [markRef]
.GetPtr(editRef) => [markRef]
.GetPtr(sourceRef) => [markRef]
.GetPtr(targetRef) => [markRef]
.GetPtr(cellRef) => [cellRef]
.GetPtr(lastMarkRef) => [lastMarkRef]

cellMatrix[row][col]-> => cellMatrix[row][col].
Ref]-> => Ref].

*editCell = prevCell; => editCell = prevCell;
*targetCell = pastedCell; => targetCell = pastedCell;
*/

namespace SmallWindows {
  template <int Rows, int Cols, class Type>
  class Matrix {
    public:
      Matrix();
      Matrix(const Matrix& matrix);
      Matrix& operator=(const Matrix& matrix);

    private:
      void Init(const Matrix<Rows,Cols,Type>& matrix);

    public:
      const Type* operator[](int row) const
                 {return ((const Type*) buffer[row]);}
      Type& operator[](const Reference& ref)
                      {return buffer[ref.Row()][ref.Col()];}
      Type operator[](const Reference& ref) const
                     {return buffer[ref.Row()][ref.Col()];}

    private:
      Type buffer[Rows][Cols];
  };

  template <int Rows, int Cols, class Type>
  Matrix<Rows, Cols, Type>::Matrix() {
    // Empty.
  }

  template <int Rows, int Cols, class Type>
  Matrix<Rows,Cols,Type>::Matrix(const Matrix<Rows,Cols,Type>&
                                 matrix) {
    Init(matrix);
  }

  template<int Rows, int Cols, class Type>
  Matrix<Rows,Cols,Type>& Matrix<Rows,Cols,Type>::operator=
                          (const Matrix<Rows,Cols,Type>& matrix) {
    if (this != &matrix) {
      Init(matrix);
    }

    return *this;
  }

  template <int Rows, int Cols, class Type>
  void Matrix<Rows, Cols, Type>::Init(const Matrix<Rows, Cols, Type>& matrix){
    for (int row = 0; row < Rows; ++row) {
      for (int col = 0; col < Cols; ++col) {
        buffer[row][col] = matrix.buffer[row][col];
      }
    }
  }
}