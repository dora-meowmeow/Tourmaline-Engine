\page 02-Logging 02. Logging - How to write log messages

Tourmaline defines all of its Logging functions under `Tourmaline/Systems/Logging.hpp`. So we will start by including the said header.

(for convenience sake, we will use namespace `Tourmaline` and `Tourmaline::Systems`)

```c++
#include <Tourmaline/Systems/Logging.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main(){

  return 0;
}
```

# Non-formatted logging

We can log by using \ref Tourmaline::Systems::Logging::Log function. This function takes the string to print, where the string originates from, the severity level, and lastly if there is a condition that needs to be **true** for it to print.

```c++
#include <Tourmaline/Systems/Logging.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

bool isStarted = false;

int main(){
  // You can additionally use Logging::LogLevel::Info
  Logging::Log("Hello Tourmaline", "Test1", Logging::Info, isStarted);
  isStarted = true;
  Logging::Log("Hello Tourmaline", "Test2", Logging::Info, isStarted);
  Logging::Log("Lazy logging -3-");

  return 0;
}
```

Output:

```bash
 [Info@Test2] Hello Tourmaline
 [Info@Unknown] Lazy logging -3-
```

As you can see only Test2 ran since `isStarted` was `false`. This is really useful when you want to output a warning.

While you can lazily log as shown above, this way of logging is strongly discouraged. We suggest specifying where a log message coming from.

# Formatted logging

Using  \ref Tourmaline::Systems::Logging::LogFormatted. We can log integers, floats, boolean, pointers, other strings etc... It is near identical to `std::format` from C++20.
This function uses [Corrade::Utility::format](https://doc.magnum.graphics/corrade/namespaceCorrade_1_1Utility.html#a6ed9378fb78a4da408fd5154a9d9a308) to format strings, please read the documentation for it to learn its full capacity.

\note Unlike \ref Tourmaline::Systems::Logging::Log, \ref Tourmaline::Systems::Logging::LogFormatted does not support the last condition argument. It will always send.

```c++
#include <Tourmaline/Systems/Logging.hpp>
#include <string>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

class x {
public:
  int coolNumber = 5;
  double twoPi = 6.18;
  char favouriteLetter = 'w';
  std::string someWords = "Tourmaline is cool!";
};

bool isStarted = false;

int main() {
  Logging::Log("Hello Tourmaline", "Test1", Logging::Info, isStarted);
  isStarted = true;
  Logging::Log("Hello Tourmaline", "Test2", Logging::Info, isStarted);
  Logging::Log("Lazy logging -3-");

  x Thing;
  Thing.coolNumber = 6;
  // LogFormatted does not support lazing logging
  Logging::LogFormatted("Here is some info x.coolNumber = {}, x.twoPi = {}, "
                        "x.favouriteLetter {:c}, x.someWords {}.",
                        "Test3", Logging::Info, Thing.coolNumber, Thing.twoPi,
                        Thing.favouriteLetter, Thing.someWords);

  Logging::LogFormatted("isStarted == true? {}", "Test4", Logging::Info,
                        isStarted == true);

  return 0;
}
```

Output:

```bash
 [Info@Test2] Hello Tourmaline
 [Info@Unknown] Lazy logging -3-
 [Info@Test3] Here is some info x.coolNumber = 6, x.twoPi = 6.18, x.favouriteLetter w, x.someWords Tourmaline is cool!.
 [Info@Test4] isStarted == true? 1
```

# Writing to a file

If you wish to write outputs to both console and a file. You can use the \ref Tourmaline::Systems:Logging::LogToFile function. If you don't specify a file path, it will write the file where ever the program file is with the name `Tourmaline-Year-Month-Day.txt`.

```c++
#include <Tourmaline/Systems/Logging.hpp>
#include <string>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

class x {
public:
  int coolNumber = 5;
  double twoPi = 6.18;
  char favouriteLetter = 'w';
  std::string someWords = "Tourmaline is cool!";
};

bool isStarted = false;

int main() {
  // When path unspecified, the log file will be called ./Tourmaline-Year-Month-Day.txt
  Logging::LogToFile();

  Logging::Log("Hello Tourmaline", "Test1", Logging::Info, isStarted);
  isStarted = true;
  Logging::Log("Hello Tourmaline", "Test2", Logging::Info, isStarted);
  Logging::Log("Lazy logging -3-");

  x Thing;
  Thing.coolNumber = 6;
  Logging::LogFormatted("Here is some info x.coolNumber = {}, x.twoPi = {}, "
                        "x.favouriteLetter {:c}, x.someWords {}.",
                        "Test3", Logging::Info, Thing.coolNumber, Thing.twoPi,
                        Thing.favouriteLetter, Thing.someWords);

  Logging::LogFormatted("isStarted == true? {}", "Test4", Logging::Info,
                        isStarted == true);

  return 0;
}
```

Output:

```bash
./program
 [Info@Test2] Hello Tourmaline
 [Info@Unknown] Lazy logging -3-
 [Info@Test3] Here is some info x.coolNumber = 6, x.twoPi = 6.18, x.favouriteLetter w, x.someWords Tourmaline is cool!.
 [Info@Test4] isStarted == true? 1

cat Tourmaline-2026-9-24.txt 
[Info@Test2] Hello Tourmaline
[Info@Unknown] Lazy logging -3-
[Info@Test3] Here is some info x.coolNumber = 6, x.twoPi = 6.18, x.favouriteLetter w, x.someWords Tourmaline is cool!.
[Info@Test4] isStarted == true? 1
```

# Logging Error and Critical

## Logging::Error

If any log that is in severity Logging::Error gets triggered, It will also throw with `std::runtime_error`. You can catch this error and correct your code accordingly.

```c++
#include <Tourmaline/Systems/Logging.hpp>
#include <stdexcept>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main() {
  try {
    Logging::Log("Something terrible happened! :(", "Test5", Logging::Error);
  } catch (std::runtime_error e) {
    Logging::Log("Thankfully we catched the error! :D", "Test5",
                 Logging::Debug);
  }
  return 0;
}
```

Output:

```bash
 [Error@Test5] Something terrible happened! :(
 [Debug@Test5] Thankfully we catched the error! :D
```

## Logging::Critical and Logging::TerminationFunction()

If any log that is in severity Logging::Critical gets triggered, the program will call `std::terminate`! However to make sure you don't randomly lose data, right before `std::terminate` is called we call \ref Tourmaline::Systems::Logging::TerminationFunction.

By itself \ref Tourmaline::Systems::Logging::TerminationFunction doesn't do anything, however each program can define what it should do.

```c++
#include <Tourmaline/Systems/Logging.hpp>

using namespace Tourmaline;
using namespace Tourmaline::Systems;

int main() {
  // You can also give a function pointer as long as it is void()()
  Logging::TerminationFunction = []() {
    Logging::Log("Doing stuff before termination...");
    // Maybe use Tourmaline::Systems::Serialization to save data here...
  };
  Logging::Log("Something irrecoveribly broke!! Terminating! D:", "Test6",
               Logging::Critical);

  return 0;
}
```

Output:

```
 [Critical@Test6] Something irrecoveribly broke!! Terminating! D:
 [Info@Unknown] Doing stuff before termination...
terminate called after throwing an instance of 'std::runtime_error'
  what():  [Critical@Test6] Something irrecoveribly broke!! Terminating! D:

Aborted                    ./program
```

That's all you need to know to use \ref Tourmaline::Systems::Logging !

You can continue learning more about Tourmaline Engine at \ref examples.
