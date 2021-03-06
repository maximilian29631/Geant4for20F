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
/// \file electromagnetic/TestEm5/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// $Id: SteppingAction.cc 91972 2015-08-12 13:48:40Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "TrackInformation.hh"
#include "StoragePlace.hh"

#include "G4PrimaryParticle.hh"
#include "G4Step.hh"

#include <sstream>
#include <algorithm>
#include <vector>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* DET,
                               EventAction* EA)
:G4UserSteppingAction(),fDetector(DET), fEventAction(EA)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume() 
  //  != fDetector->GetAbsorber()) return;
  //Only absorber    
  fEventAction->AddEnergy (aStep->GetTotalEnergyDeposit());
  G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
  TrackInformation *info  = (TrackInformation*)aStep->GetTrack()->GetUserInformation();
  G4double primcharge = info->GetOriginalParticle()->GetPDGCharge();
  G4double primen = info->GetOriginalEnergy();
  double gammaen = 1.6336;
  if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume() == fDetector->GetAbsorber())
		{
			if (primcharge || ((primen != gammaen) && (primcharge == 0)))//Primary paticle
				{	
					fEventAction->AddEnergyAbsorberBeta (aStep->GetTotalEnergyDeposit());
				//	std::cout<<"TRACK BETA: "<<trackid<<std::endl;
				}
			else
				{
					fEventAction->AddEnergyAbsorberGamma (aStep->GetTotalEnergyDeposit());
				//	std::cout<<"TRACK GAMMA: "<<trackid<<std::endl;
				}	
		}
  else 
	{
	if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() != "World")
		{
			for (int w = 0;w<4;w++)
				{
				std::stringstream name;
				name<<"Gammadet"<<w;
				if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == name.str().c_str())
					{
					fEventAction->AddEnergyGamma(aStep->GetTotalEnergyDeposit(),w);//Adding the enrgy for the histograms.
					}
				}
		}
	}
  if (charge != 0.) { 
    fEventAction->AddTrakLenCharg(aStep->GetStepLength());
    fEventAction->CountStepsCharg();
  } else {
    fEventAction->AddTrakLenNeutr(aStep->GetStepLength());
    fEventAction->CountStepsNeutr();

    /*static G4bool first = true;
  if (first) {
    first = false;
    G4int entries = G4PhysicsModelCatalog::Entries();
    for (G4int i = 0; i < entries; ++i) {
      G4cout << i << ": " << G4PhysicsModelCatalog::GetModelName(i) << G4endl;
    }
  }*/
 
  }
  //delete info;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

