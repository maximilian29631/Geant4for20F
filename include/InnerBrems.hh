#ifndef InnerBrems_H
#define InnerBrems_H 1

class InnerBrems 
	{
	private:
		double cutoff = 50.0; // Energy in keV
		double ComputeTotalProb(double e_en); //This does the numerical integration
		double ComputeDistribution(double e_en,double k_en);

	public:
		double ComputeGammaVal(double e_en);
		double GetEnergy(double e_en);
		inline double GetCutoff(void) {return cutoff;};
		inline void SetCutoff(double input) {
			cutoff = input;
			};
		};

#endif
