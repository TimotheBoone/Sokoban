c=gcc
d=-D DEBUG

client_path=client
server_path=server

all : server client

debug : server_debug client_debug

server : src/server.c
	$(c) $< lib/libServer.c lib/libCore.c lib/libGame.c -o $(server_path) -lncurses -lpthread

client : src/client.c
	$(c) $< lib/libClient.c lib/libCore.c lib/libGame.c -o $(client_path) -lncurses -lpthread

server_debug : src/server.c
	$(c) $(d) $< lib/libServer.c lib/libCore.c lib/libGame.c -o $(server_path) -lncurses -lpthread -g

client_debug : src/client.c
	$(c) $(d) $< lib/libClient.c lib/libCore.c lib/libGame.c -o $(client_path) -lncurses -lpthread -g


clean : 
	rm server client