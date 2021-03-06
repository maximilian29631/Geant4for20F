//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm5/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 98752 2016-08-09 13:44:40Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SubtractionSolid.hh"  //LR

#include <sstream>
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fAbsorberMaterial(0),fWorldMaterial(0),fDefaultWorld(true),
 fSolidWorld(0),fLogicWorld(0),fPhysiWorld(0),
 fSolidAbsorber(0),fLogicAbsorber(0),fPhysiAbsorber(0),
 fDetectorMessenger(0)
{
  // default parameter values of the calorimeter
  fAbsorberThickness = 9.76*cm;
  fAbsorberSizeYZ    = 5.*cm;
  fXposAbs           = 0.*cm;
  ComputeCalorParameters();
  
  // materials  
  DefineMaterials();
  SetWorldMaterial   ("G4_Galactic");
  SetAbsorberMaterial("G4_CESIUM_IODIDE");
 
  // create commands for interactive definition of the calorimeter  
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  //This function illustrates the possible ways to define materials
 
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  

  G4int ncomponents, natoms;
  G4double fractionmass;
  G4double temperature, pressure;
  
  //
  // define Elements
  //

  G4Element* H  = new G4Element("Hydrogen",symbol="H",  z= 1, a=   1.01*g/mole);
  G4Element* C  = new G4Element("Carbon",  symbol="C",  z= 6, a=  12.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N",  z= 7, a=  14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen",  symbol="O",  z= 8, a=  16.00*g/mole);
  G4Element* Na = new G4Element("Sodium",  symbol="Na", z=11, a=  22.99*g/mole);
  G4Element* Ar = new G4Element("Argon",   symbol="Ar", z=18, a=  39.95*g/mole);
  G4Element* I  = new G4Element("Iodine",  symbol="I" , z=53, a= 126.90*g/mole);
  G4Element* Xe = new G4Element("Xenon",   symbol="Xe", z=54, a= 131.29*g/mole);
  G4Element* Mg = new G4Element("Magnesium", "Mg",12., 24.3050*g/mole);

  //
  // define simple materials
  //

  new G4Material("H2Liq"    , z= 1, a= 1.01*g/mole, density= 70.8*mg/cm3);
  new G4Material("Beryllium", z= 4, a= 9.01*g/mole, density= 1.848*g/cm3);
  new G4Material("Aluminium", z=13, a=26.98*g/mole, density= 2.700*g/cm3);
  new G4Material("Silicon"  , z=14, a=28.09*g/mole, density= 2.330*g/cm3);

  G4Material* lAr = 
    new G4Material("liquidArgon", density= 1.390*g/cm3, ncomponents=1);
  lAr->AddElement(Ar, natoms=1);

  new G4Material("Iron",     z=26, a= 55.85*g/mole, density= 7.870*g/cm3);
  new G4Material("Copper",   z=29, a= 63.55*g/mole, density= 8.960*g/cm3);
  new G4Material("Germanium",z=32, a= 72.61*g/mole, density= 5.323*g/cm3);
  new G4Material("Silver",   z=47, a=107.87*g/mole, density= 10.50*g/cm3);
  new G4Material("Tungsten", z=74, a=183.85*g/mole, density= 19.30*g/cm3);
  new G4Material("Gold",     z=79, a=196.97*g/mole, density= 19.32*g/cm3);
  new G4Material("Lead",     z=82, a=207.19*g/mole, density= 11.35*g/cm3);

  //
  // define a material from elements.   case 1: chemical molecule
  //

  G4Material* H2O = new G4Material("Water",density= 1.000*g/cm3,ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
  H2O->GetIonisation()->SetMeanExcitationEnergy(78*eV);

  G4Material* CH = new G4Material("Plastic",density= 1.04*g/cm3,ncomponents=2);
  CH->AddElement(C, natoms=1);
  CH->AddElement(H, natoms=1);

  G4Material* NaI = new G4Material("NaI", density= 3.67*g/cm3, ncomponents=2);
  NaI->AddElement(Na, natoms=1);
  NaI->AddElement(I , natoms=1);
  NaI->GetIonisation()->SetMeanExcitationEnergy(452*eV);

  //
  // define a material from elements.   case 2: mixture by fractional mass
  //

  G4Material* Air = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);

  G4Material* Air20 = 
    new G4Material("Air20", density= 1.205*mg/cm3, ncomponents=2,
                   kStateGas, 293.*kelvin, 1.*atmosphere);
  Air20->AddElement(N, fractionmass=0.7);
  Air20->AddElement(O, fractionmass=0.3);

  //Graphite
  //
  G4Material* Graphite = 
    new G4Material("Graphite", density= 1.7*g/cm3, ncomponents=1);
  Graphite->AddElement(C, fractionmass=1.);

  //Havar
  //
  G4Element* Cr = new G4Element("Chrome", "Cr", z=25, a=  51.996*g/mole);
  G4Element* Fe = new G4Element("Iron"  , "Fe", z=26, a=  55.845*g/mole);
  G4Element* Co = new G4Element("Cobalt", "Co", z=27, a=  58.933*g/mole);
  G4Element* Ni = new G4Element("Nickel", "Ni", z=28, a=  58.693*g/mole);
  G4Element* W  = new G4Element("Tungsten","W", z=74, a= 183.850*g/mole);

  G4Material* Havar = 
    new G4Material("Havar", density= 8.3*g/cm3, ncomponents=5);
  Havar->AddElement(Cr, fractionmass=0.1785);
  Havar->AddElement(Fe, fractionmass=0.1822);
  Havar->AddElement(Co, fractionmass=0.4452);
  Havar->AddElement(Ni, fractionmass=0.1310);
  Havar->AddElement(W , fractionmass=0.0631);

  //
  // examples of gas
  //  
  new G4Material("ArgonGas", z=18, a=39.948*g/mole, density= 1.782*mg/cm3,
                 kStateGas, 273.15*kelvin, 1*atmosphere);
                           
  new G4Material("XenonGas", z=54, a=131.29*g/mole, density= 5.458*mg/cm3,
                 kStateGas, 293.15*kelvin, 1*atmosphere);
                           
  G4Material* CO2 =
    new G4Material("CarbonicGas", density= 1.977*mg/cm3, ncomponents=2);
  CO2->AddElement(C, natoms=1);
  CO2->AddElement(O, natoms=2);

  G4Material* ArCO2 =
    new G4Material("ArgonCO2",   density= 1.8223*mg/cm3, ncomponents=2);
  ArCO2->AddElement (Ar,  fractionmass=0.7844);
  ArCO2->AddMaterial(CO2, fractionmass=0.2156);

  //another way to define mixture of gas per volume
  G4Material* NewArCO2 =
    new G4Material("NewArgonCO2", density= 1.8223*mg/cm3, ncomponents=3);
  NewArCO2->AddElement (Ar, natoms=8);
  NewArCO2->AddElement (C,  natoms=2);
  NewArCO2->AddElement (O,  natoms=4);

  G4Material* ArCH4 = 
    new G4Material("ArgonCH4",    density= 1.709*mg/cm3,  ncomponents=3);
  ArCH4->AddElement (Ar, natoms=93);
  ArCH4->AddElement (C,  natoms=7);
  ArCH4->AddElement (H,  natoms=28);

  G4Material* XeCH = 
    new G4Material("XenonMethanePropane", density= 4.9196*mg/cm3, ncomponents=3,
                   kStateGas, 293.15*kelvin, 1*atmosphere);
  XeCH->AddElement (Xe, natoms=875);
  XeCH->AddElement (C,  natoms=225);
  XeCH->AddElement (H,  natoms=700);

  G4Material* steam = 
    new G4Material("WaterSteam", density= 1.0*mg/cm3, ncomponents=1);
  steam->AddMaterial(H2O, fractionmass=1.);
  steam->GetIonisation()->SetMeanExcitationEnergy(71.6*eV);  

  G4Material* rock1 = new G4Material("StandardRock",
                                     2.65*CLHEP::g/CLHEP::cm3, 1, kStateSolid);
  rock1->AddElement(Na, 1);
  
  G4Element* Al = new G4Element("Aluminium", "Al",13., 26.9815*g/mole);
  //MgO        = NISTman->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE"); 
  //MgO = new G4Material("MgO", 1.0*g/cm3, 2);
  //MgO->AddElement(O, 1);
  //MgO->AddElement(Mg, 1);

  // example of vacuum
  //
  density     = universe_mean_density;    //from PhysicalConstants.h
  pressure    = 3.e-18*pascal;
  temperature = 2.73*kelvin;
  new G4Material("Galactic", z=1, a=1.01*g/mole,density,
                 kStateGas,temperature,pressure);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ComputeCalorParameters()
{
  // Compute derived parameters of the calorimeter
  fXstartAbs = fXposAbs-0.5*fAbsorberThickness; 
  fXendAbs   = fXposAbs+0.5*fAbsorberThickness;

  G4double xmax = std::max(std::abs(fXstartAbs), std::abs(fXendAbs));
     
  // change world size by the flag or if the absorber is large 
  if (fDefaultWorld || 2*xmax >=  fWorldSizeX ||
      fAbsorberSizeYZ >= fWorldSizeYZ) 
    {
      fWorldSizeX = 15*xmax; 
      fWorldSizeYZ= 9*fAbsorberSizeYZ;
    }         
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter()
{ 
  // World
  //
  fSolidWorld = new G4Box("World",                                //its name
                   fWorldSizeX/2,fWorldSizeYZ/2,fWorldSizeYZ/2);  //its size
                         
  fLogicWorld = new G4LogicalVolume(fSolidWorld,          //its solid
                                   fWorldMaterial,        //its material
                                   "World");              //its name
                                   
  fPhysiWorld = new G4PVPlacement(0,                      //no rotation
                                 G4ThreeVector(0.,0.,0.), //at (0,0,0)
                                 fLogicWorld,             //its logical volume
                                 "World",                 //its name
                                 0,                       //its mother  volume
                                 false,                   //no boolean operation
                                 0);                      //copy number
                                 
  //cout << "MgO Density " << MgO->GetDensity()/(g/cm3) << endl;


  // dimensions
  G4double crystalX;
  G4double crystalY;
  G4double crystalZ;
  G4double canX;
  G4double canY;
  G4double canZ;
  G4double canThickness;          //LR
  G4double mgoX;                  //LR
  G4double mgoY;                  //LR
  G4double mgoZ;                  //LR
  G4double mgoThickness;          //LR

  G4double crystal3X;
  G4double crystal3Y;
  G4double crystal3Z;
  G4double can3X;
  G4double can3Y;
  G4double can3Z;
  G4double mgo3X;                 //LR
  G4double mgo3Y;                 //LR
  G4double mgo3Z;                 //LR
  
  G4double dead = 2.0*mm;
  crystalX     = 101.6*mm - 2*dead;
  crystalY     = 54.0*mm  - 2*dead;
  crystalZ     = 54.0*mm  - 2*dead;
  mgoThickness = 1.5*mm;
  mgoX         = (crystalX+2*mgoThickness + 2*dead)*mm;  //LR
  mgoY         = (crystalY+2*mgoThickness + 2*dead)*mm;  //LR
  mgoZ         = (crystalZ+2*mgoThickness + 2*dead)*mm;  //LR
  canThickness = 1.0*mm;                        //LR
  canX         = (crystalX + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  canY         = (crystalY + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  canZ         = (crystalZ + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  
  crystal3X    = 79.5*mm - 2*dead;
  crystal3Y    = 79.5*mm - 2*dead;
  crystal3Z    = 76.2*mm - 2*dead;
  mgo3X        = (crystal3X+2*mgoThickness + 2*dead)*mm;  //LR
  mgo3Y        = (crystal3Y+2*mgoThickness + 2*dead)*mm;  //LR
  mgo3Z        = (crystal3Z+2*mgoThickness + 2*dead)*mm;  //LR
  can3X        = (crystal3X + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  can3Y        = (crystal3Y + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  can3Z        = (crystal3Z + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  
  G4Material *CsI = G4NistManager::Instance()->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  G4Material *Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  MgO = G4NistManager::Instance()->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
  std::cout<<"MGO FORMULA: "<<MgO->GetName()<<"\n";
  G4Material* CanMaterial      = Al;
  std::cout<<"CAN MATERIAL: "<<Al->GetName()<<"\n";
  G4Material* CrystalMaterial = CsI;
  
  // Absorber
  outerCanAb = new G4Box("outerCanAb",		        //its name
		 canX/2,canY/2,canZ/2);      //size
  innerCanAb = new G4Box("innerCanAb",		        //its name
		 canX/2-canThickness,
		 canY/2-canThickness,
		 canZ/2);                      //size
  solidCanAb = new G4SubtractionSolid("CanAb", outerCanAb, innerCanAb,
			      G4Transform3D(G4RotationMatrix(),
					    G4ThreeVector(0.,0.,canThickness))); //LR
  logicCanAb = new G4LogicalVolume(solidCanAb,    //its solid
			   CanMaterial, //its material
			   "CanAb");      //name

// MgO Reflector

  outerMgoAb = new G4Box("outerMgoAb",		        //its name
		 mgoX/2,mgoY/2,mgoZ/2);         //size
  innerMgoAb = new G4Box("innerMgoAb",		        //its name
		 mgoX/2-mgoThickness,
		 mgoY/2-mgoThickness,
		 mgoZ/2);                   //size
  solidMgoAb = new G4SubtractionSolid("MgO", outerMgoAb, innerMgoAb,
			      G4Transform3D(G4RotationMatrix(),
					    G4ThreeVector(0.,0.,mgoThickness))); //LR
  logicMgoAb = new G4LogicalVolume(solidMgoAb,    //its solid
			   MgO,         //its material
			   "MgOAb");      //name
// Dead Layer
  outerDeadAb = new G4Box("outerDeadAb",		        //its name
		     crystalX/2 + dead,
		     crystalY/2 + dead,
		     crystalZ/2 + dead);               //size
  innerDeadAb = new G4Box("innerDeadAb",		        //its name
		     crystalX/2,
		     crystalY/2,
		     crystalZ/2);               //size
  solidDeadAb = new G4SubtractionSolid("MgO", outerDeadAb, innerDeadAb,
			      G4Transform3D(G4RotationMatrix(),
					    G4ThreeVector(0.,0.,dead))); //LR
  logicDeadAb = new G4LogicalVolume(solidDeadAb,    //its solid
			   CrystalMaterial,         //its material
			   "DeadAbLayer");      //name
  // 
  fSolidAbsorber = new G4Box("Absorber",//This is the same as crystal x        
		     	      crystalX/2,
		              crystalY/2,
		              crystalZ/2);               //size
                          
  fLogicAbsorber = new G4LogicalVolume(fSolidAbsorber,    //its solid
                                       fAbsorberMaterial, //its material
                                       "Absorber");       //its name
  fPhysiAbsorber = new G4PVPlacement(0,                   //no rotation
                        G4ThreeVector(fXposAbs,0.,0.),    //its position
                                fLogicAbsorber,     //its logical volume
                                "Absorber",         //its name
                                fLogicWorld,        //its mother
                                false,              //no boulean operat
                                0);                 //copy number
  fPhysiAbsorberCan = new G4PVPlacement(0,                   //no rotation
                        G4ThreeVector(fXposAbs,0.,0.),    //its position
                                logicCanAb,     //its logical volume
                                "CanAbsorber",         //its name
                                fLogicWorld,        //its mother
                                false,              //no boulean operat
                                0);                 //copy number
  
  fPhysiAbsorberMgo = new G4PVPlacement(0,                   //no rotation
                        G4ThreeVector(fXposAbs,0.,0.),    //its position
                                logicMgoAb,     //its logical volume
                                "MgoAbsorber",         //its name
                                fLogicWorld,        //its mother
                                false,              //no boulean operat
                                0);                 //copy number
  
  fPhysiAbsorberDead = new G4PVPlacement(0,                   //no rotation
                        G4ThreeVector(fXposAbs,0.,0.),    //its position
                                logicDeadAb,     //its logical volume
                                "DeadAbsorber",         //its name
                                fLogicWorld,        //its mother
                                false,              //no boulean operat
                                0);                 //copy number
                                        
  PrintCalorParameters();         
  

  // Dimensions
  //always return the physical World

  outerCan = new G4Box("outerCan",		        //its name
		 can3X/2,can3Y/2,can3Z/2);      //size
  innerCan = new G4Box("innerCan",		        //its name
		 can3X/2-canThickness,
		 can3Y/2-canThickness,
		 can3Z/2);                      //size
  solidCan = new G4SubtractionSolid("Can3", outerCan, innerCan,
			      G4Transform3D(G4RotationMatrix(),
					    G4ThreeVector(0.,0.,canThickness))); //LR
  logicCan = new G4LogicalVolume(solidCan,    //its solid
			   CanMaterial, //its material
			   "Can3");      //name

// MgO Reflector

  outerMgo = new G4Box("outerMgo",		        //its name
		 mgo3X/2,mgo3Y/2,mgo3Z/2);         //size
  innerMgo = new G4Box("innerMgo",		        //its name
		 mgo3X/2-mgoThickness,
		 mgo3Y/2-mgoThickness,
		 mgo3Z/2);                   //size
  solidMgo = new G4SubtractionSolid("MgO3", outerMgo, innerMgo,
			      G4Transform3D(G4RotationMatrix(),
					    G4ThreeVector(0.,0.,mgoThickness))); //LR
  logicMgo = new G4LogicalVolume(solidMgo,    //its solid
			   MgO,         //its material
			   "MgO3");      //name
// Dead Layer
  outerDead = new G4Box("outerDead",		        //its name
		     crystal3X/2 + dead,
		     crystal3Y/2 + dead,
		     crystal3Z/2 + dead);               //size
  innerDead = new G4Box("innerDead",		        //its name
		     crystal3X/2,
		     crystal3Y/2,
		     crystal3Z/2);               //size
  solidDead = new G4SubtractionSolid("MgO3", outerDead, innerDead,
			      G4Transform3D(G4RotationMatrix(),
					    G4ThreeVector(0.,0.,dead))); //LR
  logicDead = new G4LogicalVolume(solidDead,    //its solid
			   CrystalMaterial,         //its material
			   "DeadLayer");      //name
// Crystal

  solidCrystal = new G4Box("Crystal3",		        //its name
		     crystal3X/2,
		     crystal3Y/2,
		     crystal3Z/2);               //size
  logicCrystal = new G4LogicalVolume(solidCrystal,	//its solid
			       CrystalMaterial,	//its material
			       "Crystal3");	//its name

  int detID = 0;
  for (detID = 0;detID<numdet;detID++)
	  {
	  std::stringstream detName;
	  detName<<"Gammadet"<<detID;
	  G4RotationMatrix rotation;
	  rotation = G4RotationMatrix::IDENTITY;
	  double angle = 90.*deg * detID;
	  std::cout<<"ANGLE: "<<angle<<'\n';
	  rotation.rotateX(angle + 90*deg);

	 
	  double x =  (fAbsorberThickness - can3X)/2.0 + 2.54*cm;
	  double averagecan = (can3Z + can3Y)/2.0;
          double y = averagecan * cos(angle);
	  double z = averagecan * sin(angle);

	  detPos.setX(x);
          detPos.setY(y);
	  detPos.setZ(z);

	  physiCrystal = new G4PVPlacement(G4Transform3D(rotation,detPos),
					   logicCrystal,	//its logical volume
					   detName.str(),	        //its name
					   fLogicWorld,         //its mother  volume
					   false,		//no boolean operation
					   detID);		//copy number
	  physiDead = new G4PVPlacement(G4Transform3D(rotation,detPos),
					   logicDead,	//its logical volume
					   "DeadLayer",	        //its name
					   fLogicWorld,         //its mother  volume
					   false,		//no boolean operation
					   detID);		//copy number

	  physiMgo = new G4PVPlacement(G4Transform3D(rotation,detPos),
				       logicMgo,               //its logical volume
				       "MgO",                  //its name
				       fLogicWorld,            //its mother
				       false,                  //no boolean operat
				       detID);                 //copy number

	  physiCan = new G4PVPlacement(G4Transform3D(rotation,detPos),
				       logicCan,               //its logical volume
				       "Can",                  //its name
				       fLogicWorld,            //its mother
				       false,                  //no boolean operat
				       detID);                 //copy number
	 }
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintCalorParameters()
{
  G4cout << "\n" << fWorldMaterial    << G4endl;
  G4cout << "\n" << fAbsorberMaterial << G4endl;
    
  G4cout << "\n The  WORLD   is made of "  << G4BestUnit(fWorldSizeX,"Length")
         << " of " << fWorldMaterial->GetName();
  G4cout << ". The transverse size (YZ) of the world is " 
         << G4BestUnit(fWorldSizeYZ,"Length") << G4endl;
  G4cout << " The ABSORBER is made of " 
         <<G4BestUnit(fAbsorberThickness,"Length")
         << " of " << fAbsorberMaterial->GetName();
  G4cout << ". The transverse size (YZ) is " 
         << G4BestUnit(fAbsorberSizeYZ,"Length") << G4endl;
  G4cout << " X position of the middle of the absorber "
         << G4BestUnit(fXposAbs,"Length");
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
    G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial && fAbsorberMaterial != pttoMaterial) {
    fAbsorberMaterial = pttoMaterial;                  
    if(fLogicAbsorber) { fLogicAbsorber->SetMaterial(fAbsorberMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =
    G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);

  if (pttoMaterial && fWorldMaterial != pttoMaterial) {
    fWorldMaterial = pttoMaterial; 
    if(fLogicWorld) { fLogicWorld->SetMaterial(fWorldMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorberThickness(G4double val)
{
  fAbsorberThickness = val;
  ComputeCalorParameters();
  if(fPhysiWorld) { ChangeGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorberSizeYZ(G4double val)
{
  fAbsorberSizeYZ = val;
  ComputeCalorParameters();
  if(fPhysiWorld) { ChangeGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldSizeX(G4double val)
{
  fWorldSizeX = val;
  fDefaultWorld = false;
  ComputeCalorParameters();
  if(fPhysiWorld) { ChangeGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldSizeYZ(G4double val)
{
  fWorldSizeYZ = val;
  fDefaultWorld = false;
  ComputeCalorParameters();
  if(fPhysiWorld) { ChangeGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorberXpos(G4double val)
{
  if(!fPhysiWorld) { fXposAbs = val; }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void DetectorConstruction::ConstructSDandField()
{
 

  if ( fFieldMessenger.Get() == 0 ) {
    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    G4ThreeVector fieldValue = G4ThreeVector();
    G4GlobalMagFieldMessenger* msg =
      new G4GlobalMagFieldMessenger(fieldValue);
    //msg->SetVerboseLevel(1);
    G4AutoDelete::Register(msg);
    fFieldMessenger.Put( msg );        
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ChangeGeometry()
{
  /*fSolidWorld->SetXHalfLength(fWorldSizeX*0.5);
  fSolidWorld->SetYHalfLength(fWorldSizeYZ*0.5);
  fSolidWorld->SetZHalfLength(fWorldSizeYZ*0.5);

  fSolidAbsorber->SetXHalfLength(fAbsorberThickness*0.5);
  fSolidAbsorber->SetYHalfLength(fAbsorberSizeYZ*0.5);
  fSolidAbsorber->SetZHalfLength(fAbsorberSizeYZ*0.5);*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

