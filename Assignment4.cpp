#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

class TestAssignment4 : public TAdventClass
{

};

TEST_F(TestAssignment4, TestDayFour)
{
  auto AllInfo = ReadFileLines(RealInput());

  auto Range = [](const wxString& Line) {
    auto Tokens = Split(Line, "-");
    return TCVector<wxInt32>({ StringToInt<wxInt32>(Tokens[0]), StringToInt<wxInt32>(Tokens[1]) });
  };

  auto FullyContained = 0;
  auto Overlap = 0;
  for (auto& Line : AllInfo)
  {
    auto Sections = Split(Line, ",");
    auto First = Range(Sections[0]);
    auto Second = Range(Sections[1]);

    if ((First[0] <= Second[0] && First[1] >= Second[1])
      || (Second[0] <= First[0] && Second[1] >= First[1]))
    {
      ++FullyContained;
    }
    
    if (InLimits(Second[0], First[1], Second[1])
      || InLimits(Second[0], First[0], Second[1])
      || InLimits(First[0], Second[0], First[1])
      || InLimits(First[0], Second[1], First[1]))      
    {
      ++Overlap;
    }
  }
  EXPECT_EQ(466, RecordProperty("Fully contained: ", FullyContained));
  EXPECT_EQ(865, RecordProperty("Overlap: ", Overlap));
}
