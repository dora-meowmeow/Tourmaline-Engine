# Tourmaline Engine

Tormaline Engine is a game engine created for game development with C++23. For its stack it uses Corrade as its STD replacement (when applicable), Magnum Graphics as its graphics engine, miniaudio as its sound engine, FLECS as its main ECS controller. 

The engine aims to incorporate mruby to make ruby its optional scripting language. The goal is to make 1 to 1 parity with C++ and Ruby scirpting. So a game for example can use Ruby for its basic player controls or networking, while using C++ for its complex AI logic. A game is not strictly required to use C++ and Ruby. A game can choose how much of either language it needs.

Tourmaline Engine will be ECS only by default. ECS will be strictly imposed on engine level. Tourmaline Engine aside from ECS tries not to force the developer into its (my own) opinionated way of doing things. This is achieved by allowing the programmer to easily build modules to the engine. We expose module developers majority of the low level aspects of the project. 

Tourmaline Engine places an emphasis on good documentation, plenty of examples, and tutorials. Additionally Tourmaline Engine targets to have a large standard implementation for most day to day game development needs. Physics, shadows, sounds, video playback, OS agnostic IO interactions, etc...