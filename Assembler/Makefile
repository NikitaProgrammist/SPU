CC=g++
CHECK_STACK?=ON
CFLAGS= -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
FLAGS=-I ./headers -I ../stack/headers
FILES=main.cpp to_binary.cpp utils.cpp
SUBMODULE_FILES=stack.cpp check_stack.cpp my_printf.cpp
FILES_DIR=sources
OBJECTS_DIR=build
SUBMODULES_DIR = ../stack/sources
SOURCES=$(addprefix $(FILES_DIR)/, $(FILES))
SUB_SOURCES = $(addprefix $(SUBMODULES_DIR)/, $(SUBMODULE_FILES))
OBJECTS=$(addprefix $(OBJECTS_DIR)/, $(FILES:.cpp=.o)) $(addprefix $(OBJECTS_DIR)/, $(SUBMODULE_FILES:.cpp=.o))
EXECUTABLE=build/main

ifeq ($(CHECK_STACK), ON)
	CFLAGS += -D CHECK
endif

.PHONY: all submodule-init submodule-update run clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ $(FLAGS) $(CFLAGS) -o $@

$(OBJECTS_DIR)/%.o: $(FILES_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< $(FLAGS) -o $@

$(OBJECTS_DIR)/%.o: $(SUBMODULES_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< $(FLAGS) -o $@

submodule-init:
	git submodule update --init --recursive

submodule-update:
	git submodule update --remote --recursive

run: $(EXECUTABLE)
	./build/main

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
