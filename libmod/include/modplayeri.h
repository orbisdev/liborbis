// modplayeri.h: headers for psp modplayer code
//      
// NOTE: this is only for including in modplayer.c, use 'modplayer.h'
//       for including the public funcs in your own code
//
//////////////////////////////////////////////////////////////////////

#ifndef _MODPLAYER_H
#define _MODPLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

// These next few lines determine how the sound will be mixed. 
// Set PLAYBACK_FREQ to whatever you want. 
#define PLAYBACK_FREQ 48000

// OVERSAMPLE can be commented out to disable that function 
// (takes up less CPU time, but doesnt sound as good). 
#define OVERSAMPLE

// The NoteData structure stores the information for a single note and/or effect.
    typedef struct {
	int sample_num;		// The sample number (ie "instrument") to play, 1->31
	int period_index;	// This effectively stores the frequency to play the 
	// sample, although it's actually an index into 
	// PeriodTable.
	int effect;		// Contains the effect code
	int effect_parms;	// Used to store control parameters for the 
	// various effects
    } NoteData;

// RowData stores all the information for a single row. If there are 8 tracks
// in this mod then this structure will be filled with 8 elements of NoteData,
// one for each track.
    typedef struct {
	int numnotes;
	NoteData *note;
    } RowData;

// Pattern contains all the information for a single pattern. 
// It is filled with 64 elements of type RowData.
    typedef struct {
	int numrows;
	RowData *row;
    } Pattern;

// The Sample structure is used to store all the information for a single
// sample, or "instrument". Most of the member's should be self-explanatory.
// The "data" member is an array of bytes containing the raw sample data
// in 8-bit signed format.
    typedef struct {
	char szName[100];
	int nLength, nFineTune, nVolume, nLoopStart, nLoopLength, nLoopEnd;
	int data_length;
	char *data;
    } Sample;


// TrackData is used to store ongoing information about a particular track.
    typedef struct {
	int sample;		// The current sample being played (0 for none)
	int pos;		// The current playback position in the sample,
	// stored in fixed-point format
	int period_index;	// Which note to play, stored as an index into the 
	// PeriodTable array
	int period;		// The period number that period_index corresponds to 
	// (needed for various effects)
	float freq;		// This is the actual frequency used to do the mixing. 
	// It's a combination of the value calculated from the 
	// period member and an "adjustment" made by various effects.
	int volume;		// Volume that this track is to be mixed at
	int mixvol;		// This is the actual volume used to do the mixing. 
	// It's a combination of the volume member and an 
	// "adjustment" made by various effects.
	int porta;		// Used by the porta effect, this stores the note we're 
	// porta-ing (?) to
	int portasp;		// The speed at which to porta
	int vibspe;		// Vibrato speed
	int vibdep;		// Vibrato depth
	int tremspe;		// Tremolo speed
	int tremdep;		// Tremolo depth
	int panval;		// Pan value....this player doesn't actually do panning,
	// so this member is ignored (but included for future use)
	int sinepos;		// These next two values are pointers to the sine table. 
	// They're used to do
	int sineneg;		// various effects.
    } TrackData;

#ifdef __cplusplus
}
#endif
#endif
