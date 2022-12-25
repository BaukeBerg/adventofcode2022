//#include "AdventClass.h"
//
//#include "FileUtility.h"
//#include "LimitFunctions.h"
//#include "StringFunctions.h"
//#include "StringHandler.h"
//
//
//class TestAssignment17 : public TAdventClass
//{
//public:
//
//  class TRock : public TCVector<TCVector<wxInt32>>
//  {
//  public:
//    TRock(std::initializer_list<std::initializer_list<wxInt32>> List) {
//      for (auto& Item : List) {
//        push_back(Item);
//      }
//    }
//    wxInt32 Width(void) const {
//      return front().size();
//    }
//    wxInt32 Height(void) const {
//      return size();
//    }
//
//    TCVector<TCVector<wxInt32>> AsGrid(wxInt32 Offset) const
//    {
//      TCVector<TCVector<wxInt32>> Grid;
//      for (auto Iterator = 0; Iterator < Height(); ++Iterator)
//      {
//        Grid.push_back(Compose(Offset, at(Iterator)));
//      }
//      return Grid;
//    }
//  };
//
//  class TTower : public TCVector<TCVector<wxInt32>>
//  {
//  public:
//    TTower(wxInt32 InitialSize, TCVector<wxInt32> Fill)
//    {
//      for (; 0 < InitialSize; --InitialSize) {
//        push_back(Fill);
//      }
//    }
//
//    void Add(wxPoint Location, TRock* Rock)
//    {
//      for (auto Iterator = 0; Iterator < Rock->Height(); ++Iterator)
//      {
//        auto RockLayer = Compose(Location.x, Rock->at(Iterator));
//        if (size() <= Iterator + Location.y)
//        {
//          push_back(RockLayer);
//        }
//        else
//        {
//          for (auto Char = 0; 7 > Char; ++Char)
//          {
//            at(Location.y + Iterator).at(Char) = Maximum(at(Location.y + Iterator).at(Char), RockLayer.at(Iterator));
//          }
//        }
//      }
//    }
//
//    wxString Print(void) const
//    {
//      wxString Line;
//      for (wxInt32 Iterator = size() - 1; Iterator >= 0; --Iterator)
//      {
//        for (auto& Item : at(Iterator))
//        {
//          Line += Item == 1 ? '#' : '_';
//        }
//        Line += "\n";
//      }
//      return(Line);
//    }
//
//    bool Collides(wxPoint CurrentLocation, TRock* Rock)
//    {
//      if (CurrentLocation.y > size())
//      {
//        return false;
//      }
//      auto TowerTop = Top(CurrentLocation.y, Rock->Height());
//      auto RockInfo = Rock->AsGrid(CurrentLocation.x);
//
//
//      for (auto Iterator = 0; TowerTop.size() > Iterator; ++Iterator)
//      {
//        for (auto i = 0; i < 7; ++i)
//        {
//          if (TowerTop[Iterator][i] + RockInfo[Iterator][i] == 2)
//          {
//            return true;
//          }
//        }
//      }
//      return false;
//    }
//
//    TCVector<TCVector<wxInt32>> Top(wxInt32 Offset, wxInt32 Size)
//    {
//      TCVector<TCVector<wxInt32>> Rows(Size, TCVector<wxInt32>(7, 0));;
//      for (auto Iterator = 0; Iterator < Size; ++Iterator)
//      {
//        if (Iterator + Offset >= size())
//        {
//          break;
//        }
//        Rows.at(Iterator) = at(Iterator + Offset);
//      }
//      return Rows;
//    }
//  };
//
//  void PrintRock(const TCVector<TCVector<wxInt32>>& Rock)
//  {
//    for (auto& Item : Rock)
//    {
//      wxString Line;
//      for (wxInt32 Iterator = Rock.size() - 1; Iterator >= 0; --Iterator)
//      {
//        for (auto& Item : Rock.at(Iterator))
//        {
//          Line += Item == 1 ? '#' : '_';
//        }
//        RecordProperty(Line);
//      }
//    }
//  }
//
//  static
//  TCVector<wxInt32> Compose(wxInt32 Offset, TCVector<wxInt32> RockLayer)
//  {
//    TCVector<wxInt32> FrontPadding(Offset, 0);
//    FrontPadding += RockLayer;
//    FrontPadding.resize(7, 0);
//    return FrontPadding;
//  };
//
//  TCVector<TRock> Rocks =
//  {
//   {{1,1,1, 1}},
//   {{0,1,0},{1,1,1},{0,1,0}},
//   {{1,1,1},{0,0,1}, {0,0,1}},
//   {{1},{1},{1},{1}},
//   {{1,1},{1,1}}
//  };
//
//  void TryMove(wxInt32 CharValue, TRock *CurrentRock, wxPoint &RockPos)
//  {
//    auto Grid = Tower.Top(RockPos.y, CurrentRock->Height());
//    auto Rock = CurrentRock->AsGrid(RockPos.x);
//    switch (CharValue)
//    {
//    case '<':
//    {
//      for(auto i = 0 ; i < CurrentRock->Height() ; i++)
//      if()
//      break;
//    }
//    case '>':
//    {
//      
//      break;
//    }
//    }
//    
//    RockPos += wxPoint(CharValue == '<' ? -1 : 1, 0);
//    RockPos.x = Limit<wxInt32>(0, RockPos.x, 7 - CurrentRock->Width());
//
//  }
//
//  TTower Tower;
//
//};
//
//TEST_F(TestAssignment17, TestDayOne)
//{
//  Tower = {1, { 1,1,1,1,1,1,1 }};
//  auto CurrentRock = Rocks.begin();
//  auto Operations = ReadFile(MiniInput());
//  wxInt32 RockIndex = 0;
//  auto RockBottom = false;
//  auto RockPos = wxPoint(2, Tower.size() + 3);
//  auto OpIndex = 0;
//  auto TotalRocks = 10;
//  for (; TotalRocks > 0; )
//  {
//    if (RockBottom)
//    {
//      --TotalRocks;
//      RockIndex++;
//      if (RockIndex > Rocks.size() - 1)
//      {
//        RockIndex = 0;
//      }
//      RockBottom = false;
//      CurrentRock = &Rocks.at(RockIndex);
//      RockPos = wxPoint(2, Tower.size() + 3);
//    }
//    auto CharValue = Operations[OpIndex].GetValue();
//    TryMove(CharValue, &RockPos);
//    ++OpIndex;
//    if (OpIndex >= Operations.Len())
//    {
//      OpIndex = 0;
//    }
//
//    RockPos += wxPoint(0, -1);
//    if (Tower.Collides((RockPos), CurrentRock))
//    {
//      Tower.Add(RockPos + wxPoint(0, 1), CurrentRock);
//      RecordProperty("\n\nNew Rock:\n\n", Tower.Print());
//      RockBottom = true;
//    }
//
//  }
//}
////
////TEST_F(TestAssignment17, TestSample)
////{
////  auto MiniInput = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";
////  EXPECT_EQ(3068, ThrowStones(10, MiniInput));
////}
////
////TEST_F(TestAssignment17, RunDay)
////{
////  auto Operations = RealInput();
////  EXPECT_EQ(3175LL, RecordProperty("Tower size: ", ThrowStones(2022LL, Operations)));
////  //RecordProperty("Tower size: ", ThrowStones(1000000000000LL, Operations));
////}
////
