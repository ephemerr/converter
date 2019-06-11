TARGET=conversion

all: main.cpp
	g++ --std=c++14 main.cpp -o $(TARGET)
	
run: all
	./$(TARGET)

test:
	cat in | ./$(TARGET) > output 2>&1
	diff -a --suppress-common-lines -y ./output ./out
	
debug:
	g++ -DDEBUG --std=c++14 main.cpp -o $(TARGET)
