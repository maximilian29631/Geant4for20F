#include "Materials.hh"
#include "G4UnitsTable.hh"

 Materials:: Materials()
{
  // Elements
 
  elementH  = new G4Element("Hydrogen",  "H",  1.,  1.0079*CLHEP::g/CLHEP::mole);
  elementC  = new G4Element("Carbon",    "C",  6.,  12.011*CLHEP::g/CLHEP::mole);
  elementN  = new G4Element("Nitrogen",  "N",  7.,  14.007*CLHEP::g/CLHEP::mole);
  elementO  = new G4Element("Oxygen",    "O",  8., 15.9994*CLHEP::g/CLHEP::mole);
  elementMg = new G4Element("Magnesium", "Mg",12., 24.3050*CLHEP::g/CLHEP::mole);
  elementAl = new G4Element("Aluminium", "Al",13., 26.9815*CLHEP::g/CLHEP::mole);
  elementSi = new G4Element("Silicon",   "Si",14., 28.0855*CLHEP::g/CLHEP::mole);
  elementTi = new G4Element("Titanium",  "Ti",22.,   47.90*CLHEP::g/CLHEP::mole);
  elementV  = new G4Element("Vanadium",  "V", 23., 50.9415*CLHEP::g/CLHEP::mole);
  elementFe = new G4Element("Iron",      "Fe",26.,  55.845*CLHEP::g/CLHEP::mole);
  elementCu = new G4Element("Copper",    "Cu",29.,   63.55*CLHEP::g/CLHEP::mole);
  elementMo = new G4Element("Molybdenum","Mo",42.,   95.94*CLHEP::g/CLHEP::mole);
  elementPt = new G4Element("Platinum",  "Pt",78.,  195.08*CLHEP::g/CLHEP::mole);
  elementAu = new G4Element("Gold",      "Au",79.,  196.97*CLHEP::g/CLHEP::mole);
  elementBi = new G4Element("Bismuth",   "Bi",83.,  208.98*CLHEP::g/CLHEP::mole);
  elementCo = new G4Element("Cobalt",   "Co",27.,  58.93*CLHEP::g/CLHEP::mole);

// Materials

  vacuum = new G4Material("vacuum", 1, 1.00794*CLHEP::g/CLHEP::mole, 
     1.0E-25*CLHEP::g/CLHEP::cm3, kStateGas, 0.1*CLHEP::kelvin, 1.0E-19*CLHEP::hep_pascal);
  G4_Galactic = new G4Material("G4_Galactic", 1, 1.00794*CLHEP::g/CLHEP::mole, 
     1.0E-25*CLHEP::g/CLHEP::cm3, kStateGas, 0.1*CLHEP::kelvin, 1.0E-19*CLHEP::hep_pascal);

  LH = new G4Material("LH", 2., 2.0159*CLHEP::g/CLHEP::mole, 70.99*CLHEP::mg/CLHEP::cm3);

  G10 = new G4Material("G10", 1.70*CLHEP::g/CLHEP::cm3, 4);
  G10->AddElement(elementSi, 1);
  G10->AddElement(elementO, 2);
  G10->AddElement(elementC, 3);
  G10->AddElement(elementH, 3);

    ssteel = new G4Material("ssteel", 7.7 * CLHEP::g / CLHEP::cm3, 3);
    ssteel->AddElement(elementC, 0.04);
    ssteel->AddElement(elementFe, 0.88);
    ssteel->AddElement(elementCo, 0.08);

  polyethylene = new G4Material("polyethylene", 1.36*CLHEP::g/CLHEP::cm3, 2);
  polyethylene->AddElement(elementC, 1);
  polyethylene->AddElement(elementH, 2);

  polypropylene = new G4Material("polypropylene", 0.9*CLHEP::g/CLHEP::cm3, 2);
  polypropylene->AddElement(elementC, 1);
  polypropylene->AddElement(elementH, 2);

  MgO = new G4Material("MgO", 1.0*CLHEP::g/CLHEP::cm3, 2);
  MgO->AddElement(elementC, 1);
  MgO->AddElement(elementMg, 1);

  Pb = new G4Material("Pb", 82., 207.2*CLHEP::g/CLHEP::mole,     11.34*CLHEP::g/CLHEP::cm3);
  Fe = new G4Material("Fe", 26., 55.845*CLHEP::g/CLHEP::mole,    7.874*CLHEP::g/CLHEP::cm3);
  Al = new G4Material("Al", 13., 26.98153*CLHEP::g/CLHEP::mole,  2.70*CLHEP::g/CLHEP::cm3);
  Cu = new G4Material("Cu", 29., 63.55*CLHEP::g/CLHEP::mole,     8.96*CLHEP::g/CLHEP::cm3);
  Nb = new G4Material("Nb", 41., 92.90638*CLHEP::g/CLHEP::mole,  8.57*CLHEP::g/CLHEP::cm3);
  C  = new G4Material("C",   6., 12.011*CLHEP::g/CLHEP::mole,    2.15*CLHEP::g/CLHEP::cm3);
  Au = new G4Material("Au", 79., 196.9*CLHEP::g/CLHEP::mole,    19.32*CLHEP::g/CLHEP::cm3);
  Bi = new G4Material("Bi", 83., 208.98*CLHEP::g/CLHEP::mole,    9.78*CLHEP::g/CLHEP::cm3);
  Be = new G4Material("Be",  4., 9.012182*CLHEP::g/CLHEP::mole,  1.84*CLHEP::g/CLHEP::cm3);
  Si = new G4Material("Si", 14., 28.0855*CLHEP::g/CLHEP::mole,   2.33*CLHEP::g/CLHEP::cm3);
  Ir = new G4Material("Ir", 77., 192.217*CLHEP::g/CLHEP::mole,  22.65*CLHEP::g/CLHEP::cm3);

}

 Materials::~ Materials()
{;}
//-----------------------------------------------------------------------------
G4Material*  Materials::FindMaterial(G4String materialName)
{


   // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  

  return pttoMaterial;
  
}
