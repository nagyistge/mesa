//
//        Copyright (C) 1999, 2000, HIMSS, RSNA and Washington University
//
//        The MESA test tools software and supporting documentation were
//        developed for the Integrating the Healthcare Enterprise (IHE)
//        initiative Year 1 (1999-2000), under the sponsorship of the
//        Healthcare Information and Management Systems Society (HIMSS)
//        and the Radiological Society of North America (RSNA) by:
//                Electronic Radiology Laboratory
//                Mallinckrodt Institute of Radiology
//                Washington University School of Medicine
//                510 S. Kingshighway Blvd.
//                St. Louis, MO 63110
//        
//        THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER HIMSS, RSNA NOR
//        WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
//        PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
//        USE, FREEDOM FROM ANY DEFECTS OR COMPUTER DISEASES OR ITS CONFORMITY 
//        TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
//        THE SOFTWARE IS WITH THE USER.
//
//        Copyright of the software and supporting documentation is
//        jointly owned by HIMSS, RSNA and Washington University, and free
//        access is hereby granted as a license to use this software, copy
//        this software and prepare derivative works based upon this software.
//        However, any distribution of this software source code or supporting
//        documentation or derivative works (source code and supporting
//        documentation) must include the three paragraphs of this copyright
//        notice.

#include "MESA.hpp"
#include <fstream.h>
#include "MAMessenger.hpp"
#include "MHL7Msg.hpp"
#include "MHL7Factory.hpp"

extern int gMsgNumber;

MAMessenger::MAMessenger(const MAMessenger& cpy) :
  MLMessenger(cpy.mFactory, cpy.mDatabase),
  mLogDir(cpy.mLogDir)
{
}

MAMessenger::~MAMessenger ()
{
}

void
MAMessenger::printOn(ostream& s) const
{
  s << "MAMessenger" << endl;
}

void
MAMessenger::streamIn(istream& s)
{
  //s >> this->member;
}

// Non-boiler plate methods follow below

#if 0
MAMessenger::MAMessenger(MHL7Factory& factory, MDBOrderPlacer& database) :
  MHL7Messenger(factory),
  mDatabase(database)
{
}
#endif

MAMessenger::MAMessenger(MHL7Factory& factory, MDBOrderPlacer& database, MString LogDir) :
  MLMessenger(factory, database),
  mLogDir(LogDir)
{
}

MAMessenger*
MAMessenger::clone()
{
  MAMessenger* clone;
  
  clone = new MAMessenger(mFactory, mDatabase, mLogDir);
  return clone;
}

int
MAMessenger::acceptADT(MHL7Msg& message, const MString& event)
{
}

int
MAMessenger::acceptORM(MHL7Msg& message, const MString& event)
{
}

int
MLMessenger::acceptACK(MHL7Msg& message, const MString& event)
{
  cout << "(op_send) recieved ACK: " << message << endl;

  // Close the Socket and unregister the handler from the reactor. And tell main
 we
  // are finished listening to messages.
  // The messenger.close() call does not destroy the messenger/handler pair:
  // they may be used again for another connection.
  this->close();

  return 0;
}

