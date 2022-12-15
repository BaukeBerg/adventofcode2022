#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"


class TestAssignment15 : public TAdventClass
{
public:
  class TGrid : public TCVector<TCVector<wxInt32>>
  {
  public:
    wxInt32 Value(wxInt32 x, wxInt32 y) {
      return Value({ y, x });
    }
    wxInt32 Value(wxPoint Pos) const {
      return at(Pos.y).at(Pos.x);
    }
    wxInt32 Columns() const {
      return size();
    }
    wxInt32 Rows() const {
      return front().size();
    }
    void Set(wxPoint Pos, wxInt32 Value) {
      at(Pos.y).at(Pos.x) = Value;
    }
  };

};

TEST_F(TestAssignment15, TestSamples)
{

  auto Point = [&](const wxString& Info) {
    auto Token = Split(Info, ",");
    return wxPoint(StringToInt<wxInt32>(Token[0]), StringToInt<wxInt32>(Token[1]));
  };

  auto LineToCheck = 2000000;
  wxInt32 MinY = wxINT32_MAX;
  wxInt32 MaxY = wxINT32_MIN;
  for (auto& Line : ReadFileLines(RealInput()))
  {
    auto Info = Split(Line, "b");
    auto Scanner = Point(Info[0]);
    auto Beacon = Point(Info[1]);
    auto Steps = abs(Beacon.x - Scanner.x) + abs(Beacon.y - Scanner.y) +1;
    auto LineSize = 1;
    auto Pos = Scanner + wxPoint(0, -Steps);
    for (auto Iterator = 0; Iterator < (2 * Steps + 1); ++Iterator)
    {
      
      if (Pos.y < Scanner.y)
      {
        LineSize++;
      }
      if (Pos.y > Scanner.y)
      {
        LineSize--;
      }
      
      if (Pos.y == 2000000)
      {
        MinY = Minimum(MinY, Pos.x);
        MaxY = Maximum(MaxY, Pos.x);
        
        
      }
      Pos += wxPoint(0, 1);
    }

  }
  auto Delta = MaxY - MinY;
  RecordProperty("Blocked cells: ", Delta);
  EXPECT_EQ(4985193, Delta);

}

