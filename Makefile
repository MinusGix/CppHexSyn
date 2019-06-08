.PHONY: start build
all_files = ./src/main.cpp
build_dir = build
build_filename = program
build_full = ./$(build_dir)/$(build_filename)
compiler = g++

start : build
	./$(build_dir)/$(build_filename) ./test.hsyn ./test.out

build :
	$(compiler) -DM_TEST -g -Wall -Wshadow -Wshadow-compatible-local -pedantic -o ./$(build_dir)/$(build_filename) $(all_files)