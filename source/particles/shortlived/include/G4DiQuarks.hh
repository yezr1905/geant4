// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4DiQuarks.hh,v 1.3 1999/12/15 14:51:15 gunter Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      For information related to this code contact:
//      CERN, CN Division, ASD group
//      History: first implementation, based on object model of
//      Hisaya Kurashige, 27 June 1998
// ----------------------------------------------------------------


#ifndef G4DiQuarks_h
#define G4DiQuarks_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4VShortLivedParticle.hh"

// ######################################################################
// ###                          DiQuarks                              ###
// ######################################################################

class G4DiQuarks : public G4VShortLivedParticle
{
 public:
   G4DiQuarks(
       const G4String&     aName,        G4double            mass,
       G4double            width,        G4double            charge,   
       G4int               iSpin,        G4int               iParity,    
       G4int               iConjugation, G4int               iIsospin,   
       G4int               iIsospin3,    G4int               gParity,
       const G4String&     pType,        G4int               lepton,      
       G4int               baryon,       G4int               encoding,
       G4bool              stable,       G4double            lifetime,
       G4DecayTable        *decaytable
   );
   G4DiQuarks*    DiQuarksDefinition(){return this;};
   G4DiQuarks*    DiQuarks(){return this;};
   virtual        ~G4DiQuarks(){}
};


#endif





