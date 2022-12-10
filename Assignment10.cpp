#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

#include <set>

#include "LimitFunctions.h"

#include "ToStringConversion.h"
#include <numeric>

class TestAssignment10 : public TAdventClass
{

};

TEST_F(TestAssignment10, TestDayTen)
{
  auto AllInput = ReadFileLines(RealInput());
  auto TotalValue = 1LL;
  auto Count = -20;
  auto Summed = TCVector<wxInt64>();
  auto Position = 0;
  wxString AscII;

  for (auto& Line : AllInput) 
  {
    auto Amount = StringToInt<wxInt64>(Line);
    auto Cycles = 2;
    if (Line.StartsWith("no"))
    {
      Amount = 1;
      Cycles = 1;
    }
    for (auto Iterator = 0; Iterator < Cycles; ++Iterator)
    {
      AscII += (InLimits<wxInt32>(TotalValue - 1, Position++, TotalValue + 1) ? "0" : "_");
      if (Position % 40 == 0) 
      {
        AscII += "\n";
        Position = 0;
      }

      if (++Count % 40 == 0) 
      {
        Summed.push_back((Count + 20) * TotalValue);
      }
    }
    if (Line.StartsWith("add")) 
    {
      TotalValue += Amount;
    }
  }
  auto SummedValue = 0LL;
  for (auto& Item : Summed) 
  {
    RecordProperty("Value: ", Item);
    SummedValue += Item;
  }

  EXPECT_EQ(13680, RecordProperty("Sum: ", SummedValue));

  RecordProperty(AscII);


}
