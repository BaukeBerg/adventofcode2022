#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

#include <set>

#include "LimitFunctions.h"

#include "ToStringConversion.h"

class TestAssignment8 : public TAdventClass
{

};

TEST_F(TestAssignment8, TestDayEight)
{
  TCVector<TCVector<wxInt32>> Grid;
  TCVector<wxInt32> ScenicScore;
  auto Input = ReadFileLines(RealInput());

  Grid.resize(Input.front().length());

  for (auto Iterator = 0; Iterator < Input.size(); ++Iterator)
  {
    for (auto& Char : Input.at(Iterator))
    {
      Grid.at(Iterator).push_back(StringToInt<wxInt32>(Char));
    }
  }
  
  auto IsVisible = [&](wxInt32 Row, wxInt32 Column) 
  {
    if ((Row == 0) || (Row == Grid.front().size() - 1)
      || (Column == 0) || (Column == Grid.size() - 1))
    {
      return 1;
    }
    auto ThisValue = Grid.at(Column).at(Row);
    TCVector<wxInt32> Up;
    TCVector<wxInt32> Down;
    TCVector<wxInt32> Left;
    TCVector<wxInt32> Right;
    
    for (auto CheckColumn = Column +1; CheckColumn < Grid.size(); ++CheckColumn)
    { 
      auto Val = Up.Add(Grid.at(CheckColumn).at(Row));
      if (Val >= ThisValue)
      {
        break;
      }
    }
    for (auto CheckColumn = Column-1; CheckColumn >= 0; --CheckColumn)
    {
      auto Val = Down.Add(Grid.at(CheckColumn).at(Row));
      if (Val >= ThisValue)
      {
        break;
      }
    }
    for (auto CheckRow = Row+1; CheckRow < Grid.front().size(); ++CheckRow)
    {
      auto Val = Right.Add(Grid.at(Column).at(CheckRow));
      if (Val >= ThisValue)
      {
        break;
      }
    }
    for (auto CheckRow = Row - 1; CheckRow >= 0; --CheckRow)
    {
      auto Val = Left.Add(Grid.at(Column).at(CheckRow));
      if (Val >= ThisValue)
      {
        break;
      }
    }

    ScenicScore.push_back(Up.size() * Down.size() * Left.size() * Right.size());

    auto Check = [&](TCVector<wxInt32> List) 
    {
      List.Sort();
      return List.back() < ThisValue;
    };

    if (Check(Up) || Check(Down) || Check(Left) || Check(Right))
    {
      return 1;
    }
    return 0;
  };

  auto TotalVisible = 0;
  for (auto Column = 0; Column < Grid.size(); ++Column)
  {
    for (auto Row = 0; Row < Grid.front().size(); ++Row)
    {
      if (1 == IsVisible(Row, Column))
      {
        RecordProperty("Visible: ", wxString::Format("(%i,%i)", Column, Row));
        ++TotalVisible;
      }
    }
  }
  ScenicScore.Sort();
  EXPECT_EQ(1816, RecordProperty("Total Visible trees: ", TotalVisible));
  EXPECT_EQ(383520, RecordProperty("Scenic score: ", ScenicScore.back()));

}
