#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"

class TItem {
public:
  TItem(wxInt32 Value = wxNOT_FOUND)
  {
    this->Value = Value;
  }
  wxInt32 Value;
  TCVector<TItem*> SubList;

  wxInt32 ItemValue(void) const
  {
    if (Value != wxNOT_FOUND) {
      return Value;
    }
    if (SubList.size() == 0) {
      return wxNOT_FOUND;
    }
    return SubList.front()->ItemValue();
  }

  wxString Print(void) const {
    if ((Value == wxNOT_FOUND)
      && SubList.empty())
    {
      return "[]";
    }
    if (Value != wxNOT_FOUND)
    {
      return IntToString(Value);
    }
    wxString Cumulative = "[";
    for (auto& Item : SubList)
    {
      Cumulative += Item->Print() + ",";
    }
    Cumulative.RemoveLast();
    return Cumulative + "]";
  }

};

bool Compare(const TItem& Left, const TItem& Right)
{
  return (Left.ItemValue() < Right.ItemValue());
}

class TestAssignment13 : public TAdventClass
{
public:
  TItem Compose(wxString Input) {
    TItem List;
    TCVector<TItem*> Selected = { &List };
    auto First = true;
    while (0 < Input.length())
    {
      if (Input.StartsWith('['))
      {
        if (!First)
        {
          auto NewList = Selected.back()->SubList.Add(new TItem());
          Selected.push_back(NewList);
        }
        First = false;
        Input = Input.Mid(1);
      }
      else if (Input.StartsWith(']'))
      {
        Selected.pop_back();
        Input = Input.Mid(1);
      }
      else if (Input.StartsWith(','))
      {
        Input = Input.Mid(1);
      }
      else
      {
        auto Val = StringToInt<wxInt32>(Split(Input, ',')[0]);
        Selected.back()->SubList.Add(new TItem(Val));
        Input = Input.Mid(Val >= 10 ? 2 : 1);
      }
    }
    return List;
  }
};

TEST_F(TestAssignment13, TestSamples)
{
  auto Input1 = wxString("[1,1,3,1,1]");
  auto Input2 = wxString("[1,1,2,1,1]");
  auto Items = Compose(Input1);
  auto Items2 = Compose(Input2);
  using TSet = TCVector<TItem>;

  using TTestPair = struct {
    wxString Left; wxString Right;
  };

  auto Input = ReadFileLines(RealInput());
  TCVector<TTestPair> Pairs;
  for (auto Iterator = 0; Iterator < Input.size(); Iterator += 2)
  {
    Pairs.push_back({ Input[Iterator], Input[Iterator + 1] });
  }

  auto PairsIndices = 1;
  auto SummedValue = 0;
  for (auto Item : Pairs)
  {
    auto Left = Compose(Item.Left);
    auto Right = Compose(Item.Right);
    RecordProperty("Left: ", Left.Print());
    RecordProperty("Right: ", Right.Print());
    if (RecordProperty("Compare:", (1 == Compare(Left, Right))))
    {
      SummedValue += PairsIndices;
    }
    ++PairsIndices;
  }

  TCVector<TItem> AllItems;
  AllItems.push_back(Compose("[[6]]"));
  AllItems.push_back(Compose("[[2]]"));
  for (auto& Item : Input) {
    AllItems.push_back(Compose(Item));
  }


  std::sort(AllItems.begin(), AllItems.end(), Compare);

  auto Index1 = 0;
  auto Index2 = 0;
  for (auto Iterator = 0U; AllItems.size() > Iterator; ++Iterator)
  {
    if (AllItems.at(Iterator).Print() == "[[6]]") {
      Index1 = Iterator + 1;
    }
    if (AllItems.at(Iterator).Print() == "[[2]]") {
      Index2 = Iterator + 1;
    }

  }

  EXPECT_EQ(5393, RecordProperty("Ordered Indices:", SummedValue));
  EXPECT_EQ(26712, RecordProperty("IndexedItems", Index1 * Index2));


}
