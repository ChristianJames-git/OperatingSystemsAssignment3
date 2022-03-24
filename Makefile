# Christian James 823672623

# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb
CXXFLAGS=-std=c++11 -g

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target

# $^ is an example of a special variable.  It substitutes all dependencies
pagingwithtlb : main.o paging.o output_mode_helpers.o PageTable.o Level.o tracereader.o tlbhandling.o
	$(CXX) $(CXXFLAGS) -o pagingwithtlb $^

main.o : paging.h main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

paging.o : PageTable.h tracereader.h output_mode_helpers.h paging.h paging.cpp
	$(CXX) $(CXXFLAGS) -c paging.cpp

output_mode_helpers.o : output_mode_helpers.h output_mode_helpers.cpp
	$(CXX) $(CXXFLAGS) -c output_mode_helpers.cpp

tracereader.o : tracereader.h tracereader.cpp
	$(CXX) $(CXXFLAGS) -c tracereader.cpp

Level.o : PageTable.h Level.h Level.cpp
	$(CXX) $(CXXFLAGS) -c Level.cpp

tlbhandling.o : PageTable.h tlbhandling.h tlbhandling.cpp
	$(CXX) $(CXXFLAGS) -c tlbhandling.cpp

PageTable.o : PageTable.h PageTable.cpp
	$(CXX) $(CXXFLAGS) -c PageTable.cpp

clean :
	rm *.o