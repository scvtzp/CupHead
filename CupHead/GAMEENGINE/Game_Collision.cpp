#include "Game_Collision.h"
#include "Game_Scene.h"
#include "Game_Actor.h"

void Game_Collision::Init(int _Order /*= 0*/)
{
	PARENT(ACTOR()->TRANS());
	// ���ڽ��� Ʈ���� ���̴ϱ�

	// �̰� ������ �ִ�.
	// m_TRANSDATA
	Order(_Order);
	ACTOR()->SCENE()->PushCol(this);
}

void Game_Collision::ColCheck(Game_Ptr<Game_Collision> _Other)
{
	// �� Enter // ó���浹������
	// �� exit // �浹�� ��������
	// �� Stay // ��� �浹���϶�


	if (true == Col(m_Type, _Other.PTR, _Other.PTR->m_Type))
	{
		// �浹 �ߴ�.
		if (m_ColSet.end() == m_ColSet.find(_Other.PTR))
		{
			m_ColSet.insert(_Other.PTR);
			_Other.PTR->m_ColSet.insert(this);

			CallEnter(_Other);
			_Other->CallEnter(this);
		}
		else
		{
			CallStay(_Other);
			_Other->CallStay(this);
		}


	}
	else
	{
		if (m_ColSet.end() != m_ColSet.find(_Other.PTR))
		{
			// �浹�� ���� �ʾҴµ� �ְ� ���ȿ� �ִ�.
			// ó�� �浹�ߴ�.

			CallExit(_Other);
			_Other->CallExit(this);

			m_ColSet.erase(_Other.PTR);
			_Other.PTR->m_ColSet.erase(this);
		}
	}

}

void Game_Collision::CallEnter(Game_Collision* _Other)
{
	for (auto& _Value : m_EnterFunc)
	{
		// this == Game_Collision* const
		// Game_Ptr<Game_Collision>
		_Value(this, _Other);
	}
}
void Game_Collision::CallStay(Game_Collision* _Other)
{
	for (auto& _Value : m_StayFunc)
	{
		_Value(this, _Other);
	}
}
void Game_Collision::CallExit(Game_Collision* _Other)
{
	for (auto& _Value : m_ExitFunc)
	{
		_Value(this, _Other);
	}
}

void Game_Collision::Release()
{
	// ���� B this == B

	//         A��
	for (auto& _OtherCol : m_ColSet)
	{

		// �˾Ƽ� �ڱⰡ �����⿡ EXIT���

		// A  ====== �� ���� ��.   B
		_OtherCol->m_ColSet.erase(this);
	}


}