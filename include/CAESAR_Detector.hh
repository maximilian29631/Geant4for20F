#ifndef CAESAR_Detector_H
#define CAESAR_Detector_H 1

#include "Materials.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"  //LR
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4PVDivision.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

#include "Randomize.hh"
#include "globals.hh"

#include <iostream>
#include <iomanip>

using namespace std;

class CAESAR_Detector 
{
public:

  CAESAR_Detector(G4LogicalVolume*, Materials*);
  ~CAESAR_Detector();

  G4VPhysicalVolume *Construct(G4int, G4int,
			       G4double, G4double, G4double, 
			       G4double, G4double, G4double, 
			       G4int);
  G4LogicalVolume* GetCrystalLog(){return logicCrystal;};
  void    DopplerOn(){dopplerOn=true;}
  void    DopplerOff(){dopplerOn=false;}
  G4int   getID(){return detID;}
  G4int   getRingNum(){return ringNum;}
  G4int   getNum(){return detNum;}
  G4int   getStat(){return Stat;}
  G4int   getType(){return typeFlag;}
  G4bool  getDoppler(){return dopplerOn;}
  G4ThreeVector getPos(){return detPos;}
  G4String getDetName(G4int);

private:

  G4LogicalVolume *expHall_log;
  Materials* materials;
  G4NistManager* NISTman;
  
  // Logical volumes
  G4LogicalVolume* logicCan;
  G4LogicalVolume* logicMgo;      //LR
  G4LogicalVolume* logicCrystal;

  // Physical volumes
  G4VPhysicalVolume* physiCan;
  G4VPhysicalVolume* physiMgo;
  G4VPhysicalVolume* physiCrystal;

  // Materials
  G4Material* CsI;
  G4Material* Al;
  G4Material* MgO;
  G4Material* vacuum;

  G4Material* CanMaterial;
  G4Material* CrystalMaterial;

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

  // Solids
  G4Box* outerCan;                //LR
  G4Box* innerCan;                //LR
  G4SubtractionSolid* solidCan;   //LR
  G4Box* outerMgo;                //LR
  G4Box* innerMgo;                //LR
  G4SubtractionSolid* solidMgo;   //LR
  G4Box* solidCrystal;

  G4ThreeVector detPos;

  G4int    detID;    // ID number (Sequential, starting with ring 1, det 1)
  G4int    ringNum;  // Ring (1...10)
  G4int    detNum;   // Number (1 ...) within ring
  G4int    typeFlag; // Detector type (2 = 2x2x4, 3 = 3x3x3)
  G4int    Stat;     // Detector status (2 = on, 1 = present, 0 = absent)
  G4String detName;  // Ring(01 ... 10)Number(01 ...) = (0101, 0102, ..., 1010)

  G4bool dopplerOn;

};

#endif

