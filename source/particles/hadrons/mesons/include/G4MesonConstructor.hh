// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4MesonConstructor.hh,v 1.2 1999/12/15 14:51:04 gunter Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
// --------------------------------------------------------------
//	GEANT 4 class implementation file 
//
#ifndef G4MesonConstructor_h
#define G4MesonConstructor_h 1

#include "globals.hh"
#include "G4ios.hh"

class G4MesonConstructor
{
  //This class is a utility class for construction 
  //short lived particles

  public:
    G4MesonConstructor();
    ~G4MesonConstructor();
  
  public:
    void ConstructParticle();

  protected:
    void ConstructLightMesons();
    void ConstructCharmMesons();
    void ConstructBottomMesons();
};

#endif