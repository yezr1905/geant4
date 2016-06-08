// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ProcTblElement.cc,v 1.5 2000/11/03 03:44:52 kurasige Exp $
// GEANT4 tag $Name: geant4-03-01 $
//
// 
// ------------------------------------------------------------
//	GEANT 4 class header file 
//
//	For information related to this code contact:
//	CERN, IT Division, ASD group
//	History: first implementation, based on object model of
//	4th Aug 1998, H.Kurashige
// ------------------------------------------------------------
//   Use STL vector instead of RW vector    1. Mar 00 H.Kurashige
//
#include "G4ProcTblElement.hh"


// default constructor ////////////////////////
G4ProcTblElement::G4ProcTblElement()
   :pProcess(0),pProcMgrVector(0)
{
}

//////////////////////////
G4ProcTblElement::G4ProcTblElement(G4VProcess* aProcess):
  pProcess(aProcess)
{
  pProcMgrVector = new G4ProcMgrVector();
}

// copy constructor //////////////////////////
G4ProcTblElement::G4ProcTblElement(const G4ProcTblElement &right)
{
  *this = right;
}


// destructor ////////////////////////
G4ProcTblElement::~G4ProcTblElement()
{
  if (pProcMgrVector != 0) {
    pProcMgrVector->clear();
    delete pProcMgrVector;
  }
}


//////////////////////////
G4ProcTblElement & G4ProcTblElement::operator=(const G4ProcTblElement &right)
{
  if (this != &right) {
    pProcess       = right.pProcess;
    // copy all contents in  pProcMgrVector
    if (pProcMgrVector != 0) {
      pProcMgrVector->clear();
      delete pProcMgrVector;
    }
    pProcMgrVector = new G4ProcMgrVector();
    G4ProcMgrVector::iterator i;
    for (i = right.pProcMgrVector->begin(); i!= right.pProcMgrVector->end(); ++i) {
      pProcMgrVector->push_back(*i);
    }
  }
  return *this;
}


//////////////////////////
G4int G4ProcTblElement::operator==(const G4ProcTblElement &right) const
{
  return (this == &right);
}


//////////////////////////
G4int G4ProcTblElement::operator!=(const G4ProcTblElement &right) const
{
  return (this != &right);
}























