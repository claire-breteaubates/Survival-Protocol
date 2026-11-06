#pragma once
#include <cstddef>
#include <vector>
#include <optional>
#include "EnemyConfig.h"

enum class WaveState
{
	Waiting,
	Spawning,
	Fighting
};

struct EnemySpawn
{
	EnemyType type;
	int count;
};

struct WaveConfig
{
	std::vector<EnemySpawn> spawns;
};

class WaveManager
{
public:
	WaveManager();

	int GetTotalEnemies() const;

	void UpdateWave(float dt, std::size_t enemiesRemaining);

	WaveConfig GetWaveConfig() const;

	int GetCurrentWave() const;

	std::optional<EnemyType> GetNextSpawnType() const;

	void ConfirmEnemySpawned();

	void PrepareNextWave();

	bool HasPendingSpawn() const;

	bool IsNextWaveReady() const;

	void Reset();

	WaveState GetState() const;

private:

	int currentWave;

	float waveDelay;
	float waveTimer;

	WaveState currentState;
	WaveConfig currentWaveConfig;

	int enemiesSpawned = 0;
	float spawnTimer = 0.f;
	float spawnInterval = 0.5f;
	int pendingSpawns = 0;

};
