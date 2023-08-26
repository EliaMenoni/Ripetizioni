src_folder := src
lib_folder := lib
bin_folder := bin
log_folder := logs

server_folder := ./bibserver
client_folder := ./bibclient

server_args := -lpthread -lm

queue_lib := $(server_folder)/$(lib_folder)/unboundedqueue/unboundedqueue.c
libro_lib := $(server_folder)/$(lib_folder)/libro/libro.c

server_main_file := $(server_folder)/$(src_folder)/main.c
client_main_file := $(client_folder)/$(src_folder)/main.c

server_args := -lm -lpthread

compile server.out client.out:
	gcc $(server_main_file) -g $(queue_lib) $(libro_lib) -o $(server_folder)/$(bin_folder)/server.out $(server_args)
	gcc $(client_main_file) -g $(libro_lib) -o $(client_folder)/$(bin_folder)/client.out

.PHONY: server
server:
	$(server_folder)/$(bin_folder)/server.out

.PHONY: client
client:
	$(client_folder)/$(bin_folder)/client.out

.PHONY: clean
clean:
	rm $(server_folder)/$(bin_folder)/*
	rm $(server_folder)/$(log_folder)/*
	rm $(client_folder)/$(bin_folder)/*