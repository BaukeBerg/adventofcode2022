#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

class TestAssignment12 : public TAdventClass
{

};

TEST_F(TestAssignment12, TestDayTwelve)
{
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
  };

  TGrid Grid;
  auto Input = ReadFileLines(MiniInput());
  //Grid.resize(Input.front().Len(), {});
  wxPoint Start;
  wxPoint End;
  for (auto& Line : Input)
  {
    Grid.push_back({});
    for (auto& Char : Line)
    {
      if (Char == 'S') {
        Start = wxPoint(Line.Find(Char), Grid.Rows() - 1);
        Grid.back().push_back(1);
      }
      else if (Char == 'E') {
        End = wxPoint(Line.Find(Char), Grid.Rows() - 1);
        Grid.back().push_back(1);
      }
      else
      {
        Grid.back().push_back(Char.GetValue() - 0x60);

      }
    }
  }

  using Trail = TCVector<wxPoint>;
  using Journey = TCVector<Trail>;
  Journey PossibleTrails;
  Trail CurrentTrail;
  Journey FinishedTrails;

  CurrentTrail.push_back(Start);

  auto LastStep = Start;

  auto NextSteps = [&](wxPoint Pos)
  {
    Trail Return;
    auto Current = Grid.Value(Pos);
    auto CheckAndAdd = [&](wxPoint Pos)
    {
      if ((Grid.Value(Pos) - Current) <= 1)
      {
        Return.push_back(Pos);
      }
    };

    if (Pos.x < Grid.Columns() - 1) {
      CheckAndAdd({ Pos.x + 1, Pos.y });
    }
    if (Pos.y < Grid.Rows() - 1) {
      CheckAndAdd({ Pos.x, Pos.y + 1 });
    }
    if (Pos.x > 0) {
      CheckAndAdd({ Pos.x - 1, Pos.y });
    }
    if (Pos.y > 0) {
      CheckAndAdd({ Pos.x, Pos.y - 1 });
    }


    return Return;
  };

  PossibleTrails.push_back(CurrentTrail);
  auto ShortestPath = 10000U;
  while (InLimits(1U, PossibleTrails.size(), 100000U))
  {
    Journey FoundTrails;
    for (auto Trail : PossibleTrails)
    {
      auto Item = Trail.back();
      for (auto& Step : NextSteps(Item))
      {
        if (Step == End || Trail.size() > ShortestPath)
        {
          if (Step == End)
          {
            FinishedTrails.push_back(Trail);
            ShortestPath = Minimum(Trail.size(), ShortestPath);
          }
        }
        else
        {
          auto NewLocation = true;
          for (auto& Item : Trail) {
            if (Step == Item)
            {
              NewLocation = false;
            }
          }
          if (NewLocation)
          {
            FoundTrails.push_back(Trail);
            FoundTrails.back().push_back(Step);
          }
        }
      }
    }
    PossibleTrails = FoundTrails;
  }

  if (PossibleTrails.size() > 0) {
    RecordProperty("Trail explosion");
  }

  auto ShortestSize = 1000U;
  for (auto& Trail : FinishedTrails) {
    ShortestSize = Minimum(ShortestSize, Trail.size());
  }
  RecordProperty("Smallest route to victory: ", ShortestSize);

}
