#pragma once
#include "Game_Trans.h"
#include <unordered_set>
#include <functional>
#include <list>
#include <Game_Debug.h>

class Game_Collision :
    public Game_Trans
{
public:
	COLTYPE m_Type;

public:
	void ColType(COLTYPE _Type)
	{
		m_Type = _Type;
	}

private:
	// 일반적으로 레드블랙트리인 셋보다 빠르다.
	// 긴문자열인 경우에는
	// 너무 신경쓸 필요는 없다.
	std::unordered_set<Game_Collision*> m_ColSet;

private:
	std::list<std::function<void(Game_Collision*, Game_Collision*)>> m_EnterFunc;
	std::list<std::function<void(Game_Collision*, Game_Collision*)>> m_StayFunc;
	std::list<std::function<void(Game_Collision*, Game_Collision*)>> m_ExitFunc;

public:
	template<typename T>
	void PushEnterFunc(T* _Obj, void(T::* _Ptr)(Game_Collision*, Game_Collision*))
	{
		if (nullptr == _Ptr)
		{
			AMSG(L"nullptr인 함수를 넣어줬습니다. 실행할수 없습니다.");
		}

		m_EnterFunc.push_back(std::bind(_Ptr, _Obj, std::placeholders::_1, std::placeholders::_2));
	}
	template<typename T>
	void PushStayFunc(T* _Obj, void(T::* _Ptr)(Game_Collision*, Game_Collision*)) {
		if (nullptr == _Ptr)
		{
			AMSG(L"nullptr인 함수를 넣어줬습니다. 실행할수 없습니다.");
		}

		m_StayFunc.push_back(std::bind(_Ptr, _Obj, std::placeholders::_1, std::placeholders::_2));

	}
	template<typename T>
	void PushExitFunc(T* _Obj, void(T::* _Ptr)(Game_Collision*, Game_Collision*)) {
		if (nullptr == _Ptr)
		{
			AMSG(L"nullptr인 함수를 넣어줬습니다. 실행할수 없습니다.");
		}

		m_ExitFunc.push_back(std::bind(_Ptr, _Obj, std::placeholders::_1, std::placeholders::_2));

	}

	void PushEnterFunc(void(*_Ptr)(Game_Collision*, Game_Collision*))
	{
		if (nullptr == _Ptr)
		{
			AMSG(L"nullptr인 함수를 넣어줬습니다. 실행할수 없습니다.");
		}

		m_EnterFunc.push_back(std::bind(_Ptr, std::placeholders::_1, std::placeholders::_2));
	}
	void PushStayFunc(void(*_Ptr)(Game_Collision*, Game_Collision*))
	{
		if (nullptr == _Ptr)
		{
			AMSG(L"nullptr인 함수를 넣어줬습니다. 실행할수 없습니다.");
		}

		m_StayFunc.push_back(std::bind(_Ptr, std::placeholders::_1, std::placeholders::_2));
	}
	void PushExitFunc(void(*_Ptr)(Game_Collision*, Game_Collision*)) {
		if (nullptr == _Ptr)
		{
			AMSG(L"nullptr인 함수를 넣어줬습니다. 실행할수 없습니다.");
		}

		m_ExitFunc.push_back(std::bind(_Ptr, std::placeholders::_1, std::placeholders::_2));
	}



private:
	void CallEnter(Game_Collision* _Other);
	void CallStay(Game_Collision* _Other);
	void CallExit(Game_Collision* _Other);

public:
	void ColCheck(Game_Ptr<Game_Collision> _Other);

	void Release() override;

public:
	void Init(int _Order = 0);
	bool Collision_Check;
	bool Get_Check() { return Collision_Check; }
	void Check(bool b = true) { Collision_Check = b; }
};

