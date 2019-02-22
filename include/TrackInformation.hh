#ifndef TrackInformation_h
#define TrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class TrackInformation : public G4VUserTrackInformation 
	{
	  public:
		TrackInformation();
		TrackInformation(const G4Track* aTrack);
		TrackInformation(const TrackInformation* aTrackInfo);
		virtual ~TrackInformation();
		     
		inline void *operator new(size_t);
		inline void operator delete(void *aTrackInfo);
	
		TrackInformation& operator =(const TrackInformation& right);

		void SetTrackInformation(const G4Track *aTrack);
		void Print() const;

	private:
		G4int                 originalTrackID;
		G4ParticleDefinition* particleDefinition;
		G4ThreeVector         originalPosition;
		G4ThreeVector         originalMomentum;
		G4double              originalEnergy;
		G4double              originalTime;

	public:
		inline G4int GetOriginalTrackID() const {return originalTrackID;}
		inline G4ParticleDefinition* GetOriginalParticle() const {return particleDefinition;}
		inline G4ThreeVector GetOriginalPosition() const {return originalPosition;}
		inline G4ThreeVector GetOriginalMomentum() const {return originalMomentum;}
		inline G4double GetOriginalEnergy() const {return originalEnergy;}
		inline G4double GetOriginalTime() const {return originalTime;}
};

extern G4ThreadLocal
	G4Allocator<TrackInformation> * aTrackInformationAllocator;

inline void* TrackInformation::operator new(size_t)
	{
	if (!aTrackInformationAllocator)
		aTrackInformationAllocator = new G4Allocator<TrackInformation>;
	return (void*) aTrackInformationAllocator->MallocSingle();
	}

inline void TrackInformation::operator delete(void *aTrackInfo)
	{ aTrackInformationAllocator->FreeSingle((TrackInformation*)aTrackInfo);}

#endif

