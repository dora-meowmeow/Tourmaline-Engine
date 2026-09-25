\page 04-Random 04. Random - Random value generation

Tourmaline defines all of its Random functions under `Tourmaline/Systems/Random.hpp`. So we will start by including the said header.

(for convenience sake, we will use namespace `Tourmaline` and `Tourmaline::Systems`)

```c++
#include <Tourmaline/Systems/Random.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main(){

  return 0;
}
```

# How does random generation work under the hood?

Under the hood Tourmaline uses `Xoshiro256PP` random number generator from [Xohiro.h](https://github.com/david-cortes/xoshiro_cpp/blob/master/xoshiro.h). This number generator is quite fast and generates numbers evenly.

# Generating a random number

It's fairly straight forward to generate random numbers, you can call \ref Tourmaline::Systems::Random::Generate function. It will generate a number based on the type of maximum and minimum argument.

\note Both maximum and minimum values are inclusive. So any generated number is in \f$ min \leq x \leq max \f$.

```c++
#include <Tourmaline/Systems/Random.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main() {
  // If you want to specify the exact type you want as output, you can do so
  // However this is completely unnecessary as C++ can figure this value out for
  // you.
  uint32_t unsignedInteger = Random::Generate<uint32_t>(10, 2);

  // Minimum is always assumed to be 0, if not specified otherwise.
  int32_t signedInteger = Random::Generate(5);
  float floatValue = Random::Generate(100.245f, 27.81f);
  double doubleValue = Random::Generate(1.0369459, 0.1929203);

  // Of course you can use variable as minimum and maximum values
  int32_t max = 50, min = 20;
  int32_t result = Random::Generate(max, min);

  return 0;
}
```

# Setting a seed

You can set the seed for the generator by \ref Tourmaline::Systems::Random::SetSeed. If not set, the default seed will be unix timestamp of when the program started running.

```c++
#include <Tourmaline/Systems/Logging.hpp>
#include <Tourmaline/Systems/Random.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;
int main() {
  Random::SetSeed(1);
  for (uint64_t x = 0; x < 3; x++) {
    Logging::LogFormatted("{}", "t", Logging::Info, Random::Generate(256.0));
  }

  return 0;
}
```

Output:

```bash
./build/program
 [Info@t] 184.8576
 [Info@t] 110.45376
 [Info@t] 189.33504
./build/program
 [Info@t] 184.8576
 [Info@t] 110.45376
 [Info@t] 189.33504
```

As you can see setting the seed to a number specified at compile time makes the program very deterministic. This is why choosing a good varying seed value is important.

# Generating other stuff

Currently only other thing you can generate randomly is a \ref Tourmaline::Type::UUID. You can do so by using \ref Tourmaline::Systems::Random::GenerateUUID.

```c++
#include <Tourmaline/Systems/Logging.hpp>
#include <Tourmaline/Systems/Random.hpp>
#include <Tourmaline/Types.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main() {
  Type::UUID randomUUID = Random::GenerateUUID();
  Logging::Log(randomUUID.asString(), "test");

  return 0;
}
```

Output:

```bash
./program
 [Info@test] 43AB4BBBCD8F47B8320D2B6719AB67C2
./program
 [Info@test] E9E6BB838FC4434744055021BF9FD702
```

That's all you need to know to use \ref Tourmaline::Systems::Random !

You can continue learning more about Tourmaline Engine at \ref examples.
