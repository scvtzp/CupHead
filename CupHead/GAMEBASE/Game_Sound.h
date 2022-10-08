#pragma once
#include <fmod.hpp>
#include "Game_Io.h"
#include <map>
#include "Game_Mgr.h"

// fmod_vc.lib 릴리즈 모드때 사용
// fmodL_vc.lib 디버그 모드일때 사용

#define TEST

#ifdef TEST 
#else
#endif

#ifdef X32
#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif
#else // 64비트
#ifdef _DEBUG // 디버그 모드
#pragma comment(lib, "fmodL_vc.lib")
#else // 릴리즈 모드
#pragma comment(lib, "fmod_vc.lib")
#endif
#endif

class Game_String;
class Game_File;
class Game_Sound : public HFILEMGR<Game_Sound>
{
private:
	static FMOD::System* m_pSoundSys;

	// 실제적으로 FMOD의 사운재생은
	// FMOD::Channel* 재생정보이다.
	// 재생정보가 필요하다.
	// 사운드 재생에 대해서 이것저것 참견하고 싶은 녀석은
	// 채널을 받아서 그 채널을 통해서 참견해야 한다.

	static std::map<Game_String, FMOD::Channel*> m_pAllPlaySound;

	class Game_Start
	{
	public:
		Game_Start();
		~Game_Start();
	};
	// 배웠던 문법의 응용이다.
	static Game_Start Starter;

public:
	static void Init();
	static void Update();

	static void Load(const Game_File& _File);
	// 한번 재생하고 

	// "BulletSound.mp3"
	static void Play(const Game_String& _FileName);

	// "BGM"이라는 이름으로 "BackgroundSound.mp3"
	static void Play(const Game_String& _PlayName, const Game_String& _FileName);
	// 재생되고 난 다음
	// "BGM" 멈춰
	static void Stop(const Game_String& _Name);
	// "BGM" 재생중이야?
	static bool IsPlay(const Game_String& _Name);

	// 맴버변수
private:
	FMOD::Sound* m_pSound;

private:
	void Load();

public:
	Game_Sound() : m_pSound(nullptr)
	{

	}

};