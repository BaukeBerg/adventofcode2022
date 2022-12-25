#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"


class TestAssignment19 : public TAdventClass
{
public:
  class TRobot
  {
  public:
    TRobot(std::initializer_list<wxInt32> Items)
    {
      Cost = Items;
    }

    TCVector<wxInt32> Cost;
    wxInt32 Amount = 0;
  };

  class TBluePrint
  {
  public:
    TBluePrint(const wxString& Line)
    {
      auto Items = Split(Line, ".");
      auto First = Split(Items[0], ":");
      StringToInt(First[0], &ID);
      Robots.push_back(TRobot({ StringToInt<wxInt32>(First[1]) }));
      Robots.push_back(TRobot({ StringToInt<wxInt32>(Items[1]) }));
      auto Tokens = Split(Items[2], " ");
      Robots.push_back(TRobot({ StringToInt<wxInt32>(Tokens[4]), StringToInt<wxInt32>(Tokens[7]) }));

      Tokens = Split(Items[3], " ");
      Robots.push_back(TRobot({ StringToInt<wxInt32>(Tokens[4]), StringToInt<wxInt32>(Tokens[7]) }));
      Robots[0].Amount++;
      MaxClay = Robots[2].Cost[1];
      MaxOre = std::max({ Robots[1].Cost[0], Robots[2].Cost[0], Robots[3].Cost[0] });
    }

    wxString ProcessDay(void)
    {
      TCVector<wxInt32> Delta(4, 0);
      if ((Robots[3].Cost[1] <= Obsidian) &&
        (Robots[3].Cost[0] <= Ore))
      {
        Obsidian -= Robots[3].Cost[1];
        Ore -= Robots[3].Cost[0];
        Delta[3]++;
      }

      else if ((Robots[2].Cost[1] <= Clay) &&
        (Robots[2].Cost[0] <= Ore))
      {
        Clay -= Robots[2].Cost[1];
        Ore -= Robots[2].Cost[0];
        Delta[2]++;
        
      }
      else if ((Robots[1].Cost[0] <= Ore)
        && (Robots[0].Amount >= Robots[1].Cost[0])
        && (Robots[1].Amount <= MaxClay))
      {
        Ore -= Robots[1].Cost[0];
        Delta[1]++;
      }
      else if ((Robots[0].Cost[0] <= Ore)
        && (Robots[0].Amount < MaxOre))
      {
        Ore -= Robots[0].Cost[0];
        Delta[0]++;
        
      }

      Ore += Robots[0].Amount;
      Clay += Robots[1].Amount;
      Obsidian += Robots[2].Amount;
      Geode += Robots[3].Amount;


      for (auto Iterator = 0 ; 4 > Iterator ; ++Iterator)
      {
        Robots[Iterator].Amount += Delta[Iterator];        
      }

      return wxString::Format("Total: Ore %i, Clay %i, Obsidian %i, Geode %i. OreMiner %i, ClayMiner %i, ObsidianMinder %i, GeodeMinder %i"\
        , Ore, Clay, Obsidian, Geode, Robots[0].Amount, Robots[1].Amount, Robots[2].Amount, Robots[3].Amount);
    }

    wxInt32 QualityValue(void) const
    {
      return ID * Geode;
    }

    wxDouble GeodeToOreRatio(void)
    {
      TotalOre = Robots[3].Cost[0] + Robots[3].Cost[1] * Robots[2].Cost[0];
      TotalClay = Robots[3].Cost[1] * Robots[2].Cost[1];
      return wxDouble(TotalClay) / wxDouble(TotalOre);
    }

    wxInt32 ID;
    TCVector<TRobot> Robots;
    wxInt32 Clay = 0;
    wxInt32 Ore = 0;
    wxInt32 Obsidian = 0;
    wxInt32 Geode = 0;
    wxInt32 TotalOre = 0;
    wxInt32 TotalClay = 0;
    wxInt32 MaxOre = 0;
    wxInt32 MaxClay = 0;
  };

  TCVector<TBluePrint> Factory;

};

TEST_F(TestAssignment19, TestDayOne)
{
  for (auto& Line : ReadFileLines(RealInput()))
  {
    Factory.push_back(TBluePrint(Line));
    RecordProperty("Ratio: ", wxString("Clay: ") + Factory.back().TotalClay + ", Ore: " + Factory.back().TotalOre);
    if (Factory.size() == 3)
    {
      break;
    }
  }

  auto TotalQuality = 0;
  TCVector<wxInt32> TotalGeodes;
  for (auto& Item : Factory)
  {
    for (auto Minutes = 0; 32 > Minutes; ++Minutes)
    {
      RecordProperty(wxString::Format("Running day %i. %s", Minutes, Item.ProcessDay()));
    }
    TotalQuality += RecordProperty("Quality of this ID: ", Item.QualityValue());
    TotalGeodes.push_back(RecordProperty("Opened Geodes of this factory: " , Item.Geode));
  }
  RecordProperty("Total Quality: ", TotalQuality);
  RecordProperty("Total geodes: ", TotalGeodes[0] * TotalGeodes[1] * TotalGeodes[2]);
}