#pragma once
#include "Game_Texture.h"

class Game_Sprite : public Game_Mgr<Game_Sprite>
{
public:
	static Game_Ptr<Game_Sprite> Create(const Game_String& _Name, unsigned int _X, unsigned int _Y)
	{
		Game_Ptr<Game_Sprite> Res = CreateToInsert(_Name);

		// �ؽ�ó�� ������ ��������Ʈ�� ����� ����.
		Res->m_Tex = Game_Texture::Find(_Name);
		if (nullptr == Res->m_Tex)
		{
			AMSG(L"�������� �ʴ� �ؽ�ó���� ��������Ʈ�� ����� ������ �߽��ϴ�");
		}

		Res->Create(_X, _Y);
		return Res;
	}

	static Game_Ptr<Game_Sprite> Create(const Game_String& _TexName, const Game_String& _SpriteName, unsigned int _X, unsigned int _Y)
	{
		Game_Ptr<Game_Sprite> Res = CreateToInsert(_SpriteName);

		// �ؽ�ó�� ������ ��������Ʈ�� ����� ����.
		Res->m_Tex = Game_Texture::Find(_TexName);
		if (nullptr == Res->m_Tex)
		{
			AMSG(L"�������� �ʴ� �ؽ�ó���� ��������Ʈ�� ����� ������ �߽��ϴ�");
		}

		Res->Create(_X, _Y);
		return Res;
	}

private:
	// �ؽ�ó�� �˰� �־�� �Ѵ�.
	Game_Ptr<Game_Texture> m_Tex;
	std::vector<Game_Vector> m_SpriteData;

private:
	void Create(unsigned int _X, unsigned int _Y);

public:
	Game_Ptr<Game_Texture> Tex() {
		return m_Tex;
	}

	Game_Vector SpriteData(unsigned int _Index) 
	{
		if (m_SpriteData.size() <= _Index)
		{
			AMSG(L"�������� �ʴ� ��������Ʈ�� �ε����� �Է��߽��ϴ�.");
		}

		return m_SpriteData[_Index];
	}

};

