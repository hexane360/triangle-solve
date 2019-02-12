TARGET  ?= triangle-solve
PREFIX  ?= /usr/local
BINDIR  ?= $(PREFIX)/bin
DATADIR ?= $(PREFIX)/share

CXX      ?= cxx
CXXFLAGS ?= -Wall -O3 -std=c++14
CXXFLAGS += -DDATA_DIR=\"$(DATADIR)/$(TARGET)/\"

#can fix some std::basic_string linker errors
#CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0

INCPATH += -I/usr/include
LIBPATH += -L/usr/lib
LIBS    += -lsfml-graphics -lsfml-window -lsfml-system -lstdc++ -lm

# If the first argument is "debug"...
ifeq (debug,$(firstword $(MAKECMDGOALS)))
	CFLAGS += -g -O0
	CXXFLAGS += -g -O0
endif

OBJECTS = main.o Triangle.o Shapes.o Message.o Arc.o
DEPS = $(OBJECTS:.o=.d)

.PHONY: all
all: $(TARGET)

-include $(DEPS)

.PHONY: debug
debug: $(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPS)

%.d: %.cpp
	@$(CXX) $(CXXFLAGS) $(INCPATH) $< -MM -MT $(@:.d=.o) >$@

%.o:
	@echo "Compiling $@"
	$(CXX) $(CXXFLAGS) $(INCPATH) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo "Linking $@"
	$(CXX) $(CXXFLAGS) $(INCPATH) -o $@ $^ $(LIBPATH) $(LIBS)

.PHONY: compile
compile: $(OBJECTS)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

makefile := $(current_makefile)
#install should recompile header that uses DATADIR
.PHONY: install
install:
	touch files.h
	make .install

.PHONY: .install
.install: $(TARGET)
	@echo "Installing to $(BINDIR)"
	mkdir -p "$(BINDIR)" "$(DATADIR)/$(TARGET)"
	cp $(TARGET) "$(BINDIR)"
	cp -r data/* "$(DATADIR)/$(TARGET)"
