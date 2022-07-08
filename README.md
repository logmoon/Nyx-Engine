# Nyx-Engine
A simple game engine written in plain C with SDL2

---
### TODO
#### ECS
- [x] An entity component system.
- [x] A clean way to access scene entities in the scene update function because i don't want to have to create a component for every little piece of logic. So access them in the update, and do whatever you want with them.
##### Base Components
* Transform component.
* Sprite renderer component + system.
* Audio Player.
* Animator component + system.
* Rigidboy/Physics component + system.
* 2D Collider component + system.
* UI Text component + system.
* UI Image component + system. (If needed)
#### Other
* Audio
* A simple logger
