#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"


class TestAssignment18 : public TAdventClass
{
public:
  class TBlock
  {
  public:
    TBlock(std::initializer_list<wxInt32> Data)
    {
      x = *Data.begin();
      y = *(Data.begin() + 1);
      z = *(Data.begin() + 2);
    }

    TBlock(const wxString& Input)
    {
      auto Numbers = Split(Input, ',');
      StringToInt(Numbers[0], &x, 0);
      StringToInt(Numbers[1], &y, 0);
      StringToInt(Numbers[2], &z, 0);
    }

    wxInt32 x, y, z;
    wxInt32 VisibleSides = 6;

    bool operator==(const TBlock& Other) const
    {
      return x == Other.x && y == Other.y && z == Other.z;
    }
  };


};

TEST_F(TestAssignment18, TestDayOne)
{
  TCVector<TBlock> Blocks;
  for (auto& Line : ReadFileLines(RealInput()))
  {
    Blocks.push_back(Line);
  }

  for (auto Iterator = 0; Iterator < Blocks.size(); ++Iterator)
  {
    auto ThisBlock = &Blocks.at(Iterator);
    TCVector<TBlock> OtherBlocks({
      TBlock({ThisBlock->x - 1, ThisBlock->y, ThisBlock->z}),
      TBlock({ThisBlock->x + 1, ThisBlock->y, ThisBlock->z}),
      TBlock({ThisBlock->x, ThisBlock->y - 1, ThisBlock->z}),
      TBlock({ThisBlock->x, ThisBlock->y + 1, ThisBlock->z}),
      TBlock({ThisBlock->x, ThisBlock->y, ThisBlock->z - 1}),
      TBlock({ThisBlock->x, ThisBlock->y, ThisBlock->z + 1})
      });

    for (auto& Block : OtherBlocks)
    {
      for (auto Index = 0; Index < Blocks.size(); ++Index)
      {        
        if (Blocks.at(Index) == Block)
        {
          ThisBlock->VisibleSides--;
        }
      }
    }
  }
  auto TotalSides = 0;
  for (auto& Block : Blocks)
  {
    TotalSides += Block.VisibleSides;
  }
  EXPECT_EQ(64, RecordProperty("Open sides: ", TotalSides));


}