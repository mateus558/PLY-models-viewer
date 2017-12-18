all:
	g++ -g --std=c++11 main.cpp 3DPlyModel.cpp glcTexture.cpp  -o main -lpng -lGL -lGLU -lglut 
	#g++ -g --std=c++11 test.cpp TextureLoader.cpp  -o teste -lGL -lGLU -lglut 
