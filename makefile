CC=g++

make: src/main.cpp src/hittables/triangle_mesh.cpp src/hittables/space_divider.cpp
	$(CC) src/main.cpp src/hittables/triangle_mesh.cpp src/hittables/space_divider.cpp -pthread -std=c++11 -g -o build/main
