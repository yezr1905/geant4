// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4Isotope.hh,v 1.6 2001/03/30 14:43:16 maire Exp $
// GEANT4 tag $Name: geant4-03-01 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// class description
//
// An isotope is a chemical isotope defined by its name,
//                                                 Z (atomic number),
//                                                 N (number of nucleons), 
//                                                 A (mass of a mole).
//
// The class contains as a private static member the table of defined
// isotopes (an ordered vector of isotopes).
//
// Isotopes can be assembled into elements via the G4Element class.
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

// 30.03.01: suppression of the warnig message in GetIsotope
// 04.08.98: new method GetIsotope(isotopeName) (mma)
// 17.01.97: aesthetic rearrangement (mma)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

#ifndef G4ISOTOPE_HH
#define G4ISOTOPE_HH

#include "G4ios.hh"
#include "g4rw/tpordvec.h"
#include "globals.hh"

class G4Isotope;
typedef G4RWTPtrOrderedVector<G4Isotope> G4IsotopeTable;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

class G4Isotope
{
 public:  // with description

    // Make an isotope
    G4Isotope(const G4String& name,		//its name
                    G4int     z,		//atomic number
                    G4int     n,		//number of nucleons
                    G4double  a);		//mass of mole
                    
   virtual ~G4Isotope();

    // Retrieval methods
    G4String GetName()  const {return fName;};    
    G4int    GetZ()     const {return fZ;};
    G4int    GetN()     const {return fN;};
    G4double GetA()     const {return fA;};
    size_t   GetIndex() const {return fIndexInTable;};
    
    static  G4Isotope* GetIsotope(G4String name);
    
    static
    const G4IsotopeTable* GetIsotopeTable() {return &theIsotopeTable;};
    static size_t GetNumberOfIsotopes()     {return theIsotopeTable.length();};
    
    friend
    G4std::ostream& operator<<(G4std::ostream&, G4Isotope*);
    
    friend
    G4std::ostream& operator<<(G4std::ostream&, G4Isotope&);
    
    friend
    G4std::ostream& operator<<(G4std::ostream&, G4IsotopeTable);
     
 public:  // without description
 
    G4int operator==(const G4Isotope&) const;
    G4int operator!=(const G4Isotope&) const;
    
 private:
     
    G4Isotope(G4Isotope&);
    G4Isotope& operator=(const G4Isotope&);
     
 private:

    G4String fName;              // name of the Isotope
    G4int    fZ;                 // atomic number
    G4int    fN;                 // number of nucleons
    G4double fA;                 // mass of a mole

    static 
    G4IsotopeTable theIsotopeTable;
    size_t   fIndexInTable;      // index in the Isotope Table
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.... ....oooOO0OOooo....

inline
G4Isotope* G4Isotope::GetIsotope(G4String isotopeName)
{  
  // search the isotope by its name 
  for (G4int J=0 ; J<theIsotopeTable.length() ; J++)
   {
    if(theIsotopeTable[J]->GetName() == isotopeName)
      return theIsotopeTable[J];
   }
   
  // the isotope does not exist in the table
  return NULL;          
}

#endif