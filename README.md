# Nyx-Engine
A simple game engine written in plain C with SDL2
---

**This was an experiment to get myself used to coding in C, and at the same time trying to create a simple 2d game engine. I'm done with this project, but I will definetely give this another try in the future.**

### Features To Implement
#### Scene Manager
- [x] Scene Manager
#### Input
- [x] User Input
- [ ] Gamepad input
- [ ] Window events
#### ECS
- [x] Base ECS
- [ ] Custom components
#### IO
- [x] IO
#### Logger
- [x] simple logger
- [x] global log level that is defined on init
- [x] logging to a file with the sdl prefs directory
- [x] variable list message
- [ ] use the logger
#### Rendering
- [!] rendering layers/order
##### Sprites
- [x] sprites
- [x] texuture handling (loading and unloading)
- [x] sprite sizes
##### Menus/UI
- [x] Rendering text
- [x] text sizes
- [x] rendering images
- [ ] menus navigation logic
#### Camera
- [x] camera
- [x] camera follow target
- [ ] camera sizes
#### Animation
- [x] animation
- [x] animation switching
- [x] forward animations
- [x] ping-pong animations
- [x] animations that don't loop
- [x] freeing animations
- [!] aseprite intergration
#### Physics
- [ ] Physics
- [ ] drawing gizmos
- [x] simple velet integration
- [x] circle vs circle collision
- [x] rigidbody + gravity
- [ ] circle vs line collision
- [ ] aabb vs aabb collision
- [ ] aabb vs circle collision
#### Audio
- [x] Audio
- [x] Registering and unloading audio
- [x] Separate tracks for sfx and music
- [x] music fade in and fade out
- [x] audio control for sfx and music
#### VFX
- [ ] particle system components

---
#### Bugs
- [x] Switching scenes keeps allocating more and more memory for some reason
- [x] rendering positions with different resolutions + camera placement (maybe use level size and the native resolution instead of the actual resolution)
- [ ] figure out how to work with texture filtering
- [x] unallocated memory crash error
- [x] sudden close as soon as app start
- [x] alt tabbing out of a fullscreen window closes the app
