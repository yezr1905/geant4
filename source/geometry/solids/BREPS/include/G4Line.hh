// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Line.hh,v 1.6 2000/11/08 14:22:02 gcosmo Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// ----------------------------------------------------------------------
// Class G4Line
//
// Class description:
// 
// Definition of a generic line.

// Authors: J.Sulkimo, P.Urban.
// Revisions by: L.Broglia, G.Cosmo.
// ----------------------------------------------------------------------
#ifndef __LINE_H
#define __LINE_H 

#include "G4Curve.hh"

class G4Line : public G4Curve
{

public:  // with description

  G4Line();
  virtual ~G4Line();
    // Constructor & destructor.

  G4Line(const G4Line& orig);
  G4Line& operator=(const G4Line& right);
    // Copy constructor and assignment operator.

  G4Curve* Project(const G4Transform3D& tr=G4Transform3D::Identity);
    // Transforms and projects the line.

  G4bool Tangent(G4CurvePoint& cp, G4Vector3D& vec);
    // Returns tangent to line at a given point, if existing.
    // The tangent is computed from the 3D point representation.

  inline G4double  GetPMax() const;
  inline G4Point3D GetPoint(G4double param) const;
  inline G4double  GetPPoint(const G4Point3D& pt) const;
    // Accessors methods.

  inline G4Point3D  GetPnt() const;
  inline G4Vector3D GetDir() const;
  inline void Init(const G4Point3D& pnt0, const G4Vector3D& dir0);
    // Get/Set for the geometric data.

public:  // without description

  inline G4int IntersectRay2D(const G4Ray& ray);
  // void IntersectRay2D(const G4Ray& ray, G4CurveRayIntersection& is);

protected:

  inline void InitBounded();

private:
  
  // geometric data
  G4Point3D  pnt;
  G4Vector3D dir;
  G4Vector3D invDir;   // dir / |dir|^2 always
  G4Vector3D v;        // dir / |dir| always

};

#include "G4Line.icc"

#endif