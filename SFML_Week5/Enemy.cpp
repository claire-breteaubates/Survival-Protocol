#include "Enemy.h"

#include <algorithm>
#include <cmath>

Enemy::Enemy(
    const sf::Texture& texture,
    const sf::Vector2f& spawnPosition,
    const EnemyConfig& config)
	: sprite(texture),
    health(config.health),
    damage(config.damage),
    moveSpeed(config.moveSpeed)
{
    sprite.setOrigin({
        texture.getSize().x / 2.f,
        texture.getSize().y / 2.f
    });

    sprite.setPosition(spawnPosition);

    sprite.setColor(config.color);
}

void Enemy::Update(float dt, const sf::Vector2f& playerPosition)
{
    if (IsAlive() == false)
        return;

    Move(dt, playerPosition);
}

void Enemy::Draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

void Enemy::TakeDamage(int amount)
{
  
    health = std::max(0, health - amount);

}

int Enemy::GetDamage() const
{
    return damage;
}

sf::FloatRect Enemy::GetBounds() const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Enemy::GetPosition() const
{
    return sprite.getPosition();
}

bool Enemy::IsAlive() const
{
    return health > 0;
}

void Enemy::Move(float dt, const sf::Vector2f& playerPosition)
{
    // Get position
    sf::Vector2f enemyPos = sprite.getPosition();

    // Get direction to player
    sf::Vector2f toPlayer{
        playerPosition.x - enemyPos.x,
        playerPosition.y - enemyPos.y 
    };

    // Get distance to player 
    float distance = std::sqrt(
        toPlayer.x * toPlayer.x + 
        toPlayer.y * toPlayer.y
    );

    // Chase state
    if (distance > 0.f)
    {
        // Normalize
        toPlayer.x /= distance;
        toPlayer.y /= distance;
    }

    // Desired velocity
    sf::Vector2f desiredVelocity{ 
        toPlayer.x * moveSpeed, 
        toPlayer.y * moveSpeed 
    };

    // Steering
    sf::Vector2f steering{ desiredVelocity.x - velocity.x, 
        desiredVelocity.y - velocity.y 
    };

    // Apply steering
    velocity.x += steering.x * steeringResponsiveness * dt;
    velocity.y += steering.y * steeringResponsiveness * dt;

    //Move Enemy
    sprite.move({
        velocity.x * dt,
        velocity.y * dt
        });
}