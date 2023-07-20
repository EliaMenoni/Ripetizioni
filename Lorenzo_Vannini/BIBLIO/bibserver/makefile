src_folder := ./src
lib_folder := ./lib
bin_folder := ./bin

args := -lpthread -lm
output_name := bibserver.out

queue_lib := $(lib_folder)/unboundedqueue/unboundedqueue.c
libro_lib := $(lib_folder)/libro/libro.c

main_file := $(src_folder)/main.c

compile:
	gcc $(main_file) $(queue_lib) $(libro_lib) -o $(bin_folder)/$(output_name) $(args)

.PHONY: run
run:
	$(bin_folder)/$(output_name)

.PHONY: clean
clean:
	rm $(bin_folder)/*.out