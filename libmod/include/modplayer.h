// modplayer.h: headers for psp modplayer code
//
// All public functions for modplayer
//
//////////////////////////////////////////////////////////////////////
#ifndef _MODPLAYER_GENERAL_H
#define _MODPLAYER_GENERAL_H

#ifdef __cplusplus
extern "C" {
#endif
	
static int modplayint_channel;
typedef void (*fd_initFunc) (int);
typedef int (*fd_loadFunc) (char *);
typedef int (*fd_playFunc) (void);
typedef void (*fd_pauseFunc) (void);
typedef int (*fd_stopFunc) (void);
typedef void (*fd_endFunc) (void);
typedef void (*fd_tickFunc) (void);
typedef void (*fd_timeFunc) (char *);
typedef int (*fd_EOSFunc) (void);
/* Define printf, just to make typing easier */
//#define printf  pspDebugScreenPrintf
typedef struct {
    fd_initFunc init;
    fd_loadFunc load;
    fd_playFunc play;
    fd_pauseFunc pause;
    fd_stopFunc stop;
    fd_endFunc end;
    fd_tickFunc tick;
    fd_timeFunc time;
    fd_EOSFunc eos;		// have we reached end of playback.. ie for non-loop mode
    char extension[200];	// 4 byte entries. eg   mp3\0  or \0\0\0\0 to terminate list
} codecStubs;




//  Function prototypes for public functions
    void MODsetStubs(codecStubs * stubs);

//  Function prototypes for private functions
    void Mod_Init(int channel);
    int Mod_Play();
    void Mod_Pause();
    int Mod_Load_From_SandBox(char *filename);
    int Mod_Stop();
    void Mod_End();
    int Mod_Load(char *filename);
    void Mod_Tick();
    void Mod_Close();
    int Mod_EndOfStream();

    void Mod_GetTimeString(char *);

#ifdef __cplusplus
}
#endif
#endif
