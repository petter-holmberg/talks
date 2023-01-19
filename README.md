Programming with Concepts
-------------------------

A talk for Sweden C++ Stockholm 0x25, held at Atlas Copco Tools on January 19, 2023

2023 is the year of a new C++ ISO standard, with many new features to learn about as we are still struggling to get a grasp of previous ones. Concepts was one of the major additions to C++20 and is starting to see real-world usage outside of the standard library. But how do you get started using concepts in your own code?

This talk is an attempt at answering this question, based on my personal experience with concepts over the last few years. It is not a deep dive into syntax and technical details but focuses more on how to think about concepts and how they interact with your code.

Generisk Programmering
----------------------

A breakfast talk in Swedish for Biolit, held at Urban Deli on March 30, 2022

Includes generic.py with some supplementary Python 3 code. Released under The Unlicense.

Att skriva kod som effektivt löser specifika problem och samtidigt är maximalt återanvändbar är ingen lätt uppgift. Ofta kan det kännas som en nästan omöjlig kombination. Generisk programmering är ett angreppssätt för att uppnå det målet genom att skriva om algoritmer för konkreta problem så att de passar in i ett större sammanhang. Metoden är inspirerad av en flera tusen år gammal matematisk tradition.

I det här seminariet ska vi titta på ett exempel, där lösningen på ett av tidernas äldsta räkneproblem har vidareutvecklats till kod för att elegant lösa några av dagens viktigaste mjukvaruuppgifter. Vägen dit är också en spännande historia om några av de personer som har bidragit till att hitta lösningen genom att utmana gamla förlegade idéer.

Functional Parsing in C++20
---------------------------

A talk for Sweden C++ Stockholm 0x1F, held at Atlas Copco Tools on February 24, 2022

Includes wirth-parser, a parser generator, tested on x86-64 with GCC 10.3, Clang 11.0, and MSVC22.17. Released under The Unlicense.

https://www.meetup.com/StockholmCpp/events/283856715/

Is C++20 a language that supports a functional style of programming?
Can we write modern C++ code in a pure functional style that would easily translate into a pure functional language like Haskell, and could that C++ code end up looking just as nice while still being reasonably efficient?
In this talk we will take a practical approach and apply ideas from functional programming to a common and non-trivial problem - parsing strings - and develop a small pure functional parsing library from the ground up. On the way we will encounter many nice features from C++20 that, while optional, make this task a lot easier and results in code that can compete with functional languages for clarity and expressiveness.
This talk does not assume theoretical knowledge of functional programming concepts or practical experience with a functional language. You also don't need to know how to write parsers or have many hours of C++20 under your belt.

Ancient Math / Modern C++
-------------------------

A talk for Sweden C++ Stockholm 0x17, held at Berotec on June 13, 2019

Includes math.cpp, a demonstration of the presented concepts, tested on x86-64 with GCC 8.3, Clang 8.0, and MSVC19.20. Released under The Unlicense.

https://www.meetup.com/StockholmCpp/events/261046629/

What math should you know to become a better C++ programmer, and how can you use C++ to become better at math?
Let’s revisit some basic facts about numbers known since antiquity and taught to us in school a long time ago, but from a modern C++ programmer’s perspective!

The (dark) Art of Type Functions
--------------------------------

A lightning talk for Sweden C++ Stockholm 0x11, held at Berotec on November 22, 2018

https://www.meetup.com/StockholmCpp/events/255827241/

From type to concept
--------------------

A talk for Sweden C++ Stockholm 0x06, held at Tritech on September 14, 2017

https://www.meetup.com/StockholmCpp/events/242770874/

Concepts is the first new major language feature to make it into the draft C++20 standard. With concepts generic programming becomes easier, safer and alot more fun. In this talk we will discover what concepts are and why they are useful. We will also implement a concept that is fundamental to understanding C++ programs, and you will discover that concepts were there in your code all along, whether you knew about them or not.
