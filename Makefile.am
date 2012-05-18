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

.SUFFIXES = .large.png .small.png .png

%.large.png: %.png
	convert -extent 512x512 -gravity center -background transparent $< $@

%.small.png: %.png
	convert -resize x256 -extent 256x256 -gravity center -background transparent $< $@

all-local: large small
large: $(foreach x, walk_down walk_up walk_left walk_right waving, data/$(x)_large.png)
small: $(foreach x, walk_down walk_up walk_left walk_right waving, data/$(x)_small.png)

clean-local:
	for x in walking_down walking_up walking_horizontal waving; do rm -f $$x/*.large.png walking_down/*.small.png; done
	rm -f data/*.png

data/walk_down_large.png: $(foreach x, $(shell seq -w 0 15), walking_down/$(x).large.png)
	montage $^ -tile 4x -background transparent -geometry 512x512+0+0 $@

data/walk_down_small.png: $(foreach x, $(shell seq -w 0 15), walking_down/$(x).small.png)
	montage $^ -tile 4x -background transparent -geometry 256x256+0+0 $@

data/walk_up_large.png: $(foreach x, $(shell seq -w 0 15), walking_up/$(x).large.png)
	montage $^ -tile 4x -background transparent -geometry 512x512+0+0 $@

data/walk_up_small.png: $(foreach x, $(shell seq -w 0 15), walking_up/$(x).small.png)
	montage $^ -tile 4x -background transparent -geometry 256x256+0+0 $@

data/walk_left_large.png: $(foreach x, $(shell seq -w 1 9), walking_horizontal/$(x).large.png)
	montage $^ -tile 3x -background transparent -geometry 512x512+0+0 $@

data/walk_left_small.png: $(foreach x, $(shell seq -w 1 9), walking_horizontal/$(x).small.png)
	montage $^ -tile 3x -background transparent -geometry 256x256+0+0 $@

data/walk_right_large.png: $(foreach x, $(shell seq -w 1 9), walking_horizontal/$(x).large.png)
	montage $^ -flop -tile 3x -background transparent -geometry 512x512+0+0 $@

data/walk_right_small.png: $(foreach x, $(shell seq -w 1 9), walking_horizontal/$(x).small.png)
	montage $^ -flop -tile 3x -background transparent -geometry 256x256+0+0 $@

data/waving_large.png: $(foreach x, $(shell seq -w 0 15), waving/$(x).large.png)
	montage $^ -tile 4x -background transparent -geometry 512x512+0+0 $@

data/waving_small.png: $(foreach x, $(shell seq -w 0 15), waving/$(x).small.png)
	montage $^ -tile 4x -background transparent -geometry 256x256+0+0 $@
