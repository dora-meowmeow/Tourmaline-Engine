# Tourmaline Engine

Tormaline Engine is a game engine created for game development with C++23. 

A lot of the info regarding Tourmaline has been removed from here. This is due to project still being in early phases of development.

# 3rd Party Used Libraries Credits
- [Corrade/Magnum](https://magnum.graphics/) - graphics middleware by Vladimír "Mosra" Vondruš.
- [miniaudio](https://miniaud.io/) - audio playback and capture library by David "Mackron" Reid.
- [Xohiro](https://github.com/david-cortes/xoshiro_cpp/blob/master/xoshiro.h) implementation by David Blackman and Sebastiano Vigna

# What is Tourmaline and Why is that the name?
From [Wikipedia](https://en.wikipedia.org/wiki/Tourmaline)
> Tourmaline is a crystalline silicate mineral group in which boron is compounded with elements such as aluminium, iron, magnesium, sodium, lithium, or potassium. 

From [Find Gemstone](https://www.findgemstone.com/blog/what-is-tourmaline-used-for-in-industry/)
> Tourmaline has unique electric properties, including the ability to generate an electric charge as well as maintain electromagnetic radiation. These characteristics make tourmaline useful in the production of a range of electronics.

In short Tourmaline is a very versatile "Gem". This name is given to the third iteration of a project that once only concern itself with Video playback on Magnum graphics.

## Short History
Initially this project was called **MagnumVideo**, as goal was to allow video playback on [Magnum Graphics](https://magnum.graphics/). Due to Magnum Graphic's OpenAL wrapper being very poorly, made the project required me to include an audio engine with the video playback. 
> Original MagnumVideo was able to implement basic video playback with static frame time

So the project grew into **Overcharged Toolset** (MagnumVideo -> ChargeVideo), with goal to extend Magnum with Seamless Video, Audio, ECS, Shader, Particle, etc... systems. Each package aimed to be independent out of the box and only require each other if desired. However this was incredibly unsustainable and made development enjoyability and API quality very poor. 
> ChargeVideo improved upon MagnumVideo with more supported types and PTS based timing. ChargeAudio was added to allow audio playback with video synced.

Around the time that Overcharged has been having an identity crisis, a dear friend in a libera IRC chat has introduced me to the wonderful world of [Ruby](https://www.ruby-lang.org/en/). I spent few days trying to combine two of my favourite things ever (Ruby's ease, and C++'s speed & control), this has led me to finally ditch Overcharged and start with a clean slate.

Therefore the name is basically "Overcharged + Ruby (a gem) = Tourmaline (Gem used to make electronics)" )
