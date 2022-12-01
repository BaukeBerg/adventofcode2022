#include "AdventClass.h"

#include "FileUtility.h"

class TestAssignment1 : public TAdventClass
{

};

TEST_F(TestAssignment1, TestDayOne)
{
  TCVector<wxInt32> CaloryList(1, 0);
  for (auto& Line : ReadFileLines(RealInput(), true))
  {
    if (Line.IsEmpty())
    {
      CaloryList.push_back(0);
    }
    else
    {
      CaloryList.at(CaloryList.size() - 1) += StringToInt<wxInt32>(Line);
    }    
  }
  ASSERT_LE(3, CaloryList.size()) << "Error while filling array";
  CaloryList.Sort();
  auto Amount = CaloryList.size();
  EXPECT_EQ(71506, RecordProperty("Max: ", CaloryList.back()));
  EXPECT_EQ(209603, RecordProperty("Max 3: ", CaloryList.at(Amount-1) + CaloryList.at(Amount-2) + CaloryList.at(Amount-3)));    
}
