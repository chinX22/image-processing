From terminal -> Make "build" directory, run "cmake .." and "make" within it
Compile with g++ quantize.cpp  `pkg-config --cflags --libs opencv4`
Run with:
./a.out I/V (for image or video) path/to/source (or 0 if webcam) divisor output_name if using video file input or if you want image output