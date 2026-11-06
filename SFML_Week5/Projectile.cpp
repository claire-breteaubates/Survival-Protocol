#include "Projectile.h"
#include <iostream>


Projectile::Projectile(
	const sf::Texture& texture,
	const sf::Vector2f& spawnPosition,
	const sf::Vector2f& initialVelocity,
	const sf::Vector2f& direction,
	int initialDamage)
	: sprite(texture), 
	velocity(initialVelocity),
	damage(initialDamage)
{
	sprite.setOrigin({
		texture.getSize().x / 2.f,
		texture.getSize().y / 2.f
	});

	sprite.setPosition(spawnPosition);
	
	float rotationDegrees =
		std::atan2(direction.y, direction.x) * 180.f / 3.14159265;

	sprite.setRotation(sf::degrees(rotationDegrees + 90.f));
}

void Projectile::Update(float dt)
{
	if (!active)
		return;

	Move(dt);
}

void Projectile::Draw(sf::RenderWindow& window) const
{
	if (!active)
		return;

	window.draw(sprite);
}

int Projectile::GetDamage() const
{
	return damage;
}

sf::FloatRect Projectile::GetBounds() const
{
	return sprite.getGlobalBounds();
}

bool Projectile::IsActive() const
{
	return active;
}

void Projectile::Deactivate()
{
	active = false;
}

void Projectile::Move(float dt)
{
	sprite.move({
		velocity.x * dt,
		velocity.y * dt
		});
}