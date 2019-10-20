# kiss - c++ makefile project #

# executable and directories
EXEC = quran-image-processor
SDIR = source
IDIR = -Iinclude -I/Users/brahimboudamouz/Perso/dev/libraries/opencv/include/opencv4
LDIR = -L/Users/brahimboudamouz/Perso/dev/libraries/opencv/lib
ODIR = obj
BDIR = build

#compile and link flags
CXX     = g++
CFLAGS  = -Wall -std=c++17 $(IDIR)
LDFLAGS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
LIBS    = $(LDIR) $(LDFLAGS)

SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRC))

$(BDIR)/plot: $(OBJ)
	$(CXX) -o $(BDIR)/$(EXEC) $(CFLAGS) $(OBJ) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm $(ODIR)/* $(BDIR)/*
