#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"
#include "StringFunctions.h"
#include "StringHandler.h"

class TestAssignment16 : public TAdventClass
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

TEST_F(TestAssignment16, DISABLED_RunDay)
{
  for (auto& Input : ReadFileLines(RealInput()))
  {
    auto In = Split(Input, ";");
    In[0].Replace("Valve ", "");
    Routes.push_back(new TValve());
    auto pValve = Routes.back();
    pValve->Name = In[0].Mid(0, 2);
    pValve->FlowRate = StringToInt<wxInt32>(In[0]);
    for (auto& Item : Split(In[1], ","))
    {
      pValve->Neighbours.push_back(Item.Mid(Item.Len() - 2));
    }
  }

  auto Valve = [&](const wxString& Name) {
    for (auto& Item : Routes) {
      if (Item->Name == Name) {
        return Item;
      }
    }
    return *Routes.end();
  };

  for (auto& Item : Routes) {
    for (auto& Name : Item->Neighbours) {
      Item->pFriends.push_back(Valve(Name));
    }
  }

  TCVector<TCVector<TValve*>> Trails;


  auto Inspecting = true;
  auto CurrentValve = Valve("AA");

  TCVector<TValve*> Candidates;
  auto TotalRelief = 0LL;
  auto CurrentRelief = 0LL;
  TCVector<wxString> Route;

  struct TRouteCost {
    wxInt32 FlowRate = 0;
    wxInt32 Remaining = 30; 
    wxInt32 TotalFlow = 0;
    TValve* Current;
    TCVector<TValve*> Visited;
    
  };

  auto ComposeTrails = [&]() {

    TRouteCost Init = { 0, 30, 0, CurrentValve, {} };
    TCVector<TRouteCost> AllTrails;
    TCVector<TRouteCost> TotalTrails;
    TCVector<TRouteCost> NextTrails;

    AllTrails.push_back(Init);
    bool Changing = true;
    while (Changing)
    {
      Changing = false;
      for (auto& Trail : AllTrails)
      {
        if (Trail.Remaining == 0)
        {
          TotalTrails.push_back(Trail);
        }
        else
        {
          for (auto& Klep : Trail.Current->pFriends)
          {
            if (Klep->FlowRate > 0 && !Trail.Visited.Contains(Klep))
            {
              NextTrails.push_back(Trail);
              auto TheTrail = NextTrails.end()-1;
              TheTrail->Current = Klep;
              TheTrail->Visited.push_back(Klep);
              TheTrail->TotalFlow += TheTrail->FlowRate;
              TheTrail->FlowRate += Klep->FlowRate;
              TheTrail->Remaining -= 2;
            }
            else if (!Trail.Visited.Contains(Klep))
            {
              NextTrails.push_back(Trail);
              auto TheTrail = NextTrails.end() - 1;
              TheTrail->Current = Klep;
              TheTrail->Remaining--;
              TheTrail->TotalFlow += TheTrail->FlowRate;
              TheTrail->Visited.push_back(Klep);
              Changing = true;
            }
            else
            {
              ;
            }
          }
        }
      }
      AllTrails = NextTrails;
    }
    return TotalTrails;
  };

  wxInt32 MaxFlow = 0;

  for (auto& Trail : ComposeTrails())
  {
    RecordProperty("FLow rate: ", Trail.TotalFlow);
    MaxFlow = Maximum(Trail.TotalFlow, MaxFlow);
  }
  RecordProperty("Max flow rate: ", MaxFlow);

  for (auto Iterator = 0; Iterator < 30; ++Iterator)
  {
    TotalRelief += CurrentRelief;
    CurrentRelief = 0;

    Route.push_back(CurrentValve->Name);
    if (Inspecting)
    {
      Candidates = CurrentValve->pFriends;
      if (CurrentValve->IsOpen || CurrentValve->FlowRate == 0)
      {
        for (auto& Item : Candidates)
        {
          if (!Item->IsOpen)
          {
            CurrentValve = Item;
            Inspecting = false;
            continue;
          }
        }
      }
    }
    else
    {
      CurrentValve->IsOpen = true;
      CurrentRelief += CurrentValve->FlowRate;
      Candidates = CurrentValve->pFriends;
      Inspecting = true;
    }
  }

  RecordProperty("Followed route: ", Merge(Route, ", "));

  RecordProperty("Total relief: ", TotalRelief);

  auto Me = Valve("AA");
  auto Elephant = Valve("AA");
}

TEST_F(TestAssignment16, TestDay2)
{

  for (auto& Input : ReadFileLines(RealInput()))
  {
    auto In = Split(Input, ";");
    In[0].Replace("Valve ", "");
    Routes.push_back(new TValve());
    auto pValve = Routes.back();
    pValve->Name = In[0].Mid(0, 2);
    pValve->FlowRate = StringToInt<wxInt32>(In[0]);
    for (auto& Item : Split(In[1], ","))
    {
      pValve->Neighbours.push_back(Item.Mid(Item.Len() - 2));
    }
  }
  

  /*


        _seen = {}
        m = 0
        def f(t, pos, flow) :
        global m, Vs, Os, _seen

        if _seen.get((t, pos), -1) >= sum(flow) :
          return
          _seen[t, pos] = sum(flow)

#
          if t == 30 :
            m = max(m, sum(flow))
            print(m)
            return

            # Open valve here ?
            for k in(0, 1) :
              if k == 0 :
                if Valve-> Os[pos] or Fs[pos] <= 0 :
                  continue

                  Os[pos] = True
                  j = sum(Fs[k] for k, v in Os.items() if v)
                  f(
                    t + 1,
                    pos,
                    flow + [j]
                  )
                  Os[pos] = False
                  else:
  j = sum(Fs[k] for k, v in Os.items() if v)
    for v in Vs[pos] :
      f(
        t + 1,
        v if v is not None else pos,
        flow + [j]
      )

      f(1, "AA", [0])*/
}