\page status Roadmap and Project Status

# Roadmap

Currently the project is still trying to incorporate the following parts (in no particular order):

## Done

- [X] ECS (Builtin)
- [X] Logging (Builtin)
- [X] Random Generation (Builtin)
- [X] Serialization (Builtin)

## To-do

- [ ] Graphics (Magnum Graphics)
- [ ] Physics Integration (ReactPhysics3D)
- [ ] Audio (Builtin + miniaudio)
- [ ] Particles (Builtin)
- [ ] Video playback (Builtin + FFmpeg(subject to change))
- [ ] Asset Archiving (Searching for a library)
- [ ] UI (Magnum UI)
- [ ] Shaders (Builtin)

\note Just because something is marked as "done", it does not mean that they aren't been worked on anymore. Done here just means that it is in a usable state beta state.

# Usability Status

Tourmaline is by no means currently usable. The project is incredibly volatile with constant changes and improvements. Please wait until a release is made.

However, if you just cannot help yourself; After you compile and install Tourmaline, you can compile and link with the following command:

```bash
g++ main.cpp `pkg-config Tourmaline --cflags --libs` -o program # you could use pkgconf instead
```

Do keep in mind that the command is order-sensitive. You want your implementation files (`*.cpp`) BEFORE the pkg-config command.

\warning !! This is not meant to be the final way to compile Tourmaline. Everything here is subject to change !!
