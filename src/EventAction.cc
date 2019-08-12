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
/// \file electromagnetic/TestEm5/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
// $Id: EventAction.cc 95802 2016-02-25 11:00:43Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "Run.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
:G4UserEventAction(),
 fEnergyDeposit(0.),
 fTrakLenCharged(0.), fTrakLenNeutral(0.),
 fNbStepsCharged(0), fNbStepsNeutral(0),
 fTransmitFlag(0), fReflectFlag(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* )
{
 // initialisation per event
 fEnergyDeposit  = 0.;
 fEnergyDepositAbsorberBeta  = 0.;
 fEnergyDepositAbsorberGamma  = 0.;
 for (int w = 0;w<numgamma;w++)
	{
	fEnergyDepositGamma[w]  = 0.;
 	}
 fTrakLenCharged = fTrakLenNeutral = 0.; 
 fNbStepsCharged = fNbStepsNeutral = 0;
 fTransmitFlag   = fReflectFlag    = 0;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//std::ofstream outdep("edeposite.txt");
//std::ofstream outsource("esource.txt");

void EventAction::EndOfEventAction(const G4Event* evt)
{
 Run* run = static_cast<Run*>(
             G4RunManager::GetRunManager()->GetNonConstCurrentRun());
              
 run->AddEnergy(fEnergyDeposit);
 run->AddTrakLenCharg(fTrakLenCharged);
 run->AddTrakLenNeutr(fTrakLenNeutral);

 run->CountStepsCharg(fNbStepsCharged);
 run->CountStepsNeutr(fNbStepsNeutral);

 run->CountTransmit (fTransmitFlag);
 run->CountReflect  (fReflectFlag);
 
 //if (fEnergyDeposit > 0.)
 //   G4AnalysisManager::Instance()->FillH1(1,fEnergyDeposit);

    
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    G4PrimaryVertex* primaryVertex = evt->GetPrimaryVertex();
    G4PrimaryParticle* primaryParticle = primaryVertex->GetPrimary();
    G4double ke = primaryParticle->GetKineticEnergy();
    
    G4PrimaryVertex* primaryVertex3 = evt->GetPrimaryVertex(2);
    G4PrimaryParticle* primaryParticle3 = primaryVertex3->GetPrimary();
    G4double ke3 = primaryParticle3->GetKineticEnergy();

    analysisManager->FillH1(0, fEnergyDeposit/MeV);   
    analysisManager->FillH1(1, (fEnergyDepositAbsorberGamma + fEnergyDepositAbsorberBeta)/MeV);   
    if(ke3) analysisManager->FillH1(35, ke3/MeV); 
    analysisManager->FillH1(49, (ke+ke3)/MeV);   

//    analysisManager->FillNtupleDColumn(0,fEnergyDepositAbsorberBeta/MeV);
//    analysisManager->FillNtupleDColumn(1,fEnergyDepositAbsorberGamma/MeV);
//    analysisManager->FillNtupleDColumn(6,(ke+ke3)/MeV);  
    bool isnonzero = false;//Is there anything deposited in the gamma detectors
    bool onlyone = true;
    
    for (int w = 0;w<numgamma;w++)
	{
	int histonum = 25 + w;
	if (fEnergyDepositGamma[w]) 
		{
		if (isnonzero) onlyone = false; //Want only one.
		isnonzero = true;
		analysisManager->FillH1(histonum,fEnergyDepositGamma[w]/MeV);
		}
	//analysisManager->FillNtupleDColumn(w + 2,fEnergyDepositGamma[w]/MeV);
	}//Loop through once, and see if only one did fire.

   for (int w = 0;w<numgamma;w++)
	{
	int hist2dnum = 2*w;//I have two hists to fill now
	if (fEnergyDepositGamma[w] && onlyone)
		{
		if (fEnergyDepositAbsorberGamma)
			{
			analysisManager->FillH2(hist2dnum+1,fEnergyDepositGamma[w]/MeV,(fEnergyDepositAbsorberGamma + fEnergyDepositAbsorberBeta)/MeV);
			}
		else
			{
			analysisManager->FillH2(hist2dnum,fEnergyDepositGamma[w]/MeV,(fEnergyDepositAbsorberGamma + fEnergyDepositAbsorberBeta)/MeV);
			}
		} 
	}
   //if (isnonzero && onlyone)  analysisManager->AddNtupleRow();
   //analysisManager->AddNtupleRow();
    //outdep<<fEnergyDeposit<<G4endl;
    //outsource<<ke<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

