#ifndef PLANE_H
#define PLANE_H

#ifndef PI
#define PI 3.14159265
#endif

#ifndef NULL
#define NULL 0
#endif

#include "Vector3f.h"
//#include "matrix.h"

#include <vector>


class Plane
{
private:
	

public:

    Vector3f V1,V2,V3,V4;
    Vector3f normal;
    Vector3f origin;

    float length; //how long is the side (future enhancement)

//---Methods----------------

    Plane();
    Plane(Vector3f v1,Vector3f v2, Vector3f v3, Vector3f v4) ;

	//~Plane() ;	

    void draw(Vector3f color);

    Vector3f findNormal(Vector3f v1,Vector3f v2, Vector3f v3);

    Vector3f getNormal();

    bool onPlane(Vector3f point);

    float getDistance(Vector3f point);

    Vector3f getClosestPoint(Vector3f point);
	
};

#endif
