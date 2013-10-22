#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H


#ifndef NULL
#define NULL 0
#endif

#include <vector>

#include "Vector3f.h" //NOTE: no forward declaration, but not even guards... (suggestion from mathieu after forward declaration by chris)
#include "Particle.h"

#include "Plane.h"

#define PARTICLES_NUMBER 3000; //not yet used as it can't be the default parameter



class ParticleSystem
{
private:

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

public:

//-------Methods----------


    //--Construction/distruction
    ParticleSystem( int size = 500 ) ;

    ~ParticleSystem() ;


    //---system characteristics
    int getSystemSize();

    void setOriginPosition(Vector3f location);

    float getBlackHoleMagnitude();
    Vector3f getBlackHoleCentre();

    Plane getCollisionPlane();

    void addBlackHoleCentreY(float value);
    void addBlackHoleMagnitude(float value);

    std::vector< Vector3f >& SystemForces();
    std::vector< Vector3f > getSystemForces(); //read-safe


    //--system utilities
    void VerletStep(int i);
    void EulerStep(int i); //Integrator
    void EulerStepFab(int i);
    void FunnyStep(int i);
    void ParticleDerivative(int i, Vector3f &tempV, Vector3f &tempA); /* get deriv */
    void ScaleVector(Vector3f &tempV, Vector3f &tempA); /* scale it */ //
    void ParticleGetState(int i, Vector3f &currX, Vector3f &currV); /* get state */
    void AddVectors(Vector3f &currX, Vector3f &currV,Vector3f &tempV, Vector3f &tempA); /* add -> temp2 */ //
    void ParticleSetState(int i, Vector3f &currX, Vector3f &currV); /* update state */

    void Compute_Forces(int i);


    //--system draw/update
    void update();

    void draw();


};

#endif //PARTICLE_SYSTEM_H
/*

main creates particle system field

main handles particle system who self updates

particle system updates using particles updater (?) NO: using state variables

main calls particle system draw who iterates through particles and calls its method -> particles have a boolean flag

*/
