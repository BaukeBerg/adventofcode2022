#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

class TestAssignment5 : public TAdventClass
{

};

TEST_F(TestAssignment5, TestDayFive)
{
  TCVector<wxString> StackInput(9, "");

  auto SlowStacks = StackInput;
  auto QuickStacks = StackInput;

  bool FillingStack = true;
  for (auto& Line : ReadFileLines(RealInput(), true))
  {
    if (Line.IsEmpty())
    {
      FillingStack = false;
      SlowStacks = QuickStacks = StackInput;
      continue;
    }
    if (FillingStack)
    {
      for (auto Iterator = 1; Iterator < Line.length(); Iterator += 4)
      {
        auto Index = Iterator >> 2;
        if (InLimits<char>('A', Line.at(Iterator).GetValue(), 'Z'))
        {
          StackInput.at(Index) = Line.at(Iterator) + StackInput.at(Index);
        }
      }
    }
    else
    {
      auto FromTo = Split(Line, "f");
      auto SourceDest = Split(FromTo[1], "t");
      auto Amount = StringToInt<wxInt32>(FromTo[0]);
      auto Source = StringToInt<wxInt32>(SourceDest[0]);
      auto Dest = StringToInt<wxInt32>(SourceDest[1]);

      auto Process = [&](TCVector<wxString>* List, bool IsSlow)
      {
        auto Length = IsSlow ? 1 : Amount;
        Amount = IsSlow ? Amount : 1;
        for (auto Moved = 0; Moved < Amount; ++Moved)
        {
          auto SourceString = List->at(Source - 1);
          auto LastChar = SourceString.substr(SourceString.Len() - Length);
          List->at(Dest - 1).append(LastChar);
          SourceString.RemoveLast(Length);
          List->at(Source - 1) = SourceString;
        }
      };
      Process(&SlowStacks, true);
      Process(&QuickStacks, false);
    }

  }
  auto Phrase = [](const TCVector<wxString>& List)
  {
    wxString Phrase;
    for (auto& Stack : List)
    {
      Phrase += Stack.substr(Stack.Len() - 1);
    }
    return Phrase;
  };

  EXPECT_STR_EQ("GFTNRBZPF", RecordProperty("9000 Stacker: ", Phrase(SlowStacks)));
  EXPECT_STR_EQ("VRQWPDSGP", RecordProperty("9001 Stacker: ", Phrase(QuickStacks)));
}

