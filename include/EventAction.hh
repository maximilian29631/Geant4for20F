///
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
/// \file electromagnetic/TestEm5/include/EventAction.hh
/// \brief Definition of the EventAction class
//
// $Id: EventAction.hh 76464 2013-11-11 10:22:56Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Where is this?
#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
   ~EventAction();

  public:

    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    
    void AddEnergy      (G4double edep)   {fEnergyDeposit  += edep;};
    void AddEnergyAbsorberBeta      (G4double edep)   {fEnergyDepositAbsorberBeta  += edep;};
    void AddEnergyAbsorberGamma      (G4double edep)   {fEnergyDepositAbsorberGamma  += edep;};
    void AddEnergyGamma         (G4double edep, int num)   {if (num< numgamma) fEnergyDepositGamma[num]  += edep;};
    void AddTrakLenCharg(G4double length) {fTrakLenCharged += length;};
    void AddTrakLenNeutr(G4double length) {fTrakLenNeutral += length;};
    
    void CountStepsCharg ()               {fNbStepsCharged++ ;};
    void CountStepsNeutr ()               {fNbStepsNeutral++ ;};
    
    void SetTransmitFlag (G4int flag) 
                           {if (flag > fTransmitFlag) fTransmitFlag = flag;};
    void SetReflectFlag  (G4int flag) 
                           {if (flag > fReflectFlag)   fReflectFlag = flag;};
                                             
        
  private:
    const static int numgamma = 4;
    G4double fEnergyDeposit;
    G4double fEnergyDepositAbsorberGamma;
    G4double fEnergyDepositAbsorberBeta;
    G4double fEnergyDepositGamma[numgamma];
    G4double fTrakLenCharged, fTrakLenNeutral;
    G4int    fNbStepsCharged, fNbStepsNeutral;
    G4int    fTransmitFlag,   fReflectFlag;        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
