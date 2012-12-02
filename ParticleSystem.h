#ifndef PI
#define PI 3.14159265
#endif

#ifndef NULL
#define NULL 0
#endif

//#include "Vector.h" //NOTE: no forward declaration, but not even guards... (suggestion from mathieu after forward declaration by chris)
#include "Particle.h"

//#include <vector>

class ParticleSystem
{
private:
	

public:

	std::vector<Particle*> p; /* vector of pointers to particles */
	std::vector<Vector> forces;
	int n;		 /* number of particles */
	float t;	 /* simulation clock */
	float timestep; /* Euler's integration */
	float Vtimestep; /* Verlet's integration */


	Vector pointOrigin;

	Vector planeNormal;
	Vector blackHoleCentre;
	float blackHoleMagnitude;



//-------Methods----------


	ParticleSystem(int size) ;
	ParticleSystem() ;

	~ParticleSystem() ;


	int getSystemSize();

	void setPosition(Vector location);

	Vector getPosition();

	void VerletStep(int i);
	void EulerStep(int i);
	void ParticleDerivative(int i, Vector &tempV, Vector &tempA); /* get deriv */
	void ScaleVector(Vector &tempV, Vector &tempA); /* scale it */ //
	void ParticleGetState(int i, Vector &currX, Vector &currV); /* get state */
	void AddVectors(Vector &currX, Vector &currV,Vector &tempV, Vector &tempA); /* add -> temp2 */ //
	void ParticleSetState(int i, Vector &currX, Vector &currV); /* update state */

	void Clear_Forces();
	void Compute_Forces(int i);


	void update();
	void checkParticlesLife();

	void draw();


};

/*

main creates particle system field

main handles particle system who self updates

particle system updates using particles updater (?) NO: using state variables

main calls particle system draw who iterates through particles and calls its method -> particles have a boolean flag 

*/
