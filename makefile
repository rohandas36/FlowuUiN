path = src/
Files_hpp = $(path)Vector3.hpp $(path)Vector4.hpp $(path)constants.hpp $(path)Grid.hpp $(path)utils.hpp $(path)Matrix4.hpp $(path)Ui.hpp 
Files_cpp = $(path)Vector3.cpp $(path)Vector4.cpp $(path)constants.cpp $(path)Grid.cpp $(path)utils.cpp $(path)Matrix4.cpp $(path)Ui.cpp $(path)main.cpp 
PostFlags = -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -lpthread
PreFlags = -O3 -std=c++11 -w -o ItFows
CXX = g++

all: $(Files_hpp) $(Files_cpp)
	$(CXX) $(PreFlags) $(Files_cpp) $(PostFlags)
blob: all
	./ItFows 0 1>log/stdOut 2>log/stdErr
hfield: all
	./ItFows 1 1>log/stdOut 2>log/stdErr
SPH: all
	./ItFows 2 1>log/stdOut 2>log/stdErr
clean: 
	rm -f *.o *.ghc ItFows log/*
