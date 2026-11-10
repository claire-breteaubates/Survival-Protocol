#include "EnemyConfig.h"
#include <stdexcept>

EnemyConfig GetEnemyConfig(EnemyType type)
{
	switch (type)
	{
	case EnemyType::Normal:
		return { 100, 10, 115.f, sf::Color::White };

	case EnemyType::Fast:
		return { 50, 5, 180.f, sf::Color::Red };

	case EnemyType::Tank:
		return { 150, 15, 70.f, sf::Color::Blue };
	}

	throw std::invalid_argument("Unknown enemy type");
}