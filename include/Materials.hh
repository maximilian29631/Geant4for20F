#ifndef Materials_H
#define Materials_H 1

#include "G4Material.hh"

class Materials 
{
public:


  Materials();
  ~Materials();
  
  G4Material* FindMaterial(G4String );
    
private:
  // Elements
  
  G4Element* elementH;
  G4Element* elementC;
  G4Element* elementN;
  G4Element* elementO;
  G4Element* elementMg;
  G4Element* elementAl;
  G4Element* elementSi;
  G4Element* elementTi;
  G4Element* elementV;
  G4Element* elementFe;
  G4Element* elementCu;
  G4Element* elementMo;
  G4Element* elementPt;
  G4Element* elementAu;
  G4Element* elementBi;
  G4Element* elementCo;

  // Materials

  G4Material* Pb;
  G4Material* Fe;
  G4Material* G4_Galactic;
  G4Material* vacuum;
  G4Material* G10;
  G4Material* polyethylene;
  G4Material* polypropylene;
  G4Material* Al;
  G4Material* Cu;
  G4Material* Nb;
  G4Material* C;
  G4Material* Au;
  G4Material* Bi;
  G4Material* Ir;
  G4Material* Si;
  G4Material* Be;
  G4Material* LH;
  G4Material* MgO;
  G4Material* ssteel;

};

#endif

