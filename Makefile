# default build release, otherwise override by setting it to `debug` in the command line
BUILD := release

# TODO: Add an option about producing a manifest for a package manager like apt or brew
# MANIFEST := none

PROG := zing
SRCS := $(shell find src/ -name '*.c')
OBJS := $(patsubst src/%.c, build/%.o, $(SRCS))

STANDARD := -std=c17
INCLUDE   = -Isrc $(foreach library, $(LIBRARIES), $(shell pkg-config --cflags $(library)))
WARNINGS := -Wall -Wextra -Wno-comment
CFLAGS   := $(STANDARD) $(INCLUDE) $(WARNINGS) -MMD

LIBRARIES := zlib
LIBS       = $(foreach library, $(LIBRARIES), $(shell pkg-config --libs $(library)))

ifeq ($(OS), Windows_NT)
  ;
else
  UNAME_S = $(shell uname -s)
  ifeq ($(UNAME_S), Linux)
   	CFLAGS += -DZING_LINUX
  endif
  ifeq ($(UNAME_S), Darwin)
  	CFLAGS += -DZING_MACOS
  endif
endif

ifeq ($(BUILD), debug)
	CFLAGS += -g -O0
else
	CFLAGS += -O1
endif


all: build libraries $(PROG)
	@if [ "$(BUILD)" = "debug" ] && [ "$(UNAME_S)" = "Darwin" ]; then \
	  dsymutil $(PROG); \
	fi
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
	cc $(CFLAGS) $^ -o $@ $(LIBS)

# $< expands to the first (and only, in this case) prerequisite
build/%.o: src/%.c
	mkdir -p $(dir $@)
	cc $(CFLAGS) -c $< -o $@

build: ; @mkdir -p build

clean:
	rm -rf build
	rm -f $(PROG)

.PHONY: all clean

# dependencies
-include $(patsubst build/%.o, build/%.d, $(OBJS))