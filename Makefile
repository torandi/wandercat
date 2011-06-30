OBJS = main.o render.o logic.o network.o texture.o resource.o
CFLAGS += -Wall `sdl-config --cflags`
LDFLAGS += `sdl-config --libs` -lSDL_image -lGL -lGLEW -lfam

all: wandercat

wandercat: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -rf *.o wandercat

%.o : %.cpp
	@$(CXX) -MM $(CFLAGS) $< > $*.d
	$(CXX) $(CFLAGS) -c $< -o $@

-include $(OBJS:.o=.d)
