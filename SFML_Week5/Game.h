#pragma once

#include<SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <optional>
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "WaveManager.h"

enum class GameState
{
	Playing,
	GameOver
};

class Game
{
public:
	Game();
	void Run();

private:
	void ProcessEvents();
	void Update(float dt);
	void Render();

	// Enemies
	Enemy CreateEnemy(EnemyType type);
	void UpdateEnemies(float dt);
	void PrepareNextWave();

	// Projectiles
	void UpdateProjectiles(float dt);
	void SpawnProjectile();

	//Collision
	void CheckProjectileEnemyCollision();
	void CheckEnemyPlayerCollision();

	//Clean Up
	void RemoveInactiveProjectiles();
	void DeactivateOffscreenProjectiles();
	void RemoveDeadEnemies();
	void RestartGame();

	// Render
	void DrawEnemies();
	void DrawProjectiles();
	void DrawGameOver();
	
	void UpdateHUD();
	void DrawHUD();

private:
	// Gamestate
	GameState gameState = GameState::Playing;

	// Window
	sf::RenderWindow window;
	sf::Clock clock;
	
	// Resources
	sf::Texture enemyTexture;
	sf::Texture bulletTexture;

	std::mt19937 rng;
	std::uniform_real_distribution<float> xDist;
	std::uniform_real_distribution<float> yDist;

	// Game Objects
	Player player;
	std::vector<Enemy> enemies;
	std::vector<Projectile> projectiles;
	WaveManager waveManager;
		
	// Weapon
	float projectileSpeed = 500.f;
	float shootCooldown = 0.0f;
	float shootInterval = 0.25f;
	float muzzleOffset = 35.f;

	//Damage Cooldown
	float playerDamageCooldown = 0.f;
	float playerDamageInterval = 0.5f;

	// Text
	sf::Font font;
	std::optional<sf::Text> gameOverText;
	std::optional<sf::Text> scoreText;
	std::optional<sf::Text> waveText;

	int score = 0;
	int scorePerEnemy = 100;


	//HUD
	sf::RectangleShape healthBarBackground;
	sf::RectangleShape healthBarFill;

	float healthBarWidth = 200.f;
	float healthBarHeight = 20.f;
};
