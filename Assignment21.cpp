#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"


class TestAssignment21 : public TAdventClass
{
public:
  struct TCalling
  {
    wxString Monkey;
    wxString Call;
  };
  TCVector<TCalling> Callings;
  TCVector<TCalling> Monkeys;

  wxString Find(const wxString& Phrase) {
    for (auto& Call : Callings) {
      if (Call.Monkey == Phrase) {
        return Call.Call;
      }
    }
    return "";
  }

  bool Resolve(wxString Call, wxInt32* Value)
  {
    if (Call.Len() < 4) {
      return StringToInt(Call, Value);
    }
    auto Calls = Split(Call, "+-*/");
    auto CallOne = Find(Calls[0]);
    auto CallTwo = Find(Calls[1]);
    if ((CallOne.length() > 0)
      && CallTwo.length() > 0))
      {


      }

  }

};

TEST_F(TestAssignment21, TestDayOne)
{
  for(auto &Line : ReadFileLines(RealInput()))
  {
    auto Info = Split(Line, ":");
    Monkeys.push_back({ Info[0], Info[1] });
  }




}