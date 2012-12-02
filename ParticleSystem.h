#ifndef PI
#define PI 3.14159265
#endif

#ifndef NULL
#define NULL 0
#endif

//#include "Vector.h" //NOTE: no forward declaration, but not even guards... (suggestion from mathieu after forward declaration by chris)
#include "Particle.h"

#ifndef PLANE_H
#include "Plane.h"
#endif

//#include <vector>

class ParticleSystem
{
private:
	

public:

	std::vector<Particle*> p; /* vector of pointers to particles */
    std::vector<Vector3f> forces;
	int n;		 /* number of particles */
	float t;	 /* simulation clock */
	float timestep; /* Euler's integration */
	float Vtimestep; /* Verlet's integration */

    Plane collisionPlane; // the floor where the particles bounce

    Vector3f pointOrigin; // where particles will be originated

    Vector3f planeNormal; //will be substituted by the class Plane
    Vector3f blackHoleCentre; //position of the small attractor
    float blackHoleMagnitude; //magnitude of the attractor



//-------Methods----------


	ParticleSystem(int size) ;
	ParticleSystem() ;

	~ParticleSystem() ;


	int getSystemSize();

    void setPosition(Vector3f location);

    Vector3f getPosition();

	void VerletStep(int i);
	void EulerStep(int i);
    void EulerStepFab(int i);
    void ParticleDerivative(int i, Vector3f &tempV, Vector3f &tempA); /* get deriv */
    void ScaleVector(Vector3f &tempV, Vector3f &tempA); /* scale it */ //
    void ParticleGetState(int i, Vector3f &currX, Vector3f &currV); /* get state */
    void AddVectors(Vector3f &currX, Vector3f &currV,Vector3f &tempV, Vector3f &tempA); /* add -> temp2 */ //
    void ParticleSetState(int i, Vector3f &currX, Vector3f &currV); /* update state */

	void Clear_Forces();
	void Compute_Forces(int i);


	void update();

    void draw();


};

/*

main creates particle system field

main handles particle system who self updates

particle system updates using particles updater (?) NO: using state variables

main calls particle system draw who iterates through particles and calls its method -> particles have a boolean flag 

*/
