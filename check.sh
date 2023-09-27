#!/bin/bash
g++ -Wall -std=c++17 gen.cpp && ./a.out && rm a.out
g++ -Wall -std=c++17 line.cpp && ./a.out < line.in > my.out && rm a.out
diff my.out line.out
if (($? == 0))
then
	echo "Correct!"
fi
