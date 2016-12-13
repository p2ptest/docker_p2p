INCLUDE=-I/usr/include/mysql/
LIBS=-L$/usr/local/mysql/lib 
CFLAGS = -g -Wall -fPIC
TARGET=../lib/libjson.a
		
BINARY = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all:$(TARGET)
	@cp *.h ../include/json

%.o:%.cpp
	$(CXX) $< -o $@ -c $(INCLUDE) $(CFLAGS) 

clean:
	@rm -rf ${BINARY} ${TARGET}
	@rm -f ../include/json/*

$(TARGET):$(BINARY)
	ar rcs $@ $^
