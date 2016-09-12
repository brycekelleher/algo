OBJECTS	= noise2.o perlin.o

#ifeq ($(APPLE),1)
CFLAGS += -I/usr/X11R6/include -DGL_GLEXT_PROTOTYPES
LDFLAGS = -L/usr/X11R6/lib
LDLIBS  = -lm -lGL -lglut
#endif

noise2: $(OBJECTS)

clean:
	rm -rf noise2 *.o
