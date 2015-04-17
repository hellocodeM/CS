cpp = clang++
CPPFLAGS = -std=c++11

OBJS = lexparser.o
TARGET = cs
TESTS = test-lexparser.o

all: $(OBJS)
	$(cpp) $(CPPFLAGS) -o $(TARGET) $(OBJS)

test: all $(TESTS)
	$(cpp) -o test $(CPPFLAGS) $(TESTS)
	./test

clean:
	rm ./*.o
	rm ./$(TARGET)
