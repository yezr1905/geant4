// This code implementation is the intellectual property of
// neutron_hp -- header file
// J.P. Wellisch, Nov-1996
// A prototype of the low energy neutron transport model.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4InterpolationScheme.hh,v 1.2 1999/07/02 09:58:05 johna Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
#ifndef G4InterpolationScheme_h
#define G4InterpolationScheme_h 1

#include "globals.hh"

// carthesian interpolation
// unit base interpolation
// corresponding point interpolation

enum G4InterpolationScheme 
{
  START,  HISTO,  LINLIN,  LINLOG,  LOGLIN,  LOGLOG, RANDOM, 
  CSTART, CHISTO, CLINLIN, CLINLOG, CLOGLIN, CLOGLOG, CRANDOM, 
  USTART, UHISTO, ULINLIN, ULINLOG, ULOGLIN, ULOGLOG, URANDOM 
};

#endif