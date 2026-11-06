#pragma once

#include<SFML/Graphics.hpp>

class Projectile

{
public:
	Projectile(
		const sf::Texture& texture,
		const sf::Vector2f& spawnPosition,
		const sf::Vector2f& velocity,
		const sf::Vector2f& direction,
		int damage);

	void Update(float dt);
	void Draw(sf::RenderWindow& window) const;

	int GetDamage() const;
	sf::FloatRect GetBounds() const;

	bool IsActive() const;
	void Deactivate();

private:

	void Move(float dt);

private:
	sf::Sprite sprite;

	sf::Vector2f velocity{ 0.f,0.f };

	int damage = 25;
	bool active = true;
};
