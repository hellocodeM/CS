cpp = clang++
CPPFLAGS = -std=c++11

TARGET = cs
TESTSTARGET = test

all: $(OBJS)
	$(cpp) $(CPPFLAGS) -o $(TARGET) $(OBJS)

test: all $(TESTS)
	$(cpp) -o $(TESTTARGET) $(CPPFLAGS) $(TESTS)
	./$(TESTTARGET)

clean:
	rm ./*.o
	rm ./$(TARGET)
	rm ./$(TESTTARGET)
