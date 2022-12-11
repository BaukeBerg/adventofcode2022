#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

#include <set>

#include "LimitFunctions.h"

#include "ToStringConversion.h"
#include <numeric>
#include <StringHandler.h>

// C++ program to find LCM of n elements
//#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;

// Utility function to find
// GCD of 'a' and 'b'
int gcd(int a, int b)
{
  if (b == 0)
    return a;
  return gcd(b, a % b);
}

// Returns LCM of array elements
ll findlcm(int arr[], int n)
{
  // Initialize result
  ll ans = arr[0];

  // ans contains LCM of arr[0], ..arr[i]
  // after i'th iteration,
  for (int i = 1; i < n; i++)
    ans = (((arr[i] * ans)) /
      (gcd(arr[i], ans)));

  return ans;
}

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
    TMonkey(TCVector<TCVector<TItemInfo>>* Items, wxInt64* LCD, bool WithDivision = false)
    {

      this->Index = Items->size();
      this->Items = Items;
      this->LCD = LCD;
      this->WithDivision = WithDivision;
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
      Value = DoOperation(Value);
      Value = Value % (*LCD);//if (Index != (Real ? 6 : 2))
      if (WithDivision)
      {
        Value = Value / 3;
      }
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
    wxInt64* LCD = nullptr;
    bool WithDivision = false;
  };

  TCVector<TMonkey> Monkeys;
  TCVector<TCVector<TItemInfo>> ItemList;

  TCVector<TMonkey> Monkeys20;
  TCVector<TCVector<TItemInfo>> ItemList20;

  auto Input = ReadFileLines(RealInput());
  auto TheMagicNumber = 0LL;
  for (auto& Line : Input)
  {
    if (Line.StartsWith("Monkey"))
    {
      Monkeys.push_back(TMonkey(&ItemList, &TheMagicNumber));
      Monkeys20.push_back(TMonkey(&ItemList20, &TheMagicNumber, true));
    }
    else
    {
      Monkeys.back().Parse(Line);
      Monkeys20.back().Parse(Line);
    }
  }

  TCVector<wxInt32> Divisors;
  for (auto& Monkey : Monkeys)
  {
    RecordProperty(Monkey.AsString());
    Divisors.Add(Monkey.Divider);
  }
  TheMagicNumber = findlcm(&Divisors.at(0), Divisors.size());

  auto Store = [&](const TCVector<TMonkey>& List) {
    TCVector<wxInt64> Cycles;
    for (auto& Monkey : List)
    {
      Cycles.push_back(RecordProperty("Cycles: ", Monkey.InspectionCycles));
    }
    Cycles.Sort();
    return Cycles.at(Cycles.size() - 1) * Cycles.at(Cycles.size() - 2);
  };

  auto ValueAt20 = 0LL;
  for (auto Iterator = 0; 10000 > Iterator; ++Iterator)
  {
    if (20 == Iterator)
    {
      ValueAt20 = Store(Monkeys20);
    }
    for (auto Iterator = 0; Monkeys.size() > Iterator; ++Iterator)
    {
      Monkeys[Iterator].OperateItems();
      Monkeys20[Iterator].OperateItems();
    }
  }
  EXPECT_EQ(54752, RecordProperty("After 20 Cycles: ", ValueAt20));
  EXPECT_EQ(13606755504, RecordProperty("After 1000 Cycles: ", Store(Monkeys)));
}
