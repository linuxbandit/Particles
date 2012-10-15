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

#include "Vector.h" //NOTE: no forward declaration, but not even guards... (suggestion from mathieu after forward declaration by chris)
//class Vector;

#include <vector>


class Particle
{
private:
	

public:
	float size; //for evolution

	float mass; //not used for particles
	Vector previousPos; //Verlet integration

	/*Phase vectors*/
	Vector position;
	Vector velocity;

	float speed; //wanted to be a damping parameter, it's unused

	float damping; //another dumping

	Vector appliedForce; //vector containing the cumulative force applied on the particle

	int ttl; //time to live
	int time; //time existed
	Vector startPos; //when ttl expires, I reset its position to this one

	Particle(Vector pos) ;

	//~Particle() ;

	//add status, for the "should I draw it or not"
	

	float getSize();

	void setPosition(Vector location); //useless

	Vector getPosition(); //useless

	void draw(Vector color);

	void forceAccumulate(std::vector<Vector> forces); //moved

	void reset();

	void coolCollDet(Vector planeNormal) ;


	//gonna substitute those methods
	void update();

	void updateForce(Vector newForcePulse);

	void addImpulse(Vector carDirection);
};
