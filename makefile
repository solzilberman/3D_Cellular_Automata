CFLAGS = -g -std=c++11

COMPILE = g++ $(CFLAGS) -c

LINK = g++ $(CFLAGS)

LFLAGS = -lGL -lGLEW -lglut

main: main.cpp ./headers/Shader.h ./headers/VertexBuffer.h ./shaders/shader.vs ./shaders/shader.fs
	$(LINK) main.cpp $(LFLAGS) -o main
run: main
	./env.sh & ./main
clean:
	rm -f main main.o