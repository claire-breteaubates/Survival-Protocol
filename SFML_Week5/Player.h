#pragma once

#include<SFML/Graphics.hpp>

class Player
{
public:
	Player();

	void Update(float dt, const sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window) const;

	void TakeDamage(int amount);
	void StartInvulnerability(float invulnerableDuration);

	sf::Vector2f GetPosition() const;
	sf::Vector2f GetForwardDirection() const;
	float GetRotation() const;
	sf::FloatRect GetBounds() const;
	
	int GetHealth() const;
	int GetMaxHealth() const;
	
	bool IsAlive() const;

	void Reset();

private:
	void HandleMovement(float dt);
	void ClampToScreen(const sf::RenderWindow& window);
	void RotateToMouse(const sf::RenderWindow& window);

private:
	sf::Texture texture;
	sf::Sprite sprite;

	float speed = 200.f;
	
	int maxHealth = 100;
	int health = maxHealth;

	bool invulnerable = false;
	float invulnerabilityTimer = 0.f;
	float invulnerabilityDuration = 1.f;
};