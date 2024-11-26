CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lcrypto

TARGET = bsgs_hash160
SRC = bsgs_hash160.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
