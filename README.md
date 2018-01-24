# Fully contained, portable, readable, constexpr fizzbuzz


## Why?

Because sometimes you want your fizzbuzz lightning fast&dagger; on any computer&Dagger;

&dagger;Compilation time may be extremely long (several minutes)

&Dagger;Tested only to work on VS 17 (up to FizzBuzz<497> due to template limitations) and GCC 7+ (up to FizzBuzz<285> until GCC crashed)

## No but really, why?

I saw a few other compile time fizzbuzzes but they all had some weird quirk: being completely unreadable, requiring external libraries, using variadic templates (well that's not that bad, but it can exhaust memory faster and could take even longer compilation time), printing to a stream every template (which just makes it a chain of prints rather than a single string) or just have a really weird hack like having a constant pool of memory.

I wondered if it was possible to make a ~~better~~ different version.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

Hat tip to Stackoverflow as always.