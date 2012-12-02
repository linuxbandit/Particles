#ifndef PI
#define PI 3.14159265
#endif

#ifndef NULL
#define NULL 0
#endif

/*
#ifdef _WIN32
#define DIVISION 10
#endif

#ifdef _linux
#define DIVISION 100000
#endif
*/

#include "Vector3f.h" //NOTE: no forward declaration, but not even guards... (suggestion from mathieu after forward declaration by chris)
//class Vector3f;

#ifndef PLANE_H
#include "Plane.h"
#endif

#include <vector>


class Particle
{
private:
	

public:
	float size; //for evolution
    Vector3f colour;//for evolution (works better)

    float mass; //not used for particles (=1)

	/*Phase vectors*/
    Vector3f position;
    Vector3f velocity;

    Vector3f previousPos; //Verlet integration

	float speed; //wanted to be a damping parameter, it's unused
    float damping; //another damping

    Vector3f appliedForce; //vector containing the cumulative force applied on the particle

	int ttl; //time to live
	int time; //time existed
    Vector3f startPos; //when ttl expires, I reset its position to this one


//-------Methods----------


    Particle(Vector3f pos) ;

	//~Particle() ;

	float getSize();

    void setPosition(Vector3f location); //useless
    Vector3f getPosition(); //useless

    void draw();

	void reset();

    void coolCollDet(Vector3f planeNormal) ;

    void update(Plane collisionPlane);

};
