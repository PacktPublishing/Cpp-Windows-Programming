#include "..\\SmallWindows\\SmallWindows.h"

namespace SmallWindows {
  const Reference ZeroReference(0, 0);

  Reference::Reference()
   :row(0),
    col(0) {
    // Empty.
  }

  Reference::Reference(int row, int col)
   :row(row),
    col(col) {
    // Empty.
  }

  Reference::Reference(const Reference& ref)
   :row(ref.row),
    col(ref.col) {
    // Empty.
  }

  Reference& Reference::operator=(const Reference& ref) {
    if (this != &ref) {
      row = ref.row;
      col = ref.col;
    }

    return *this;
  }

  bool operator==(const Reference& ref1, const Reference& ref2) {
    return (ref1.row == ref2.row) && (ref1.col == ref2.col);
  }

  bool operator!=(const Reference& ref1, const Reference& ref2) {
    return !(ref1 == ref2);
  }

  bool operator<(const Reference& ref1, const Reference& ref2) {
    return (ref1.row < ref2.row) ||
           ((ref1.row == ref2.row) && (ref1.col < ref2.col));
  }

  bool operator<=(const Reference& ref1, const Reference& ref2) {
    return (ref1 < ref2) || (ref1 == ref2);
  }

  bool operator>(const Reference& ref1, const Reference& ref2) {
    return !(ref1 <= ref2);
  }

  bool operator>=(const Reference& ref1, const Reference& ref2) {
    return !(ref1 < ref2);
  }

  Reference& Reference::operator+=(const Reference& ref) {
    row += ref.row;
    col += ref.col;
    return *this;
  }

  Reference& Reference::operator-=(const Reference& ref) {
    row -= ref.row;
    col -= ref.col;
    return *this;
  }

  Reference operator+(const Reference& ref1,
                      const Reference& ref2) {
    return Reference(ref1.row + ref2.row, ref1.col + ref2.col);
  }

  Reference operator-(const Reference& ref1,
                      const Reference& ref2) {
    return Reference(ref1.row - ref2.row, ref1.col - ref2.col);
  }

  String Reference::ToString() const {
    String result;

    if (row >= 0) {
      result.push_back((TCHAR) (col + TEXT('a')));
    }
    else {
      result.push_back(TEXT('?'));
    }

    if (col >= 0) {
      result.append(to_String(row + 1));
    }
    else {
      result.push_back(TEXT('?'));
    }

    return result;
  }

  bool Reference::Inside(Reference minRef, Reference maxRef) {
    return ((minRef.row <= row) && (row <= maxRef.row) &&
            (minRef.col <= col) && (col <= maxRef.col));
  }

  bool Reference::WriteReferenceToStream(ostream& outStream)const{
    outStream.write((char*) &row, sizeof row);
    outStream.write((char*) &col, sizeof col);
    return ((bool) outStream);
  }

  bool Reference::ReadReferenceFromStream(istream& inStream) {
    inStream.read((char*) &row, sizeof row);
    inStream.read((char*) &col, sizeof col);
    return ((bool) inStream);
  }

  void Reference::WriteReferenceToClipboard(InfoList& infoList)
                                            const {
    infoList.AddValue<int>(row);
    infoList.AddValue<int>(col);
  }

  void Reference::ReadReferenceFromClipboard(InfoList& infoList) {
    infoList.GetValue<int>(row);
    infoList.GetValue<int>(col);
  }

  struct ReferenceStruct {int row, col;};

  void Reference::WriteReferenceToRegistry(String key) const {
    ReferenceStruct writeStruct = {row, col};
    Registry::WriteBuffer(key, &writeStruct, sizeof writeStruct);
  }

  void Reference::ReadReferenceFromRegistry(String key,
                      Reference defaultRef /* = ZeroReference */){
    ReferenceStruct readStruct;
    ReferenceStruct defaultStruct = {defaultRef.row, defaultRef.col};
    Registry::ReadBuffer(key, &readStruct, sizeof readStruct, &defaultStruct);
    row = readStruct.row;
    col = readStruct.col;
  }
}