# Makefile for C project

# Source file
SRC = aurc.c

# Binary file
BIN = aurc

# Base location
PREFIX ?= /usr/

# Binary Folder
INSTALLPATH = ${DESTDIR}${PREFIX}/bin

# Compiler
CC = gcc

install : build

	@echo "installing binary file to ${INSTALLPATH}/${BIN}"
	@cp build/${BIN} ${INSTALLPATH}
	@chmod 0755 ${INSTALLPATH}/${BIN}
	@echo "installed"

# Clean build files
clean:
	rm -rf build

# Build the project
build:
	mkdir build
	$(CC) -o build/$(BIN) $(SRC)

# Run the project
run: build
	./$(BIN)
