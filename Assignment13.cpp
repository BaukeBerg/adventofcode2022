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
    return SubList.front()->Value;
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

bool IsOrdered(const TItem& Left, const TItem& Right)
{

  auto Ordered = true;
  for (auto Iterator = 0; Iterator < Minimum(Left.SubList.size(), Right.SubList.size()); ++Iterator)
  {
    auto BothValues = Left.SubList.empty() && Right.SubList.empty();
    if (Left.SubList.at(Iterator)->ItemValue() > Right.SubList.at(Iterator)->ItemValue())
    {
      Ordered = false;
      break;
    }
  }
  return Ordered;
};

bool operator<(const TItem& Left, const TItem& Right)
{
  return IsOrdered(Left, Right);
}

class TestAssignment13 : public TAdventClass
{
public:
  TItem Compose(wxString& Input) {
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
    if (RecordProperty("Compare:", IsOrdered(Left, Right)))
    {
      SummedValue += PairsIndices;
    }
    ++PairsIndices;
  }
   

  EXPECT_EQ(5393, RecordProperty("Ordered Indices:", SummedValue));
  
}
