#include "EnemyConfig.h"
#include <stdexcept>

EnemyConfig GetEnemyConfig(EnemyType type)
{
	switch (type)
	{
	case EnemyType::Normal:
		return { 100, 10, 150.f, sf::Color::White };

	case EnemyType::Fast:
		return { 50, 5, 200.f, sf::Color::Red };

	case EnemyType::Tank:
		return { 150, 15, 80.f, sf::Color::Blue };
	}

	throw std::invalid_argument("Unknown enemy type");
}