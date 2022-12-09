#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

#include <set>

#include "LimitFunctions.h"

#include "ToStringConversion.h"
#include <numeric>

class TestAssignment9 : public TAdventClass
{

};

TEST_F(TestAssignment9, TestDayNine)
{
  TCVector<TCVector<wxInt32>> Grid;
  auto Input = ReadFileLines(RealInput());

  Grid.resize(500, TCVector<wxInt32>(500, 0));
  auto SmallTail = Grid;

  wxPoint HeadPosition(Grid.size() >> 1, Grid.size() >> 1);
  TCVector<wxPoint> TailPosition(9, HeadPosition);

  auto MarkPos = [&]() 
  {
    Grid[TailPosition[8].x][TailPosition[8].y] = 1;
    SmallTail[TailPosition[0].x][TailPosition[0].y] = 1;
  };

  MarkPos();

  auto ProcessTail = [&](wxInt32 Index) 
  {
    wxPoint Compare = Index == 0 ? HeadPosition : TailPosition[Index - 1];

    if ((abs(TailPosition[Index].x - Compare.x) <= 1)
      && (abs(TailPosition[Index].y - Compare.y) <= 1))
    {
      return;
    }
    else
    {
      if (Compare.x > TailPosition[Index].x) TailPosition[Index].x++;
      if (Compare.y > TailPosition[Index].y) TailPosition[Index].y++;
      if (Compare.x < TailPosition[Index].x) TailPosition[Index].x--;
      if (Compare.y < TailPosition[Index].y) TailPosition[Index].y--;
    }
  };
  
  auto ProcessTails = [&]()
  {
    for (auto Iterator = 0U; Iterator < TailPosition.size(); ++Iterator)
    {
      ProcessTail(Iterator);
    }
    MarkPos();
  };

  for (auto& Line : Input)
  {
    auto Amount = StringToInt(Line, 0);
    if (Line[0] == 'U') 
    {
      for (auto Iterator = 0; Amount > Iterator; ++Iterator) 
      {
        HeadPosition.y++;
        ProcessTails();
      }
    }
    else if (Line[0] == 'R') 
    {
      for (auto Iterator = 0; Amount > Iterator; ++Iterator) 
      {
        HeadPosition.x++;
        ProcessTails();
      }
    }
    else if (Line[0] == 'D') 
    {
      for (auto Iterator = 0; Amount > Iterator; ++Iterator) 
      {
        HeadPosition.y--;
        ProcessTails();
      }
    }
    else if (Line[0] == 'L') 
    {
      for (auto Iterator = 0; Amount > Iterator; ++Iterator) 
      {
        HeadPosition.x--;
        ProcessTails();
      }
    }
  }

  auto LargeTail = 0;
  auto SmallTotal = 0;
  for (auto Column = 0U; Grid.size() > Column; ++Column)
  {
    for (auto Cell = 0U; Grid.size() > Cell; ++Cell)
    {
      LargeTail += Grid[Column][Cell];
      SmallTotal += SmallTail[Column][Cell];
    }
  }

  EXPECT_EQ(6087, RecordProperty("Small tail total cells: ", SmallTotal));
  EXPECT_EQ(2493, RecordProperty("Large tail total cells: ", LargeTail));




}
