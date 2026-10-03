# default build release, otherwise override by setting it to `debug` in the command line
BUILD := release

# TODO: Add an option about producing a manifest for a package manager like apt or brew
# MANIFEST := none

# TODO: Check for libraries: zlib and cURL (maybe)

# TODO: Check for built in crypto systems for sha256 hashing (maybe if necessary?)

PROG := zing
SRCS := $(shell find src/ -name '*.c')
OBJS := $(patsubst src/%.c, build/%.o, $(SRCS))

LIBRARIES := zlib

STANDARD := c17
INCLUDE  = -Isrc $(foreach library, $(LIBRARIES), $(shell pkg-config --cflags $(library)))
LIBS     = $(foreach library, $(LIBRARIES), $(shell pkg-config --libs $(library)))
WARNINGS := -Wall -Wextra -Wno-comment


all: build libraries $(PROG)
	ifeq ($(BUILD), debug)
	  dsymutil $(PROG)
	endif
	# deal with the package manifest here also
	@echo "program built"


libraries:
	@for lib in $(LIBRARIES); do \
		if pkg-config --exists $$lib; then \
			echo "found library '$$lib'"; \
		else \
			echo "[ERROR] library '$$lib' not found!"; \
			exit 1; \
		fi; \
	done

# $^ expands to ALL of the prerequisites
$(PROG): $(OBJS)
	cc -std=c17 -Isrc -MMD $^ -o $@

# $< expands to the first (and only, in this case) prerequisite
build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -std=c17 -Isrc -MMD -c $< -o $@

build: ; @mkdir -p build

clean:
	rm -rf build
	rm -f $(PROG)

.PHONY: all clean

# dependencies
-include $(patsubst build/%.o, build/%.d, $(OBJS))