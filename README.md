
# Survival Protocol

A 2D top-down survival shooter developed in **C++20** using **SFML 3.1.0**.

The player must survive increasingly difficult waves of enemies, using movement and shooting to avoid damage and defeat opponents.

## Gameplay

### In-game

![Survival Protocol gameplay - Wave 7](docs/gameplay.png)

### Game Over

![Survival Protocol game over screen](docs/game-over.png)

## Features

- **Wave-based enemy spawning** with increasing difficulty
- **Three enemy types:** Normal, Fast and Tank
- **Enemy AI:** Enemies move towards the player's position
- **Distance-constrained spawning:** Enemies spawn away from the player
- **Combat system:** Projectiles, collision detection, damage and health
- **Player invulnerability:** Brief protection after taking damage
- **Scoring system** and on-screen HUD
- **Game states:** Playing, Game Over and restart functionality
- **Responsive window resizing:** Maintains the original game aspect ratio without stretching sprites
- **Consistent movement speed:** Normalised diagonal movement

## Technical Implementation

The project uses object-oriented C++ and separates gameplay responsibilities into dedicated classes:

| Component | Responsibility |
|---|---|
| `Game` | Main game loop, input handling, collision detection and rendering |
| `Player` | Player movement, health, invulnerability and aiming |
| `Enemy` | Enemy behaviour, movement and damage |
| `Projectile` | Projectile movement and collision behaviour |
| `WaveManager` | Enemy spawning, wave progression and state transitions |
| `EnemyConfig` | Configuration for different enemy types |

### Key Programming Concepts

- Object-oriented programming and encapsulation
- State-machine-based wave management
- Delta-time-based updates
- Standard library containers and algorithms
- Random number generation with `std::mt19937`
- Vector mathematics and collision detection
- Normalised movement vectors and mouse-to-world coordinate conversion
- Aspect-ratio preservation using `sf::View`
- Separation of gameplay systems into reusable components
- Dependency management and reproducible builds using CMake

## Technologies

- **Language:** C++20
- **Graphics and input:** SFML 3.1.0
- **Build system:** CMake 3.28+
- **Development environment:** Visual Studio 2026 Community
- **Version control:** Git and GitHub

## Building and Running

### Requirements

- Windows 10 or 11
- Visual Studio 2022 or later with the **Desktop development with C++** workload
- CMake 3.28 or later
- Git
- Internet connection for the initial dependency download

**SFML does not need to be installed manually.** CMake automatically downloads SFML 3.1.0 using `FetchContent`.

### Build Instructions (Visual Studio)

1. Clone this repository:

   ```bash
   git clone https://github.com/claire-breteaubates/Survival-Protocol.git
   ```

2. Open **Visual Studio**.

3. Select **File → Open → Folder** and open the cloned `Survival-Protocol` directory.

   **Do not open the legacy `.sln` file.** The recommended build method uses CMake.

4. Wait for Visual Studio to detect `CMakeLists.txt` and complete CMake configuration.

   On the first configuration, CMake downloads and builds the required SFML dependencies. This may take a few minutes.

5. Select a build configuration:

   - `x64-Debug` — For development and debugging.
   - `x64-Release` — For an optimised build.

6. Select `SurvivalProtocol.exe` as the startup target.

7. Select **Build → Build All**.

8. Run the game using **Ctrl + F5**, or select **Debug → Start Without Debugging**.

### Build Notes

- The project uses **C++20** and **SFML 3.1.0**.
- SFML is downloaded automatically using CMake `FetchContent`.
- SFML is linked statically, so separate SFML DLL files do not need to be copied into the build directory.
- Game textures and fonts are copied into the executable's output directory automatically.
- CMake-generated build directories are excluded from version control through `.gitignore`.
- The game uses a logical resolution of **800 × 600**.
- When the window is resized, the game preserves its original **4:3 aspect ratio**, adding unused space where necessary instead of stretching the graphics.

## Controls

| Action | Control |
|---|---|
| Move up | `W` |
| Move down | `S` |
| Move left | `A` |
| Move right | `D` |
| Aim | Move the mouse cursor |
| Shoot | `Spacebar` |
| Restart after Game Over | `R` |

### How to Play

1. Use **WASD** to move around the arena.
2. Aim your weapon by moving the **mouse cursor**.
3. Press **Spacebar** to fire in the direction your player is facing.
4. Avoid enemies while defeating them to increase your score.
5. Survive successive waves of Normal, Fast and Tank enemies.
6. When your health reaches zero, press **R** to restart.

## Future Improvements

- Sound effects and background music
- Improved enemy behaviours
- Additional weapons and power-ups
- Difficulty balancing and gameplay polish
- Expanded widescreen support without letterboxing

## About

This project was developed as part of my C++ gameplay programming learning journey, with a focus on building a complete playable game and practising software design, game loops, object-oriented programming and state management.

It also demonstrates practical experience with CMake, dependency management, version control and maintaining a structured C++ codebase.
