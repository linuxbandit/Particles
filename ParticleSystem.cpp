#include <stdlib.h>
#include "ParticleSystem.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


    ParticleSystem::~ParticleSystem()
    {
        //for (std::vector<Particle*> member = p.begin(); member != p.end(); member++) //iterators workkkkkk.........
        //  delete *member;
        for(unsigned int i=0; i<p.size(); i++)
            delete p.at(i);
        p.clear();
    }

    ParticleSystem::ParticleSystem(int size /*= 500*/):
        pointOrigin(Vector3f(0,5,0)),
        n(size),
        t(0),
        timestep(0.09),
        Vtimestep(58*timestep),
        blackHoleCentre(Vector3f(0,1,0)),
        blackHoleMagnitude(0.6),
        collisionPlane(Plane(Vector3f(25,-8,25),Vector3f(25,-8,-25),Vector3f(-25,0,-25),Vector3f(-25,0,25)))
    {

        Vector3f random((float)rand() / RAND_MAX *3, (float)rand() / RAND_MAX *3, (float)rand() / RAND_MAX *3);

        //p = std::vector<Particle*>(1); //note: is this calling the default constructor? shouldn't, since it's full of pointers..not objects..

        for(int i=0; i<n; i++){ //probably it will double the size! since i constructed with dimension,say,5 and then i am pushing 5 times other stuff ->5+5 =10
            random.x = ((int)rand()%2 == 0 ? (float)rand() / RAND_MAX : -(float)rand() / RAND_MAX ) *6;
            random.y = ((int)rand()%2 == 0 ? (float)rand() / RAND_MAX : -(float)rand() / RAND_MAX ) *6;
            random.z = ((int)rand()%2 == 0 ? (float)rand() / RAND_MAX : -(float)rand() / RAND_MAX ) *6;

            //std::cout<<pointOrigin+random<<std::endl;

            //big cloud of points
            //p.push_back( new Particle(pointOrigin+random) );// so i create them now

            //sprinkler effect
            p.push_back( new Particle(pointOrigin) );
            p[i]->setVelocity(random*0.1);
        }

        forces.push_back(Vector3f(0,0,0)); // the vector is populated by only one force, a null force

        std::cout << "Particle system built" << std::endl;
    }

    int ParticleSystem::getSystemSize() { return n; }

    void ParticleSystem::setOriginPosition(Vector3f location) { pointOrigin = location; }

    float ParticleSystem::getBlackHoleMagnitude() { return blackHoleMagnitude;  }
    Vector3f ParticleSystem::getBlackHoleCentre() { return blackHoleCentre;  }

    Plane ParticleSystem::getCollisionPlane() { return collisionPlane;  }

    void ParticleSystem::addBlackHoleCentreY(float value) { blackHoleCentre.y += value; }
    void ParticleSystem::addBlackHoleMagnitude(float value) { blackHoleMagnitude += value; }


    std::vector< Vector3f >& ParticleSystem::SystemForces()    { return forces; }
    std::vector< Vector3f >  ParticleSystem::getSystemForces() { return forces; } //this is safe for reading


    // from http://www.xbdev.net/physics/Verlet/index.php
    void ParticleSystem::VerletStep(int i)
    {
        Compute_Forces(i);
        Vector3f newPos;
        newPos = p[i]->getPosition()*2 - p[i]->getPrevPosition() + p[i]->getForce() * (Vtimestep*Vtimestep);
        p[i]->setPrevPosition(p[i]->getPosition());
        p[i]->setPosition(newPos);

        t += Vtimestep; /* update time */
    }

    //from Fabrizio (just less dispersive)
    void ParticleSystem::EulerStepFab(int i)
    {
        Vector3f tempV, tempA, currX, currV; //used for Euler integration at each step
        Compute_Forces(i); /* magic force function */
        tempV = p[i]->getVelocity(); /* get deriv */
        tempA = p[i]->getForce(); /* get deriv */
            tempV *= timestep; /* scale it */
            tempA *= timestep; /* scale it */
            currX = p[i]->getPosition(); /* get state */
            currV = p[i]->getVelocity(); /* get state */
            currX += tempV; /* add */
            currV += tempA; /* add */
            p[i]->setPosition(currX); /* update state */
            p[i]->setVelocity(currV); /* update state */

        t += timestep; /* update time */
    }

    //lol this is funny
    void ParticleSystem::FunnyStep(int i)
    {
        Vector3f tempV, tempA, currX, currV; //used for Euler integration at each step
        Compute_Forces(i); /* magic force function */
            tempV.zero(); tempA.zero();
            tempV += p[i]->getVelocity();
            tempA += p[i]->getForce();
            tempV += tempA;
            tempV *= 0.9;
            tempA *= 0.9;
            p[i]->addPosition(tempV);
        t += timestep; /* update time */
    }


    void ParticleSystem::Compute_Forces(int i)
    {

            try {

            //p.at(i)->setForce(vZero) ; //not needed anymore because I don't add to appliedForce *directly* anymore
            Vector3f tempForce(vZero);

            //compute the vortex centre of gravity (in case I want fancy vortex)
            //p[i]->appliedForce += blackHoleCentre - p[i]->position * blackHoleMagnitude * ( 1/p[i]->position.distanceSq(blackHoleCentre) ) ;

            //simms formula (adds another division by the distance)
            tempForce += (blackHoleCentre - p[i]->getPosition()) /
                                ( (p[i]->getPosition().distanceSq(blackHoleCentre)) * (p[i]->getPosition().distanceSq(blackHoleCentre))// * (p[i]->getPosition.distanceSq(blackHoleCentre))
                                ) * blackHoleMagnitude ;


            for(unsigned int k=0; k < forces.size(); k++)
                tempForce += forces[k];

            p[i]->setForce(tempForce);//cheaper than adding in the for above as I access only once (I am talking about the access to the object, not to the array)

            }catch (std::exception& e) {
                std::cout << "Element " << i << ": index exceeds vector dimensions." << std::endl;
            }

    }


    //scans the array to integrate & update particle by particle.
    //the particle's position is integrated with the integrator
    //of this class and then updated with the particle's update method
    void ParticleSystem::update()
    {

        for(unsigned int i=0; i < p.size(); i++){
            EulerStepFab(i); //integration; I expect the particles to move
            //VerletStep(i);
            //p[i]->coolCollDet(planeNormal);
            p[i]->update(collisionPlane); // I update their time (for death), the color for evolution.
        }
    }

    //scans the particles array and calls their draw method
    void ParticleSystem::draw()
    {
        for(unsigned int i=0; i < p.size(); i++)		p[i]->draw() ;
    }
