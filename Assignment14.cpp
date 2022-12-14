#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"


class TestAssignment14 : public TAdventClass
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
      return front().size();
    }
    wxInt32 Rows() const {
      return size();
    }
    void Set(wxPoint Pos, wxInt32 Value) {
      at(Pos.y).at(Pos.x) = Value;
    }
  };

};

TEST_F(TestAssignment14, TestSamples)
{
  TGrid Grid;

  auto Point = [](wxString Input)
  {
    auto Xy = Split(Input, ",");
    return wxPoint(StringToInt<wxInt32>(Xy[0]), StringToInt<wxInt32>(Xy[1]));
  };

  auto MaxY = 0;

  Grid.resize(3000, TCVector<wxInt32>(3000, wxNOT_FOUND));
  for (auto& Line : ReadFileLines(RealInput()))
  {

    auto Tokens = Split(Line, ">");

    for (auto Index = 1; Index < Tokens.size(); ++Index)
    {
      auto To = Point(Tokens.at(Index));
      auto From = Point(Tokens.at(Index - 1));
      From.x += 1000;
      To.x += 1000;
      Grid.Set(From, 1);
      Grid.Set(To, 1);
      MaxY = Maximum(From.y, MaxY);
      MaxY = Maximum(To.y, MaxY);

      if (To.x == From.x)
      {
        auto Increment = From.y < To.y ? 1 : -1;
        for (auto Iterator = From.y; Iterator != To.y; Iterator += Increment)
        {
          Grid.Set({ From.x, Iterator }, 1);
        }
      }
      else if (From.y == To.y)
      {
        auto Increment = From.x < To.x ? 1 : -1;
        for (auto Iterator = From.x; Iterator != To.x; Iterator += Increment)
        {
          Grid.Set({ Iterator, From.y }, 1);
        }
      }
    }
  }
  for (auto Iterator = 0; Iterator < 3000; ++Iterator) {
    Grid.Set({ Iterator, MaxY + 2 }, 1);
  }

  //RecordProperty("Grid Filled");
  //for (auto& Item : Grid) {
  //  wxString Line = "";
  //  for (auto& Char : Item) {
  //    Line += Char == wxNOT_FOUND ? '.' : '#' ;
  //  }
  //  RecordProperty(Line);
  //}
  wxInt32 SandDrops = 0, TotalSand = 0;
  bool SandDropped = false;
  for (auto Iterator = 0; !SandDropped; ++Iterator)
  {
    wxPoint Start(1500, 0);
    while (true)
    {
      Start += wxPoint(0, 1);
      if (Start.y >= 1500)
      {
        SandDrops = Iterator;
        RecordProperty(wxString("Off the grid after: ") + Iterator + "Drops of sand");
        SandDropped = true;
        break;
      }
      else if (Grid.Value(Start) != wxNOT_FOUND)
      {
        if (Grid.Value(Start + wxPoint(-1, 0)) == wxNOT_FOUND)
        {
          Start += wxPoint(-1, 0);
        }
        else if (Grid.Value(Start + wxPoint(1, 0)) == wxNOT_FOUND)
        {
          Start += wxPoint(1, 0);
        }
        else
        {
          Grid.Set(Start + wxPoint(0, -1), 0); // All sand / rock => Block right above    
          if (Grid.Value({ 1500,0 }) != wxNOT_FOUND)
          {
            TotalSand = Iterator++;
            RecordProperty(wxString("Grid full after: ") + TotalSand + "Drops of sand");
            SandDropped = true;
            break;
          }
          break;
        }
      }
    }


  }

  EXPECT_EQ(888, SandDrops);
  EXPECT_EQ(26461, TotalSand);

  for (auto Index = 0; Index <= MaxY; ++Index) {
    wxString Line = "";
    for (auto& Char : Grid[Index])
    {
      switch (Char)
      {
      case -1:
      {
        Line += '_';
        break;
      }
      case 0:
      {
        Line += '0';
        break;
      }
      case 1:
      {
        Line += '#';
        break;
      }
      }
    }
    RecordProperty(Line);
  }
}

