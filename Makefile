c=gcc
d=-D DEBUG

client_path=client
server_path=server

all : server client

debug : server_debug client_debug

server : src/server.c
	$(c) $< lib/libServer.c lib/libCore.c -o $(server_path) -pthread

client : src/client.c
	$(c) $< lib/libClient.c lib/libCore.c -o $(client_path) -pthread

server_debug : src/server.c
	$(c) $(d) $< lib/libServer.c lib/libCore.c -o $(server_path) -pthread

client_debug : src/client.c
	$(c) $(d) $< lib/libClient.c lib/libCore.c -o $(client_path) -pthread -g


clean : 
	rm server client