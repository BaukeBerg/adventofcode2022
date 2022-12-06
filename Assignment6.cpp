#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

class TestAssignment6 : public TAdventClass
{

};

TEST_F(TestAssignment6, TestDaySix)
{
  wxString Processed = "";
  auto LookingForFirst = true;
  auto IsUnique = [](const wxString& Item) {
    for (auto Iterator = 0U; Iterator < Item.Len(); ++Iterator)
    {
      if (wxNOT_FOUND  != Item.find(Item.at(Iterator), Iterator + 1))
      {
        return false;
      }
    }
    return true;
  };
    
  for (auto& Content : ReadFile(RealInput()))
  {
    Processed += Content;
    if (Processed.Len() < 14)
    {
      continue;
    }
    auto LastFour = Processed.substr(Processed.Len() - 4);
    auto LastFourTeen = Processed.substr(Processed.Len() - 14);
    if (IsUnique(LastFour) && LookingForFirst)
    {
      EXPECT_EQ(1929, RecordProperty("Chars: ", Processed.Len()));
      RecordProperty("Marker 4: ", LastFour);
      LookingForFirst = false;
    }
    if (IsUnique(LastFourTeen))
    {
      EXPECT_EQ(3298, RecordProperty("Chars: ", Processed.Len()));
      RecordProperty("Marker 14:", LastFourTeen);
      return;
    }
  }
}
