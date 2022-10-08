#pragma once

class Game_Ref
{
public:
	int REFCOUNT;

public:
	void ADDREFCOUNT() {
		++REFCOUNT;
	}
	void DECREFCOUNT(/*HREF* const this*/)
	{
		--REFCOUNT;
		if (0 >= REFCOUNT)
		{
			delete this;
		}
	}

public:
	Game_Ref() : REFCOUNT(0) {}
	virtual ~Game_Ref() {}
};

template<typename T>
class Game_Ptr
{
public:
	T* PTR;

public:
	// ������ ������ �����ε�
	T* operator->()
	{
		return PTR;
	}

	operator T*()
	{
		return PTR;
	}

	// ������ ��ȯ�ϸ� ���������� �ʴ�.
	// ��ũ�� �巡���� �Ǿ������ ���°� �߻��Ѵ�.
	// const_cast; 
	// const�� const�� �ƴϰ� �����. ������� ������
	// static_cast;
	// �̳༮�� �츮�� �˰� �ִ� �Ϲ����� mamcpy�� ���� ĳ��Ʈ�� �ٸ� ����.
	// ũ��˻縸 �ϴ� ĳ���ÿ� ������.
	// reinterpret_cast;
	// ���� ��ﵵ �ȳ�. 
	// dynamic_cast;
	// ��Ӱ����϶��� ĳ������ ���ش�
	// ������ �����Լ� ���̺��� ���� ������� ĳ������ ���ֱ� ������
	// VIRTUAL�� �θ��ʿ� �־�� �Ѵ�.
	// �����Ҷ��� ����ϰڴ�
	template<typename OTHERTYPE>
	operator Game_Ptr<OTHERTYPE>()
	{
		return dynamic_cast<OTHERTYPE*>(PTR);
	}

	bool operator==(void* _nullptr) const
	{
		return PTR == _nullptr;
	}

	bool operator==(const Game_Ptr<T>& _Other) const
	{
		return PTR == _Other.PTR;
	}


	bool operator!=(void* _nullptr) const
	{
		return PTR != _nullptr;
	}

	Game_Ptr& operator=(const Game_Ptr<T>& _Other)
	{
		if (nullptr != PTR)
		{
			PTR->DECREFCOUNT();
		}


		PTR = _Other.PTR;
		if (nullptr != PTR)
		{
			PTR->ADDREFCOUNT();
		}
		return *this;
	}

	Game_Ptr& operator=(T* const _Other)
	{
		if (nullptr != PTR)
		{
			PTR->DECREFCOUNT();
		}


		PTR = _Other;
		if (nullptr != PTR)
		{
			PTR->ADDREFCOUNT();
		}
		return *this;
	}

public:
	Game_Ptr(const Game_Ptr<T>& _Other) : PTR(_Other.PTR)
	{
		if (nullptr == PTR)
		{
			return;
		}
		PTR->ADDREFCOUNT();
	}

	Game_Ptr() : PTR(nullptr)
	{
	}

	Game_Ptr(T* _Ptr) : PTR(_Ptr)
	{
		if (nullptr == _Ptr)
		{
			return;
		}

		_Ptr->ADDREFCOUNT();
	}
	~Game_Ptr()
	{
		if (nullptr == PTR)
		{
			return;
		}

		PTR->DECREFCOUNT();
	}
};

template<typename T>
bool operator!=(void* _Ptr, const Game_Ptr<T> _TypeName) 
{
	return _TypeName.operator!=(_Ptr);
}

template<typename T>
bool operator==(void* _Ptr, const Game_Ptr<T> _TypeName)
{
	return _TypeName.operator==(_Ptr);
}


template<typename T>
class Game_WPtr
{
public:
	T* PTR;

public:
	// ������ ������ �����ε�
	T* operator->()
	{
		return PTR;
	}

	Game_WPtr& operator=(const Game_WPtr<T>& _Other)
	{
		PTR = _Other.PTR;
		return *this;
	}

public:
	Game_WPtr(const Game_Ptr<T>& _Other) : PTR(_Other.PTR)
	{
	}

	Game_WPtr() : PTR(nullptr)
	{
	}

	Game_WPtr(T* _Ptr) : PTR(_Ptr)
	{
	}
	virtual ~Game_WPtr()
	{
	}
};