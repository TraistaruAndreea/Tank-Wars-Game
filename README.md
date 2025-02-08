Overview

This project implements a 2D tank battle game using OpenGL and the graphics library provided in the Lab framework. The game features two tanks on a destructible terrain, with the ability to aim and fire projectiles at each other.

Features

2D Tank Models: Each tank is represented with trapezoid, circle, and rectangle meshes.

Projectile Physics: Bullets follow a trajectory based on gravity and angle.

Destructible Terrain: Projectiles deform the terrain upon impact.

Tank Health System: Each tank has a health bar that decreases when hit.

Tank Movement: Tanks move left and right across the terrain.

Turret Rotation: The player can rotate the turret to aim shots.

Projectile Collision: Detects collisions with tanks and terrain.

Visual Effects: Includes explosion effects and terrain deformation.

Controls

Tank 1 (Left Tank)

A / D - Move Left/Right

W / S - Rotate Turret Up/Down

Space - Fire Projectile

Tank 2 (Right Tank)

Left Arrow / Right Arrow - Move Left/Right

Up Arrow / Down Arrow - Rotate Turret Up/Down

Enter - Fire Projectile

Game Mechanics

Each tank starts with full health.

Projectiles are affected by gravity and travel in a realistic arc.

When a projectile hits a tank, it reduces the tank's health.

If a projectile hits the terrain, it creates a crater.

Tanks are removed from play when their health reaches zero.
