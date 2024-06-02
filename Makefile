CC = g++
CFLAGS = -Wall -std=c++11

SRCDIR = src
INCDIR = include

# Listar todos los archivos fuente excepto cliente.cpp y servidor.cpp
COMMON_SOURCES = $(filter-out $(SRCDIR)/cliente.cpp $(SRCDIR)/servidor.cpp, $(wildcard $(SRCDIR)/*.cpp))
COMMON_OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(SRCDIR)/%.o, $(COMMON_SOURCES))

# Agregar archivos específicos de cliente y servidor
CLIENT_SOURCES = $(SRCDIR)/cliente.cpp
CLIENT_OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(SRCDIR)/%.o, $(CLIENT_SOURCES))

SERVER_SOURCES = $(SRCDIR)/servidor.cpp
SERVER_OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(SRCDIR)/%.o, $(SERVER_SOURCES))

HEADERS = $(wildcard $(INCDIR)/*.h)

EJECUTABLES = servidor cliente

all: $(EJECUTABLES)

servidor: $(SERVER_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

cliente: $(CLIENT_OBJECTS) $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR)

clean:
	rm -f $(SRCDIR)/*.o $(EJECUTABLES)
