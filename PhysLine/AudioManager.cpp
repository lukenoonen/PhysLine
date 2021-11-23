#include "AudioManager.h"
#include "RenderManager.h"

bool CC_Aud_Play( const CLinkedList<const char *> &command )
{
	if (command.Count() < 1)
		return false;

	CIterator<const char *> iterator = command.StartIterating();

	const char *sAudioFile = iterator.Iterate();

	int iLoops = 0;
	double dVolume = 0.5;
	int iChannel = -1;

	if (iterator.Iterating())
		iLoops = UTIL_atoi( iterator.Iterate() );

	if (iterator.Iterating())
		dVolume = UTIL_atod( iterator.Iterate() );

	if (iterator.Iterating())
		iChannel = UTIL_atoi( iterator.Iterate() );

	int iAudioIndex = pAudioManager->LoadAudio( sAudioFile );
	if (iAudioIndex == -1)
		return false;

	pAudioManager->PlayAudio( iAudioIndex, iLoops, dVolume, iChannel );
	return true;
}
CConCommand cc_aud_play( "aud_play", CC_Aud_Play, "aud_play [audio path] (loops) (volume) (channel)" );

bool CC_Aud_Pause( const CLinkedList<const char *> &command )
{
	pAudioManager->PauseAudio( (command.Count() < 1) ? -1 : UTIL_atoi( command.GetHead() ) );
	return true;
}
CConCommand cc_aud_pause( "aud_pause", CC_Aud_Pause, "aud_pause (channel)" );

bool CC_Aud_Resume( const CLinkedList<const char *> &command )
{
	pAudioManager->ResumeAudio( (command.Count() < 1) ? -1 : UTIL_atoi( command.GetHead() ) );
	return true;
}
CConCommand cc_aud_resume( "aud_resume", CC_Aud_Resume, "aud_resume (channel)" );

bool CC_Aud_Stop( const CLinkedList<const char *> &command )
{
	pAudioManager->StopAudio( (command.Count() < 1) ? -1 : UTIL_atoi( command.GetHead() ) );
	return true;
}
CConCommand cc_aud_stop( "aud_stop", CC_Aud_Stop, "aud_stop (channel)" );

CAudioManager::CAudioManager()
{
	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 );
	Mix_AllocateChannels( 128 );

	for (int i = 0; i < MAX_AUDIOS; i++)
	{
		m_pAudios[i] = NULL;
		m_SoundNames[i] = NULL;
	}

	m_iAudioCount = 0;
}

CAudioManager::~CAudioManager()
{
	Reset();
	Mix_Quit();
}

void CAudioManager::Reset( void )
{
	StopAudio();

	for (int i = 0; i < m_iAudioCount; i++)
	{
		if (m_pAudios[i])
		{
			Mix_FreeChunk( m_pAudios[i] );
			m_pAudios[i] = NULL;
		}

		if (m_SoundNames[i])
		{
			delete m_SoundNames[i];
			m_SoundNames[i] = NULL;
		}
	}

	m_iAudioCount = 0;
}

int CAudioManager::LoadAudio( const char *sFileName )
{
	if (m_iAudioCount >= MAX_AUDIOS)
		return -1;

	for (int i = 0; i < m_iAudioCount; i++)
	{
		if (UTIL_strcmp( m_SoundNames[i], sFileName ) == 0)
			return i;
	}

	m_pAudios[m_iAudioCount] = Mix_LoadWAV( sFileName );

	if (!m_pAudios[m_iAudioCount])
		return -1;

	m_SoundNames[m_iAudioCount] = UTIL_stredit( sFileName );

	return m_iAudioCount++;
}

int CAudioManager::PlayAudio( int iAudioIndex, int iLoops /*= 0*/, double dVolume /*= 0.5*/, int iChannel /*= -1*/ )
{
	if (iAudioIndex < 0 || iAudioIndex >= m_iAudioCount)
		return -1;

	iChannel = Mix_PlayChannel( iChannel, m_pAudios[iAudioIndex], iLoops );
	AudioVolume( dVolume, iChannel );
	// AudioPanning( 0.0, iChannel );
	return iChannel;
}

int CAudioManager::PlayAudio( const Vector &vecPosition, int iAudioIndex, double dVolume /*= 0.5*/, int iChannel /*= -1*/ )
{
	const Vector &vecCameraPosition = pRenderManager->GetScreenPosition();
	double dXDelta = (vecPosition.x - vecCameraPosition.x) / 10.0;
	AudioPanning( UTIL_Clamp( dXDelta, -1.0, 1.0 ), iChannel );
	return PlayAudio( iAudioIndex, 0, dVolume / (UTIL_Dist( vecCameraPosition, vecPosition ) + 1.0), iChannel );
}

void CAudioManager::AudioVolume( double dVolume, int iChannel /*= -1*/ )
{
	Mix_Volume( iChannel, (int)(dVolume * MIX_MAX_VOLUME) );
}

void CAudioManager::AudioPanning( double dPanning, int iChannel /*= -1*/ )
{
	Mix_SetPanning( iChannel, 127 - (int)(127 * dPanning), 127 + (int)(127 * dPanning) );
	Mix_SetPanning( iChannel, 255, 0 );
}

void CAudioManager::PauseAudio( int iChannel /*= -1*/ )
{
	Mix_Pause( iChannel );
}

void CAudioManager::ResumeAudio( int iChannel /*= -1*/ )
{
	Mix_Resume( iChannel );
}

void CAudioManager::StopAudio( int iChannel /*= -1*/ )
{
	Mix_HaltChannel( iChannel );
}