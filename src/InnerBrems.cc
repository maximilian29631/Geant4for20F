#include "InnerBrems.hh"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "Randomize.hh"


double InnerBrems::ComputeGammaVal(double e_en);// e_en is in keV
	{
		TRandom3 *rand = new TRandom3();
		rand->SetSeed(0);
		double kmax = e_en;
		double rana,ranb;
		rana = RandFalt::shoot();
		ranb = RandFalt::shoot();
		double k = kmax * randa;
		double discutoff = ComputeDistribution(e_en,cutoff);
		double disatvalue = ComputeDistribution(e_en,k);
		rand->SetSeed(0);
		double verval = discutoff * randb; 
		if (verval <= disatvalue)
			{
			return k;
			}
		else 
			{
			return 0;
			}
	}


double InnerBrems::ComputeDistribution(double e_en,double k_en);
	{
	if (k_en < cutoff)
		{
		std::cout<<"BELOW CUTOFF!";
		return 0;
		}
	const double me=0.510998928 * 1000;		// Electron mass in keV/c^2;  PDG PRD86 (2012) 010001
	const double alpha=1/137.035999074;	// Fine-structure constant (ibid)
	double p_e = sqrt(e_en * e_en - me * me);
	double pi = 3.14159265358979323846; // Value of pie
	double coeff = (alpha * (p_e - k_en))/(pi * p_e * k_en);
	double val1 = ((e_en*e_en) + (e_en - k_en)*(e_en - k_en))/(e_en * (p_e - k_en))*log(e_en + p_e - 2 * k_en);
	double full = coeff*(val1 - 2);
	return full;
	}
