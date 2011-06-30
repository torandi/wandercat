OBJS = main.o render.o logic.o
CFLAGS += -Wall `sdl-config --cflags`
LDFLAGS += `sdl-config --libs`  -lGL

all: wandercat

wandercat: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -rf *.o wandercat

%.o : %.cpp
	@$(CXX) -MM $(CFLAGS) $< > $*.d
	$(CXX) $(CFLAGS) -c $< -o $@

-include $(OBJS:.o=.d)
