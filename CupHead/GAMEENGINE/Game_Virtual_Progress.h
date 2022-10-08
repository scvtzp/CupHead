#pragma once
class Game_Virtual_Progress
{
public:
	bool m_IsDebug;

private:
	int  m_Order;
	bool m_bIsUpdate;
	bool m_bIsDeath;

public:
	void DebugOn()
	{
		m_IsDebug = true;
	}

	void DebugCheck()
	{
		if (true == m_IsDebug)
		{
			int a = 0;
		}
	}


	int Order() {
		return m_Order;
	}
	void Order(int _Order) 
	{
		m_Order = _Order;
	}

	virtual bool IsUpdate() 
	{
		return m_bIsUpdate;
	}
	virtual bool IsDeath() 
	{
		return m_bIsDeath;
	}

	void Death()
	{
		m_bIsDeath = true;
	}

	void On()
	{
		m_bIsUpdate = true;
	}

	void Off()
	{
		m_bIsUpdate = false;
	}


public:
	virtual void Init() {}

	virtual void UpdatePrev() {	}
	virtual void Update() {	}
	virtual void UpdateNext() {	}

	virtual void RenderPrev() {	}
	virtual void RenderNext() {	}

	virtual void CollisionPrev() {}
	virtual void CollisionNext() {}

	virtual void SceneChangeEnd() {	}
	virtual void SceneChangeStart() {}
	virtual void Release() {}


public:
	Game_Virtual_Progress() : m_bIsUpdate(true), m_bIsDeath(false), m_IsDebug(false)
	{}
	virtual ~Game_Virtual_Progress() {}
};

#define DEBUGCHECK DebugCheck();

