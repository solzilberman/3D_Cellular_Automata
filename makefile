CFLAGS = -g -std=c++11

COMPILE = g++ $(CFLAGS) -c

LINK = g++ $(CFLAGS)

LFLAGS = -lGL -lGLEW -lglut -pthread

main: main.cpp ./headers/Shader.h ./headers/VertexBuffer.h ./headers/Objects.h ./headers/Camera.h ./headers/WireFrame.h ./headers/TimerCA.h ./shaders/shader.vs ./shaders/shader.fs
	$(LINK) main.cpp $(LFLAGS) -o main
run: main
	. ./utilities/env.sh & ./main
clean:
	rm -f main main.o
