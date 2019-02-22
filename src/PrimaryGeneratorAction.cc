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
/// \file electromagnetic/TestEm5/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
// $Id: PrimaryGeneratorAction.cc 77583 2013-11-26 10:28:15Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "StoragePlace.hh"
#include "InnerBrems.hh"

#include "G4PhysicalConstants.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

#include<iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction*)
: G4VUserPrimaryGeneratorAction(),
fParticleGun(0)
{
    //fParticleGun = new G4GeneralParticleSource();
    fParticleGun = InitializeGPS(0);
    fParticleGun->SetMultipleVertex(true);
    fParticleGun->AddaSource(1);
    fParticleGun->SetCurrentSourceto(1);
    fParticleGun = InitializeGPS(1);
    fParticleGun->AddaSource(2);
    fParticleGun->SetCurrentSourceto(2);
    fParticleGun = InitializeGPS(2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent) ;
    G4ThreeVector pos = anEvent->GetPrimaryVertex(0)->GetPosition();
    G4ThreeVector dirbefore = anEvent->GetPrimaryVertex(1)->GetPrimary()->GetMomentum();
    double posx = pos.x();
    double posy = pos.y();
    double posz = pos.z();
    anEvent->GetPrimaryVertex(1)->SetPosition(posx,posy,posz);
    anEvent->GetPrimaryVertex(2)->SetPosition(posx,posy,posz);
    G4ThreeVector dirafter = anEvent->GetPrimaryVertex(1)->GetPrimary()->GetMomentum();
    InnerBrems brems;
    double en1 = anEvent->GetPrimaryVertex(0)->GetPrimary()->GetTotalEnergy()/keV;
    double en3 = 0;
//    while(!en3)
//	    {
 // 	    en3 = brems.ComputeGammaVal(en1);
 // 	    }
    //en3 = brems.ComputeGammaVal(en1);
    en3 = brems.GetEnergy(en1);
    double ennew = (en1 - en3)/1000.0;
   // std::cout<<"OLD: "<<en1<<std::endl;
    //std::cout<<"NEW: "<<ennew/MeV<<std::endl;
    anEvent->GetPrimaryVertex(0)->GetPrimary()->SetTotalEnergy(ennew/MeV);
    anEvent->GetPrimaryVertex(2)->GetPrimary()->SetTotalEnergy(en3/1000.0);
 //   std::cout<<"ENERGY GAMMA: "<<en3<<std::endl;
    //std::cout<<"z before: "<<dirbefore.z()<<" z after: "<<dirafter.z()<<std::endl;
    double engamma = anEvent->GetPrimaryVertex(1)->GetPrimary()->GetTotalEnergy()/MeV;
    GammaEn.energy = engamma;
    //Put the verticies at the same place.	  
}

G4GeneralParticleSource* PrimaryGeneratorAction::InitializeGPS(int parttype)
{
    G4GeneralParticleSource * gps = new G4GeneralParticleSource();
    
    // setup details easier via UI commands see muon.mac
 
    // particle type
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* electron = particleTable->FindParticle("e-");
    G4ParticleDefinition* gamma = particleTable->FindParticle("gamma");
    gps->SetCurrentSourceto(parttype);

    // set energy distribution
    if (!parttype)
	    {
	    gps->GetCurrentSource()->SetParticleDefinition(electron);
	    G4SPSEneDistribution *eneDist = gps->GetCurrentSource()->GetEneDist() ;
	    eneDist->SetEnergyDisType("Mono"); // or gauss
	    eneDist->SetMonoEnergy(1*MeV);
	    std::cout<<"ADDING ELECTRON!"<<std::endl;
	    }
    else
	    {
	    gps->GetCurrentSource()->SetParticleDefinition(gamma);
	    G4SPSEneDistribution *eneDist = gps->GetCurrentSource()->GetEneDist() ;
	    eneDist->SetEnergyDisType("Mono"); // or gauss
	    eneDist->SetMonoEnergy(1.633602*MeV);
	    std::cout<<"ADDING GAMMA!"<<std::endl;
	    }

    // degrader at 42
    /*G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
    posDist->SetPosDisType("Volume");  // or Point,Plane,Volume,Beam
    posDist->SetCentreCoords(G4ThreeVector(2.44*cm,1.8549*cm,0.0*cm));
    posDist->SetPosDisShape("Para");
    posDist->SetHalfX(2.3*mm);
    posDist->SetHalfY(0.735*mm);//
    posDist->SetHalfZ(2.25*mm);*/
    
   //42 bottom
    /*G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
    posDist->SetPosDisType("Volume");  // or Point,Plane,Volume,Beam
    posDist->SetCentreCoords(G4ThreeVector(2.44*cm,1.8549*cm,0.0*cm));
    posDist->SetPosDisShape("Para");
    posDist->SetHalfX(2.3*mm);
    posDist->SetHalfY(1*mm);//
    posDist->SetHalfZ(2.25*mm);*/
    
    //42 top
    /*G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
    posDist->SetPosDisType("Volume");  // or Point,Plane,Volume,Beam
    posDist->SetCentreCoords(G4ThreeVector(2.44*cm,1.8549*cm,0.0*cm));
    posDist->SetPosDisShape("Para");
    posDist->SetHalfX(2.3*mm);
    posDist->SetHalfY(0.5*mm);//
    posDist->SetHalfZ(2.25*mm);*/
    
    //1.8349(6651),1.8459(6451), 1.8749(6251), 1.9(60), 1.95(55), 2.00(50),2.05(45),2.10(40),2.15(35),2.2(30),2.25 (25)
    /*G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
    posDist->SetPosDisType("Volume");  // or Point,Plane,Volume,Beam
    posDist->SetCentreCoords(G4ThreeVector(2.44*cm,2.20*cm,0.0*cm));
    posDist->SetPosDisShape("Para");
    posDist->SetHalfX(2.3*mm);
    posDist->SetHalfY(0.735*mm);//
    posDist->SetHalfZ(2.25*mm);*/
    
  //  double sourcesizex = 3.6; // mm    
  //  double sourcesizey = 3.5; // mm   
  //  double sourcesizez = 0.4; 


    // For flourine 20    
    double sourcesizex = 0.4; // mm horizontal
    double sourcesizey = 3.5; // mm verticle  
    double sourcesizez = 3.6; // mm depth  

    double sourcedepth = 1.156; //cm 


    double halfsize = 9.76/2.0 + 0.2; //Size of detector in cm, including dead layer
    double sourcelocal = halfsize - sourcedepth;//In cm.
    //For helium 6
    /*double sourcesizex = 2.3; // mm horizontal
    double sourcesizey = 0.73; // mm verticle  
    double sourcesizez = 2.25; // mm depth  
*/
   
     ///degrader at 0
    G4SPSPosDistribution *posDist = gps->GetCurrentSource()->GetPosDist();
    posDist->SetPosDisType("Volume");  // or Point,Plane,Volume,Beam
    posDist->SetCentreCoords(G4ThreeVector(sourcelocal*cm,0.0*cm,0.0*cm)); //Source position. F20
   // posDist->SetCentreCoords(G4ThreeVector(2.44*cm,1.2674*cm,0.0*cm)); //Source position. He6
    posDist->SetPosDisShape("Para");
    posDist->SetHalfX(sourcesizex/2.0*mm);//y
    posDist->SetHalfY(sourcesizey/2.0*mm);//z
    posDist->SetHalfZ(sourcesizez/2.0*mm);//x

    /*posDist->SetHalfX(1.15*mm);//y
     posDist->SetHalfY(0.73*mm);//z
     posDist->SetHalfZ(1.125*mm);*/
    
    // set angular distribution
    G4SPSAngDistribution *angDist = gps->GetCurrentSource()->GetAngDist();
    angDist->SetAngDistType("iso");
    //angDist->SetParticleMomentumDirection( G4ThreeVector(0., 0., 1.) );
    
    // set energy distribution
    
    return gps;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
