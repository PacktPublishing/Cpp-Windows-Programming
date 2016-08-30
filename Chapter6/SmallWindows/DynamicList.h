namespace SmallWindows {
  template <class Type>
  class DynamicList {
    public:
      typedef bool (*IfFuncPtr)(const Type& value, void* voidPtr);
      typedef void (*DoFuncPtr)(Type& value, void* voidPtr);

      DynamicList();
      DynamicList(const DynamicList& list);
      DynamicList& operator=(const DynamicList& list);
      ~DynamicList();

      bool Empty() const;
      int Size() const;
      void Clear();
      int IndexOf(Type& value) const;

      Type* begin();
      const Type* begin() const;
      Type* end();
      const Type* end() const;

      Type operator[](int index) const;
      Type& operator[](int index);

      Type Front() const {return (*this)[0];}
      Type& Front() {return (*this)[0];}
      Type Back() const {return (*this)[size - 1];}
      Type& Back() {return (*this)[size - 1];}

      void PushBack(const Type& value);
      void PushBack(const DynamicList& list);
      void PushFront(const Type& value);
      void PushFront(const DynamicList& list);
      void Insert(int index, const Type& value);
      void Insert(int index, const DynamicList& list);

      void Erase(int deleteIndex);
      void Remove(int firstIndex = 0, int lastIndex = -1);
      
      void Copy(DynamicList& copyArray, int firstIndex = 0,
                int lastIndex = -1) const;
      void CopyRemove(DynamicList& copyArray,
                      int firstIndex = 0, int lastIndex = -1);

      bool AnyOf(IfFuncPtr ifFuncPtr, void* ifVoidPtr = nullptr)
                 const;
      bool AllOf(IfFuncPtr ifFuncPtr, void* ifVoidPtr = nullptr)
                 const;
      bool FirstOf(IfFuncPtr ifFuncPtr,Type& value,
                   void* ifVoidPtr = nullptr) const;
      bool LastOf(IfFuncPtr ifFuncPtr, Type& value,
                  void* ifVoidPtr = nullptr) const;

      void Apply(DoFuncPtr doFuncPtr, void* ifVoidPtr = nullptr);
      void ApplyIf(IfFuncPtr ifFuncPtr, DoFuncPtr doFuncPtr,
                   void* ifVoidPtr = nullptr,
                   void* doVoidPtr = nullptr);
      void CopyIf(IfFuncPtr ifFuncPtr, DynamicList& copyArray,
                  void* ifVoidPtr = nullptr) const;
      void RemoveIf(IfFuncPtr ifFuncPtr,
                    void* ifVoidPtr = nullptr);
      void CopyRemoveIf(IfFuncPtr ifFuncPtr, DynamicList& copyArray,
                        void* ifVoidPtr = nullptr);
      void ApplyRemoveIf(IfFuncPtr ifFuncPtr, DoFuncPtr doFuncPtr,
                         void* ifVoidPtr=nullptr,
                         void* doVoidPtr=nullptr);

  private:
      int size;
      Type* buffer;
  };

  template <class Type>
  DynamicList<Type>::DynamicList()
   :size(0),
    buffer(nullptr) {
    // Empty.
  }

  template <class Type>
  DynamicList<Type>::DynamicList(const DynamicList& list)
   :size(list.size),
    buffer(new Type[list.size]) {
    assert(buffer != nullptr);

    for (int index = 0; index < size; ++index) {
      buffer[index] = list.buffer[index];
    }
  }

  template <class Type>
  DynamicList<Type>& DynamicList<Type>::operator=
                                      (const DynamicList& list) {
    if (this != &list) {
      delete[] buffer;
      size = list.size;
      assert((buffer = new Type[size]) != nullptr);

      for (int index = 0; index < size; ++index) {
        buffer[index] = list.buffer[index];
      }
    }

    return *this;
  }

  template <class Type>
  DynamicList<Type>::~DynamicList() {
    delete[] buffer;
  }

  template <class Type>
  bool DynamicList<Type>::Empty() const {
    return (size == 0);
  }

  template <class Type>
  int DynamicList<Type>::Size() const {
    return size;
  }

  template <class Type>
  void DynamicList<Type>::Clear() {
    size = 0;
    delete[] buffer;
    buffer = nullptr;
  }

  template <class Type>
  int DynamicList<Type>::IndexOf(Type& value) const {
    for (int index = 0; index < size; ++index) {
      if (buffer[index] == value) {
        return index;
      }
    }

    return -1;
  }
  
  template <class Type>
  Type* DynamicList<Type>::begin() {
    return &buffer[0];
  }

  template <class Type>
  const Type* DynamicList<Type>::begin() const {
    return &buffer[0];
  }

  template <class Type>
  Type* DynamicList<Type>::end() {
    return &buffer[size];
  }

  template <class Type>
  const Type* DynamicList<Type>::end() const {
    return &buffer[size];
  }

  template <class Type>
  Type DynamicList<Type>::operator[](int index) const {
    if (!((index >= 0) && (index < size))) {
      int i = 1;
    }

    assert((index >= 0) && (index < size));
    return buffer[index];
  }

  template <class Type>
  Type& DynamicList<Type>::operator[](int index) {
    if (!((index >= 0) && (index < size))) {
      int i = 1;
    }

    assert((index >= 0) && (index < size));
    return buffer[index];
  }

  template <class Type>
  void DynamicList<Type>::PushFront(const Type& value) {
    Type* newBuffer = new Type[size + 1];
    assert(newBuffer != nullptr);
    newBuffer[0] = value;

    for (int index = 0; index < size; ++index) {
      newBuffer[index + 1] = buffer[index];
    }

    delete[] buffer;
    buffer = newBuffer;
    ++size;
  }

  template <class Type>
  void DynamicList<Type>::PushFront(const DynamicList& list) {
    Type* newBuffer = new Type[size + list.size];
    assert(newBuffer != nullptr);

    for (int index = 0; index < list.size; ++index) {
      newBuffer[index] = list.buffer[index];
    }

    for (int index = 0; index < size; ++index) {
      newBuffer[index + list.size] = buffer[index];
    }

    delete[] buffer;
    buffer = newBuffer;
    size += list.size;
  }

  template <class Type>
  void DynamicList<Type>::PushBack(const Type& value) {
    Type* newBuffer = new Type[size + 1];
    assert(newBuffer != nullptr);

    for (int index = 0; index < size; ++index) {
      newBuffer[index] = buffer[index];
    }

    newBuffer[size++] = value;
    delete[] buffer;
    buffer = newBuffer;
  }

  template <class Type>
  void DynamicList<Type>::PushBack(const DynamicList& list) {
    Type* newBuffer = new Type[size + list.size];
    assert(newBuffer != nullptr);

    for (int index = 0; index < size; ++index) {
      newBuffer[index] = buffer[index];
    }

    for (int index = 0; index < list.size; ++index) {
      newBuffer[size + index] = list.buffer[index];
    }

    delete[] buffer;
    buffer = newBuffer;
    size += list.size;
  }

  template <class Type>
  void DynamicList<Type>::Insert(int insertIndex,
                                 const Type& value) {
    assert((insertIndex >= 0) && (insertIndex <= size));
    Type* newBuffer = new Type[size + 1];
    assert(newBuffer != nullptr);

    for (int index = 0; index < insertIndex; ++index) {
      newBuffer[index] = buffer[index];
    }

    newBuffer[insertIndex] = value;

    for (int index = 0; index < (size - insertIndex); ++index) {
      newBuffer[insertIndex + index + 1] =
        buffer[insertIndex + index];
    }

    delete[] buffer;
    buffer = newBuffer;
    ++size;
  }

  template <class Type>
  void DynamicList<Type>::Insert(int insertIndex,
                                 const DynamicList& list){
    assert((insertIndex >= 0) && (insertIndex <= size));
    Type* newBuffer = new Type[size + list.size];
    assert(newBuffer != nullptr);

    for (int index = 0; index < insertIndex; ++index) {
      newBuffer[index] = buffer[index];
    }

    for (int index = 0; index < list.size; ++index) {
      newBuffer[insertIndex + index] = list.buffer[index];
    }

    for (int index = 0; index < (size - insertIndex); ++index) {
      newBuffer[insertIndex + index + list.size] =
        buffer[insertIndex + index];
    }

    delete[] buffer;
    buffer = newBuffer;
    size += list.size;
  }

  template <class Type>
  void DynamicList<Type>::Erase(int eraseIndex) {
    assert((eraseIndex >= 0) && (eraseIndex < size));
    Type* newBuffer = new Type[size - 1];
    assert(newBuffer != nullptr);

    for (int index = 0; index < eraseIndex; ++index) {
      newBuffer[index] = buffer[index];
    }

    for (int index = 0; index < (size - (eraseIndex + 1));
         ++index) {
      newBuffer[eraseIndex + index] =
        buffer[eraseIndex + index + 1];
    }

    delete[] buffer;
    buffer = newBuffer;
    --size;
  }

  template <class Type>
  void DynamicList<Type>::Remove(int firstIndex /* = 0 */, 
                                 int lastIndex /* = -1 */) {
    if (lastIndex == -1) {
      lastIndex = size - 1;
    }

    assert((firstIndex >= 0) && (firstIndex < size));
    assert((lastIndex >= 0) && (lastIndex < size));
    assert(firstIndex <= lastIndex);

    int removeSize = lastIndex - firstIndex + 1;
    Type* newBuffer = new Type[size - removeSize];
    assert(newBuffer != nullptr);

    for (int index = 0; index < firstIndex; ++index) {
      newBuffer[index] = buffer[index];
    }

    for (int index = 0;
         index < (size - (firstIndex + removeSize)); ++index){
      newBuffer[firstIndex + index] =
        buffer[firstIndex + index + removeSize];
    }

    delete[] buffer;
    buffer = newBuffer;
    size -= removeSize;
  }

  template <class Type>
  void DynamicList<Type>::Copy(DynamicList& copyArray, int firstIndex/* =0 */,
                               int lastIndex /* = -1 */) const {
    if (lastIndex == -1) {
      lastIndex = size - 1;
    }

    assert((firstIndex >= 0) && (firstIndex < size));
    assert((lastIndex >= 0) && (lastIndex < size));
    assert(firstIndex <= lastIndex);

    for (int index = firstIndex; index <= lastIndex; ++index) {
      copyArray.PushBack(buffer[index]);
    }
  }

  template <class Type>
  void DynamicList<Type>::CopyRemove(DynamicList& copyArray,
                          int firstIndex /* = 0 */,int lastIndex /* = -1 */) {
    copy(copyArray, firstIndex, lastIndex);
    remove(firstIndex, lastIndex);
  }

  template <class Type>
  bool DynamicList<Type>::AnyOf(IfFuncPtr ifFuncPtr,
                          void* ifVoidPtr /* = nullptr */) const {
    for (int index = 0; index < size; ++index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        return true;
      }
    }

    return false;
  }

  template <class Type>
  bool DynamicList<Type>::AllOf(IfFuncPtr ifFuncPtr,
                          void* ifVoidPtr /* = nullptr */) const {
    for (int index = 0; index < size; ++index) {
      if (!ifFuncPtr(buffer[index], ifVoidPtr)) {
        return false;
      }
    }

    return true;
  }

  template <class Type>
  bool DynamicList<Type>::FirstOf(IfFuncPtr ifFuncPtr,
              Type& value, void* ifVoidPtr /* = nullptr */) const{
    for (int index = 0; index < size; ++index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        value = buffer[index];
        return true;
      }
    }

    return false;
  }

  template <class Type>
  bool DynamicList<Type>::LastOf(IfFuncPtr ifFuncPtr, Type& value,
                          void* ifVoidPtr /* = nullptr */) const {
    for (int index = (size - 1); index >= 0; --index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        value = buffer[index];
        return true;
      }
    }

    return false;
  }

  template <class Type>
  void DynamicList<Type>::Apply(DoFuncPtr doFuncPtr,
                                void* doVoidPtr /* = nullptr */) {
    for (int index = 0; index < size; ++index) {
      doFuncPtr(buffer[index], doVoidPtr);
    }
  }

  template <class Type>
  void DynamicList<Type>::ApplyIf(IfFuncPtr ifFuncPtr,
         DoFuncPtr doFuncPtr, void* ifVoidPtr /* = nullptr */,
         void* doVoidPtr /* = nullptr */){
    for (int index = 0; index < size; ++index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        doFuncPtr(buffer[index], doVoidPtr);
      }
    }
  }

  template <class Type>
  void DynamicList<Type>::CopyIf(IfFuncPtr ifFuncPtr,
                          DynamicList& copyList,
                          void* ifVoidPtr /* = nullptr */) const {
    for (int index = 0; index < size; ++index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        copyList.PushBack(buffer[index]);
      }
    }
  }

  template <class Type>
  void DynamicList<Type>::RemoveIf(IfFuncPtr ifFuncPtr,
                                void* ifVoidPtr /* = nullptr */) {
    for (int index = 0; index < size; ++index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        Erase(index--);
      }
    }
  }

  template <class Type>
  void DynamicList<Type>::CopyRemoveIf(IfFuncPtr ifFuncPtr,
         DynamicList& copyArray, void* ifVoidPtr /* = nullptr */) {
    for (int index = 0; index < size; ++index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        copyArray.PushBack(buffer[index]);
        Erase(index--);
      }
    }
  }

  template <class Type>
  void DynamicList<Type>::ApplyRemoveIf(IfFuncPtr ifFuncPtr,
         DoFuncPtr doFuncPtr, void* ifVoidPtr /* = nullptr */,
         void* doVoidPtr /* = nullptr */) {
    for (int index = 0; index < size; ++index) {
      if (ifFuncPtr(buffer[index], ifVoidPtr)) {
        doFuncPtr(buffer[index], doVoidPtr);
        Erase(index--);
      }
    }
  }
};