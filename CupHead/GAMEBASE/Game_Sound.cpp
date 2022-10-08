#include "Game_Sound.h"
#include <assert.h>
#include "Game_String.h"

Game_Sound::Game_Start::Game_Start()
{
	Init();
}
Game_Sound::Game_Start::~Game_Start()
{

}


std::map<Game_String, FMOD::Channel*> Game_Sound::m_pAllPlaySound;
FMOD::System* Game_Sound::m_pSoundSys = nullptr;
Game_Sound::Game_Start Game_Sound::Starter;

void Game_Sound::Init()
{
	// m_pSoundSys�� �־��ָ� ���� ä���� �������ְڴٴ� �Ҹ��̴�.
	FMOD::System_Create(&m_pSoundSys);

	// ����ī�� ������ nullptr�� ���ü� �ִ�.
	if (nullptr == m_pSoundSys)
	{
		assert(true);
	}

	// 32 ä�ΰ��� ũ�� �Ű澵�ʿ� ���°�
	// 32 ä���̸� 8126�� �������
	if (FMOD_OK != m_pSoundSys->init(32, FMOD_DEFAULT, nullptr))
	{
		assert(true);
	}
}

void Game_Sound::Update()
{
	m_pSoundSys->update();
}

void Game_Sound::Play(const Game_String& _Name)
{
	Game_Ptr<Game_Sound> Sound = Find(_Name);

	if (nullptr == Sound)
	{
		assert(false);
	}

	m_pSoundSys->playSound(Sound->m_pSound, nullptr, false, nullptr);
}

bool Game_Sound::IsPlay(const Game_String& _Name)
{
	std::map<Game_String, FMOD::Channel*>::iterator Find = m_pAllPlaySound.find(_Name);

	if (Find == m_pAllPlaySound.end())
	{
		return false;
	}

	bool Playing = false;

	Find->second->isPlaying(&Playing);

	return Playing;
}

void Game_Sound::Stop(const Game_String& _Name) {
	std::map<Game_String, FMOD::Channel*>::iterator Find = m_pAllPlaySound.find(_Name);

	if (Find == m_pAllPlaySound.end())
	{
		return;
	}

	Find->second->stop();
}

void Game_Sound::Play(const Game_String& _PlayName, const Game_String& _FileName)
{
	Game_Ptr<Game_Sound> Sound = Find(_FileName);

	if (nullptr == Sound)
	{
		assert(false);
	}

	std::map<Game_String, FMOD::Channel*>::iterator Find = m_pAllPlaySound.find(_PlayName);

	if (Find == m_pAllPlaySound.end())
	{
		FMOD::Channel* Ptr = nullptr;
		m_pSoundSys->playSound(Sound->m_pSound, nullptr, false, &Ptr);

		if (nullptr == Ptr)
		{
			assert(false);
		}

		m_pAllPlaySound.insert(std::map<Game_String, FMOD::Channel*>::value_type(_PlayName, Ptr));

		return;
	}

	// �Ͻ� ���� ���.
	// Find->second->setPaused(true)

	Find->second->stop();

	FMOD::Channel* Ptr = nullptr;
	m_pSoundSys->playSound(Sound->m_pSound, nullptr, false, &Ptr);

	if (nullptr == Ptr)
	{
		assert(false);
	}

	Find->second = Ptr;

	// Ptr->setVolume()
	// Ptr->stop()

	// Ptr ����� ���� ����.

}

// �̳༮�� �����Դϴ�.
void Game_Sound::Load(const Game_File& _File)
{
	if (nullptr != Find(_File.FileName()))
	{
		assert(false);
	}

	Game_Sound* NewSound = CreateToInsertToFileObj(_File);
	NewSound->Load();
}
// �̳༮�� �ɹ�������
void Game_Sound::Load()
{
	std::string Path = GameFile.FullPath();

	if (FMOD_OK != m_pSoundSys->createSound(Path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		assert(true);
	}

}