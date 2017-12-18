#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <cmath>
#include <ostream>

//Estrutura para usar um ponto
struct Point{
	float x, y, z;
	
	Point(){
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	
	Point(float c){
		x = c;
		y = c;
		z = c;
	}
	
	Point(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	double distance(Point p){
		return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2) + pow(p.z - z, 2));
	}
	
	float dot(Point p){
		return x * p.x + y * p.y + z * p.z; 
	}
	
	float norm(){
		return sqrt(x*x + y*y + z*z);
	}	
		
	void normalize(){
		float norm = sqrt(x*x + y*y + z*z);
		
		*this/=norm;
	}
	
	void operator=(float c){
		x = c;
		y = c;
		z = c;
	}
	
	void operator=(const Point& p){
		x = p.x;
		y = p.y;
		z = p.z;
	}
	
	void operator*=(float c){
		x *= c;
		y *= c;
		z *= c;
	}
	
	void operator*=(const Point& p){
		x *= p.x;
		y *= p.y;
		z *= p.z;
	}
	
	void operator+=(float c){
		x += c;
		y += c;
		z += c;
	}
	
	void operator+=(const Point& p){
		x += p.x;
		y += p.y;
		z += p.z;
	}
	
	void operator-=(float c){
		x -= c;
		y -= c;
		z -= c;
	}
	
	void operator-=(const Point& p){
		x -= p.x;
		y -= p.y;
		z -= p.z;
	}
	
	void operator/=(float c){
		x /= c;
		y /= c;
		z /= c;
	}	
	
	void operator/=(const Point& p){
		x /= p.x;
		y /= p.y;
		z /= p.z;
	}
		
	friend std::ostream &operator<<( std::ostream &output, const Point &point ){
		output << "[" << point.x << ", " << point.y << ", " << point.z << "]";

		return output;
	}
	
	Point operator+(const Point& q){
		Point res;
		
		res.x = this->x + q.x;
		res.y = this->y + q.y;
		res.z = this->z + q.z;
		
		return res;
	}
	
	Point operator-(const Point& q){
		Point res;
		
		res.x = this->x - q.x;
		res.y = this->y - q.y;
		res.z = this->z - q.z;
		
		return res;
	}
	
	Point operator/(const Point& q){
		Point res;
		
		res.x = this->x / q.x;
		res.y = this->y / q.y;
		res.z = this->z / q.z;
		
		return res;
	}
	
	Point operator*(const Point& q){
		Point res;
		
		res.x = this->x * q.x;
		res.y = this->y * q.y;
		res.z = this->z * q.z;
		
		return res;
	}
};

#endif
