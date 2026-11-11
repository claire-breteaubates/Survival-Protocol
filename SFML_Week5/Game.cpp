#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>

Game::Game()
	: window(sf::VideoMode({ 800u, 600u }), "Survival Protocol"),
	rng(std::random_device{}()),
	xDist(50.f, 750.f),
	yDist(50.f, 550.f)
{
	if (!enemyTexture.loadFromFile("assets/textures/enemy.png"))
	{
		throw std::runtime_error("Failed to load enemy texture.");
	}

	if (!bulletTexture.loadFromFile("assets/textures/bullet.png"))
	{
		throw std::runtime_error("Failed to load bullet texture.");
	}

	if (!font.openFromFile("assets/fonts/DejaVuSans.ttf"))
	{
		throw std::runtime_error("Failed to load font!.");
	}

	scoreText.emplace(font, "");
	scoreText->setCharacterSize(24);
	scoreText->setFillColor(sf::Color::White);
	scoreText->setPosition({ 620.f, 20.f });

	waveText.emplace(font, "");
	waveText->setCharacterSize(24);
	waveText->setFillColor(sf::Color::White);
	waveText->setPosition({ 20.f, 50.f });

	gameOverText.emplace(font, "GAME OVER");

	gameOverText->setCharacterSize(48);
	gameOverText->setFillColor(sf::Color::Red);
	
	const sf::FloatRect bounds = gameOverText->getLocalBounds();
	gameOverText->setOrigin({
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	});
	gameOverText->setPosition({
		window.getSize().x / 2.f,
		window.getSize().y / 2.f
	});

	healthBarBackground.setSize({
		healthBarWidth,
		healthBarHeight
	});

	healthBarBackground.setPosition({ 20.f, 20.f });
	healthBarBackground.setFillColor(sf::Color(60, 60, 60));

	healthBarFill.setSize({
		healthBarWidth,
		healthBarHeight
	});

	healthBarFill.setPosition({ 20.f, 20.f });
	healthBarFill.setFillColor(sf::Color::Green);

	UpdateHUD();
	
	PrepareNextWave();
}

void Game::Run()
{
	while (window.isOpen())
	{
		float dt = std::min(clock.restart().asSeconds(), 0.05f);

		ProcessEvents();
		Update(dt);
		Render();
	}
}

void Game::ProcessEvents()
{
	while (std::optional<sf::Event> event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (gameState == GameState::GameOver &&
				keyPressed->scancode == sf::Keyboard::Scancode::R) 
			{
				RestartGame();
			}
		}
	}

}

void Game::Update(float dt)
{
	if (gameState == GameState::GameOver)
		return;

	shootCooldown -= dt;

	player.Update(dt, window);

	UpdateEnemies(dt);
	UpdateProjectiles(dt);

	DeactivateOffscreenProjectiles();
	CheckProjectileEnemyCollision();

	CheckEnemyPlayerCollision();

	RemoveInactiveProjectiles();
	RemoveDeadEnemies();

	UpdateHUD();

	if (!player.IsAlive())
	{
		gameState = GameState::GameOver;
		return;
	}

	waveManager.UpdateWave(dt, enemies.size());

	if (waveManager.IsNextWaveReady())
	{
		PrepareNextWave();
	}

	if (waveManager.HasPendingSpawn())
	{
		std::optional<EnemyType> nextEnemy = waveManager.GetNextSpawnType();
		
		if (nextEnemy)
		{
			enemies.push_back(CreateEnemy(*nextEnemy));
			waveManager.ConfirmEnemySpawned();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
		shootCooldown <= 0)
	{
		shootCooldown = shootInterval;
		SpawnProjectile();
	}
}

Enemy Game::CreateEnemy(EnemyType type)
{
	constexpr float minSpawnDistance = 200.f;
	constexpr int maxSpawnAttempts = 100;

	const sf::Vector2f playerPosition = player.GetPosition();

	sf::Vector2f spawnPosition;

	for (int attempt = 0; attempt < maxSpawnAttempts; ++attempt)
	{
		spawnPosition = {
			xDist(rng),
			yDist(rng)
		};

		// Calculate distance from player
		sf::Vector2f difference = spawnPosition - playerPosition;

		float distanceSquared =
			difference.x * difference.x +
			difference.y * difference.y;

		//Compare squared distance to avoid sqrt()
		if (distanceSquared >= minSpawnDistance * minSpawnDistance)
		{
			EnemyConfig config = GetEnemyConfig(type);
			return Enemy(enemyTexture, spawnPosition, config);
		}
	}

	// Fallback: find the furthest corner of the spawn area
	// so we still respect the minimum distance

	const sf::Vector2f corners[] = {
		{ 50.f, 50.f },
		{ 750.f, 50.f },
		{ 50.f, 550.f },
		{ 750.f, 550.f}
	};

	float maxDistanceSquared = -1.f;

	for (const auto& corner : corners)
	{
		sf::Vector2f difference = corner - playerPosition;

		float distanceSquared =
			difference.x * difference.x +
			difference.y * difference.y;

		if (distanceSquared > maxDistanceSquared)
		{
			maxDistanceSquared = distanceSquared;
			spawnPosition = corner;
		}
	}

	EnemyConfig config = GetEnemyConfig(type);
	return Enemy(enemyTexture, spawnPosition, config);
}

void Game::UpdateEnemies(float dt)
{
	for (auto& enemy : enemies)
	{
		enemy.Update(dt, player.GetPosition());
	}
}

void Game::UpdateProjectiles(float dt)
{
	for (auto& projectile : projectiles)
	{
		projectile.Update(dt);
	}
}

void Game::CheckProjectileEnemyCollision()
{
	for (auto& projectile : projectiles)
	{
		if (!projectile.IsActive())
			continue;

		for (auto& enemy : enemies)
		{
			if (!enemy.IsAlive())
				continue;

			if (projectile.GetBounds().findIntersection(enemy.GetBounds()))
			{
				const bool wasAlive = enemy.IsAlive();

				enemy.TakeDamage(projectile.GetDamage());
				projectile.Deactivate();
				
				if (wasAlive && !enemy.IsAlive())
				{
					score += scorePerEnemy;
				}

				break;
			}
		}
	}
}

void Game::CheckEnemyPlayerCollision()
{
	if (!player.IsAlive())
		return;

	const auto playerBounds = player.GetBounds();

	for (auto& enemy : enemies)
	{
		if (!enemy.IsAlive())
			continue;

		if (enemy.GetBounds().findIntersection(playerBounds))
		{
			player.TakeDamage(enemy.GetDamage());
			break;
		}
	}
}

void Game::RemoveInactiveProjectiles()
{
	projectiles.erase(
		std::remove_if(
			projectiles.begin(),
			projectiles.end(),
			[](const Projectile& projectile)
			{
				return !projectile.IsActive();
			}),
		projectiles.end()
	);
}

void Game::DeactivateOffscreenProjectiles()
{
	const sf::FloatRect worldBounds(
		{ 0.f, 0.f },
		sf::Vector2f(window.getSize())
	);

	for (auto& projectile : projectiles)
	{
		if (!worldBounds.findIntersection(projectile.GetBounds()))
		{
			projectile.Deactivate();
		}
	}
}

void Game::RemoveDeadEnemies()
{
	enemies.erase(std::remove_if(
		enemies.begin(),
		enemies.end(),
		[](const Enemy& enemy)
		{
			return !enemy.IsAlive();
		}
	),
		enemies.end());
}

void Game::SpawnProjectile()
{
	sf::Vector2f playerPos = player.GetPosition();
	sf::Vector2f direction = player.GetForwardDirection();

	sf::Vector2f spawnPosition = playerPos + (direction * muzzleOffset);

	sf::Vector2f projectileVelocity = direction * projectileSpeed;


	projectiles.emplace_back(bulletTexture,
		spawnPosition,
		projectileVelocity,
		direction,
		25
	);
}

void Game::PrepareNextWave()
{
	waveManager.PrepareNextWave();
	enemies.reserve(waveManager.GetTotalEnemies());
}

void Game::RestartGame()
{
	player.Reset();
	waveManager.Reset();

	enemies.clear();
	projectiles.clear();

	shootCooldown = 0.f;
	gameState = GameState::Playing;

	score = 0;

	PrepareNextWave();
	UpdateHUD();
}

void Game::Render()
{
	window.clear();

	player.Draw(window);
	DrawEnemies();
	DrawProjectiles();

	DrawHUD();

	if (gameState == GameState::GameOver)
	{
		DrawGameOver();
	}

	window.display();
}

void Game::DrawEnemies()
{
	for (const auto& enemy : enemies)
	{
		if (enemy.IsAlive())
			enemy.Draw(window);
	}
}

void Game::DrawProjectiles()
{
	for (const auto& projectile : projectiles)
	{
		if (projectile.IsActive())
			projectile.Draw(window);
	}
}

void Game::DrawGameOver()
{
	window.draw(*gameOverText);
}

void Game::UpdateHUD()
{
	const float healthPercentage = std::clamp(
		static_cast<float>(player.GetHealth()) /
		static_cast<float>(player.GetMaxHealth()),
		0.f,
		1.f
	);

	healthBarFill.setSize({
		healthBarWidth * healthPercentage,
		healthBarHeight
	});

	if (scoreText)
	{
		scoreText->setString(
			"Score: " + std::to_string(score)
		);
	}

	if (waveText)
	{
		waveText->setString(
			"Wave: " + 
			std::to_string(waveManager.GetCurrentWave())
		);
	}
}

void Game::DrawHUD()
{
	window.draw(healthBarBackground);
	window.draw(healthBarFill);
	
	if (scoreText)
	{
		window.draw(*scoreText);
	}

	if (waveText)
	{
		window.draw(*waveText);
	}
}