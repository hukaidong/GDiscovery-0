.PHONY: all
all:
	-g++ main.cpp --std=c++11 -O3 -o main.app
	-g++ ptnsort.cpp --std=c++11 -O3 -o sort.app

.PHONY: debug
debug:
	-g++ main.cpp --std=c++11 -ggdb -Wall -DDEBUG -o main.app
	-g++ ptnsort.cpp --std=c++11 -ggdb -Wall -o sort.app

.PHONY: clean
clean:
	-rm -rf *.out *.app
