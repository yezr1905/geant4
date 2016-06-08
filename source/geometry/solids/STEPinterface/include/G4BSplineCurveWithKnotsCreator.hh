// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4BSplineCurveWithKnotsCreator.hh,v 1.3 2000/11/09 16:35:43 gcosmo Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// ----------------------------------------------------------------------
// Class G4BSplineCurveWithKnotsCreator
//
// Class description:
//
//

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
//
// History:
//   18-Nov-1999: First step of re-engineering - G.Cosmo
// ----------------------------------------------------------------------
#ifndef G4BSPLINECURVEWITHKNOTSCREATOR_HH
#define G4BSPLINECURVEWITHKNOTSCREATOR_HH

#include "G4GeometryCreator.hh"

class G4BSplineCurveWithKnotsCreator: private G4GeometryCreator 
{
  public:
  
    // Constructor & destructor

      G4BSplineCurveWithKnotsCreator();
      ~G4BSplineCurveWithKnotsCreator();

    // Member functions

      void CreateG4Geometry(STEPentity&);
      void CreateSTEPGeometry(void* G4obj);
      const char* Name() const { return "B_Spline_Curve_With_Knots"; }
      static G4BSplineCurveWithKnotsCreator GetInstance() { return csc; }

    // Members

  private:

    static G4BSplineCurveWithKnotsCreator csc;
};

#endif