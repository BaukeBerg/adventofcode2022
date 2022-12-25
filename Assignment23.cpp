#include "AdventClass.h"

#include "FileUtility.h"

#include <numeric>

#include "LimitFunctions.h"

using List = TCVector<wxInt32>;

class TGrid : public TCVector<TCVector<wxInt32>>
{
public:
  void Add(const wxString& Line)
  {
    TCVector<wxInt32> Items;
    for (auto& Char : Line)
    {
      Items.push_back(Char.GetValue());
    }
    push_back(Items);
  }

  void Set(wxPoint Point, wxInt32 Val)
  {
    at(Point.y).at(Point.x) = Val;
  }

  TCVector<wxPoint> NeighBours(wxPoint This)
  {
    TCVector<wxPoint> NeighBours;
    for (auto& Point : { This + wxPoint(-1,0), This + wxPoint(1,0), This + wxPoint(0,-1), This + wxPoint(0,1) })
    {
      if (InLimits(0, Point.x, X() - 1)
        && InLimits(0, Point.y, Y() - 1)
        // && (!At(Point).Contains('#'))
        )
      {
        NeighBours.push_back(Point);
      }
    }
    return NeighBours;
  }

  wxInt32 At(wxPoint Point) {
    return at(Point.y).at(Point.x);
  }

  wxPoint EndPoint(void) const
  {
    return wxPoint(X() - 1, Y() - 1);
  }

  wxInt32 Y(void) const {
    return size();
  }

  wxInt32 X(void) const {
    if (size() > 0) {
      return front().size();
    }
    return 0;
  }

  wxString AsString(void) const
  {
    wxString Total = IntToString((*Count)++) + "\nGrid:\n";
    for (auto& X : *this)
    {
      for (auto& Char : X)
      {
        Total += Char == '.' ? '_' : wxUniChar(Char);
      }
      Total += "\n";
    }
    return Total;
  }
  wxInt32* Count = nullptr;
};



class TestAssignment23 : public TAdventClass
{
public:
  void ProposeChanges(TGrid* Grid)
  {
    struct TProposal {
      wxPoint Location;
      wxInt32 Proposal;
    };
    struct TSet {
      bool DupeNorth = false;
      bool DupeSouth = false;
      bool DupeEast = false;
      bool DupeWest = false;
      TCVector<wxInt32> Proposals;

    };
    TCVector<TProposal> AllSteps;
    TSet P;
    wxPoint Current(0, 0);

    auto EmptyBlock = [&](std::set<wxPoint> Items)
    {
      for (auto& Item : Items)
      {
        if (Grid->At(Item) == '#')
        {
          return false;
        }
      }
      return true;
    };

    auto Propose = [&]() 
    {

      std::set<wxPoint> North({ Current + wxPoint(-1,-1), Current + wxPoint(0, -1), Current + wxPoint(1,-1) });
      std::set<wxPoint> South({ Current + wxPoint(-1,1), Current + wxPoint(0, 1), Current + wxPoint(1,1) });
      std::set<wxPoint> East({ Current + wxPoint(1,-1), Current + wxPoint(1, 0), Current + wxPoint(1,-1) });
      std::set<wxPoint> West({ Current + wxPoint(-1,-1), Current + wxPoint(-1, 0), Current + wxPoint(-1,1) });
      std::set<wxPoint> All = North;
      All.insert(South.begin(), South.end());
      All.insert(East.begin(), East.end());
      All.insert(West.begin(), West.end());

      if (EmptyBlock(All))
      {
        return;      }

      if (EmptyBlock(North))
      {
        if (P.Proposals.Contains('N'))
        {
          P.DupeNorth = true;
        }
        AllSteps.push_back({ Current, P.Proposals.Add('N') });
      }

      if (EmptyBlock(South))
      {
        if (P.Proposals.Contains('S'))
        {
          P.DupeSouth = true;
        }
        AllSteps.push_back({ Current, P.Proposals.Add('S') });
      }
      if (EmptyBlock(East))
      {
        if (P.Proposals.Contains('E'))
        {
          P.DupeEast = true;
        }
        AllSteps.push_back({ Current, P.Proposals.Add('E') });
      }
      if (EmptyBlock(West))
      {
        if (P.Proposals.Contains('W'))
        {
          P.DupeWest = true;
        }
        AllSteps.push_back({ Current, P.Proposals.Add('W') });
      }

    };
    while (Current != Grid->EndPoint())
    {
      if (Grid->At(Current) == '#')
      {
        Propose();
      }
      for (auto& Proposal : AllSteps)
      {
        switch (Proposal.Proposal)
        {
        case 'N':
        {
          if (P.DupeNorth)
          {
            continue;
          }
          Grid->Set(Proposal.Location + wxPoint(0, -1), '#');
          Grid->Set(Proposal.Location, '.');
          break;
        }
        case 'E':
        {
          if (P.DupeEast)
          {
            continue;
          }
          Grid->Set(Proposal.Location + wxPoint(1, 0), '#');
          Grid->Set(Proposal.Location, '.');
          break;
        }
        case 'S':
        {
          if (P.DupeSouth)
          {
            continue;
          }
          Grid->Set(Proposal.Location + wxPoint(0, 1), '#');
          Grid->Set(Proposal.Location, '.');
          break;
        }
        case 'W':
        {
          if (P.DupeWest)
          {
            continue;
          }
          Grid->Set(Proposal.Location + wxPoint(-1, 0), '#');
          Grid->Set(Proposal.Location, '.');
          break;
        }
        }
      }
    }
  }



};

TEST_F(TestAssignment23, RunTest)
{
  TGrid Grid;
  auto Count = 0;
  Grid.Count = &Count;
  for (auto& Line : ReadFileLines(MiniInput()))
  {
    Grid.Add(Line);
  }
  RecordProperty("Grid: ", Grid.AsString());

  ProposeChanges(&Grid);

}