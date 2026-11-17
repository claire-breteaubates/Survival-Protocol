#include "WaveManager.h"
#include <cassert>


WaveManager::WaveManager()
	: currentWave(0),
	waveDelay(2.0f),
	waveTimer(0.f),
	currentState(WaveState::Spawning)
{
}

int WaveManager::GetTotalEnemies() const
{
	int totalEnemies = 0;

	for (const EnemySpawn& spawn : currentWaveConfig.spawns)
	{
		totalEnemies += spawn.count;
	}

	return totalEnemies;
}

void WaveManager::UpdateWave(float dt, std::size_t enemiesRemaining)
{
	if (currentState == WaveState::Spawning)
	{
		spawnTimer += dt;

		int totalEnemies = GetTotalEnemies();

		while (spawnTimer >= spawnInterval && 
			enemiesSpawned + pendingSpawns < totalEnemies)
		{
			spawnTimer -= spawnInterval;
			++pendingSpawns;
		}
	}

	else if (currentState == WaveState::Fighting && enemiesRemaining == 0)
	{
		currentState = WaveState::Waiting;
		waveTimer = waveDelay;
	}

	else if (currentState == WaveState::Waiting)
	{
		waveTimer -= dt;
	}
}

WaveConfig WaveManager::GetWaveConfig() const
{
	constexpr int startingEnemies = 2;
	constexpr int fastStartWave = 3;
	constexpr int tankStartWave = 6;
	constexpr int fastIncreaseInterval = 3;
	constexpr int tankIncreaseInterval = 3;

	const int nextWave = currentWave + 1;
	const int totalEnemies = nextWave + startingEnemies;

	const int fastCount = (nextWave >= fastStartWave) 
		? 1 +  (nextWave - fastStartWave) / fastIncreaseInterval
		: 0;

	const int tankCount = (nextWave >= tankStartWave)
		? 1 + (nextWave - tankStartWave) / tankIncreaseInterval
		: 0;

	// Fill the remaining slots with normal enemies.
	const int normalCount = totalEnemies - fastCount - tankCount;

	assert(normalCount >= 0 &&
		"Wave configuration has too many special enemies");

	return { {
		{ EnemyType::Normal, normalCount },
		{ EnemyType::Fast, fastCount },
		{ EnemyType::Tank, tankCount }
	} };

}


int WaveManager::GetCurrentWave() const
{
	return currentWave;
}

std::optional<EnemyType> WaveManager::GetNextSpawnType() const
{
	int cumulativeCount = 0;

	for (const EnemySpawn& spawn : currentWaveConfig.spawns)
	{
		cumulativeCount += spawn.count;

		if (enemiesSpawned < cumulativeCount)
		{
			return spawn.type;
		}
	}

	return std::nullopt;
}

void WaveManager::ConfirmEnemySpawned()
{
	int totalEnemies = GetTotalEnemies();

	if (pendingSpawns > 0 && enemiesSpawned < totalEnemies)
	{
		++enemiesSpawned;
		--pendingSpawns;

		if (enemiesSpawned == totalEnemies)
		{
			currentState = WaveState::Fighting;
		}
	}
}

void WaveManager::PrepareNextWave()
{
	currentWaveConfig = GetWaveConfig();
	++currentWave;

	waveTimer = 0.f;
	enemiesSpawned = 0;
	pendingSpawns = 0;
	spawnTimer = 0.f;
	currentState = WaveState::Spawning;
}

bool WaveManager::HasPendingSpawn() const
{
	return pendingSpawns > 0;
}

bool WaveManager::IsNextWaveReady() const
{
	return currentState == WaveState::Waiting && waveTimer <= 0.f;
}

void WaveManager::Reset()
{
	currentWave = 0;
	waveTimer = 0.f;
	spawnTimer = 0.f;

	enemiesSpawned = 0;
	pendingSpawns = 0;
	currentWaveConfig = {};
	currentState = WaveState::Spawning;

}

WaveState WaveManager::GetState() const
{
	return currentState;
}