#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"

class TestAssignment17 : public TAdventClass
{
public:
  class TGrid : public TCVector<TCVector<wxInt32>>
  {
  public:
    wxInt32 Value(wxInt32 x, wxInt32 y) {
      return Value({ y, x });
    }
    wxInt32 Value(wxPoint Pos) const {
      return at(Pos.y).at(Pos.x);
    }
    wxInt32 Columns() const {
      return size();
    }
    wxInt32 Rows() const {
      return front().size();
    }
    void Set(wxPoint Pos, wxInt32 Value) {
      at(Pos.y).at(Pos.x) = Value;
    }
  };

  class TValve
  {
  public:
    wxString Name;
    wxInt32 FlowRate;
    TCVector<wxString> Neighbours;
    TCVector<TValve*> pFriends;
    bool IsOpen = false;
  };

  TCVector<TValve*> Routes;
};

TEST_F(TestAssignment17, RunDay)
{
  auto Operations = ReadFile(RealInput());

  class TRock : public TCVector<TCVector<wxInt32>>
  {
  public:
    TRock(std::initializer_list<std::initializer_list<wxInt32>> List) {
      for (auto& Item : List) {
        push_back(Item);
      }
    }
    wxInt32 Width(void) const {
      return front().size();
    }
    wxInt32 Height(void) const {
      return size();
    }

  };

  TCVector<TRock> Rocks =
  {
   {{1, 1,1, 1}},
   {{0,1,0},{1,1,1},{0,1,0}},
   {{0,0,1},{0,0,1}, {1,1,1}},
   {{1},{1},{1},{1}},
   {{1,1},{1,1}}
  };

  TCVector<TCVector<wxInt32>> Tower = { { 1,1,1,1,1,1,1 } };

  wxInt32 RockIndex = 0;
  auto RockBottom = false;
  auto MaxX = 0;
  auto RockPos = wxPoint(2, Tower.size() + 3);
  for (auto& Operation : Operations) {
    if (RockBottom)
    {
      RockIndex++;
      if (RockIndex > Rocks.size() - 1) {
        RockIndex = 0;
      }
      RockBottom = false;
      RockPos = wxPoint(2, Tower.size() + 3);
    }
    RockPos += wxPoint(Operation.GetValue() == '<' ? -1 : 1, 0);
    RockPos.x = Limit<wxInt32>(0, 7 - Rocks[RockIndex].front().size(), 7);

    if (Tower.size() < RockPos.y) {
      RockPos += wxPoint(0, -1);
    }
    else
    {
      auto TopLayer = Tower.back();

      auto Compose = [&](TCVector<wxInt32> Rock) {
        auto RockLayer = Rock;
        for (auto FrontRocks = 0; RockPos.x > FrontRocks; ++FrontRocks) {
          RockLayer.AddToFront(0);
        }
        RockLayer.resize(7, 0);
        return RockLayer;
      };

      auto BottomRock = Compose(Rocks[RockIndex].back());
      for (auto Iterator = 0; Iterator < 7; ++Iterator)
      {
        if ((TopLayer.at(Iterator) + BottomRock.at(Iterator) == 2))
        {
          RockBottom = true;
          for (wxInt32 Item = Rocks.at(RockIndex).size() - 1; Item >= 0; --Item)
          {
            Tower.push_back(Compose(Rocks.at(RockIndex).at(Item)));              
          }
        }
      }

    }
  }
  EXPECT_EQ(3175, RecordProperty("Tower size: ", Tower.size()));

  }