OBJECTS= ./build/robotsParser.o ./build/downloader.o

all: ${OBJECTS}
	g++ ./test.cpp ${OBJECTS} -g -o ./bin/main -lcurl
./build/robotsParser.o:
	g++ ./RobotsParser.cpp -o ./build/robotsParser.o -g -c
./build/downloader.o:
	g++ ./Downloader.cpp -o ./build/downloader.o -g -c -lcurl
clean:
	rm ./bin/main
	rm -rf ${OBJECTS}
