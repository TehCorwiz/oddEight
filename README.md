# oddEight

## Introduction

### Goals

This is a learning project. The goals are:

* Develop a playable Chip8 interpreter.
* To expand knowledge of emulator design.
* To learn modern C++ (11 - 17)
* Have fun!

Resources used in this project:

* http://en.cppreference.com/w/cpp/language
* http://www.emulator101.com/welcome.html
* https://en.wikipedia.org/wiki/CHIP-8
* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
* http://www.stroustrup.com/Programming/PPP-style.pdf
* https://isocpp.org/faq
* https://github.com/dmatlack/chip8/tree/master/roms (Just for the roms, I haven't looked at the code. :D )
* And probably a bunch more. I'm not counting, but I'll update this with the resources I most used and rank them in terms of time spent reading them.

### Back story

It's been a while since I learned a new language. I wouldn't consider c++ to be a new language to me, but it has been over a decade since I did any significant tinkering with it. About three years ago I had to modify something to read from a named pipe instead of a file descriptor which is fairly trivial on win32. Suffice to say I'm rusty enough and enough has changed that I'll call it a new language. And while I've done some C embedded tinkering in the last few years, that's a whole separate world as far as working environment limitations and build systems go.

Recently work has had me writing Ruby, PHP, and JS primarily. But personally I've developed a preference for static typing, both from a code clarity standpoint as well as the fact that it lets my tools do static analysis, smart autocomplete, etc. I feel more productive and spend less time trying to reason about or catch invalid types.

## Design notes.

So, at the outset my design is something like a hierarchy. A system class (Chip8) which ties together the various components. These components are the CPU, the Memory, IO, and the Display. It's debatable whether some of these actually need separate objects, or would be better served with structs. The disadvantage of using classes for each object is that I'd have to pass object pointers around, which on its face sounds inefficient, but it also feels very much like how the traces on a circuit board work. For example:

* Chip8 class is the circuit board, providing the user facing interface (power control, basic initialization, and wiring things together)
* Object pointers passed to constructors would represent the traces on the board between various components. Would establish firmer separation of concerns.
* Each component class would be focused on its own concerns. Memory would read, CPU would interpret, etc.

Now, I'm sure there are downsides to this, and I'll probably bump into them, but I want to see how it feels  doing it this way. If it doesn't work, I'll change it. :)

## Code Style Guide

### Naming conventions

There are far too many C++ styles to choose from. Coming from C#, Python, JS etc. I'm inclined to use CamelCase, however I'm tweaking things depending on visibility issues. So far I'm working as follows:

* ThisIsAClassName
* thisIsAMemberName
* thisIsAConstStaticMember
* _thisIsAPrivateMemberName
* this_is_a_local_variable

I can see the benefit in the hungarian style that older MS software uses, but I think it's too cramped in some ways.

#### A note about abbreviations

If the length of an abbreviation is 1 or 2 characters then they can both be uppercase, any longer and it's CamelCase. The primary benefit is that it's more pleasing on the eye, eg. register names.

## General Thoughts

So, I wanted to touch upon a few ideas I keep in mind, practices I've picked up over the years. These are aphorisms, and of course there are exceptions to each one.

### Once, twice, refactor.

After you've repeated a pattern multiple times it's probably time to break off and formalize that pattern. If I think of an exception I'll include it. The simplest example of this is creating functions to hold general code (Chip8::_fileSize function for example). A more complex example was a home rolled e-commerce platform that had multiple search methods for the same data on the same page. The takeaway should be that code should move to where it helps you. Sometimes this means creating an abstract class and subclassing your objects to provide a consistent interface, or maybe creating entirely new stand-alone services.

### Avoid abstraction for the sake of abstraction. 

This is the dramatic foil to "Once, Twice, Refactor". Don't abstract something just because you can. Keep your code as close as is comfortable to the data it operates on.

### Don't solve problems you don't have.

Lots, and I mean LOTS of developers I've worked with like to solve scaling problems that they'll never have, and create clever solutions to what-ifs that never come about. Solve the problems you have, then stop. If you know that you'll run into a problem, or a feature is on the roadmap then by all means take it into account when designing your system components but don't implement that until necessary. Don't build a boat in the desert just because you see a mirage of water off in the distance, you'll end up carrying that boat for nothing.
