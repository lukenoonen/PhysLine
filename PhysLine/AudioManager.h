#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "Global.h"
#include "SDL_mixer.h"

#define MAX_AUDIOS 4096

class CAudioManager
{
public:
	DECLARE_CLASS_NOBASE( CAudioManager );

	CAudioManager();
	~CAudioManager();

	void Reset( void );

	int LoadAudio( const char *sFileName );
	int PlayAudio( int iAudioIndex, int iLoops = 0, double dVolume = 0.5, int iChannel = -1 );
	int PlayAudio( const Vector &vecPosition, int iAudioIndex, double dVolume = 0.5, int iChannel = -1 );

	void AudioVolume( double dVolume, int iChannel = -1 );
	void AudioPanning( double dPanning, int iChannel = -1 );

	void PauseAudio( int iChannel = -1 );
	void ResumeAudio( int iChannel = -1 );
	void StopAudio( int iChannel = -1 );

private:
	Mix_Chunk *m_pAudios[MAX_AUDIOS];
	int m_iAudioCount;

	char *m_SoundNames[MAX_AUDIOS];
};

#endif // AUDIOMANAGER_H