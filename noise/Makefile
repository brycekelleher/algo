OBJECTS	= main.o perlin.o

#ifeq ($(APPLE),1)
CFLAGS += -I/usr/X11R6/include -DGL_GLEXT_PROTOTYPES
LDFLAGS = -L/usr/X11R6/lib
LDLIBS  = -lm -lGL -lglut
#endif

main: $(OBJECTS)

clean:
	rm -rf main *.o
