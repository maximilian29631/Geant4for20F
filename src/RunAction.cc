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
/// \file electromagnetic/TestEm5/src/RunAction.cc
/// \brief Implementation of the RunAction class
//
// $Id: RunAction.cc 78394 2013-12-16 16:35:20Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "Run.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4MPImanager.hh"
#include <iomanip>
#include "G4GeneralParticleSource.hh"
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
:G4UserRunAction(),fDetector(det), fPrimary(kin), fRun(0), fHistoManager(0)
{ 
  // Book predefined histograms
  fHistoManager = new HistoManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ 
  delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector);
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // save Rndm status
  ////  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  if (isMaster) G4Random::showEngineStatus();
     
  // keep run condition
  //Put them at same starting pos?
  if ( fPrimary ) {
   // std::cout<<"RUN START! "<<std::endl;
    fPrimary->GetGeneralParticleSource()->SetCurrentSourceto(0);
    G4ParticleDefinition* particle 
    = fPrimary->GetGeneralParticleSource()->GetParticleDefinition();
    G4double energy = fPrimary->GetGeneralParticleSource()->GetParticleEnergy();
    G4ThreeVector Pos = fPrimary->GetGeneralParticleSource()->GetParticlePosition();
    std::cout<<"ENERGY 1:"<<energy<<std::endl;

    fPrimary->GetGeneralParticleSource()->SetCurrentSourceto(1);
      
    G4ParticleDefinition* particle2 
    = fPrimary->GetGeneralParticleSource()->GetParticleDefinition();
    G4double energy2 = fPrimary->GetGeneralParticleSource()->GetParticleEnergy();
    
    fPrimary->GetGeneralParticleSource()->SetCurrentSourceto(2);
      
    G4ParticleDefinition* particle3 
    = fPrimary->GetGeneralParticleSource()->GetParticleDefinition();
    G4double energy3 = fPrimary->GetGeneralParticleSource()->GetParticleEnergy();
    //fRun->SetPrimary(particle, energy);
    fRun->SetPrimaries(particle,energy,particle2,energy2,particle3,energy3);

  }
  
  //histograms
  //        

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int rank = G4MPImanager::GetManager()->GetRank();
  G4String filename = analysisManager->GetFileName();
  std::stringstream newfilename;
  newfilename<<filename<<"_t"<<rank;

  analysisManager->SetFileName(newfilename.str());

  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{  
  // print Run summary
  //
  if (isMaster) fRun->EndOfRun();    
      
  // save histograms
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();  
  if ( analysisManager->IsActive() ) {    
    analysisManager->Write();
    analysisManager->CloseFile();
  }  

  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
