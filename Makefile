###############################
# kiss - c++ makefile project #
###############################

###################################
# executable and main directories #
###################################
EXEC := quran-image-processor
SDIR := source
IDIR := include
ODIR := obj
BDIR := build

########################
# additional libraries #
########################
IDIR1 := /Users/brahimboudamouz/Perso/dev/libraries/opencv/include/opencv4
IDIRS := $(IDIR) $(IDIR1) # $(IDIR2) $(IDIR3) ...
LDIR1 := /Users/brahimboudamouz/Perso/dev/libraries/opencv/lib
LDIRS := $(LDIR1) # $(LDIR2) $(LDIR3) ...

##########################
# compile and link flags #
##########################
CXX      := g++
CXXFLAGS := -g -Wall -Wextra -std=c++17
CPPFLAGS := $(foreach inc, $(IDIRS),-I$(inc))
LIBS     := $(foreach lib, $(LDIRS),-L$(lib))
LDFLAGS  := $(LIBS) -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

################
# dependencies #
################
SRCS := $(wildcard $(SDIR)/*.cpp)
OBJS := $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRCS))
DEPS = $(OBJS:%.o=%.d)

###########################
# compilation and linking #
###########################
# list of non-file based targets:
.PHONY: depend clean all

# default target
all: $(BDIR)/$(EXEC)

# build executable - link
$(BDIR)/$(EXEC): $(OBJS) | $(BDIR)
	$(CXX) -o $(BDIR)/$(EXEC) $(OBJS) $(LDFLAGS)

# include all .d files to track if an header has been modified without any implementation modification
-include $(DEPS)

# build objects - compile
$(ODIR)/%.o: $(SDIR)/%.cpp | $(ODIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -c -o $@ $<

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
# only remove directory content
clean:
	rm $(ODIR)/*.o $(ODIR)/*.d $(BDIR)/$(EXEC)

# remove directories
dist-clean:
	rm -r $(ODIR)/ $(BDIR)/

