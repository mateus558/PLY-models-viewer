#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glut.h>

#include "3DPlyModel.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b)) 

using namespace std;

float convertRange(float amin, float amax, float bmin, float bmax, float input){
	return bmin + ((bmax - bmin) / (amax - amin)) * (input - amin);
}

bool exist(vector<int> v, int ele){
	int i, n = v.size();
	
	for(i = 0; i < n; i++){
		if(v[i] == ele){
			return true;
		}
	}
	return false;
}

void PlyModel::setMaterial(GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *shininess){
	mat.ambient = ambient;
	mat.diffuse = diffuse;
	mat.specular = specular;
	mat.shininess = shininess;
	mat.hasMaterial = true;
}

void PlyModel::findMinMaxLimits(){
	int i, n = points.size();
	
	if(!minMaxAxisComputed){
		for(i = 0; i < n; i++){
			if(points[i].x < min.x){
				min.x = points[i].x;
			}
			if(points[i].y < min.y){
				min.y = points[i].y;
			}
			if(points[i].z < min.z){
				min.z = points[i].z;
			}
			if(points[i].x > max.x){
				max.x = points[i].x;
			}
			if(points[i].y > max.y){
				max.y = points[i].y;
			}
			if(points[i].z > max.z){
				max.z = points[i].z;
			}
		}
		minMaxAxisComputed = !minMaxAxisComputed;
	}
}

void PlyModel::setCoordinatesLimits(Point cmin, Point cmax){
	int i, n = points.size();
	
	findMinMaxLimits();

	for(i = 0; i < n; i++){
		points[i].x = convertRange(min.x, max.x, cmin.x, cmax.x, points[i].x);
		points[i].y = convertRange(min.y, max.y, cmin.y, cmax.y, points[i].y);
		points[i].z = convertRange(min.z, max.z, -50, 50, points[i].z);
	}
}

void PlyModel::readFromFile(const char* fileName){
	bool dimSet = 0, endWhile = false, isNeigh = false;
	int np, nf, dim, i, j, k, l;
	char deli = 32;
	ifstream plyFile(fileName);
	string str, item, end("end_header");	
	vector<string> items;
	vector<int> neigh;
	
	plyFile >> str;
	
	if(str != string("ply")){
		cerr << "Wrong file format. (Ply loading)" << endl;
		return;
	}
	
	while(getline(plyFile, str)){
		istringstream ss(str);
		
		endWhile = true;
		for(i = 0; i < end.size(); i++){
			if(str[i] != end[i]){
				endWhile = false;
				break;
			}
		}
		if(endWhile) break;
		
		while(getline(ss, item, deli)){
			items.push_back(item);
		}
		
		for(auto itr = items.begin(); itr != items.end(); itr++){
			if(*(itr) == string("format") && *(itr+1) != string("ascii")){
				cerr << "Codification not supported. (Ply loading)" << endl;
				return;
			}
			
			if(*(itr) == string("vertex")){
				np = stoi(*(itr+1));
				points.resize(np);
			}
			if(*(itr) == string("face")){
				nf = stoi(*(itr+1));
				faces.resize(nf);
			}
			if(*(itr) == string("nx")){
				hasNormals = true;
				normals.resize(np);
			}
			if(*(itr) == string("s")){
				hasTexture = true;
				uv_coordinates.resize(np);
			}
		}
		items.clear();
	}
	
	for(i = 0; i < np; i++){
		getline(plyFile, str);
		istringstream ss(str);
		
		while(getline(ss, item, deli)){
			items.push_back(item);
		}

		points[i] = Point(stod(items[0]), stod(items[1]), stod(items[2]));
		if(hasNormals){
			normals[i] = Point(stod(items[3]), stod(items[4]), stod(items[5]));
		}
		
		if(hasTexture){
			uv_coordinates[i] = UV(stod(items[6]), stod(items[7]));
		}
		items.clear();
	}
	
	for(i = 0; i < nf; i++){
		getline(plyFile, str);
		istringstream ss(str);
				
		while(getline(ss, item, deli)){
			items.push_back(item);
		}
		
		if(!dimSet){
			dim = stoi(items[0]);
			facePoints = dim;
			dimSet = true;
		}
		
		faces[i].resize(dim);
		
		for(j = 0; j < dim; j++){
			faces[i][j] = stoi(items[j+1]);
		}
		items.clear();
	}
	
	if(useGouraud){
		if(!hasNormals) computeNormals();
		cout << "Computing gouraud..." << endl; 
		gouraud.resize(np);
		for(i = 0; i < np; i++){
			for(j = 0; j < nf; j++){
				for(k = 0; k < dim; k++){
					if(faces[j][k] == i){
						for(l = 0; l < k; l++){
							neigh.push_back(faces[j][l]);
						}
						for(l = k+1; l < dim; l++){
							neigh.push_back(faces[j][l]);
						}
					}
				}
			}
			
			l = neigh.size();
			for(j = 0; j < l; j++){
				gouraud[i] += normals[neigh[j]];
			}
			gouraud[i] /= l;
			
			neigh.clear();
		}
	}
	
	plyFile.close();
	this->getCentroid();
}

Point PlyModel::getCentroid(){
	int i, n = points.size();
	Point sum;
	
	if(centroidComputed) return this->centroid;
	
	for(i = 0; i < n; i++){
		sum += points[i];
	}
	
	sum /= n;
	this->centroid = sum;
	centroidComputed = !centroidComputed;
	
	return this->centroid;
}

void PlyModel::translate(Point trans){
	int i, n = points.size();
	
	for(i = 0; i < n; i++){
		points[i] -= trans;
	}
	
	centroidComputed = false;
	this->getCentroid();
}

void PlyModel::normalize(){
	int i, n = points.size();
	
	for(i = 0; i < n; i++){
		points[i].normalize();
	}
	normalized = true;
}

void PlyModel::unitize(){
	int i, n = points.size();
	float cx, cy, cz, scaleFactor, w, h, d;
	
	if(!centralized){
		findMinMaxLimits();
		
		cx = (max.x - min.x) / 2.0f;
		cy = (max.y - min.y) / 2.0f;
		cz = (max.z - min.z) / 2.0f;
	
		w = fabs(max.x) + fabs(min.x);
		h = fabs(max.y) + fabs(min.y);
		d = fabs(max.z) + fabs(min.z);
	
		scaleFactor = 2.0f / MAX(MAX(w, h), d);
		
		for(i = 0; i < n; i++){
			points[i].x -= min.x + (max.x - min.x) / 2.0f;
			points[i].y -= min.y + (max.y - min.y) / 2.0f;
			points[i].z -= min.z + (max.z - min.z) / 2.0f;
		}
		
		scale(scaleFactor);		
	
		centralized = !centralized;
	}	
}

void PlyModel::scale(Point scale){
	int i, j, n = points.size();
	
	for(i = 0; i < n; i++){
		points[i] *= scale;
	}
	normalized = false;
}

void PlyModel::computeNormals(){
	int i, nf = faces.size();
	float norm = 0;
	Point vec, vec1;
			
	if(!hasNormals && !normalComputed){
		normals.resize(nf);
	
		for(i = 0; i < nf; i++){  
			vec.x = points[faces[i][0]].x - points[faces[i][1]].x;
			vec.y = points[faces[i][0]].y - points[faces[i][1]].y;
			vec.z = points[faces[i][0]].z - points[faces[i][1]].z;
			
			vec1.x = points[faces[i][0]].x - points[faces[i][2]].x;
			vec1.y = points[faces[i][0]].y - points[faces[i][2]].y;
			vec1.z = points[faces[i][0]].z - points[faces[i][2]].z;
			
			normals[i] = Point(vec.y * vec1.z - vec.z * vec1.y,
							   vec.z * vec1.x - vec.x * vec1.z,
							   vec.x * vec1.y - vec.y * vec1.x);
			
			norm = normals[i].norm();
			normals[i] = Point(normals[i].x/norm, normals[i].y/norm, normals[i].z/norm);
		}
		
		normalComputed = true;
	}
}

void PlyModel::draw(DrawMode t){
	int i, j, nf = faces.size(), np = points.size(), ns;
	
	if(mat.hasMaterial){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat.shininess);
	}
	
	switch(t){
		case WIREFRAME:
			glColor3f(1, 1, 1);
			for(i = 0; i < nf; i++){
				glBegin(GL_LINE_LOOP);
				for(j = 0; j < facePoints; j++){
					glVertex3f(points[faces[i][j]].x, points[faces[i][j]].y, points[faces[i][j]].z);
				}
				glEnd();	
			}
			break;
		case FLAT_SURFACE:
			for(i = 0; i < nf; i++){
				
				glBegin(GL_POLYGON);
					for(j = 0; j < facePoints; j++){
						if(!hasNormals && normalComputed && !useGouraud){
							glNormal3f(normals[i].x, normals[i].y, normals[i].z);
						}else if(hasNormals && !useGouraud){	
							glNormal3f(normals[faces[i][j]].x, normals[faces[i][j]].y, normals[faces[i][j]].z);
						}
						if(useGouraud){
							glNormal3f(gouraud[faces[i][j]].x, gouraud[faces[i][j]].y, gouraud[faces[i][j]].z);
						}
						if(hasTexture){
							glTexCoord2f(uv_coordinates[faces[i][j]].u, uv_coordinates[faces[i][j]].v);
						}
						glVertex3f(points[faces[i][j]].x, points[faces[i][j]].y, points[faces[i][j]].z);
					}
				glEnd();
			}
			break;
		default:
			cerr << "Invalid draw mode." << endl;
			break;
	}
}	

int PlyModel::getTrianglesNumber(){
	return (facePoints == 3) ? faces.size() : faces.size()*2;
}
