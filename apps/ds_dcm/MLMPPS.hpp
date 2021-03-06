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

// $Id: MLMPPS.hpp,v 1.3 2001/11/01 15:27:56 aeh Exp $ $Author: aeh $ $Revision: 1.3 $ $Date: 2001/11/01 15:27:56 $ $State: Exp $
//
// ====================
//  = LIBRARY
//	xxx
//
//  = FILENAME
//	MLMPPS.hpp
//
//  = AUTHOR
//	Author Name
//
//  = COPYRIGHT
//	Copyright Washington University, 1999
//
// ====================
//
//  = VERSION
//	$Revision: 1.3 $
//
//  = DATE RELEASED
//	$Date: 2001/11/01 15:27:56 $
//
//  = COMMENTTEXT
//	Comments

#ifndef MLMPPSISIN
#define MLMPPSISIN

#include <iostream>
#include <string>

#include "MESA.hpp"
#include "MSOPHandler.hpp"

class MDBImageManager;

using namespace std;

class MLMPPS : public MSOPHandler
// = TITLE
//	Short title/description
//
// = DESCRIPTION
//	Describe the class and how to use it
{
public:
  // = The standard methods in this framework.
  //MLMPPS();
  // Default constructor
  MLMPPS(const MLMPPS& cpy);
  virtual ~MLMPPS();
  virtual void printOn(ostream& s) const;
  // This method is used in conjunction with the streaming operator <<
  // to print the current state of MLMPPS

  virtual void streamIn(istream& s);

  MLMPPS(MDBImageManager* manager, const MString& logDir,
	 const MString& storageDir);

  int initialize();

  CONDITION handleNCreateCommand(DUL_PRESENTATIONCONTEXT* ctx,
				 MSG_N_CREATE_REQ** message,
				 MSG_N_CREATE_RESP* response,
				 DUL_ASSOCIATESERVICEPARAMETERS* params,
				 MString& directoryName);

  CONDITION handleNCreateDataSet(DUL_PRESENTATIONCONTEXT* ctx,
				 MSG_N_CREATE_REQ** message,
				 MSG_N_CREATE_RESP* response,
				 DUL_ASSOCIATESERVICEPARAMETERS* params,
				 MString& directoryName);

  CONDITION handleNSetCommand(DUL_PRESENTATIONCONTEXT* ctx,
			      MSG_N_SET_REQ** message,
			      MSG_N_SET_RESP* response,
			      DUL_ASSOCIATESERVICEPARAMETERS* params,
			      MString& directoryName);

  CONDITION handleNSetDataSet(DUL_PRESENTATIONCONTEXT* ctx,
			      MSG_N_SET_REQ** message,
			      MSG_N_SET_RESP* response,
			      DUL_ASSOCIATESERVICEPARAMETERS* params,
			      MString& directoryName);

  
  // = Class specific methods.
private:
  MDBImageManager* mImageManager;
  const MString mLogDir;
  const MString mStorageDir;
};

inline ostream& operator<< (ostream& s, const MLMPPS& c) {
	  c.printOn(s);
	  return s;
};

inline istream& operator >> (istream& s, MLMPPS& c) {
  c.streamIn(s);
  return s;
}

#endif
