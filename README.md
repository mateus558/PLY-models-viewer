## Compilation

For the project compilation step you just need to execute the `make` command in your linux terminal from the project folder.

If you are on windows, make sure that you have the g++ compiler added to your environment path and execute the following command in your command line:

`g++ -g --std=c++11 main.cpp 3DPlyModel.cpp glcTexture.cpp  -o main -lpng -lGL -lGLU -lglut`

## Usage

After compiling the project, you can start to vizualize your ply model with the following command on the project compilation folder:

`./main <PLY file>`

For example, for the model airplane.ply, we execute the command `./main airplane.ply` for getting the following screen:

[![https://imgur.com/fGiFPZv.png](https://imgur.com/fGiFPZv.png)](https://imgur.com/fGiFPZv.png)

The keyboard keys listed on the terminal must be used on the model visualization to take effect.
