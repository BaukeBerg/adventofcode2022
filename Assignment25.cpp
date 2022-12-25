#include "AdventClass.h"

#include "FileUtility.h"

struct TSnafuset
{
  wxInt32 Dec;
  wxString SnafuNote;
};

using TestSet = TCVector<TSnafuset>;

class TestAdvent25 : public TAdventClass
{
public:
  wxString ReverseSnafu(wxInt64 Number)
  {
    auto Index = 0;
    while (pow(5, Index) < Number)
    {
      ++Index;
    }

    auto Value = Number;
    auto Current = 0LL;
    Current = pow(5, Index);
    auto Next = 0LL;
    wxString Total = "";
    wxInt32 Digit = 0;
    TCVector<wxInt32> Digits;
    while (Index >= 0)
    {
      while (Value >= Current)
      {
        ++Digit;
        Value -= Current;
      }
      Digits.AddToFront(Digit);
      Digit = 0;
      Current = pow(5, --Index);
    }
    Digits.push_back(0);
    for (auto Iterator = 0; Iterator < Digits.size()-1; ++Iterator)
    {
      if (Digits[Iterator] == 3)
      {
        Digits[Iterator] = -2;
        Digits[Iterator + 1]++;
      }
      if (Digits[Iterator] == 4)
      {
        Digits[Iterator] = -1;
        Digits[Iterator + 1]++;
      }
      if (Digits[Iterator] == 5)
      {
        Digits[Iterator] = 0;
        Digits[Iterator + 1]++;
      }
    }
    for (auto& Item : Digits)
    {
      switch (Item)
      {
      case -1:
      {
        Total = "-" + Total;
        break;
      }
      case -2:
      {
        Total = "=" + Total;
        break;
      }
      default:
      {
        Total = IntToString(Item) + Total;
      }
      }
    }
    
    while (Total.StartsWith("0"))
    {
      Total = Total.Mid(1);
    }
    return Total;
  }
  wxInt64 Snafu(const wxString& Line)
  {
    auto Index = 0;
    auto TotalLength = Line.Len();
    wxInt64 TotalValue = 0;
    wxInt64 MinusAccu = 0;
    while (Index < Line.Len())
    {
      auto Char = Line.at(TotalLength - Index - 1);
      switch (Char.GetValue())
      {
      case '-':
        MinusAccu += pow(5, Index);
        break;
      case '=':
        MinusAccu += 2 * pow(5, Index);
        break;
      default:
        TotalValue += (Char.GetValue() - 0x30) * pow(5, Index) - MinusAccu;
        MinusAccu = 0;
      }
      ++Index;
    }
    return TotalValue;
  }



};

TEST_F(TestAdvent25, SnafuSet)
{

  TCVector<TSnafuset> Items = {
    {1,"1"},
    {2,"2"},
    {3,"1="},
    {4,"1-"},
    {5,"10"},
    {6,"11"},
    {7,"12"},
    {8,"2="},
    {9,"2-"},
    {10,"20"},
    {15,"1=0"},
    {20,"1-0"},
    {2022,"1=11-2"},
    {12345,"1-0---0"},
    {314159265,"1121-1110-1=0"}
  };

  for (auto& Item : Items)
  {
    EXPECT_EQ(Item.Dec, RecordProperty("Snafu: ", Snafu(Item.SnafuNote)));
    EXPECT_STR_EQ(Item.SnafuNote, RecordProperty("Reverse Snafu: ", ReverseSnafu(RecordProperty("Dec:", Item.Dec)))) << "Failed for: " << Item.Dec;
  }
}

TEST_F(TestAdvent25, Single)
{
  TestSet Set =
  {
    {8, "2="},
    {5, "10"},
    {2022,"1=11-2"},
    {12345,"1-0---0"},
    {314159265,"1121-1110-1=0"},
    {15, "1=0"},
    {20, "1-0"},
    {6, "11"},
    {8, "2="},


  };

  for (auto& Item : Set)
  {
    EXPECT_STR_EQ(Item.SnafuNote, RecordProperty("Reverse Snafu: ", ReverseSnafu(RecordProperty("Dec:", Item.Dec)))) << "Failed for: " << Item.Dec;
  }

}

TEST_F(TestAdvent25, RunMini)
{

  auto Items = ReadFileLines(MiniInput());
  TCVector<wxInt32> Expected = {
    1747,
    906,
    198,
    11,
    201,
    31,
    1257,
    32,
    353,
    107,
    7,
    3,
    37
  };

  auto Index = 0;
  for (auto& Item : Items)
  {
    EXPECT_EQ(Expected[Index++], Snafu(Item));
  }

}

TEST_F(TestAdvent25, RunDay)
{
  auto Items = ReadFileLines(RealInput());
  wxInt64 Total = 0;
  for (auto& Item : Items)
  {
    Total += Snafu(Item);
  }



  RecordProperty("Sum: ", Total);
  RecordProperty("SNAFU:", ReverseSnafu(29694520452605));

}