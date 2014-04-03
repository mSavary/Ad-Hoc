CC=g++
CFLAGS=-w -lboost_system -lboost_thread -lpthread
EXEC=AdHocNetworks_Linux_v6

all: $(EXEC)

AdHocNetworks_Linux_v6: Controller.o Node.o IPv6.o Route.o Destination.o Listener.o RoutingTable.o Message.o Tc.o Hello.o
	$(CC) -o AdHocNetworks_Linux_v6 Controller.o Node.o IPv6.o Route.o Destination.o Listener.o RoutingTable.o Message.o Tc.o Hello.o $(CFLAGS)

Controller.o: Controller.cpp
	$(CC) -o Controller.o -c Controller.cpp $(CFLAGS)

Node.o: Node.cpp Node.h
	$(CC) -o Node.o -c Node.cpp $(CFLAGS)

IPv6.o: IPv6.cpp IPv6.h
	$(CC) -o IPv6.o -c IPv6.cpp $(CFLAGS)

Route.o: Route.cpp Route.h
	$(CC) -o Route.o -c Route.cpp $(CFLAGS)

Destination.o: Destination.cpp Destination.h
	$(CC) -o Destination.o -c Destination.cpp $(CFLAGS)

Listener.o: Listener.cpp Listener.h
	$(CC) -o Listener.o -c Listener.cpp $(CFLAGS)

RoutingTable.o: RoutingTable.cpp RoutingTable.h
	$(CC) -o RoutingTable.o -c RoutingTable.cpp $(CFLAGS)

Message.o: Message.cpp Message.h
	$(CC) -o Message.o -c Message.cpp $(CFLAGS)

Tc.o: Tc.cpp Tc.h
	$(CC) -o Tc.o -c Tc.cpp $(CFLAGS)

Hello.o: Hello.cpp Hello.h
	$(CC) -o Hello.o -c Hello.cpp $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

				
