// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Step.cc,v 1.4 1999/12/15 14:53:56 gunter Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
//
//---------------------------------------------------------------
//
//  G4Step.cc
//
//  Description:
//    This class represents the Step of a particle tracked.
//    It includes information of 
//      1) List of Step points which compose the Step,
//      2) static information of particle which generated the 
//         Step, 
//      3) trackID and parent particle ID of the Step,
//      4) termination condition of the Step,
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
// ---------------------------------------------------------------

#include "G4Step.hh"

////////////////
G4Step::G4Step()
////////////////
{
  fpPreStepPoint  = new G4StepPoint();
  fpPostStepPoint = new G4StepPoint();
}

/////////////////
G4Step::~G4Step()
/////////////////
{
  delete fpPreStepPoint;
  delete fpPostStepPoint;
}