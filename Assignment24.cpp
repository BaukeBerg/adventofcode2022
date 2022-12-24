#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

using List = TCVector<wxInt32>;

wxString Print(List TheItems)
{
  wxString Total = "{";
  auto Size = TheItems.size();
  switch (Size)
  {
  case 0:
  {
    return "{_}";
  }
  case 1:
  {
    return wxString("{") + wxUniChar(TheItems[0]) + "}";
  }
  default:
  {
    return wxString("{") + Size + "}";
  }
  }
}
  
//  if (TheItems.size() > 1)
//  {
//    Total += IntToString(TheItems.size());
//    Total += "}";
//    return Total;
//  }
//
//  for (auto& Item : TheItems)
//  {
//    Total += wxUniChar(Item);
//    Total += ",";
//  }
//  if (TheItems.size() > 0)
//  {
//    Total.RemoveLast();
//  }
//  else
//  {
//    Total += "_";
//  }
//  Total += "}";
//  return Total;
//}

List Fill(wxUniCharRef Val)
{
  List Items;
  Items.push_back(Val.GetValue());
  return Items;
}

wxInt32 Count = 0;

class TGrid : public TCVector<TCVector<List>>
{
public:
  void Add(const wxString& Line)
  {
    TCVector<List> Items;
    for (auto& Char : Line)
    {
      Items.push_back({ (wxInt32)Char.GetValue() });
    }
    push_back(Items);
  }

  void Set(wxPoint Point, List Val) {
    at(Point.y).at(Point.x) = Val;
  }

  TCVector<wxPoint> NeighBours(wxPoint This)
  {
    TCVector<wxPoint> NeighBours;
    for (auto& Point : { This + wxPoint(-1,0), This + wxPoint(1,0), This + wxPoint(0,-1), This + wxPoint(0,1) })
    {
      if (InLimits(0, Point.x, X() - 1)
        && InLimits(0, Point.y, Y() - 1)
        && (!At(Point).Contains('#')))
      {
        NeighBours.push_back(Point);
      }
    }
    return NeighBours;
  }

  List At(wxPoint Point) {
    return at(Point.y).at(Point.x);
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
    wxString Total = IntToString(Count++) + "\nGrid:\n";
    for (auto& X : *this)
    {
      for (auto& Char : X)
      {
        Total += Print(Char);
      }
      Total += "\n";
    }
    return Total;
  }
};

class TestAssignment24 : public TAdventClass
{
public:
  void PerformOperation(TGrid* Grid)
  {
    TGrid Next;
    Next.resize(Grid->Y(), TCVector<List>(Grid->X(), {}));
    wxPoint Current(0, 0);
    while (true)
    {
      for (auto& Item : Grid->At(Current))
      {
        auto NextPoint = Current;
        wxPoint Reset = Current;
        switch (Item)
        {
        case '#':
        {
          Next.Set(Current, { Item });
          continue;
        }
        case '.':
        {
          continue;
          break;
        }
        case '>':
        {
          NextPoint += wxPoint(1, 0);
          Reset = wxPoint(1, NextPoint.y);
          break;
        }
        case '<':
        {
          NextPoint += wxPoint(-1, 0);
          Reset = wxPoint(Grid->X() - 2, NextPoint.y);
          break;
        }
        case 'v':
        {
          NextPoint += wxPoint(0, 1);
          Reset = wxPoint(NextPoint.x, 1);
          break;
        }
        case '^':
        {
          NextPoint += wxPoint(0, -1);
          Reset = wxPoint(NextPoint.x, Grid->Y() - 2);
          break;
        }

        }
        if (Grid->At(NextPoint).Contains('#'))
        {
          Next.Set(NextPoint, { '#' });
          NextPoint = Reset;
        }

        auto Now = Next.At(NextPoint);
        Now.push_back(Item);
        Next.Set(NextPoint, Now);
      }
      Current += wxPoint(1, 0);
      if (Current.x == Grid->X())
      {
        Current = wxPoint(0, Current.y + 1);
      }
      if (Current.y == Grid->Y())
      {
        *Grid = Next;
        return;
      }
    }
  }
};

TEST_F(TestAssignment24, RunIt)
{
  TGrid Grid;
  for (auto& Line : ReadFileLines(RealInput()))
  {
    Grid.Add(Line);
  }
  RecordProperty(Grid.AsString());

  auto RunSession = [&](wxPoint Start, wxPoint End)
  {
    TCVector<TCVector<wxPoint>> ExploredRoutes;
    TCVector<TCVector<wxPoint>> TotalRoutes;
    auto Shortest = 500;
    ExploredRoutes.push_back({ Start });
    for (auto Iterator = 1; ExploredRoutes.size() > 0; ++Iterator)
    {
      PerformOperation(&Grid);
      //RecordProperty(Grid.AsString());
      TCVector<TCVector<wxPoint>> NextIteration;
      TCVector<wxPoint> Visited;
      for (auto& Route : ExploredRoutes)
      {
        auto ThisPoint = Route.back();
        auto PointsToCheck = Grid.NeighBours(ThisPoint);
        PointsToCheck.push_back(ThisPoint);
        for (auto& Item : PointsToCheck)
        {
          if (Item == End)
          {
            Route.push_back(Item);
            TotalRoutes.push_back(Route);
            Shortest = Iterator;
            return Shortest;// goto end;
          }
          auto GridContent = Grid.At(Item);
          if (GridContent.empty()
            && (Route.size() < Shortest)
            && !Visited.Contains(Item))
          {
            Route.push_back(Item);
            NextIteration.push_back(Route);
            Visited.push_back(Item);
          }
        }
      }

      ExploredRoutes = NextIteration;
      std::sort(ExploredRoutes.begin(), ExploredRoutes.end(), [](const wxVector<wxPoint>& Left, const wxVector<wxPoint>& Right) { return Left.size() < Right.size(); });
    }
  };

end:
  wxPoint Start = wxPoint(1, 0);
  wxPoint End = wxPoint(Grid.X() - 2, Grid.Y() - 1);
  auto Minutes = 0;
  Minutes = RecordProperty("Shortest path: ", RunSession(Start, End));
  Minutes += RecordProperty("Back to start: ", RunSession(End, Start));
  Minutes += RecordProperty("Back to end: ", RunSession(Start, End));
  RecordProperty("Total trip: ", Minutes);
}