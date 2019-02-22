#include "InnerBrems.hh"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "Randomize.hh" //Random number generate
#include <stdio.h>

InnerBrems brems;

double InnerBrems::ComputeDistribution(double e_en,double k_en) //e_en is the electron energy in keV, k_en the photon energy
	{ //This computes the distribution.
	  //I fixed the unit issue in this verious
	if (k_en < cutoff) // I define my cutoff as 2 keV in the .hh file.
		{
		std::cout<<"BELOW CUTOFF!";
		return 0;
		}
	const double me=0.510998928 * 1000;		// Electron mass in keV/c^2;  PDG PRD86 (2012) 010001
	const double alpha=1/137.035999074;	// Fine-structure constant (ibid)
	double k_en_unitless = k_en/me;//Making the energy unitless
	double e_en_unitless = e_en/me;
	double e_p_unitless = sqrt(e_en * e_en - me * me)/me; //Momentum of the electron
	double e_p_after_unitless = sqrt(e_p_unitless * e_p_unitless + k_en_unitless * k_en_unitless - 2 * k_en_unitless * e_en_unitless);
	double pi = 3.14159265358979323846; // Value of pi
	double coeff = (alpha * (e_p_after_unitless))/(pi * e_p_unitless * k_en_unitless);
//	std::cout<<"COEFF: "<<coeff<<std::endl;
	double val1 = ((e_en_unitless*e_en_unitless) + (e_en_unitless - k_en_unitless)*(e_en_unitless - k_en_unitless))/(e_en_unitless * (e_p_after_unitless))*log(e_en_unitless + e_p_after_unitless - k_en_unitless);
	double full = coeff*(val1 - 2);
	//std::cout<<"FULL: "<<full<<std::endl;
	return full;
	}


double InnerBrems::ComputeGammaVal(double e_en)// e_en is in keV
	{ // This computes the gamma value from the distrubition
		double kmax = e_en; // The maximum energy of gamma ray 
		double randa,randb;
		randa = G4RandFlat::shoot(); //This command gives me a random number between 0 and 1.
		randb = G4RandFlat::shoot();
		double k = (kmax  - cutoff)* randa + cutoff; //This picks the gamma energy I will test against distribution. I pick a random energy between the cutoff and the maximum energy
		double discutoff = ComputeDistribution(e_en,cutoff); //The maximuma value of the distribution (at the cutoff energy)
		double disatvalue = ComputeDistribution(e_en,k); //The value at my test gamma energy
		double verval = discutoff * randb; 
		if (verval <= disatvalue)
			{
			return k; //I generate a gamma of energy k (in keV)
			}
		else 
			{
			return 0;
			}
	}


double InnerBrems::ComputeTotalProb(double e_en) //This does the numerical integration
	{
	const double me=0.510998928 * 1000;		// Electron mass in keV/c^2;  PDG PRD86 (2012) 010001
	double lowval = cutoff;
	double numsteps = 1e3;
	long double totalprob = 0;
	double deltaE = (e_en - cutoff)/numsteps;
	for (int k = 0;k<numsteps;k++)// Here I'm using the trapazoidal rule. I think 1e5 steps should be enough
		{
		double xvall = deltaE * k + lowval;
		double xvalh = deltaE * (k + 1) + lowval;
		if (xvalh > e_en)// Check that I'm not above the max 
			{	
		//	std::cout<<"ABOVE VAL!\n";
			break;
			}
		double a = ComputeDistribution(e_en,xvall);
		double b = ComputeDistribution(e_en,xvalh);
		if (std::isnan(a) || std::isnan(b)) continue;
		double smallval = (b + a)/2.0 * deltaE/me; // I devide by me since the function I integrate is unitless. The x-val should be too. 
		if (smallval > 1) std::cout<<"ERROR: "<<smallval<<"!\n";
		totalprob += smallval; // Adding up probability bins
		}
	return totalprob;
	}


double InnerBrems::GetEnergy(double e_en)
	{
	double rand;
	rand = G4RandFlat::shoot();
	double prob = ComputeTotalProb(e_en);
	//std::cout<<"PROB: "<<prob<<std::endl;
	if (prob > 1) std::cout<<"SOMETHING SCREWY!\n";
	if (rand > prob)
		{
		return 0;
		}// If I fail the probality, return zero.	
	double GammaEn = 0;
	while (!GammaEn)
		{
		GammaEn = ComputeGammaVal(e_en); // Compute a gamma value that isn't zero
		} 
	return GammaEn;
	}

