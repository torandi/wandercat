OBJS = main.o
CFLAGS += -Wall `sdl-config --cflags`
LDFLAGS += `sdl-config --libs`

all: wandercat

wandercat: $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -rf *.o wandercat

%.o : %.cpp
	@$(CXX) -MM $(CFLAGS) $< > $*.d
	$(CXX) $(CFLAGS) -c $< -o $@

-include $(OBJS:.o=.d)
