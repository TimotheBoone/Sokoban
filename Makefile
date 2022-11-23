c=gcc

all : main

% : src/%.c
	$(c) $< -o $@ -pthread

clean : 
	rm main