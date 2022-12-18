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

    wxInt32 Max(void) const
    {
      return std::max({ x,y,z });
    }
  };

  class TCube : public TCVector<TCVector<TCVector<wxInt32>>>
  {
  public:
    void Set(TBlock Info, wxInt32 Value)
    {
      at(Info.x)[Info.y][Info.z] = Value;
    }

    wxInt32 At(TBlock Info)
    {
      return at(Info.x)[Info.y][Info.z];
    }

    bool Water(TBlock Point)
    {
      return 2 == At(Point);
    }

    bool Stone(TBlock Point)
    {
      return 1 == At(Point);
    }

    bool Valid(TBlock Point) const
    {
      wxInt32 Max = size() - 1;
      return InLimits(0, Point.x, Max)
        && InLimits(0, Point.y, Max)
        && InLimits(0, Point.z, Max);

    }


  };


};

TEST_F(TestAssignment18, TestDayOne)
{
  TCVector<TBlock> Blocks;
  auto MaxSize = 0;
  for (auto& Line : ReadFileLines(RealInput()))
  {
    Blocks.push_back(Line);
    MaxSize = Maximum(MaxSize, Blocks.back().Max());
  }

  auto OtherBlocks = [&](TBlock ThisBlock)
  {
    return TCVector<TBlock>({
      TBlock({ ThisBlock.x - 1, ThisBlock.y, ThisBlock.z }),
      TBlock({ ThisBlock.x + 1, ThisBlock.y, ThisBlock.z }),
      TBlock({ ThisBlock.x, ThisBlock.y - 1, ThisBlock.z }),
      TBlock({ ThisBlock.x, ThisBlock.y + 1, ThisBlock.z }),
      TBlock({ ThisBlock.x, ThisBlock.y, ThisBlock.z - 1 }),
      TBlock({ ThisBlock.x, ThisBlock.y, ThisBlock.z + 1 })
      });
  };

  RecordProperty("Greatest dimension: ", MaxSize);

  for (auto Iterator = 0; Iterator < Blocks.size(); ++Iterator)
  {
    auto ThisBlock = &Blocks.at(Iterator);


    for (auto& Block : OtherBlocks(*ThisBlock))
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
  auto TotalSides = [&]() {
    auto Total = 0;
    for (auto& Block : Blocks)
    {
      Total += Block.VisibleSides;
    }
    return Total;
  };

  TCube Cube;
  ++MaxSize;
  Cube.resize(MaxSize, TCVector<TCVector<wxInt32>>(MaxSize, TCVector<wxInt32>(MaxSize, 0)));
  for (auto& Block : Blocks)
  {
    Cube.Set(Block, 1);
  }

  auto Total = TotalSides();
  EXPECT_EQ(4310, RecordProperty("Open sides: ", Total));

  TCVector<TBlock> Positions = { {0,0,0} };
  TCVector<TBlock> NextSteps;
  while (Positions.size() > 0)
  {
    NextSteps.clear();
    for (auto& Item : Positions)
    {
      if (Cube.Valid(Item) && 0 == Cube.At(Item))
      {
        Cube.Set(Item, 2);
        NextSteps += OtherBlocks(Item);
      }
    }
    Positions = NextSteps;
  }

  wxString TotalCube = "";
  auto TotalPoints = 0;
  for (auto x = 0; x < MaxSize; ++x)
  {
    for (auto y = 0; y < MaxSize; ++y)
    {
      for (auto z = 0; z < MaxSize; ++z)
      {
        TBlock This({ x,y,z });
        for (auto& Item : OtherBlocks(This))
        {
          if (Cube.Stone(This) && (!Cube.Valid(Item) || (Cube.Water(Item))))
          {
            ++TotalPoints;
          }
        }
        TotalCube += Cube.Stone(This) ? "#" : Cube.Water(This) ? "0" : "_";
      }
      TotalCube += "\n";
    }
    TotalCube += wxString::Format("\n\n Row %i:\n\n", x);
  }

  RecordProperty(TotalCube);

  EXPECT_EQ(2466, RecordProperty("Without airpockets: ", (TotalPoints)));
}