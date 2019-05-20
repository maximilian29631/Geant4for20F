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
/// \file electromagnetic/TestEm5/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
//
// $Id: HistoManager.cc 83921 2014-09-23 09:14:40Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("testem5")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);    // enable inactivation of histograms

  const G4int numen = 7;
  const G4String nameNtuple[] = {"absorberbeta","absorbergamma","gamma0","gamma1","gamma2","gamma3","primaryenergy"};
  int index = analysisManager->CreateNtuple("Energies","Energies");
  std::cout<<"INDEX: "<<index<<'\n';
  for (int j = 0;j<numen;j++)
	{
	index = analysisManager->CreateNtupleDColumn(nameNtuple[j]);	
//	std::cout<<"INDEX: "<<index<<'\n';
//	std::cout<<"J: "<<j<<'\n';
	}  
  analysisManager->FinishNtuple();
// Define histograms start values
  const G4int kMaxHisto = 50;
  const G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                         "10","11","12","13","14","15","16","17","18","19",
                         "20","21","22","23","24","25","26","27","28","29",
                         "30","31","32","33","34","35","36","37","38","39",
                         "40","41","42","43","44","45","46","47","48","49"
			};
                        
  const G4String title[] =
                { "energy deposit all",	                                  //0
                  "energy deposit in absorber",                           //1
                  "energy of charged secondaries at creation",            //2
                  "energy of neutral secondaries at creation",            //3
                  "energy of charged at creation (log scale)",            //4
                  "energy of neutral at creation (log scale)",            //5
                  "x_vertex of charged secondaries (all)",                //6
                  "x_vertex of charged secondaries (not absorbed)",       //7
                  "dummy","dummy",                                        //8-9
                  "(transmit, charged) : kinetic energy at exit",         //10
                  "(transmit, charged) : ener fluence: dE(MeV)/dOmega",   //11
                  "(transmit, charged) : space angle: dN/dOmega",         //12
                  "(transmit, charged) : projected angle at exit",        //13
                  "(transmit, charged) : projected position at exit",     //14
                  "(transmit, charged) : radius at exit",                 //15
                  "energy of Auger e- at creation",                       //16
                  "energy of fluorescence gamma at creation",             //17
                  "energy of Auger e- at creation (log scale)",           //18
                  "energy of fluorescence gamma at creation (log scale)", //19
                  "(transmit, neutral) : kinetic energy at exit",         //20
                  "(transmit, neutral) : ener fluence: dE(MeV)/dOmega",   //21
                  "(transmit, neutral) : space angle: dN/dOmega",         //22
                  "(transmit, neutral) : projected angle at exit",        //23
                  "dummy",						  //24
		  "energy deposit in gamma 0",				  //25
		  "energy deposit in gamma 1",				  //26
		  "energy deposit in gamma 2",				  //27
		  "energy deposit in gamma 3",				  //28
		  "dummy",      					  //29
                  "(reflect , charged) : kinetic energy at exit",         //30
                  "(reflect , charged) : ener fluence: dE(MeV)/dOmega",   //31
                  "(reflect , charged) : space angle: dN/dOmega",         //32
                  "(reflect , charged) : projected angle at exit",        //33
                  "dummy",						  //34
		  "Inner Brems Input",					  //35
		  "Inner Brems Output",					  //36
		  "dummy","dummy","dummy",  		     		  //37-39
                  "(reflect , neutral) : kinetic energy at exit",         //40
                  "(reflect , neutral) : ener fluence: dE(MeV)/dOmega",   //41
                  "(reflect , neutral) : space angle: dN/dOmega",         //42
                  "(reflect , neutral) : projected angle at exit",        //43
                  "energy of PIXE Auger e- at creation",                  //44
                  "energy of PIXE gamma at creation",                     //45
                  "energy of PIXE Auger e- at creation (log scale)",      //46
                  "energy of PIXE gamma at creation (log scale)",         //47
                  "ebem",                                                 //48
                  "source energy spectrum"				  //49                       	
		 };

  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1("h"+id[k], title[k], nbins,vmin,vmax);
    analysisManager->SetH1Activation(ih, false);
  }

  const int num2d = 8;

  const G4String title2d[] =
		{
		"gammaup2Dnogamma",
		"gammaup2Dwithgamma",
		"gammaleft2Dnogamma",
		"gammaleft2Dwithgamma",
		"gammadown2Dnogamma",
		"gammadown2Dwithgamma",
		"gammaright2Dnogamma",		
		"gammaright2Dwithgamma"		
		};


  for (G4int k = 0;k<num2d;k++)
	{
	G4int ih = analysisManager->CreateH2("h2D"+id[k], title2d[k], nbins,vmin,vmax,nbins,vmin,vmax);
	std::cout<<"NEW 2D HISTS: "<<ih<<"\n";
	analysisManager->SetH2Activation(ih, false);
	}
}
