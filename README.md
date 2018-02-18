# eighteight

## Introduction
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
* And probably a bunch more. I'm nto counting, but I'll update this with the resources I most used and rank them in terms of time spent reading them.

## Design notes.

So, at the outset my design is something like a hierarchy. A system class (Chip8) which ties together the various components. These components are the CPU, the Memory, IO, and the Display. It's debatable whether some of these actually need separate objects, or would be better served with structs. The disadvantage of using classes for each object is that I'd have to pass object pointers around, which on its face sounds inefficient, but it also feels very much like how the traces on a circuit board work. For example:

* Chip8 class is the circuit board, providing the user facing interface (power control, basic initialization, and wiring things together)
* Object pointers passed to constructors would represent the traces on the board between various components. Would establish firmer separation of concerns.
* Each component class would be focused on its own concerns. Memory would read, CPU would interpret, etc.

Now, I'm sure there are downsides to this, and I'll probably bump into them, but I want to see how it feels  doing it this way. If it doesn't work, I'll change it. :)

## Naming conventions

There are far too many C++ styles to choose from. Coming from C#, Python, JS etc. I'm inclined to use CamelCase, however I'm tweaking things depending on visibility issue. So far I'm working as follows:

* ThisIsAClassName
* thisIsAMemberName
* thisIsAConstStaticMember
* _thisIsAPrivateMemberName
* this_is_a_local_variable

I can see the benefit in the hungarian style that older MS software uses, but I think it's too cramped in some ways.

### A note about abbreviations

If the length of an abbreviation is 1 or 2 characters then they can both be uppercase, any longer and it's CamelCase. The primary benefit is that it's more pleasing on the eye, eg. register names.