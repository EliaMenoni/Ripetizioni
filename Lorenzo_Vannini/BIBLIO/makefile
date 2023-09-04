src_folder := src
lib_folder := lib
bin_folder := bin
log_folder := logs

server_folder := ./bibserver
client_folder := ./bibclient

server_args := -lpthread -lm

queue_lib := $(server_folder)/$(lib_folder)/unboundedqueue/unboundedqueue.c
libro_lib := $(server_folder)/$(lib_folder)/libro/libro.c
comprot_lib := $(server_folder)/$(lib_folder)/comprot/comprot.c
factory_lib := $(server_folder)/$(lib_folder)/factory/factory.c

server_main_file := $(server_folder)/$(src_folder)/main.c
client_main_file := $(client_folder)/$(src_folder)/main.c

server_args := -lm -lpthread

compile server.out client.out:
	gcc $(server_main_file) -g $(queue_lib) $(libro_lib) $(comprot_lib) $(factory_lib) -o $(server_folder)/$(bin_folder)/server.out $(server_args)
	gcc $(client_main_file) -g $(libro_lib) $(comprot_lib) -o $(client_folder)/$(bin_folder)/client.out

.PHONY: clean
clean:
	rm $(server_folder)/$(bin_folder)/*
	rm $(server_folder)/$(log_folder)/*
	rm $(client_folder)/$(bin_folder)/*

.PHONY: server
server:
	$(server_folder)/$(bin_folder)/server.out 8001 BibliotecaCivica $(server_folder)/data/biblioteca_1.txt 5

.PHONY: client
client:
	$(client_folder)/$(bin_folder)/client.out -titolo potere -anno 2018

test: server1 server2 server3 server4 server5 sleep client_test end
client_test: gruppo1 gruppo2 gruppo3 gruppo4 gruppo5 gruppo6 gruppo7 gruppo8

gruppo1: query1 query2 loan1 loan2
gruppo2: query3 query4 loan3 loan4
gruppo3: query1 query3 loan1 loan3
gruppo4: query2 query4 loan2 loan4
gruppo5: loan1 loan1 loan1 loan1
gruppo6: query1 query1 query1 query1
gruppo7: loan1 loan2 query2 query1
gruppo8: loan3 loan4 query4 query3

query1:
	$(client_folder)/$(bin_folder)/client.out -titolo potere -anno 2018
query2:
	$(client_folder)/$(bin_folder)/client.out -titolo archi -autore gatto
query3:
	$(client_folder)/$(bin_folder)/client.out -anno 2018
query4:
	$(client_folder)/$(bin_folder)/client.out -titolo programmazione nella pratica

loan1:
	$(client_folder)/$(bin_folder)/client.out -titolo potere -anno 2018 -p
loan2:
	$(client_folder)/$(bin_folder)/client.out -titolo archi -autore gatto -p
loan3:
	$(client_folder)/$(bin_folder)/client.out -anno 2018 -p
loan4:
	$(client_folder)/$(bin_folder)/client.out -titolo programmazione nella pratica -p

sleep:
	sleep 1

end:
	sleep 5
	killall $(server_folder)/$(bin_folder)/server.out

server1:
	$(server_folder)/$(bin_folder)/server.out 8001 bib1 $(server_folder)/data/bib1.txt 1

server2:
	$(server_folder)/$(bin_folder)/server.out 8002 bib2 $(server_folder)/data/bib2.txt 2

server3:
	$(server_folder)/$(bin_folder)/server.out 8003 bib3 $(server_folder)/data/bib3.txt 3

server4:
	$(server_folder)/$(bin_folder)/server.out 8004 bib4 $(server_folder)/data/bib4.txt 4

server5:
	$(server_folder)/$(bin_folder)/server.out 8005 bib5 $(server_folder)/data/bib5.txt 5
