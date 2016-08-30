#if 0
struct PageInfo {
  PageInfo();
  PageInfo(int firstParagraph, int lastParagraph, int height);

  bool WritePageInfoToStream(ostream& outStream) const;
  bool ReadPageInfoFromStream(istream& inStream);

  int m_firstParagraph, m_lastParagraph, m_height;
};
#endif