#pragma once

#include<SFML/Graphics.hpp>
#include"EnemyConfig.h"

class Enemy
{
public:
	Enemy(const sf::Texture& texture, 
		const sf::Vector2f& spawnPosition,
		const EnemyConfig& config);

	void Update(float dt, const sf::Vector2f& playerPosition);
	void Draw(sf::RenderWindow& window) const;

	void TakeDamage(int amount);

	int GetDamage() const;

	sf::FloatRect GetBounds() const;
	sf::Vector2f GetPosition() const;
	
	bool IsAlive() const;

private:
	void Move(float dt, const sf::Vector2f& playerPosition);

	sf::Sprite sprite;

	sf::Vector2f velocity{ 0.f, 0.f };

	int health;
	int damage;
	float moveSpeed;

	float steeringResponsiveness = 4.f;
};
