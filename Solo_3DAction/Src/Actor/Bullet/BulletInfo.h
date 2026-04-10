#ifndef BULLET_INFO_H_
#define BULLET_INFO_H_

class BulletInfo {
public:
	enum Bullet_ID {
		Normal,
		Bazooka,
		Grenade,
		Missile
	};
	struct Status
	{
		int atk_{};			// UŒ‚—Í
		float speed_{};		// ’e‘¬
		bool is_stun_{};	// ƒXƒ^ƒ“‘®«‚ğ‚Â‚©H
		float duration_{};	// ”­ËŠÔŠu
	};
};
#endif