# default build release, otherwise override by setting it to `debug` in the command line
BUILD := release

# TODO: Add an option about producing a manifest for a package manager like apt or brew
# MANIFEST := none

# TODO: Check for libraries: zlib and cURL (maybe)

# TODO: Check for built in crypto systems for sha256 hashing (maybe if necessary?)

PROG := zing
SRCS := $(shell find src/ -name '*.c')
OBJS := $(patsubst src/%.c, build/%.o, $(SRCS))

all: build $(PROG) ; @echo "program built"

# $^ expands to ALL of the prerequisites
$(PROG): $(OBJS)
	cc -std=c17 -Isrc -MMD $^ -o $@

# $< expands to the first (and only, in this case) prerequisite
build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -std=c17 -Isrc -MMD -c $< -o $@

build: ; @mkdir -p build

#.PHONY: all

# dependencies
-include $(patsubst build/%.o, build/%.d, $(OBJS))