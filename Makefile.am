ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = wandercat

wandercat_CXXFLAGS = -Wall $(SDL_CFLAGS)
wandercat_LDADD = $(SDL_LIBS) -lSDL_image -lGL -lGLU -lGLEW
wandercat_SOURCES = \
	main.cpp \
	render.cpp \
	logic.cpp \
	network.cpp \
	texture.cpp \
	resource.cpp

if HAVE_FAM
wandercat_LDADD += -lfam
endif
