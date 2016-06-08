// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ios.cc,v 1.4 1999/11/23 15:00:06 gcosmo Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
// --------------------------------------------------------------
//      GEANT 4 class implementation file
//
// G4ios.cc
//
// History 1998 Nov. 3 Masayasu Nagamatu

#include "G4ios.hh"


#include "G4strstreambuf.hh"

G4strstreambuf G4coutbuf;
G4strstreambuf G4cerrbuf;
G4std::ostream G4cout(&G4coutbuf);
G4std::ostream G4cerr(&G4cerrbuf);


