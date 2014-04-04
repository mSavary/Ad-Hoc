CC=g++
CFLAGS=-w -lboost_system -lboost_thread -lpthread
EXEC=AdHocNetworks_Linux_v6

all: $(EXEC)

AdHocNetworks_Linux_v6: src/Controller.o src/Node.o src/IPv6.o src/Route.o src/Destination.o src/Listener.o src/RoutingTable.o src/Message.o src/Tc.o src/Hello.o
	$(CC) -o AdHocNetworks_Linux_v6 src/Controller.o src/Node.o src/IPv6.o src/Route.o src/Destination.o src/Listener.o src/RoutingTable.o src/Message.o src/Tc.o src/Hello.o $(CFLAGS)

src/Controller.o: src/Controller.cpp
	$(CC) -o src/Controller.o -c src/Controller.cpp $(CFLAGS)

src/Node.o: src/Node.cpp src/Node.h
	$(CC) -o src/Node.o -c src/Node.cpp $(CFLAGS)

src/IPv6.o: src/IPv6.cpp src/IPv6.h
	$(CC) -o src/IPv6.o -c src/IPv6.cpp $(CFLAGS)

src/Route.o: src/Route.cpp src/Route.h
	$(CC) -o src/Route.o -c src/Route.cpp $(CFLAGS)

src/Destination.o: src/Destination.cpp src/Destination.h
	$(CC) -o src/Destination.o -c src/Destination.cpp $(CFLAGS)

src/Listener.o: src/Listener.cpp src/Listener.h
	$(CC) -o src/Listener.o -c src/Listener.cpp $(CFLAGS)

src/RoutingTable.o: src/RoutingTable.cpp src/RoutingTable.h
	$(CC) -o src/RoutingTable.o -c src/RoutingTable.cpp $(CFLAGS)

src/Message.o: src/Message.cpp src/Message.h
	$(CC) -o src/Message.o -c src/Message.cpp $(CFLAGS)

src/Tc.o: src/Tc.cpp src/Tc.h
	$(CC) -o src/Tc.o -c src/Tc.cpp $(CFLAGS)

src/Hello.o: src/Hello.cpp src/Hello.h
	$(CC) -o src/Hello.o -c src/Hello.cpp $(CFLAGS)

clean:
	rm -rf src/*.o

mrproper: clean
	rm -rf $(EXEC)

				
