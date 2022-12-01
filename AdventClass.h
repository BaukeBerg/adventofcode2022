#pragma once

#include "GoogleTest.h"

class TAdventClass : public ::testing::TUnitTest
{
public:
  wxString MiniInput(void)
  {
    return RecordProperty("Mini input: ", wxString("MiniInput") + Number());
  }

  wxString MidiInput(void)
  {
    return RecordProperty("Midi input: ", wxString("MidiInput") + Number());
  }

  wxString TestInput(void)
  {
    return RecordProperty("Test input: ", wxString("TestInput") + Number());
  }

  wxString RealInput(void)
  {
    return RecordProperty("Real input: ", wxString("Assignment") + Number());
  }

  wxString Number(void) const
  {
    return wxString("") + StringToInt(TestSuite(), 0) + wxString(".txt");
  }
};
