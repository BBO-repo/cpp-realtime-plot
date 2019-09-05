# kiss - c++ makefile project #

# executable and directories
EXEC = plot
SDIR = source
IDIR = include
LDIR = -L/home/brahim/Work/tools/matplotpp/lib
ODIR = obj
BDIR = build

#compile and link flags
CXX    = g++ -std=c++14 -Wall
CFLAGS = -I$(IDIR) -I/home/brahim/Work/tools/matplotpp/include
LIBS   = $(LDIR) -rdynamic -lmatplotpp -lGLU -lGL -lglut -lpthread

SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRC))

$(BDIR)/$(EXEC): $(OBJ)
	$(CXX) -o $(BDIR)/$(EXEC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(LIBS) 

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm $(ODIR)/* $(BDIR)/$(EXEC)
