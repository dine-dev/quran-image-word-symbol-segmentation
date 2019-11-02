# kiss - c++ makefile project #

# executable and directories
EXEC = quran-image-processor
SDIR = source
ODIR = obj
BDIR = build

IDIR = -Iinclude -I/Users/brahimboudamouz/Perso/dev/libraries/opencv/include/opencv4
LDIR = -L/Users/brahimboudamouz/Perso/dev/libraries/opencv/lib

#compile and link flags
CXX     = g++
CFLAGS  = -Wall -std=c++17 $(IDIR)
LDFLAGS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
LIBS    = $(LDIR) $(LDFLAGS)

SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRC))


###########################
# compilation and linking #
###########################
# build executable - link
$(BDIR)/$(EXEC): $(OBJ) | $(BDIR)
	$(CXX) -o $(BDIR)/$(EXEC) $(CFLAGS) $(OBJ) $(LIBS)

# build objects - compile
$(ODIR)/%.o: $(SDIR)/%.cpp | $(ODIR)
	$(CXX) $(CFLAGS) -c $< -o $@


###############################################
# build directories if do not already present #
###############################################
# build object directory if not already exist
$(ODIR):
	echo "[INFO] object directory is missing so building directory:" $(ODIR)
	mkdir -p $@

# build build directory if not already exist
$(BDIR):
	echo "[INFO] build directory is missing so building directory:" $(BDIR)
	mkdir -p $@


############
# cleaning #
############
clean:
	rm $(ODIR)/* $(BDIR)/*

