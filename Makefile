## Update the list of Object files the compiler should generate
## We will need an object file for each of our cpp files
OBJECTS = main.o Triangle.o Shapes.o Message.o Arc.o

########################################
## DO NOT EDIT ANYTHING BELOW THIS LINE!
TARGET = main

CXX    = clang
CFLAGS = -Wall -g -O3 -std=c++1y -stdlib=libstdc++ -D_GLIBCXX_USE_CXX11_ABI=0

INCPATH += -I/usr/include
LIBPATH += -L/usr/lib
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lstdc++ -lm

# If the first argument is "debug"...
ifeq (debug,$(firstword $(MAKECMDGOALS)))
  CFLAGS += -ggdb3 -o0
endif

all: $(TARGET)

debug: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

depend:
	rm -f Makefile.bak
	mv Makefile Makefile.bak
	sed '/^# DEPENDENCIES/,$$d' Makefile.bak > Makefile
	echo '# DEPENDENCIES' >> Makefile
	$(CXX) -MM *.cpp >> Makefile

.c.o:
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cc.o:
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cpp.o:
	@echo "Compiling $@"
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

$(TARGET): $(OBJECTS)
	@echo "Linking $@"
	$(CXX) $(CFLAGS) $(INCPATH) -o $@ $^ $(LIBPATH) $(LIBS)

compile: $(OBJECTS)

run: $(TARGET)
	./$(TARGET)

# DEPENDENCIES
