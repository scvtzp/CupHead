#pragma once
#include <vector>
#include <assert.h>
#include "Game_String.h"
#include <map>
#include "GAMEMATH.h"

class Game_Input
{
public:
	class Game_Key
	{
	public:
		friend Game_Input;

	public:
		Game_String Name;
		std::vector<int> m_KeyVec;

		bool Down; // 처음 눌렸다
		bool Press; // 계속 눌리고 있다
		bool Up; // 키가 떼졌다
		bool Free; // 계속 떼진 상태다

	private:
		bool KeyCheck();
		void Update();

	private:
		Game_Key() : Down(false), Press(false), Up(false), Free(true) {}
		~Game_Key() {}
	};




public:
	class Game_Static
	{
	public:
		Game_Static();
		~Game_Static();
	};
	static Game_Static StaticInst;

private:
	static std::map<Game_String, Game_Key*> AllKey;

public:
	static Game_Key* FindKey(const Game_String& _KeyName)
	{
		if (AllKey.end() == AllKey.find(_KeyName))
		{
			return nullptr;
		}

		// 오버로딩 바로 값만 리턴해준다.
		return AllKey[_KeyName];
	}

	template<typename ... REST>
	static void CreateKey(const Game_String& _KeyName, REST ... Arg)
	{
		Game_Key* NewKey = FindKey(_KeyName);

		if (nullptr != NewKey)
		{
			assert(false);
		}

		NewKey = new Game_Key();
		NewKey->Name = _KeyName;
		NewKey->m_KeyVec.reserve(sizeof...(Arg));
		PushKey(NewKey, Arg...);
	}

private:
	template<typename T, typename ... REST>
	static void PushKey(Game_Key* _NewKey, T _Value, REST ... Arg)
	{
		_NewKey->m_KeyVec.push_back(_Value);
		PushKey(_NewKey, Arg...);
	}

	static void PushKey(Game_Key* _NewKey)
	{
		AllKey.insert(std::map<Game_String, Game_Key*>::value_type(_NewKey->Name, _NewKey));
	}

public:
	static bool Down(const Game_String& _Key)
	{
		Game_Key* Ptr = FindKey(_Key);
		if (nullptr == Ptr) { assert(false); }
		return Ptr->Down;
	}
	static bool Press(const Game_String& _Key) {
		Game_Key* Ptr = FindKey(_Key);
		if (nullptr == Ptr) { assert(false); }
		return Ptr->Press;
	}
	static bool Up(const Game_String& _Key) {
		Game_Key* Ptr = FindKey(_Key);
		if (nullptr == Ptr) { assert(false); }
		return Ptr->Up;
	}
	static bool Free(const Game_String& _Key) {
		Game_Key* Ptr = FindKey(_Key);
		if (nullptr == Ptr) { assert(false); }
		return Ptr->Free;
	}

public:
	static Game_Vector PrevPos;
	static Game_Vector MouseDir;
	static Game_Vector MousePos3D();
	static Game_Vector MouseDir3D();
	static Game_Vector MouseDir3DNormal();

public:
	static void Update();

};

