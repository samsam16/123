cp moneta_code.cpp build/code.cpp
g++-9 -O0 -c -Wall -Werror -pg      -I/course/cse141pp-archlab/pcm -pthread  -I/course/cse141pp-archlab/libarchlab -I/course/cse141pp-archlab -I/usr/local/include -I/googletest/googletest/include -I/course/CSE141pp-SimpleCNN -I./build/ -I/home/jovyan/work/moneta/ -std=gnu++11  build/code.cpp -o build/code.o
g++-9 -O0 -c -Wall -Werror -pg      -I/course/cse141pp-archlab/pcm -pthread  -I/course/cse141pp-archlab/libarchlab -I/course/cse141pp-archlab -I/usr/local/include -I/googletest/googletest/include -I/course/CSE141pp-SimpleCNN -I./build/ -I/home/jovyan/work/moneta/ -std=gnu++11  main.cpp -o main.o
g++-9 -O0 build/code.o main.o    -pthread  -L/usr/local/lib -L/course/cse141pp-archlab/libarchlab -L/course/cse141pp-archlab/pcm -larchlab -lpcm -lpapi -lboost_program_options  -pg -o code.exe

