#ifndef GUN_INFO_H_
#define GUN_INFO_H_

class GunInfo
{
public:
	enum Gun_ID
	{
		MachineGun,
		BazookaLauncher,
		GrenadeLauncher,
		MissileLauncher
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