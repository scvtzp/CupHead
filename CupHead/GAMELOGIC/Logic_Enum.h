#pragma once

enum class RENDERORDER
{
	RENDERORDER_BACKGROUND, // ���
	RENDERORDER_MAP, // �浹���
	RENDERORDER_COLMAP,
	RENDERORDER_ACTOR,
	RENDERORDER_UI,
	RENDERORDER_Debug,
	RENDERORDER_Noise,

};

enum class COLORDER
{
	COLORDER_PLAYER_ForHit, //���Ϳ� �浹��
	COLORDER_PLAYER_ForUnknown, //���������� ���.
	Player_Parry_ForMonster, //�丮 (������ o)
	Player_Parry, //�丮 (������X)

	COLORDER_MONSTER_ForPlayer, //�÷��̾� �������
	COLORDER_Monster_ForBullet, //�Ѿ˿� �±��
	COLORDER_Monster_ForUnknown, //���������� ���.
	Monster_Parry, //�丮
	Monster_Bullet, //���Ͱ� ��� �Ѿ�

	COLORDER_Bullet, //�÷��̾ ����Ѿ�

	Map_Entrance, //�� �Ա��� <-> unknown

	End
};