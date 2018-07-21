CC=gcc
CFLAGS=-std=c99 -o stella -g -Wall
TARGET=stella.c stella.h
stella:$(TARGET)
	$(CC) $(TARGET) $(CFLAGS)
