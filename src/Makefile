cpp = clang++
CPPFLAGS = -std=c++11

TARGET = CS
TESTTARGET = test

all: $(OBJS)
	$(cpp) $(CPPFLAGS) CS.cc -o $(TARGET) 

test: all
	$(cpp) $(CPPFLAGS) test.cc -o $(TESTTARGET)
	./$(TESTTARGET)

clean:
	rm ./$(TARGET)
	rm ./$(TESTTARGET)
