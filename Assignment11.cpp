#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

#include <set>

#include "LimitFunctions.h"

#include "ToStringConversion.h"
#include <numeric>
#include <StringHandler.h>

#include <iostream>

using namespace std;

#include "int512.h"

class TestAssignment11 : public TAdventClass
{

};

TEST_F(TestAssignment11, TestDayEleven)
{
  struct TItemInfo {
    int512 Level;
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
          int512 Value = r0();
          StringToInt(Item, &Value.x[0]);
          Items->at(Index).push_back({ Value, Index });
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

    TItemInfo PerformActions(int512 Value) const
    {
      Value = DoOperation(Value);
      int512 Divider = r0();
      Divider.x[0] = this->Divider;
      auto Target = (mod(Value, Divider).x[0] == 0) ? TargetTrue : TargetFalse;
      return { Value, Target };
    }

    int512 DoOperation(int512 Value) const
    {
      int512 TheValue = r0();
      switch (Index)
      {
      case 0:
      {
        TheValue.x[0] = (Real() ? 7 : 19);
        return multiplicar(Value, TheValue);
      }
      case 1:
      {
        TheValue.x[0] = Real() ? 11 : 6;
        return Real() ? multiplicar(Value, TheValue) : sumar(Value, TheValue);
      }
      case 2:
      {
        TheValue.x[0] = 8;
        return Real() ? sumar(Value, TheValue) : multiplicar(Value, Value);
      }
      case 3:
      {
        TheValue.x[0] = Real() ? 7 : 3;

        return sumar(Value, TheValue);
      }
      case 4:
      {
        TheValue.x[0] = 5;
        return sumar(Value, TheValue);
      }
      case 5: {
        TheValue.x[0] = 4;
        return sumar(Value, TheValue);
      }
      case 6:
      {
        return multiplicar(Value, Value);
      }
      case 7:
      {
        TheValue.x[0] = 3;
        return sumar(Value, TheValue);//3LL);
      }
      TLogError("Invisble monkey");
      return TheValue;
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
        //    ReturnValue += IntToString<int512>(Item.Level) + ", ";
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

  // 14399759997 == TooHigh

  for (auto Iterator = 0; 20 > Iterator; ++Iterator)
  {
    for (auto& Monkey : Monkeys)
    {
      Monkey.OperateItems();
    }
  }
  TCVector<wxInt64> Cycles;
  for (auto& Monkey : Monkeys)
  {
    Cycles.push_back(RecordProperty("Cycles: ", Monkey.InspectionCycles));
  }
  Cycles.Sort();
  auto Value = Cycles.at(Cycles.size() - 1) * Cycles.at(Cycles.size() - 2);
  EXPECT_EQ(54752, RecordProperty("CycleValue: ", Value));
}
