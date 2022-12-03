#include "AdventClass.h"

#include "FileUtility.h"

class TestAssignment3 : public TAdventClass
{

public:
  wxInt32 SumIt(const TCVector<wxInt32>& Numbers) const
  {
    auto Total = 0;
    for (auto& Item : Numbers)
    {
      Total += Item > 0x60 ? Item - 0x60 : Item - 0x40 + 26;
    }
    return Total;
  }
};

TEST_F(TestAssignment3, TestDayThree)
{
  auto AllData = ReadFileLines(RealInput());
  TCVector<wxInt32> CommonItem;
  
  for (auto& Item : AllData)
  {
    for (auto Index = 0U; ; ++Index)
    {
      auto FoundPos = Item.find_last_of(Item.at(Index));
      if (Index < FoundPos && (FoundPos >= Item.Len() >> 1))
      {
        CommonItem.push_back(Item.at(Index).GetValue());
        break;
      }
    }
  }
  EXPECT_EQ(7785, RecordProperty("Sum of priorities: ", SumIt(CommonItem))) << "Sum of prio";
}

TEST_F(TestAssignment3, TestPartTwo)
{
  TCVector<TCVector<wxString>> GroupInfo;
  auto AllInfo = ReadFileLines(RealInput());
  for (auto Index = 0U; Index < AllInfo.size(); Index += 3)
  {
    GroupInfo.push_back({ AllInfo.at(Index), AllInfo.at(Index + 1), AllInfo.at(Index + 2) });
  }

  auto FindCommons = [](TCVector<wxString> GroupInfo) {
    for (auto Index = 0U; ; ++Index) {
      auto Char = GroupInfo[0][Index];
      if ((wxNOT_FOUND != GroupInfo[1].find(Char))
        && (wxNOT_FOUND != GroupInfo[2].find(Char)))
      {
        return Char.GetValue();
      }
    }
    return 0U;
  };

  TCVector<wxInt32> Badges;
  for (auto& Group : GroupInfo)
  {
    Badges.push_back(FindCommons(Group));
  }
  
  EXPECT_EQ(2633, RecordProperty("Sum of badges: ", SumIt(Badges)));
}
