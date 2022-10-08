#pragma once

enum class RENDERORDER
{
	RENDERORDER_BACKGROUND, // 배경
	RENDERORDER_MAP, // 충돌용맵
	RENDERORDER_COLMAP,
	RENDERORDER_ACTOR,
	RENDERORDER_UI,
	RENDERORDER_Debug,
	RENDERORDER_Noise,

};

enum class COLORDER
{
	COLORDER_PLAYER_ForHit, //몬스터와 충돌용
	COLORDER_PLAYER_ForUnknown, //유동적으로 써라.
	Player_Parry_ForMonster, //페리 (데미지 o)
	Player_Parry, //페리 (데미지X)

	COLORDER_MONSTER_ForPlayer, //플레이어 때리기용
	COLORDER_Monster_ForBullet, //총알에 맞기용
	COLORDER_Monster_ForUnknown, //유동적으로 써라.
	Monster_Parry, //페리
	Monster_Bullet, //몬스터가 쏘는 총알

	COLORDER_Bullet, //플레이어가 쏘는총알

	Map_Entrance, //맵 입구용 <-> unknown

	End
};