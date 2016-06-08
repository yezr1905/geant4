// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4VisCommandsSceneAdd.cc,v 1.14 2001/02/23 15:43:27 johna Exp $
// GEANT4 tag $Name: geant4-03-01 $

// /vis/scene commands - John Allison  9th August 1998

#include "G4VisCommandsSceneAdd.hh"

#include "G4VisManager.hh"
#include "G4TransportationManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeModel.hh"
#include "G4LogicalVolumeModel.hh"
#include "G4ModelingParameters.hh"
#include "G4HitsModel.hh"
#include "G4TrajectoriesModel.hh"
#include "G4PhysicalVolumeSearchScene.hh"
#include "G4VGlobalFastSimulationManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4FlavoredParallelWorldModel.hh"
#include "G4ApplicationState.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "g4std/strstream"


////////////// /vis/scene/add/ghosts ///////////////////////////////////////

G4VisCommandSceneAddGhosts::G4VisCommandSceneAddGhosts () {
  G4bool omitable;
  fpCommand = new G4UIcmdWithAString ("/vis/scene/add/ghosts", this);
  fpCommand -> SetGuidance
    ("/vis/scene/add/ghosts [<particle-name>]");
  fpCommand -> SetGuidance
    ("Adds ghost volumes (G4FlavoredParallelWorld) to the current scene.");
  fpCommand -> SetGuidance
    ("Selects by particle (default = \"all\").");
  fpCommand -> SetParameterName ("particle", omitable = true);
  fpCommand -> SetDefaultValue ("all");
}

G4VisCommandSceneAddGhosts::~G4VisCommandSceneAddGhosts () {
  delete fpCommand;
}

G4String G4VisCommandSceneAddGhosts::GetCurrentValue (G4UIcommand* command) {
  return "";
}

void G4VisCommandSceneAddGhosts::SetNewValue (G4UIcommand* command,
					      G4String newValue) {
  G4Scene* pCurrentScene = fpVisManager -> GetCurrentScene ();
  const G4String& currentSceneName = pCurrentScene -> GetName ();

  G4SceneList& sceneList = fpVisManager -> SetSceneList ();
  if (sceneList.empty ()) {
    G4cout << "No scenes - please create one before adding anything."
	   << G4endl;
    return;
  }

  G4VGlobalFastSimulationManager* theGlobalFastSimulationManager;
  if(!(theGlobalFastSimulationManager = 
       G4VGlobalFastSimulationManager::GetConcreteInstance ())){
    G4cout<< "WARNING: no G4GlobalFastSimulationManager" << G4endl;
    return;
  }
  
  G4ParticleTable* theParticleTable=G4ParticleTable::GetParticleTable();
  
  if(newValue=="all") {
    G4VFlavoredParallelWorld* CurrentFlavoredWorld;
    G4bool successful;
    for (G4int iParticle=0; iParticle<theParticleTable->entries(); 
	 iParticle++)
      if(CurrentFlavoredWorld=theGlobalFastSimulationManager->
	 GetFlavoredWorldForThis(theParticleTable->
				 GetParticle(iParticle)))
	successful = pCurrentScene -> AddRunDurationModel
	  (new G4FlavoredParallelWorldModel (CurrentFlavoredWorld));
    UpdateVisManagerScene ();
    if (successful) {
      G4cout << "Ghosts added to the Scene, refresh the view to see it."
	     << G4endl;
    }
    return;
  }
  
  G4ParticleDefinition* currentParticle = 
    theParticleTable->FindParticle(newValue);
  if (currentParticle == NULL) {
    G4cout << "\"" << newValue << "\": not found this particle name!" << G4endl;
    return;
  }

  G4VFlavoredParallelWorld* worldForThis;
  if(worldForThis=theGlobalFastSimulationManager->
     GetFlavoredWorldForThis(currentParticle)) {
    G4bool successful = pCurrentScene -> AddRunDurationModel
      (new G4FlavoredParallelWorldModel (worldForThis));
    UpdateVisManagerScene (currentSceneName);
    if (successful) {
      G4cout << "Ghosts added to the Scene, refresh the view to see it."
	     << G4endl;
    }
  }
  else G4cout << "There are no ghosts for \""<<newValue<<"\""<<G4endl;
}


////////////// /vis/scene/add/hits ///////////////////////////////////////

G4VisCommandSceneAddHits::G4VisCommandSceneAddHits () {
  fpCommand = new G4UIcmdWithoutParameter ("/vis/scene/add/hits", this);
  fpCommand -> SetGuidance
    ("Adds hits to current scene.");
  fpCommand -> SetGuidance
    ("Hits are drawn at end of event when the scene in which"
     " they are added is current.");
}

G4VisCommandSceneAddHits::~G4VisCommandSceneAddHits () {
  delete fpCommand;
}

G4String G4VisCommandSceneAddHits::GetCurrentValue (G4UIcommand* command) {
  return "";
}

void G4VisCommandSceneAddHits::SetNewValue (G4UIcommand* command,
						G4String newValue) {
  G4SceneList& list = fpVisManager -> SetSceneList ();
  if (list.empty ()) {
    G4cout << "No scenes - please create one before adding anything."
	   << G4endl;
    return;
  }

  G4HitsModel* model = new G4HitsModel;
  G4Scene* pCurrentScene = fpVisManager -> GetCurrentScene ();
  const G4String& currentSceneName = pCurrentScene -> GetName ();
  pCurrentScene -> AddEndOfEventModel (model);
  G4cout << "Hits will be drawn in scene \""
	 << currentSceneName << "\"."
	 << G4endl;
}

////////////// /vis/scene/add/logicalVolume //////////////////////////////////

G4VisCommandSceneAddLogicalVolume::G4VisCommandSceneAddLogicalVolume () {
  G4bool omitable;
  fpCommand = new G4UIcommand ("/vis/scene/add/logicalVolume", this);
  fpCommand -> SetGuidance
    ("/vis/scene/add/logicalVolume <logical-volume-name>"
     " [<depth-of-descending>]");
  fpCommand -> SetGuidance ("Adds a logical volume to the current scene.");
  fpCommand -> SetGuidance
    ("1st parameter: volume name.");
  //  fpCommand -> SetGuidance  // Not implemented - should be in geom?
  //    ("               \"list\" to list all volumes.");
  fpCommand -> SetGuidance
    ("2nd parameter: depth of descending geometry hierarchy (default 1).");
  G4UIparameter* parameter;
  parameter = new G4UIparameter ("volume", 's', omitable = false);
  fpCommand -> SetParameter (parameter);
  parameter = new G4UIparameter ("depth", 'i', omitable = true);
  parameter -> SetDefaultValue (1);
  fpCommand -> SetParameter (parameter);
}

G4VisCommandSceneAddLogicalVolume::~G4VisCommandSceneAddLogicalVolume () {
  delete fpCommand;
}

G4String G4VisCommandSceneAddLogicalVolume::GetCurrentValue (G4UIcommand*) {
  return "";
}

void G4VisCommandSceneAddLogicalVolume::SetNewValue (G4UIcommand* command,
						     G4String newValue) {
  G4SceneList& sceneList = fpVisManager -> SetSceneList ();
  if (sceneList.empty ()) {
    G4cout << "No scenes - please create one before adding anything."
	   << G4endl;
    return;
  }

  G4String name;
  G4int requestedDepthOfDescent;
  const char* s = newValue;
  G4std::istrstream is ((char*)s);
  is >> name >> requestedDepthOfDescent;

  G4LogicalVolumeStore *pLVStore = G4LogicalVolumeStore::GetInstance();
  int nLV = pLVStore -> entries ();
  int iLV;
  G4LogicalVolume* pLV;
  for (iLV = 0; iLV < nLV; iLV++ ) {
    pLV = (*pLVStore) [iLV];
    if (pLV -> GetName () == name) break;
  }
  if (iLV == nLV) {
    G4cout << "Logical volume " << name
	   << " not found in logical volume Store." << G4endl;
    return;
  }

  G4VModel* model = new G4LogicalVolumeModel (pLV, requestedDepthOfDescent);
  G4Scene* pScene = fpVisManager -> GetCurrentScene ();
  const G4String& currentSceneName = pScene -> GetName ();
  G4bool succesful = pScene -> AddRunDurationModel (model);
  UpdateVisManagerScene (currentSceneName);
  if (succesful) {
    G4cout << "Logical volume \"" << pLV -> GetName ()
	   << " with requested depth of descent "
	   << requestedDepthOfDescent
	   << ",\n  has been added to scene \"" << currentSceneName << "\""
	   << G4endl;
  }
}


////////////// /vis/scene/add/trajectories ///////////////////////////////////

G4VisCommandSceneAddTrajectories::G4VisCommandSceneAddTrajectories () {
  fpCommand = new G4UIcmdWithoutParameter
    ("/vis/scene/add/trajectories", this);
  fpCommand -> SetGuidance
    ("Adds trajectories to current scene.");
  fpCommand -> SetGuidance
    ("Trajectories are drawn at end of event when the scene in which"
     " they are added is current.");
}

G4VisCommandSceneAddTrajectories::~G4VisCommandSceneAddTrajectories () {
  delete fpCommand;
}

G4String G4VisCommandSceneAddTrajectories::GetCurrentValue (G4UIcommand* command) {
  return "";
}

void G4VisCommandSceneAddTrajectories::SetNewValue (G4UIcommand* command,
					      G4String newValue) {
  G4SceneList& list = fpVisManager -> SetSceneList ();
  if (list.empty ()) {
    G4cout << "No scenes - please create one before adding anything."
	   << G4endl;
    return;
  }

  G4TrajectoriesModel* model = new G4TrajectoriesModel;
  G4Scene* pCurrentScene = fpVisManager -> GetCurrentScene ();
  const G4String& currentSceneName = pCurrentScene -> GetName ();
  pCurrentScene -> AddEndOfEventModel (model);
  G4cout << "Trajectories will be drawn in scene \""
	 << currentSceneName << "\"."
	 << G4endl;
}

////////////// /vis/scene/add/volume ///////////////////////////////////////

G4VisCommandSceneAddVolume::G4VisCommandSceneAddVolume () {
  G4bool omitable;
  fpCommand = new G4UIcommand ("/vis/scene/add/volume", this);
  fpCommand -> SetGuidance
    ("/vis/scene/add/volume [<physical-volume-name>] [<copy-no>] [<depth-of-descending>]");
  fpCommand -> SetGuidance ("Adds a physical volume to the current scene.");
  fpCommand -> SetGuidance ("Note: adds first occurence only.");
  fpCommand -> SetGuidance
    ("1st parameter: volume name (default \"world\").");
  //  fpCommand -> SetGuidance  // Not implemented - should be in geom?
  //    ("               \"list\" to list all volumes.");
  fpCommand -> SetGuidance
    ("2nd parameter: copy number (default 0).");
  fpCommand -> SetGuidance
    ("3rd parameter: depth of descending geometry hierarchy"
     " (default G4Scene::UNLIMITED (-1)).");
  G4UIparameter* parameter;
  parameter = new G4UIparameter ("volume", 's', omitable = true);
  parameter -> SetDefaultValue ("world");
  fpCommand -> SetParameter (parameter);
  parameter = new G4UIparameter ("copy-no", 'i', omitable = true);
  parameter -> SetDefaultValue (0);
  fpCommand -> SetParameter (parameter);
  parameter = new G4UIparameter ("depth", 'i', omitable = true);
  parameter -> SetDefaultValue (G4Scene::UNLIMITED);
  fpCommand -> SetParameter (parameter);
}

G4VisCommandSceneAddVolume::~G4VisCommandSceneAddVolume () {
  delete fpCommand;
}

G4String G4VisCommandSceneAddVolume::GetCurrentValue (G4UIcommand* command) {
  return "world 0 -1";
}

void G4VisCommandSceneAddVolume::SetNewValue (G4UIcommand* command,
					      G4String newValue) {
  G4SceneList& sceneList = fpVisManager -> SetSceneList ();
  if (sceneList.empty ()) {
    G4cout << "No scenes - please create one before adding anything."
	   << G4endl;
    return;
  }

  G4String name;
  G4int copyNo;
  G4int requestedDepthOfDescent;
  const char* s = newValue;
  G4std::istrstream is ((char*)s);
  is >> name >> copyNo >> requestedDepthOfDescent;
  G4VPhysicalVolume* world =
    G4TransportationManager::GetTransportationManager ()
    -> GetNavigatorForTracking () -> GetWorldVolume ();
  G4PhysicalVolumeModel* model = 0;
  G4VPhysicalVolume* foundVolume = 0;
  G4int foundDepth = 0;

  if (name == "world") {
    if (world) {
      model = new G4PhysicalVolumeModel (world);
      foundVolume = world;
    }
    else {
      G4cerr << "G4VisCommandSceneAddVolume::SetNewValue: *** ERROR ***"
	     << "\n  No world - shouldn't happen if G4ApplicationState is"
	     << " being properly noted!!" << G4endl;
    }
  }
  else {
 
    // Create search scene, model and modeling parameters with
    // long-enough life...
    G4PhysicalVolumeSearchScene searchScene (name, copyNo);
    G4PhysicalVolumeModel searchModel (world);
    G4ModelingParameters mp;
    searchModel.SetModelingParameters (&mp);

    // Initiate search...
    searchModel.DescribeYourselfTo (searchScene);

    // OK, what have we got...?
    foundVolume = searchScene.GetFoundVolume ();
    foundDepth = searchScene.GetFoundDepth ();
    const G4Transform3D&
      transformation = searchScene.GetFoundTransformation ();

    if (foundVolume) {
      model = new G4PhysicalVolumeModel (foundVolume,
					 requestedDepthOfDescent,
					 transformation);
    }
    else {
      G4cout << "Volume \"" << name << "\", copy no. " << copyNo
	     << " not found." << G4endl;
    }
  }

  if (model) {
    G4Scene* pScene = fpVisManager -> GetCurrentScene ();
    const G4String& currentSceneName = pScene -> GetName ();
    G4bool successful = pScene -> AddRunDurationModel (model);
    UpdateVisManagerScene (currentSceneName);
    if (successful) {
      G4cout << "First occurrence of \"" << foundVolume -> GetName ()
	     << "\", copy no. " << copyNo
	     << ", found at depth " << foundDepth
	     << ",\n  with further requested depth of descent "
	     << requestedDepthOfDescent
	     << ", has been added to scene \"" << currentSceneName << "\""
	     << G4endl;
    }
  }
}