CFLAGS = -g -std=c++11

COMPILE = g++ $(CFLAGS) -c

LINK = g++ $(CFLAGS)

LFLAGS = -lGL -lGLEW -lglut

<<<<<<< HEAD
main: main.cpp ./headers/Shader.h ./headers/VertexBuffer.h ./shaders/shader.vs ./shaders/shader.fs
=======
main: main.cpp ./headers/Shader.h ./headers/VertexBuffer.h ./headers/Objects.h ./shaders/shader.vs ./shaders/shader.fs
>>>>>>> parent of 53ab33f... basic 3dCA working
	$(LINK) main.cpp $(LFLAGS) -o main
run: main
	./env.sh & ./main
clean:
	rm -f main main.o