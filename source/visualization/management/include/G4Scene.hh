// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Scene.hh,v 1.8 2001/02/23 15:43:15 johna Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
// Scene  John Allison  19th July 1996.
//
// Class Description:
//
// Defines the viewable scene.

#ifndef G4SCENE_HH
#define G4SCENE_HH

#include "globals.hh"
#include "G4ios.hh"

class G4VPhysicalVolume;

#include "G4VisExtent.hh"
#include "G4Point3D.hh"
#include "G4VModel.hh"
#include "g4std/vector"

class G4Scene {

public: // With description

  friend G4std::ostream& operator << (G4std::ostream& os, const G4Scene& d);

  enum {UNLIMITED = -1};

  G4Scene (const G4String& name = "scene-with-unspecified-name");
  ~G4Scene ();

  // Makes use of default (compiler generated) copy constructor and
  // assignment operator.

  G4bool operator == (const G4Scene&) const;
  G4bool operator != (const G4Scene&) const;

  //////////////////////////////////////////////////////
  // Get functions...

  const G4String& GetName () const;

  G4bool IsEmpty () const;

  const G4std::vector<G4VModel*>& GetRunDurationModelList () const;
  // Contains models which are expected to last for the duration of
  // the run, for example geometry volumes.

  const G4std::vector<G4VModel*>& GetEndOfEventModelList () const;
  // Contains models which are described at the end of event when the
  // scene is current.

  const G4VisExtent& GetExtent () const;
  // Overall extent of all objects in the scene.

  const G4Point3D& GetStandardTargetPoint () const;
  // Usually centre of extent.  See G4ViewParameters for definition.

  //////////////////////////////////////////////
  // Add, Set, Clear functions...

  G4bool AddRunDurationModel (G4VModel*);
  // Adds models of type which are expected to last for the duration
  // of the run, for example geometry volumes.
  // Returns false if model is already in the list.

  G4bool AddWorldIfEmpty ();
  // In some situations, if the user asks for a drawing and has not
  // yet set any run duration models it makes sense to put the "world"
  // in there by default.
  // Returns false if model is already in the list.

  G4bool AddEndOfEventModel (G4VModel*);
  // Adds models of type which are described at the end of event when
  // the scene is current.
  // Returns false if model is already in the list.

  G4std::vector<G4VModel*>& SetRunDurationModelList ();
  // Allows you to change the model list - do with care!

  G4std::vector<G4VModel*>& SetEndOfEventModelList ();
  // Allows you to change the model list - do with care!

  void Clear ();
  // Clears and destroys models in all lists.

private:
  G4String fName;
  G4std::vector<G4VModel*> fRunDurationModelList;
  G4std::vector<G4VModel*> fEndOfEventModelList;
  G4VisExtent fExtent;
  G4Point3D   fStandardTargetPoint;
};

#include "G4Scene.icc"

#endif