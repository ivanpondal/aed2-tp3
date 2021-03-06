# Compilador que será utilizado
CC=clang

# flags de compilación
CFLAGS=-g -Wall -O2

# flags de linkeo
LDFLAGS=-lstdc++

# Agrear acá los directorios a incluir en la compilación
INCDIR=. #../../../../../Modulos-CPP

# Agregar acá los archivos .cpp a compilar
SOURCES=test.cpp Red.cpp DCNet.cpp Driver.cpp

# Objetos que serán generados (no tocar)
OBJECTS=$(SOURCES:.cpp=.o)

# Nombre del ejecutable a generar
EXECUTABLE=test

all: $(SOURCES) tests

$(EXECUTABLE): clean $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

tests: $(EXECUTABLE)
	./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	valgrind --leak-check=full --track-origins=yes ./$(EXECUTABLE)

.cpp.o:
	$(CC) -I$(INCDIR) $(CFLAGS) $< -c -o $@
