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


  struct TBeaconInfo {
    wxPoint Scanner;
    wxPoint Beacon;
    wxInt32 Steps;
  };

  bool PointInDiamond(TBeaconInfo Info, wxPoint Pos)
  {
    auto Dist = Info.Scanner.y - Pos.y;
    auto SizeOfDiamond = Info.Steps - Dist;
    return InLimits(Info.Scanner.x-SizeOfDiamond, Pos.x, Info.Scanner.x+SizeOfDiamond);
  }

  bool OutsideAllDiamonds(wxPoint Pos) 
  {
    if (InLimits(0, Pos.x, 4000000)
      && InLimits(0, Pos.y, 4000000))
    {
      bool UniquePos = true;
      for (auto& Item : AllBeacons)
      {
        if(PointInDiamond(Item, Pos))
        {          
          return false;
        }
      }
      return true;
    }
    return false;
  }


  TCVector<TBeaconInfo> AllBeacons;
};

TEST_F(TestAssignment15, TestSamples)
{
  auto Point = [&](const wxString& Info) {
    auto Token = Split(Info, ",");
    return wxPoint(StringToInt<wxInt32>(Token[0]), StringToInt<wxInt32>(Token[1]));
  };

  auto i = 0;
  for (auto& Line : ReadFileLines(RealInput()))
  {
    auto Info = Split(Line, "b");
    auto Scanner = Point(Info[0]);
    auto Beacon = Point(Info[1]);
    auto Steps = abs(Scanner.x - Beacon.x) + abs(Scanner.y - Beacon.y);
    AllBeacons.push_back({ Scanner, Beacon, Steps });
  }
  
  for (auto& Item : AllBeacons)
  {
    for (auto Iterator = 0; Iterator < Item.Steps; ++Iterator)
    {
      TCVector<wxPoint> Points = { 
        Item.Scanner + wxPoint(Item.Steps - Iterator+1, Iterator),
        Item.Scanner + wxPoint(-Item.Steps + Iterator-1, -Iterator),
        Item.Scanner + wxPoint(Iterator+1, Item.Steps - Iterator),
        Item.Scanner + wxPoint(-Iterator-1, -Item.Steps + Iterator) };

      for (auto& Point : Points)
      {
        if (OutsideAllDiamonds(Point))
        {
          EXPECT_EQ(11583882601918LL, RecordProperty("Beacon value: " + wxInt64(Point.x) * 4000000LL + wxInt64(Point.y)));
        }
      }
    }
  }
  return;
}
//
//void CheckPart1(void)
//{
//  auto LineToCheck = 2000000;
//  wxInt32 MinY = wxINT32_MAX;
//  wxInt32 MaxY = wxINT32_MIN;
//
//  struct TInfo {
//    wxPoint Start;
//    wxInt32 Steps;
//    bool I
//  };
//
//  TCVector<TInfo> AllSteps;
//
//  wxInt32 DistressFrequency = 0;
//
//
//    auto Steps = abs(Beacon.x - Scanner.x) + abs(Beacon.y - Scanner.y) + 1;
//    AllSteps.push_back({ Scanner, Steps });
//  }
//
//  for (auto& Item : AllSteps)
//  {
//     TCVector<wxPoint> Points = { Item.Start + wxPoint(0, Item.Steps), Item.Start + wxPoint(Item.Steps, 0), Item.Start + wxPoint(-Item.Steps, 0), Item.Start + wxPoint(0, -Item.Steps) };
//     /*for (auto& Pos : Points)
//     {
//       if (!InLimits(0, Pos.x, 4000000)
//         || !InLimits(0, Pos.y, 4000000))
//       {
//         continue;
//       }
//       RecordProperty("Beacon frequency: ", (4000000 * Pos.x) + Pos.y);
//
//     }
//
//
//   }*/
//   /*
//   for(auto Item : AllSteps)
//   {
//     auto LineSize = 1;
//     auto Pos = Scanner + wxPoint(0, -Steps);
//
//
//     for (auto Iterator = 0; Iterator < (2 * Steps + 1); ++Iterator)
//     {
//
//       if (Pos.y <= Scanner.y)
//       {
//         LineSize++;
//       }
//       if (Pos.y > Scanner.y)
//       {
//         LineSize--;
//       }
//
//       if (Pos.y == 2000000)
//       {
//         MinY = Minimum(MinY, Pos.x);
//         MaxY = Maximum(MaxY, Pos.x);
//       }
//       Pos += wxPoint(0, 1);
//     }
//
//
//
//
//   }
//   a
//
//   uto Delta = MaxY - MinY;
//   */
//    RecordProperty("Blocked cells: ", Delta);
//    EXPECT_EQ(4985193, Delta);
//  }
//  EXPECT_EQ(11583882601918, DistressFrequency);
//
//}
//
