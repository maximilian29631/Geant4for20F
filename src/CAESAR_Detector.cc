#include "CAESAR_Detector.hh"


CAESAR_Detector::CAESAR_Detector(G4LogicalVolume* experimentalHall_log, Materials* mat)
 
{
  expHall_log=experimentalHall_log;
  materials = mat;

  // define materials from the G4 NIST database
  NISTman = G4NistManager::Instance();
  CsI        = NISTman->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  Al         = NISTman->FindOrBuildMaterial("G4_Al");
  // MgO is defined in MAterials.cc so we can tune the density to match peak-to-total
  //MgO        = NISTman->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE"); 
  MgO        = materials->FindMaterial("MgO");
  //cout << "MgO Density " << MgO->GetDensity()/(g/cm3) << endl;

  vacuum     = NISTman->FindOrBuildMaterial("G4_Galactic");

  CanMaterial      = Al;
  CrystalMaterial  = CsI;

  // Dimensions
  G4double dead = 2.0*mm;
  crystalX     = 54.0*mm  - 2*dead;
  crystalY     = 54.0*mm  - 2*dead;
  crystalZ     = 101.6*mm - 2*dead;
  mgoThickness = 1.5*mm;
  mgoX         = (crystalX+2*mgoThickness + 2*dead)*mm;  //LR
  mgoY         = (crystalY+2*mgoThickness + 2*dead)*mm;  //LR
  mgoZ         = (crystalZ+2*mgoThickness + 2*dead)*mm;  //LR
  canThickness = 1.0*mm;                        //LR
  canX         = (crystalX + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  canY         = (crystalY + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  canZ         = (crystalZ + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  
  crystal3X    = 79.5*mm - dead;
  crystal3Y    = 79.5*mm - dead;
  crystal3Z    = 76.2*mm - dead;
  mgo3X        = (crystal3X+2*mgoThickness + 2*dead)*mm;  //LR
  mgo3Y        = (crystal3Y+2*mgoThickness + 2*dead)*mm;  //LR
  mgo3Z        = (crystal3Z+2*mgoThickness + 2*dead)*mm;  //LR
  can3X        = (crystal3X + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  can3Y        = (crystal3Y + 2*mgoThickness + 2*dead + 2*canThickness)*mm;
  can3Z        = (crystal3Z + 2*mgoThickness + 2*dead + 2*canThickness)*mm;

  DopplerOn();

}


CAESAR_Detector::~CAESAR_Detector()
{
}

G4VPhysicalVolume* CAESAR_Detector::Construct(G4int tF,
					      G4int ID,
					      G4double rX,
					      G4double rY,
					      G4double rZ,
					      G4double pX,
					      G4double pY,
					      G4double pZ,
					      G4int s)
{

  Stat     = s;

  typeFlag = tF;
  detID    = ID;

  G4RotationMatrix rotation;
  rotation = G4RotationMatrix::IDENTITY;
  rotation.rotateX(rX);
  rotation.rotateY(rY);
  rotation.rotateZ(rZ);

  detPos.setX(pX);
  detPos.setY(pY);
  detPos.setZ(pZ);

  // Set the ring and detector numbers
  ringNum = (int)((detID-1)/24.0);
  detNum  = (detID-1)%24;

  if(ringNum == 0){
    if(detNum > 9) {
      ringNum = 1;
      detNum -= 10;
    }
  } else if(ringNum == 7) {
    if(detNum > 13) {
      ringNum = 9;
      detNum -= 14;
    } else {
      ringNum = 8;
    }
  } else {
    ringNum += 1;
  }

  // Set the detector name
  std::ostringstream stream; 
  char ring = 65+ringNum;
  //if(ringNum < 10)
  //    stream << "0";
  stream << ring;
  if(detNum < 9)
      stream << "0";
  stream << detNum+1;

  // Count from 1 instead of 0 by convention.
  ringNum +=1;
  detNum +=1;

  detName = "CAESAR_"+stream.str();

  if(typeFlag == 2) {

    // Can
    
    outerCan = new G4Box("outerCan",		        //its name
			 canX/2,canY/2,canZ/2);         //size
    innerCan = new G4Box("innerCan",		        //its name
			 canX/2-canThickness,
			 canY/2-canThickness,
			 canZ/2);                       //size
    solidCan = new G4SubtractionSolid("Can", outerCan, innerCan,
				      G4Transform3D(G4RotationMatrix(),
						    G4ThreeVector(0.,0.,canThickness))); //LR
    logicCan = new G4LogicalVolume(solidCan,    //its solid
				   CanMaterial, //its material
				   "Can");      //name
  
    // MgO Reflector

    outerMgo = new G4Box("outerMgo",		        //its name
			 mgoX/2,mgoY/2,mgoZ/2);         //size
    innerMgo = new G4Box("innerMgo",		        //its name
			 mgoX/2-mgoThickness,
			 mgoY/2-mgoThickness,
			 mgoZ/2);                   //size
    solidMgo = new G4SubtractionSolid("MgO", outerMgo, innerMgo,
				      G4Transform3D(G4RotationMatrix(),
						    G4ThreeVector(0.,0.,mgoThickness))); //LR
    logicMgo = new G4LogicalVolume(solidMgo,    //its solid
				   MgO,         //its material
				   "MgO");      //name

    // Crystal

    solidCrystal = new G4Box("Crystal",		        //its name
			     crystalX/2,
			     crystalY/2,
			     crystalZ/2);               //size
    logicCrystal = new G4LogicalVolume(solidCrystal,	//its solid
				       CrystalMaterial,	//its material
				       "Crystal");	//its name

    //printf("Creating a 2x2x4\n");

  } else if(typeFlag ==3) {

    // Can
    
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

    // Crystal

    solidCrystal = new G4Box("Crystal3",		        //its name
			     crystal3X/2,
			     crystal3Y/2,
			     crystal3Z/2);               //size
    logicCrystal = new G4LogicalVolume(solidCrystal,	//its solid
				       CrystalMaterial,	//its material
				       "Crystal3");	//its name

    //printf("Creating a 3x3x3\n");

  } else {
    printf("\n***\nERROR ... detector type flag does not match known ");
    printf("detector type!\n");
    printf("Flag received was %i. Should be '2' or '3'\n***\n",typeFlag);
  }

  // Visualization 

  G4VisAttributes* CrystalVisAtt;
  if (Stat > 1) {
    CrystalVisAtt=new G4VisAttributes(G4Colour(0.0,1.0,0.0,1.0));
  } else {
    CrystalVisAtt=new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.0));
  }

  G4VisAttributes* mgoVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9, 1.0));
  mgoVisAtt->SetVisibility(true);
  logicMgo->SetVisAttributes(mgoVisAtt);
    
  G4VisAttributes* canVisAtt=new G4VisAttributes(G4Colour(0.76,0.76,0.76,0.2));
  //G4VisAttributes* gapVisAtt=new G4VisAttributes(G4Colour(0.0,0.0,1.0)); //LR

  canVisAtt->SetVisibility(true);
  logicCan->SetVisAttributes(canVisAtt);
  //logicCan->SetVisAttributes(G4VisAttributes::Invisible);

  //gapVisAtt->SetVisibility(true);         //LR
  //logicGap->SetVisAttributes(gapVisAtt);  //LR
  //logicGap->SetVisAttributes(G4VisAttributes::Invisible);

  CrystalVisAtt->SetVisibility(true);
  logicCrystal->SetVisAttributes(CrystalVisAtt);
  //logicCrystal->SetVisAttributes(G4VisAttributes::Invisible);

  // Place the decector components in the experiment hall.

  physiCrystal = new G4PVPlacement(G4Transform3D(rotation,detPos),
				   logicCrystal,	//its logical volume
				   detName,	        //its name
				   expHall_log,         //its mother  volume
				   false,		//no boolean operation
				   detID);		//copy number

  physiMgo = new G4PVPlacement(G4Transform3D(rotation,detPos),
			       logicMgo,               //its logical volume
			       "MgO",                  //its name
			       expHall_log,            //its mother
			       false,                  //no boolean operat
			       detID);                 //copy number

  physiCan = new G4PVPlacement(G4Transform3D(rotation,detPos),
			       logicCan,               //its logical volume
			       "Can",                  //its name
			       expHall_log,            //its mother
			       false,                  //no boolean operat
			       detID);                 //copy number
 
  // G4cout << "detID = " << detID << " ringNum = " << ringNum 
  // 	 << " detNum = " << detNum 
  // 	 << " " << detName << G4endl;

  return physiCan;

}
//---------------------------------------------------------------------
