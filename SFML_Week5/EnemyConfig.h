#pragma once

#include <SFML/Graphics/Color.hpp>

enum class EnemyType
{
	Normal,
	Fast,
	Tank
};

struct EnemyConfig
{
	int health;
	int damage;
	float moveSpeed;
	sf::Color color;
};

EnemyConfig GetEnemyConfig(EnemyType type);
