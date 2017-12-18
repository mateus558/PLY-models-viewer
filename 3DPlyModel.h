#ifndef PLYMODEL_H_INCLUDED
#define PLYMODEL_H_INCLUDED

#include <vector>
#include <string>
#include "Point.h"

enum  DrawMode {WIREFRAME = 0, FLAT_SURFACE = 1};

struct Material {
	bool hasMaterial = false;
	GLfloat *ambient, *diffuse, *specular;
	GLfloat *shininess;
};

struct UV {
	float u, v;
	UV(){}
	UV(float u, float v){
		this->u = u;
		this->v = v;
	}
};

class PlyModel {
public:
	PlyModel(){
		min = Point(10000, 10000, 10000);
		max = Point(-10000, -10000, -10000);
	};
	void setMaterial(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat *shininess);
	void readFromFile(const char* fileName);
	void computeNormals();
	void normalize();
	void scale(Point scale);
	void draw(DrawMode t);
	void unitize();
	void translate(Point trans);
	void setCoordinatesLimits(Point cmin, Point cmax);
	int getTrianglesNumber();
	void is2D(bool flag){twoD = flag;}
	void activateGouraud(bool flag){useGouraud = flag;}
	Point getCentroid();
	std::vector<Point> getPoints() {return points;}
	std::vector<Point> getNormals() {return normals;}
	std::vector<UV> getUVCoords() {return uv_coordinates;} 
	std::vector<std::vector<int> > getFaces(){return faces;}
	
private:
	bool hasNormals = false, normalComputed = false, normalized = false, minMaxAxisComputed = false, hasTexture = false;
	bool centralized = false, centroidComputed = false, twoD = false, useGouraud = true;
	int facePoints = 0;
	Point min, max, centroid;
	Point coorMin, coordMax;
	
	Material mat;
	std::vector<Point> points;
	std::vector<Point> normals;
	std::vector<Point> gouraud;
	std::vector<UV> uv_coordinates;
	std::vector<Point> colors;
	std::vector<std::vector<int> > faces;

	void findMinMaxLimits();
};

#endif
