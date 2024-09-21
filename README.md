From terminal -> Make "build" directory, run "cmake .." and "make" within it
Compile with g++ color-reduct.cpp -o cr `pkg-config --cflags --libs opencv4`
run with ./cr pizza.png 40