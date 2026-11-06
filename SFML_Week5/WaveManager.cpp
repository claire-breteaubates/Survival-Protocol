#include "WaveManager.h"
#include <algorithm>


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
	int nextWave = currentWave + 1;

	// Total enemies increase by 1 each wave
	int totalEnemies = nextWave + 2;

	// Introduce fast enemies on wave 3.
	// Add another fast enemy every 3 waves.
	int fastCount = (nextWave >= 3) 
		? 1 +  (nextWave - 3) / 3
		: 0;

	// Introduce tank on wave 5.
	// Add another tank every 3 waves.
	int tankCount = (nextWave >= 6)
		? 1 + (nextWave - 6) / 3
		: 0;

	// Fill the remaining slots with normal enemies.
	int normalCount = totalEnemies - fastCount - tankCount;
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