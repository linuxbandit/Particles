//#include <stdlib.h>
#include "Plane.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef GLUT_INC
#define GLUT_INC
#ifdef _WIN32
#include <glut.h> // GLUT
#endif

#ifdef __gnu_linux__ // or __linux__ as well
#include <GL/glut.h> // GLUT
#endif
#endif

    Plane::Plane() {}

    Plane::Plane(Vector3f v1, Vector3f v2, Vector3f v3, Vector3f v4)//IMPROVE: pass normal and origin, deduce 4 vertices with the parametric form
    {
        V1 = v1;
        V2 = v2;
        V3 = v3;
        V4 = v4;
        normal = findNormal(V1, V2, V3);
        origin.x = (V2.x+V4.x)/2;
        origin.y = (V2.y+V4.y)/2;
        origin.z = (V2.z+V4.z)/2;
    }

    Vector3f Plane::findNormal(Vector3f v1, Vector3f v2, Vector3f v3) //FIX
    {
        Vector3f p1 = V1;
        Vector3f p2 = V2;
        Vector3f p3 = V3;

        // normal = p2-p1 cross p3-p1
        Vector3f U = (p2-p1);
        Vector3f V = (p3-p1);
        Vector3f normal = U.crossProduct(V);
        normal.normalize();
        return normal;
    }

    Vector3f Plane::getNormal()
    {
        std::cout<<normal<<std::endl;
        return normal;
    }

    void Plane::draw(Vector3f colour)
    {
        glPushMatrix(); //position in the world
        //glPushAttrib(GL_LIGHTING_BIT); //keeps the colour of the plane independent
        //(note: I presume it is good practice to define the colour of whatever I am drawing
        //immediately before drawing it, so may not have sense pushing and popping everytime)

            //global transformations
            //glTranslatef(position[0],position[1],position[2]); //the aim is NOT to use gl translations!

            glColor3f(colour[0],colour[1],colour[2]);

            glBegin(GL_QUADS);
            glNormal3f( normal.x, normal.y, normal.z);
                glVertex3f( V1[0], V1[1], V1[2] );//V1
                glVertex3f( V2[0], V2[1], V2[2] );//V2
                glVertex3f( V3[0], V3[1], V3[2] );//V3
                glVertex3f( V4[0], V4[1], V4[2] );//V4
            glEnd();

        //glPopAttrib();
        glPopMatrix(); //position in the world
    }

    bool Plane::onPlane(Vector3f point) //IMPROVE
    {
        //add control whether the point is on the drawn plane (i.e. (point-origin).x must be within (v?-v?).x max)
        float dp = normal.dotProduct(point-origin);
        return dp <= 0.00001 && dp >= -0.00001; //finite memory: approximation error
    }

    float Plane::getDistance(Vector3f point)
    {
        return (point-origin).dotProduct(normal);
    }

    Vector3f Plane::getClosestPoint(Vector3f point)
    {
        return point - normal*getDistance(point);
    }


