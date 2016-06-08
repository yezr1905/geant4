// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4OpenGLXmBox.hh,v 1.5 2001/03/07 14:56:15 johna Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
//Box container class

#ifdef G4VIS_BUILD_OPENGLXM_DRIVER

#ifndef G4OPENGLXMBOX_HH
#define G4OPENGLXMBOX_HH

#include "G4OpenGLXmVWidgetContainer.hh"
#include "globals.hh"
#include <Xm/Frame.h>
#include <Xm/RowColumn.h>

class G4OpenGLXmVWidgetComponent;
class G4OpenGLXmVWidgetShell;

class G4OpenGLXmBox : public G4OpenGLXmVWidgetContainer
{

public:
  G4OpenGLXmBox (const char* = NULL,
		 G4bool = False);   //constructor
  virtual ~G4OpenGLXmBox ();  //destructor

  void AddChild (G4OpenGLXmVWidgetComponent*);
  void AddYourselfTo (G4OpenGLXmVWidgetShell*);

  Widget* GetPointerToParent ();
  Widget* GetPointerToWidget ();
  
  const char* GetName ();
  void SetName (const char*);

protected:
  const char* name;
  Widget* parent;
  Widget box_row_col;
  G4bool radio;

private:
  G4OpenGLXmBox (const G4OpenGLXmBox&);
  G4OpenGLXmBox& operator = (const G4OpenGLXmBox&);
};

#endif

#endif