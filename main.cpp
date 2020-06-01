#include <iostream>
#include <vector>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string>

#include "3DPlyModel.h"
#include "glcTexture.h"

using namespace std;

float rotationX = 25.0, rotationY = 0.0,X=0.0,Y=0.0;
int   last_x, last_y;
int width = 640, height = 480;
bool light     = true;
int mode = 0;
bool zbuffer   = true;
bool backface  = true;
bool centralized = false;
vector<PlyModel> models;
vector<string> files;
glcTexture *textureManager = new glcTexture();
string file;
int distOrigem = 180, obj = 0;

// Defini parametros da luz
GLfloat cor_luz[]			= { 1.0f, 1.0f, 1.0f, 1.0};
GLfloat cor_luz_spec[]	= { 1.0f, 1.0f, 1.0f, 1.0};
GLfloat cor_luz_amb[]	= { 0.1, 0.1, 0.1, 1.0};
GLfloat posicao_luz[] = { 50.0, 50.0, 50.0, 1.0};

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_DEPTH_TEST);               // Habilita Z-buffer
	glEnable(GL_CULL_FACE); 				// Habilita Backface-Culling  
	glEnable(GL_LIGHTING);                 // Habilita luz
	glEnable(GL_LIGHT0);                   // habilita luz 0
	glEnable(GL_NORMALIZE);
	// Posicao da fonte de luz. Ultimo parametro define se a luz sera direcional (0.0) ou tera uma posicional (1.0)
	glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
	
	/*textureManager = new glcTexture();
	textureManager->SetNumberOfTextures(1);
	textureManager->SetWrappingMode(GL_REPEAT);
	
	textureManager->CreateTexture("Textures/text.png", 0);
    */
}

void setMaterial(void)
{
	GLfloat objeto_ambient[]   = { 0.19225, 0.19225, 0.19225, 1.0 };
	GLfloat objeto_difusa[]    = { 0.50754, 0.50754, 0.50754, 1.0 };
	GLfloat objeto_especular[] = { 0.508273, 0.508273, 0.508273, 1.0 };
	GLfloat objeto_brilho[]    = { 0.4 * 128.0};
	
	// Define os parametros da superficie a ser iluminada
	glMaterialfv(GL_FRONT, GL_AMBIENT, objeto_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, objeto_difusa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, objeto_especular);
	glMaterialfv(GL_FRONT, GL_SHININESS, objeto_brilho);
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(45.0, (GLfloat) width/(GLfloat) height, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
	gluLookAt (0.0, 0.0, distOrigem, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	textureManager->Bind(0);
	if(models.size() > 0){
		setMaterial();
		glPushMatrix();
		  glRotatef( rotationY, 0.0, 1.0, 0.0 );
		  glRotatef( rotationX, 1.0, 0.0, 0.0 );
		  if(mode) models[obj].draw(FLAT_SURFACE); else models[obj].draw(WIREFRAME);
		glPopMatrix();
	}
	glutSwapBuffers();
}

void idle ()
{
	glutPostRedisplay();
}

void reshape (int w, int h)
{
	width = w;
	height = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard (unsigned char key, int x, int y)
{
	int i, k = key - '0';
	
	switch (tolower(key))
	{
		case '+' :
			distOrigem--;
	    	// if(distOrigem<20) distOrigem=20;
		break;
		case '-' :
			distOrigem++;
 			//if(distOrigem>180) distOrigem=180;
		break;
		case 27:
			exit(0);
		break;
	}

	switch (tolower(key)){
		case 'l'	:
			light = !light;
			(!light) ? glDisable(GL_LIGHTING)        : glEnable(GL_LIGHTING);
			(light) ? cout <<("Iluminacao ligada.\n") : cout<<("Iluminacao desligada.\n");
			break;
		
		case 'z'	:
			zbuffer = !zbuffer;
			(!zbuffer) ? glDisable(GL_DEPTH_TEST)   : glEnable(GL_DEPTH_TEST)     ;
			(zbuffer) ? cout <<("Z-Buffer ligado.\n"): cout<<("Z-Buffer desligado.\n");
			break;
		
		case 'b'	:
			backface = !backface;
			(!backface) ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE)  ;
			(backface) ? cout<<("Backface Culling ligado.\n")   : cout<<("Backface Culling desligado.\n");
			break;
		
		case 'f':
			mode = 0;
			break;
		
		case 's':
			mode = 1;
			break;
		
		case 'r':
			cout << "Enter the ply model name: ";
			cin >> file;
			
			models.push_back(PlyModel());
			models[models.size()-1].readFromFile(file.c_str());
			models[models.size()-1].unitize();
			models[models.size()-1].scale(50);
			models[models.size()-1].computeNormals();
			files.push_back(file);
			
			string title(" - Number of triangles: ");
			title = file + title + to_string(models[models.size()-1].getTrianglesNumber());
			glutSetWindowTitle(title.c_str());
			obj = models.size()-1;
			
			break;

	}
	
	if(k < models.size()){
		for(i = 0; i <= 9; i++){
			if(k == i){
				obj = k;
			}
		}
		string title(" - Number of triangles: ");
		title = files[obj] + title + to_string(models[obj].getTrianglesNumber());
		glutSetWindowTitle(title.c_str());
	}
	
}

// Motion callback
void motion(int x, int y )
{
	rotationX += (float) (y - last_y);
	rotationY += (float) (x - last_x);

	last_x = x;
	last_y = y;
}

// Mouse callback
void mouse(int button, int state, int x, int y)
{
   if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
   {
      last_x = x;
      last_y = y;
   }
}

int main(int argc, char** argv)
{
	int i, n;
	string title;
	
	if(argc > 1){
		n = argc - 1;
		files.resize(n);
		models.resize(n);
		
		for(i = 1; i < argc; i++){
	   		files[i-1] = string(argv[i]);
			models[i-1].readFromFile(argv[i]);	
			models[i-1].unitize();
			models[i-1].scale(50);		
			models[i-1].computeNormals();
		}
		title = " - Number of triangles: ";
		title = files[obj] + title + to_string(models[obj].getTrianglesNumber());
	}
	
	cout << "+/- - zoom." << endl;
	cout << "0..9 - change model." << endl;
	cout << "r - read a 3D model from terminal." << endl;
	cout << "f - Wireframe mode." << endl;
	cout << "s - Flat Surface mode." << endl;
	cout << "ESC - to Exit." << endl;
	
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (width, height);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (title.c_str());
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc( mouse );
	glutMotionFunc( motion );
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
