c=gcc

all : server client

% : src/%.c
	$(c) $< -o $@ -pthread

clean : 
	rm server client