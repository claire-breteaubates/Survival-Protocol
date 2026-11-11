#include "Player.h"
#include <iostream>
#include <cmath>
#include <algorithm>


Player::Player()
	: sprite(texture)
{
	if (!texture.loadFromFile("assets/textures/player.png"))
	{
		throw std::runtime_error("Failed to load player texture.");
	}

	sprite.setTexture(texture, true);

	sprite.setOrigin({
		texture.getSize().x / 2.f,
		texture.getSize().y / 2.f
	});

	sprite.setPosition({ 100.0f, 100.f });
}

void Player::Update(float dt, const sf::RenderWindow& window)
{
	if (invulnerable)
	{
		invulnerabilityTimer -= dt;

		if (invulnerabilityTimer <= 0.f)
		{
			invulnerabilityTimer = 0.f;
			invulnerable = false;
		}
	}

	if (!IsAlive())
		return;

	HandleMovement(dt);
	ClampToScreen(window);
	RotateToMouse(window);
}

void Player::Draw(sf::RenderWindow& window) const
{
	window.draw(sprite);
}

void Player::TakeDamage(int amount)
{
	if (amount <= 0 || invulnerable || !IsAlive())
		return;

	health = std::max(0, health - amount);

	if (IsAlive())
	{
		StartInvulnerability(invulnerabilityDuration);
	}
}

void Player::StartInvulnerability(float invulnerableDuration)
{
	if (invulnerabilityDuration <= 0.f)
		return;
	
	invulnerable = true;
	invulnerabilityTimer = invulnerableDuration;
}

sf::Vector2f Player::GetPosition() const
{
	return sprite.getPosition();
}

sf::Vector2f Player::GetForwardDirection() const
{
	float angle = (sprite.getRotation() - sf::degrees(90.f)).asRadians();

	return {
		std::cos(angle),
		std::sin(angle)
	};
}

float Player::GetRotation() const
{
	return sprite.getRotation().asDegrees();
}

sf::FloatRect Player::GetBounds() const
{
	return sprite.getGlobalBounds();
}

int Player::GetHealth() const
{
	return health;
}

int Player::GetMaxHealth() const
{
	return maxHealth;
}

bool Player::IsAlive() const
{
	return health > 0;
}

void Player::Reset()
{
	health = maxHealth;

	invulnerable = false;
	invulnerabilityTimer = 0.f;

	sprite.setPosition({ 100.f, 100.f });
	sprite.setRotation({ sf::degrees(0.f) });
}

void Player::HandleMovement(float dt)
{
	sf::Vector2f direction{ 0.f, 0.f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		direction.x += 1.f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		direction.x -= 1.f;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		direction.y -= 1.f;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		direction.y += 1.f;

	// Avoid normalising a zero-length vector
	if (direction.lengthSquared() > 0.f)
	{
		direction = direction.normalized();
		sprite.move(direction * speed * dt);
	}
}

void Player::ClampToScreen(const sf::RenderWindow& window)
{
	sf::Vector2f pos = sprite.getPosition();

	const sf::FloatRect bounds = sprite.getGlobalBounds();

	const sf::Vector2f worldSize = window.getView().getSize();

	const float halfWidth = bounds.size.x / 2.f;
	const float halfHeight = bounds.size.y / 2.f;

	pos.x = std::clamp(
		pos.x,
		halfWidth,
		worldSize.x - halfWidth
	);

	pos.y = std::clamp(
		pos.y,
		halfHeight,
		worldSize.y - halfHeight
	);

	sprite.setPosition(pos);
}

void Player::RotateToMouse(const sf::RenderWindow& window)
{
	sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
	sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

	sf::Vector2f toMouse{
		mouseWorld.x - sprite.getPosition().x,
		mouseWorld.y - sprite.getPosition().y
	};
	

	if (toMouse.lengthSquared() < 0.0001f)
		return;

	const float angleRadians = std::atan2(toMouse.y, toMouse.x);

	sprite.setRotation(sf::radians(angleRadians) + sf::degrees(90.f));
}