// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4KnotVector.hh,v 1.6 2000/11/08 14:22:02 gcosmo Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// ----------------------------------------------------------------------
// Class G4KnotVector
//
// Class description:
// 
// Utility class for operations on a vector of knots.

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
// ----------------------------------------------------------------------
#ifndef __KNOTVECTOR_H
#define __KNOTVECTOR_H

#include <math.h>
#include "geomdefs.hh"

class G4KnotVector
{

 public:  // with description
 
  G4KnotVector();
  G4KnotVector(G4int sz);
  ~G4KnotVector();
    // Constructors & destructor.

  G4KnotVector(const G4KnotVector& orig);
    // Copy constructor.

  G4KnotVector& operator=(const G4KnotVector& right);
    // Assignment operator.

  inline G4int GetSize() const;
    // Gets number of knots.

  inline G4double GetKnot(G4int knot_number) const;
    // Retrieves knot from knot vector index knot_number.

  inline void PutKnot(G4int knot_number, G4double value);
    // Sets knot vector index knot_number to value.

  G4KnotVector* MultiplyKnotVector(G4int num, G4double value);  
    // Adds the internal knots to the new knot vector.

  G4double* MergeKnotVector(const G4double *knots_to_add, G4int add_size);
    // Creates the new vector by merging the old vector with the
    // knots in the vector knots_to_add.

  G4int CheckKnotVector(G4double val) const;
    // Finds out how many times val occurs in the knot vector.

  void ExtractKnotVector(G4KnotVector* kv, G4int upper, G4int lower);
    // Copies either the first half or the second half of
    // the new knot vector values to the knot vectors of the
    // new surfaces created by splitting.

  G4int GetKnotIndex(G4double k_value, G4int order) const;
    // Searches the knot vector for the value and returns the index
    // This is used in the evaluation of the intersection to find
    // out between which knots the intersection point is on the b-spline
    // surface.

 private:

  inline G4double ApxEq(G4double x, G4double y) const;

 private:

  G4int k_size;
    // Number of knots.

  G4double *knots;
    // Knot vector.

};

#include "G4KnotVector.icc"

#endif