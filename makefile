SOURCE_DIRS := \
	src \
	src/Base \
	src/Compiler \
	src/Interpreter \
	src/Interpreter/Objects \
	src/Interpreter/Primitives \
	src/Syntax \
	src/Syntax/AST

OBJECT_DIR := objects

VPATH += $(SOURCE_DIRS)
SOURCES = $(notdir $(foreach dir,$(SOURCE_DIRS),$(wildcard $(dir)/*.cpp)))

CFLAGS += $(foreach dir,$(SOURCE_DIRS),-iquote$(dir))

# The makefile doesn't build to the same relative path as the XCode project and
# the code hackishly requires the main.fin base library to be in a specific
# relative path from the executable. This fixes it to use the right path.
CFLAGS +=	-DHACK_ROOT_BASE_PATH

OBJECTS = $(foreach source,$(SOURCES),$(OBJECT_DIR)/$(source:.cpp=.o))

all: $(OBJECT_DIR) finch

finch: $(OBJECTS)
	@ echo "Linking..."
	@ $(CXX) $^ -o $@

$(OBJECT_DIR):
	mkdir $@

$(OBJECT_DIR)/%.o: %.cpp
	@ echo "Compiling $^..."
	@ $(CXX) $(CFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	rm -rf finch $(OBJECT_DIR)


# Makefile debugging.

.PHONY: show-sources show-objects show-cflags

show-sources:
	@ echo $(SOURCES)
show-objects:
	@ echo $(OBJECTS)
show-cflags:
	@ echo $(CXX)
	@ echo $(CFLAGS)

