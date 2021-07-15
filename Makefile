CC=g++
C_FLAGS=-Wall

all:	main.o glad.o shader.o shaderProgram.o texture.o
	$(CC) $(C_FLAGS) main.o glad.o shader.o shaderProgram.o texture.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	
main.o: main.cpp
	$(CC) $(C_FLAGS) -Iinclude/ -c main.cpp

glad.o: src/glad.c
	$(CC) $(C_FLAGS) -Iinclude/ -c src/glad.c

shader.o: src/shader.cpp
	$(CC) $(C_FLAGS) -Iinclude/ -c src/shader.cpp

shaderProgram.o: src/shaderProgram.cpp
	$(CC) $(C_FLAGS) -Iinclude/ -c src/shaderProgram.cpp

texture.o: src/texture.cpp
	$(CC) $(C_FLAGS) -Iinclude/ -c src/texture.cpp

clean:
	rm *.o