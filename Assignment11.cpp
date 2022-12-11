#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

#include <set>

#include "LimitFunctions.h"

#include "ToStringConversion.h"
#include <numeric>
#include <StringHandler.h>

class TestAssignment11 : public TAdventClass
{

};

TEST_F(TestAssignment11, TestDayEleven)
{
  struct TItemInfo {
    wxInt64 Level;
    wxInt64 NextMonkey;
  };

  class TMonkey
  {
  public:
    TMonkey(wxInt64 Index, TCVector<TCVector<TItemInfo>>* Items)
    {
      
      this->Index = Index;
      this->Items = Items;
    }

    void Parse(wxString Line)
    {
      Line = Line.Trim(false);
      Line.Replace(":", ";");
      if (Line.StartsWith("Start"))
      {
        auto Content = Split(Line, ";")[1];
        Items->push_back(TCVector<TItemInfo>());
        for (auto& Item : Split(Content, ","))
        {
          Items->at(Index).push_back({ StringToInt<wxInt64>(Item), Index });
        }
      }
      else if (Line.StartsWith("Operation"))
      {
        ;

      }
      else if (Line.StartsWith("Test"))
      {
        Divider = StringToInt<wxInt64>(Line);
      }
      else if (Line.StartsWith("If true"))
      {
        TargetTrue = StringToInt < wxInt64>(Line);
      }
      else if (Line.StartsWith("If false"))
      {
        TargetFalse = StringToInt < wxInt64>(Line);
      }
    }
    void OperateItems(void)
    {
      TCVector<TItemInfo> UpdatedWorryLevel;
      for (auto& Item : Items->at(Index))
      {
        UpdatedWorryLevel.push_back(PerformActions(Item.Level));
        ++InspectionCycles;
      }
      Items->at(Index).clear();
      for (auto& Item : UpdatedWorryLevel)
      {
        Items->at(Item.NextMonkey).push_back(Item);
      }      
    }

    TItemInfo PerformActions(wxInt64 Value) const
    {
      //if (Index != (Real ? 6 : 2))
      {
        Value = DoOperation(Value);
      }
      Value = Value / 3;
      auto Target = ((Value % Divider) == 0LL) ? TargetTrue : TargetFalse;
      if (Value < 0)
      {
        TLogError("Overflow");
      }
      return { Value, Target };
    }
    

    wxInt64 DoOperation(wxInt64 Value) const 
    {
      switch (Index)
      {
      case 0: return Real() ? (Value * 7LL) : (Value * 19LL);
      case 1: return Real() ? (Value * 11LL) : (Value + 6LL);
      case 2: return Real() ? (Value + 8LL) : (Value * Value);
      case 3: return Real() ? (Value + 7LL) : (Value + 3LL);
      case 4: return (Value + 5LL);
      case 5: return (Value + 4LL);
      case 6: return (Value * Value);
      case 7: return (Value + 3LL);
      }
    }

    bool Real(void) const {
      return Items->size() > 4;
    }

    wxString AsString(void) const
    {
      wxString ReturnValue("Monkey ");
      ReturnValue += ToString(Index) + ". Content: {";
      for (auto& Item : Items->at(Index)) {
        ReturnValue += IntToString<wxInt64>(Item.Level) + ", ";
      }
      ReturnValue.RemoveLast(2);
      return ReturnValue + "}";
    }
    wxInt64 Divider;
    wxString Operation;
    TCVector<TCVector<TItemInfo>>* Items;
    wxInt64 Index;
    wxInt64 TargetTrue;
    wxInt64 TargetFalse;
    wxInt64 InspectionCycles = 0;
  };

  TCVector<TMonkey> Monkeys;
  TCVector<TCVector<TItemInfo>> ItemList;
  auto Input = ReadFileLines(RealInput());
  for (auto& Line : Input)
  {
    if (Line.StartsWith("Monkey"))
    {
      Monkeys.push_back(TMonkey(Monkeys.size(), &ItemList));
    }
    else
    {
      Monkeys.back().Parse(Line);
    }
  }

  for (auto& Monkey : Monkeys)
  {
    RecordProperty(Monkey.AsString());
  }

  for (auto Iterator = 0; 20 > Iterator; ++Iterator)
  {
    for (auto& Monkey : Monkeys)
    {
      Monkey.OperateItems();
    }
    /*for (auto& Monkey : Monkeys)
    {
      RecordProperty(Monkey.AsString());
    }*/

  }
  TCVector<wxInt64> Cycles;
  for (auto& Monkey : Monkeys)
  {
    Cycles.push_back(RecordProperty("Cycles: ", Monkey.InspectionCycles));
    //EXPECT_EQ(Expected.at(Monkeys.IndexOf(Monkey)), Cycles.back());
   // RecordProperty("Content: ", Monkey.AsString());

  }
  Cycles.Sort();
  auto Value = Cycles.at(Cycles.size()-1) * Cycles.at(Cycles.size()-2);
  EXPECT_EQ(54752, RecordProperty("CycleValue: ", Value));
}
