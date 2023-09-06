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

all server.out client.out:
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


test: server1 server2 server3 server4 server5 client_test
client_test:
	sleep 1
	make gruppo1 -j
	sleep 1
	make gruppo2 -j
	sleep 1
	make gruppo3 -j
	sleep 1
	make gruppo4 -j
	sleep 1
	make gruppo5 -j
	make end

gruppo1: query1 query2 loan1 loan2 query4
gruppo2: query3 query4 loan3 loan4 loan4
gruppo3: query1 query3 loan1 loan3 loan4
gruppo4: query2 query4 loan2 loan4 query4
gruppo5: loan1 loan1 loan1 loan1 query2
gruppo6: query1 query1 query1 query1 query3
gruppo7: loan1 loan2 query2 query1 loan3
gruppo8: loan3 loan4 query4 query3 query1

query1:
	$(client_folder)/$(bin_folder)/client.out -titolo potere -anno 2018
query2:
	$(client_folder)/$(bin_folder)/client.out -titolo archi -autore gatto
query3:
	$(client_folder)/$(bin_folder)/client.out -anno 2018
query4:
	$(client_folder)/$(bin_folder)/client.out -titolo Progr

loan1:
	$(client_folder)/$(bin_folder)/client.out -titolo potere -anno 2018 -p
loan2:
	$(client_folder)/$(bin_folder)/client.out -titolo archi -autore gatto -p
loan3:
	$(client_folder)/$(bin_folder)/client.out -anno 2018 -p
loan4:
	$(client_folder)/$(bin_folder)/client.out -titolo Progr -p

end:
	sleep 5
	killall $(server_folder)/$(bin_folder)/server.out
	sleep 10
	make analize

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

analize:
	./bibaccess.sh --QUERY bib1.log bib2.log bib3.log bib4.log bib5.log
	./bibaccess.sh --LOAN bib1.log bib2.log bib3.log bib4.log bib5.log