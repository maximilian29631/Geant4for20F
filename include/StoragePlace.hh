#include <vector>

struct storageplace
	{
  	  std::vector <int> 	  fBetaIDs;
   	  std::vector <int> 	  fGammaIDs;

	  void clear(void)
		  {
			fBetaIDs.clear();
			fGammaIDs.clear();
		  	fBetaIDs.shrink_to_fit();
			fGammaIDs.shrink_to_fit(); //Make it small again
		  }
	};

static thread_local storageplace StoreIDs;
