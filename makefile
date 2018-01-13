all: clean clients server

CC=g++ -std=c++11
TUPLE_DIR= src/tuple/
CLIENT_DIR= src/client/
SERVER_DIR= src/server/
TUPLE_OBJS= src/tuple/TupleSpace.o src/tuple/Tuple.o src/tuple/Pattern.o
CLIENT_OBJS= src/client/Client.o
SERVER_OBJS= src/server/Server.o src/server/Request.o src/server/CMDParser.o

$(TUPLE_DIR)%.o: $(TUPLE_DIR)%.cpp $(TUPLE_DIR)%.h
	$(CC) -c -o $@ $<

$(CLIENT_DIR)%.o: $(CLIENT_DIR)%.cpp $(CLIENT_DIR)%.h $(TUPLE_OBJS)
	$(CC) -c -o $@ $<

$(SERVER_DIR)%.o: $(SERVER_DIR)%.cpp $(SERVER_DIR)%.h $(CLIENT_OBJS) $(TUPLE_OBJS)
	$(CC) -c -o $@ $<
server: $(SERVER_OBJS) $(CLIENT_OBJS) $(TUPLE_OBJS)
	$(CC) -o server main.cpp $^ -lpthread


clients: $(CLIENT_OBJS) $(TUPLE_OBJS)
	g++ -std=c++11 clients/ask10sForInt.cpp -o ask10sForInt $^ -lpthread -lrt
	g++ -std=c++11 clients/createIntAfter2s.cpp -o createIntAfter2s $^ -lpthread -lrt
	g++ -std=c++11 clients/timeoutAndInput.cpp -o timeoutAndInput $^ -lpthread -lrt
	g++ -std=c++11 clients/client2.cpp -o client2 $^ -lpthread -lrt
	g++ -std=c++11 clients/inputOutput.cpp -o inputOutput $^ -lpthread -lrt
	g++ -std=c++11 clients/output-input.cpp -o outputinput $^ -lpthread -lrt
	g++ -std=c++11 clients/interactive.cpp -o interactive $^ -lpthread -lrt
clean:
	rm -f server client *.o

#run:
#	./server ./client
