#pragma once
namespace Category
{
	enum Type
	{
		None = 0,
		Scene = 1 << 0, // Default category for general scene nodes.
		PlayerAircraft = 1 << 1,
		AlliedAircraft = 1 << 2,
		EnemyAircraft = 1 << 3,
	};
}


