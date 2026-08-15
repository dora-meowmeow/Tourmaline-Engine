# Tourmaline Engine

Tourmaline Engine is a game engine created for C++23.

- [Source Code](https://git.thenight.club/cat/Tourmaline-Engine/).
- [Visit the documentation website](https://docs.tourmalineengine.dev/).

### Currently the project is still trying to incorporate the following parts (in no particular order)

- [X] ECS (Builtin)
- [X] Logging (Builtin)
- [X] Random Generation (Builtin)
- [ ] Graphics (Magnum Graphics)
- [ ] Physics Integration (ReactPhysics3D)
- [ ] Audio (Builtin + miniaudio)
- [ ] Particles (Builtin)
- [ ] Video playback (Builtin + FFmpeg(subject to change))
- [ ] Asset Archiving (Searching for a library)
- [ ] UI (Magnum UI)
- [ ] Shaders (Builtin)

# Usability Status

Tourmaline is by no means currently usable. The project is incredibly volatile with constant changes and improvements. Please wait until a release is made.

However, if you just cannot help yourself, there is currently a temporary compile script [here](https://git.thenight.club/cat/TourmalineTests/src/branch/main/compile.sh) (there is also a debug variant [here](https://git.thenight.club/cat/TourmalineTests/src/branch/main/compileWithCoverage.sh)).

### !! These scripts are used to test Tourmaline, they are not meant to be the final way to compile Tourmaline

# 3rd Party Libraries Credits

- [Corrade/Magnum](https://magnum.graphics/) - graphics middleware by Vladimír "Mosra" Vondruš.
- [miniaudio](https://miniaud.io/) - audio playback and capture library by David "Mackron" Reid.
- [Xohiro](https://github.com/david-cortes/xoshiro_cpp/blob/master/xoshiro.h) implementation by David Blackman and Sebastiano Vigna.

# Special Thanks

- [Lars "harmonyisdead"](https://github.com/larsl2005) for hosting the thenight.club services and renting the tourmalineengine.dev and tourmalineengine.com domains.
- [Vladimír "Mosra" Vondruš](https://github.com/mosra) for their mentorship on my C++ knowledge/projects, and the people at the [magnum gitter channel](https://matrix.to/#/#mosra_magnum:gitter.im), for their continuous support and feedback on this project.
- [Goma "Buren"](https://git.thenight.club/buren) for proofreading, generally helping with the upkeep of the project, and being my other half.
- [Kae "voxelfoxkae"](https://voxelfox.co.uk/) for their mentorship.

# Not-so-frequently asked questions

### Versioning Scheme

Tourmaline uses a single digit versioning scheme that ticks up by 1 for each version. There are no major or minor updates or versions. The project can be seen as a rolling release project. There is yet to be a release.

### What is Tourmaline and Why is that the name?

From [Wikipedia](https://en.wikipedia.org/wiki/Tourmaline)
> Tourmaline is a crystalline silicate mineral group in which boron is compounded with elements such as aluminium, iron, magnesium, sodium, lithium, or potassium.

From [Find Gemstone](https://www.findgemstone.com/blog/what-is-tourmaline-used-for-in-industry/)
> Tourmaline has unique electric properties, including the ability to generate an electric charge as well as maintain electromagnetic radiation. These characteristics make tourmaline useful in the production of a range of electronics.

In short, Tourmaline is a very versatile "Gem". This is the name given to the third iteration of a project that once only concerned itself with Video playback on Magnum graphics.
