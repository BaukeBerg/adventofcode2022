#include "AdventClass.h"

#include "FileUtility.h"

class TestAssignment2 : public TAdventClass
{


};

TEST_F(TestAssignment2, TestDayTwo)
{
  auto Win = [](const wxString& Info) {
    if (Info.StartsWith("A"))
    {
      if (Info.EndsWith("X")) { return 3; }
      if (Info.EndsWith("Y")) { return 6; }
    }
    if (Info.StartsWith("B"))
    {
      if (Info.EndsWith("Y")) { return 3; }
      if (Info.EndsWith("Z")) { return 6; }
    }
    if (Info.StartsWith("C"))
    {
      if (Info.EndsWith("Z")) { return 3; }
      if (Info.EndsWith("X")) { return 6; }
    }
    return 0;
  };

  auto Score = [](const wxString& Info) {
    if (Info.EndsWith("X")) { return 1; }
    if (Info.EndsWith("Y")) { return 2; }
    if (Info.EndsWith("Z")) { return 3; }
    return 0;
  };

  auto PredictOutcome = [](const wxString& Input) {
    if (Input.EndsWith("X"))
    {
      if (Input.StartsWith("A")) { return 3; }
      if (Input.StartsWith("B")) { return 1; }
      return 2;
    }
    if (Input.EndsWith("Y"))
    {
      if (Input.StartsWith("A")) { return 4; }
      if (Input.StartsWith("B")) { return 5; }
      return 6;

    }
    if (Input.EndsWith("Z"))
    {
      if (Input.StartsWith("A")) { return 8; }
      if (Input.StartsWith("B")) { return 9; }
      return 7;
    }
    return 0;
  };

  wxInt32 TotalScore = 0;
  wxInt32 PredictedScore = 0;

  for (auto& Input : ReadFileLines(RealInput()))
  {
    auto ThisRound = Win(Input);
    TotalScore += Score(Input);
    TotalScore += Win(Input);
    PredictedScore += PredictOutcome(Input);
  }

  EXPECT_EQ(9241, RecordProperty("Output: ", TotalScore));
  EXPECT_EQ(14610, RecordProperty("Output predicted: ", PredictedScore));
}
