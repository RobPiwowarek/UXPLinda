all: clean client server
files= src/server/Server.cpp src/server/Server.h src/tuple/TupleSpace.cpp src/tuple/TupleSpace.h src/tuple/Tuple.cpp src/tuple/Tuple.h src/tuple/Pattern.cpp src/tuple/Pattern.h src/server/Request.cpp src/server/Request.h src/server/CMDParser.cpp src/server/CMDParser.h
clientFiles= src/client/Client.cpp src/client/Client.h   src/tuple/TupleSpace.cpp src/tuple/TupleSpace.h src/tuple/Tuple.cpp src/tuple/Tuple.h src/tuple/Pattern.cpp src/tuple/Pattern.h src/server/Request.cpp src/server/Request.h 
server:
	g++ -std=c++11 main.cpp  $(files) -o server -lpthread -lrt

client:
	g++ -std=c++11 clients/client.cpp  $(clientFiles) -o client -lpthread -lrt
	g++ -std=c++11 clients/client2.cpp $(clientFiles)  -o client2 -lpthread -lrt

clean:
	rm -f server client *.o

#run:
#	./server ./client