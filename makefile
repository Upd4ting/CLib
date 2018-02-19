LIB = $(HOME)/Ecole/CLib
APP = $(HOME)/Ecole/App

GPP = g++ -lssl -lcrypto -lpthread -m64 -Wall -std=c++11 -D SUN -I $(LIB)

OBJ_LIBRARY = Input.o CSVFile.o PropFile.o Menu.o Logger.o Socket.o Server.o Client.o SocketUDP.o SocketTCP.o ServerTCP.o ServerUDP.o PTServer.o ClientUDP.o ClientTCP.o

#OBJ_SERVER = servermain.o
OBJ_CLIENT = json.o clientmain.o

.SILENT:

#all: Server Client
all: Client

#Compil: $(OBJ_LIBRARY)
#	echo 'Compilation basique'
#"	$(GPP) -o Compil $(OBJ_LIBRARY)

# Compilation des applications
#Server: $(OBJ_LIBRARY) $(OBJ_SERVER)
#	echo 'Compilation du server'
#	$(GPP) -o Server $(OBJ_LIBRARY) $(OBJ_SERVER)

Client: $(OBJ_LIBRARY) $(OBJ_CLIENT)
	echo 'Compilation du client'
	$(GPP) -o Client $(OBJ_LIBRARY) $(OBJ_CLIENT)

# Compilation OBJ DE LA LIB
Input.o: $(LIB)/Input.cpp $(LIB)/Input.h
	$(GPP) -c $(LIB)/Input.cpp

CSVFile.o: $(LIB)/CSVFile.cpp $(LIB)/CSVFile.h
	$(GPP) -c $(LIB)/CSVFile.cpp

PropFile.o: $(LIB)/PropFile.cpp $(LIB)/PropFile.h
	$(GPP) -c $(LIB)/PropFile.cpp

Menu.o: $(LIB)/Menu.cpp $(LIB)/Menu.h
	$(GPP) -c $(LIB)/Menu.cpp

Logger.o: $(LIB)/Logger.cpp $(LIB)/Logger.h
	$(GPP) -c $(LIB)/Logger.cpp

Socket.o: $(LIB)/Socket.cpp $(LIB)/Socket.h
	$(GPP) -c $(LIB)/Socket.cpp

Server.o: $(LIB)/Server.cpp $(LIB)/Server.h
	$(GPP) -c $(LIB)/Server.cpp

Client.o: $(LIB)/Client.cpp $(LIB)/Client.h
	$(GPP) -c $(LIB)/Client.cpp

PTServer.o: $(LIB)/PTServer.cpp $(LIB)/PTServer.h
	$(GPP) -c $(LIB)/PTServer.cpp

SocketTCP.o: $(LIB)/SocketTCP.cpp $(LIB)/SocketTCP.h
	$(GPP) -c $(LIB)/SocketTCP.cpp

SocketUDP.o: $(LIB)/SocketUDP.cpp $(LIB)/SocketUDP.h
	$(GPP) -c $(LIB)/SocketUDP.cpp

ServerUDP.o: $(LIB)/ServerUDP.cpp $(LIB)/ServerUDP.h
	$(GPP) -c $(LIB)/ServerUDP.cpp

ServerTCP.o: $(LIB)/ServerTCP.cpp $(LIB)/ServerTCP.h
	$(GPP) -c $(LIB)/ServerTCP.cpp

ClientUDP.o: $(LIB)/ClientUDP.cpp $(LIB)/ClientUDP.h
	$(GPP) -c $(LIB)/ClientUDP.cpp

ClientTCP.o: $(LIB)/ClientTCP.cpp $(LIB)/ClientTCP.h
	$(GPP) -c $(LIB)/ClientTCP.cpp

# Compilation OBJ CLIENT
json.o: $(APP)/json.cpp
	$(GPP) -c $(APP)/json.cpp

clientmain.o: $(APP)/clientmain.cpp
	$(GPP) -c $(APP)/clientmain.cpp

# Compilation OBJ SERVER
#servermain.o: $(DOSSIER1)/servermain.cpp
#	$(GPP) -c $(DOSSIER1)/servermain.cpp
