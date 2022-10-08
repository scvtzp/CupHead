#pragma once
#include <fmod.hpp>
#include "Game_Io.h"
#include <map>
#include "Game_Mgr.h"

// fmod_vc.lib ������ ��嶧 ���
// fmodL_vc.lib ����� ����϶� ���

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
#else // 64��Ʈ
#ifdef _DEBUG // ����� ���
#pragma comment(lib, "fmodL_vc.lib")
#else // ������ ���
#pragma comment(lib, "fmod_vc.lib")
#endif
#endif

class Game_String;
class Game_File;
class Game_Sound : public HFILEMGR<Game_Sound>
{
private:
	static FMOD::System* m_pSoundSys;

	// ���������� FMOD�� ��������
	// FMOD::Channel* ��������̴�.
	// ��������� �ʿ��ϴ�.
	// ���� ����� ���ؼ� �̰����� �����ϰ� ���� �༮��
	// ä���� �޾Ƽ� �� ä���� ���ؼ� �����ؾ� �Ѵ�.

	static std::map<Game_String, FMOD::Channel*> m_pAllPlaySound;

	class Game_Start
	{
	public:
		Game_Start();
		~Game_Start();
	};
	// ����� ������ �����̴�.
	static Game_Start Starter;

public:
	static void Init();
	static void Update();

	static void Load(const Game_File& _File);
	// �ѹ� ����ϰ� 

	// "BulletSound.mp3"
	static void Play(const Game_String& _FileName);

	// "BGM"�̶�� �̸����� "BackgroundSound.mp3"
	static void Play(const Game_String& _PlayName, const Game_String& _FileName);
	// ����ǰ� �� ����
	// "BGM" ����
	static void Stop(const Game_String& _Name);
	// "BGM" ������̾�?
	static bool IsPlay(const Game_String& _Name);

	// �ɹ�����
private:
	FMOD::Sound* m_pSound;

private:
	void Load();

public:
	Game_Sound() : m_pSound(nullptr)
	{

	}

};