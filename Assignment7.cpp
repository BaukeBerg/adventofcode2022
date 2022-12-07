#include "AdventClass.h"

#include "FileUtility.h"
#include "LimitFunctions.h"

#include <set>

#include "LimitFunctions.h"

class TestAssignment7 : public TAdventClass
{

};

TEST_F(TestAssignment7, TestDaySeven)
{

  struct TFile {
    wxString Name;
    wxInt64 Size;
  };

  struct TDirectory {
  public:
    TDirectory(const wxString& Name) {
      this->Name = Name;
    }
    wxString Name;
    TCVector<TFile> Files;
    TCVector<TDirectory*> Directories;
    void AddFile(const wxInt64 Size, const wxString& Name)
    {
      Files.push_back({ Name, Size });
    }
    void DirSize(TCVector<TFile>* DirSizes) const
    {
      wxInt64 Total = 0;
      for (auto& File : Files)
      {
        Total += File.Size;
      }
      for (auto& Dir : Directories)
      {
        Dir->DirSize(DirSizes);
        Total += DirSizes->back().Size;
      }
      DirSizes->Add({ Name, Total });
    }
  };

  TCVector<TDirectory*> FileSystem;

  auto Input = ReadFileLines(RealInput());
  TCVector<TDirectory*> Stack;
  auto FileMode = false;

  for (auto& Line : Input)
  {
    if (Line.StartsWith("$ cd /"))
    {
      Stack.push_back(new TDirectory("root"));
    }
    else if (Line.StartsWith("$ cd .."))
    {
      Stack.pop_back();
    }
    else if (Line.StartsWith("$ cd"))
    {
      Stack.push_back(Stack.back()->Directories.Add(new TDirectory(Line.substr(5))));
    }
    else if (Line.StartsWith("$ ls"))
    {
      FileMode = true;
    }
    else if (Line.StartsWith("dir"))
    {
      continue;
    }
    else
    {
      auto Info = Split(Line, " ");
      Stack.back()->AddFile(StringToInt<wxInt32>(Info[0]), Info[1]);
    }
  }

  wxInt64 Sum = 0;
  TCVector<TFile> Items;
  Stack.front()->DirSize(&Items);

  for (auto& Item : Items)
  {

    RecordProperty(wxString("Dir ") + Item.Name + ", Size: " + Item.Size);
    if (Item.Size <= 100000) {
      Sum += Item.Size;
    }
  }
  RecordProperty("Sum of dirs: ", Sum);

  auto TotalSpace = 70000000;
  auto MaxOccupied = 30000000;
  auto TotalSize = 44804833;

  auto CurrentFree = TotalSpace - TotalSize;
  auto RequiredSize = MaxOccupied - CurrentFree;
  auto TheFolder = wxINT64_MAX;
  for (auto& Item : Items)
  {
    if (Item.Size >= RequiredSize)
    {
      TheFolder = Minimum(TheFolder, Item.Size);
    }
  }
  RecordProperty("Folder dize:", TheFolder);
}
