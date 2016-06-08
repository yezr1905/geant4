// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: ExN05MinEkineCuts.hh,v 1.3 1999/12/15 14:49:28 gunter Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
//	For information related to this code contact:
//	CERN, CN Division, ASD group
// ------------------------------------------------------------
//                  14 Aug. 1998  H.Kurashige
// ------------------------------------------------------------

#ifndef ExN05MinEkineCuts_h
#define ExN05MinEkineCuts_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "ExN05SpecialCuts.hh"


class ExN05MinEkineCuts : public ExN05SpecialCuts
{
  public:     

     ExN05MinEkineCuts(const G4String& processName ="ExN05MinEkineCuts" );

     virtual ~ExN05MinEkineCuts();

     // PostStep GPIL
     virtual G4double PostStepGetPhysicalInteractionLength(
                             const G4Track& track,
			     G4double   previousStepSize,
			     G4ForceCondition* condition
			    );
            
			    
  private:
  
  // hide assignment operator as private 
      ExN05MinEkineCuts(ExN05MinEkineCuts&);
      ExN05MinEkineCuts& operator=(const ExN05MinEkineCuts& right);

};

#endif
