#include <stdlib.h>
#include <GL/glut.h>// GLUT
#include "ParticleSystem.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


	ParticleSystem::ParticleSystem(int size = 1) // NOT DEFAULT CONSTRUCTOR
	{
		std::cout<<"constructed"<<std::endl;
		n = size;
		//p = std::vector<Particle*>(size); //note: is this calling the default constructor? no, since it's full of pointers and not of objects (anyway there's no def.ctor)
		
		for(int i=0; i<n; i++) //probably it will double the size! since i constructed with dimension,say,5 and then i am pushing 5 times other stuff ->5+5 =10
			p.push_back( new Particle(Vector(5,5,5)) );// so i create them now
		
		t = 0;
		forces.push_back(Vector(0,0,0)); // the vector is populated by only one force, a null force
		timestep = 0.01;
	}// NOT DEFAULT CONSTRUCTOR

	ParticleSystem::~ParticleSystem()
	{
		//for (std::vector<Particle*> member = p.begin(); member != p.end(); member++) //iterators workkkkkk.........
		//  delete *member;
		for(int i=0; i<p.size(); i++)
			delete p.at(i);
		p.clear();
	}

	ParticleSystem::ParticleSystem()
	{
		pointOrigin = Vector(0,5,0);

		Vector random((float)rand() / RAND_MAX *3, (float)rand() / RAND_MAX *3, (float)rand() / RAND_MAX *3);

		n = 3000;
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
			p[i]->velocity = random*0.1;
		}

		t = 0;
		forces.push_back(Vector(0,0,0)); // the vector is populated by only one force, a null force
		timestep = 0.09;
		Vtimestep = 8 * timestep;

		planeNormal = Vector(0,1,0);
		blackHoleCentre = Vector(0,1,0);
		blackHoleMagnitude = 0.6;
	}

	int ParticleSystem::getSystemSize()
	{
		return n;
	}

	void ParticleSystem::EulerStep(int i)
	{
		Vector tempV, tempA, currX, currV; //used for Euler integration at each step
		ParticleDerivative(i,tempV,tempA); /* get deriv */
		ScaleVector(tempV,tempA); /* scale it */
		ParticleGetState(i,currX,currV); /* get state */
		AddVectors(currX,currV,tempV,tempA); /* add */
		ParticleSetState(i,currX,currV); /* update state */
		t += timestep; /* update time */
	}

	void ParticleSystem::VerletStep(int i)
	{
		// from http://www.xbdev.net/physics/Verlet/index.php
		Compute_Forces(i);

		p[i]->position = p[i]->position*2 - p[i]->previousPos + p[i]->appliedForce * (Vtimestep*Vtimestep);
		p[i]->previousPos = p[i]->position;

		t += Vtimestep; /* update time */
	}


	/* gather state from the particles into dst */
	void ParticleSystem::ParticleGetState(int i, Vector &currX, Vector &currV){//temp2
		//for(int i=0; i < p.size(); i++){
			currX = p[i]->position;
			currV = p[i]->velocity;
	}

	/* scatter state from src into the particles */
	void ParticleSystem::ParticleSetState(int i, Vector &currX, Vector &currV)
	{
			p[i]->position = currX;
			p[i]->velocity = currV;
	}

	/* calculate derivative, place in dst */
	void ParticleSystem::ParticleDerivative(int i, Vector &tempV, Vector &tempA)//temp1
	{
		//Clear_Forces(); /* zero the force accumulators */
		Compute_Forces(i); /* magic force function */
			tempV = p[i]->velocity; /* xdot = v */
			tempA = p[i]->appliedForce; /* vdot = f/m */
	}

	void ParticleSystem::Clear_Forces()
	{
		for(int i=0; i < p.size(); i++) p[i]->appliedForce.zero();
	}

	void ParticleSystem::Compute_Forces(int i)
	{

			try {

			p.at(i)->appliedForce.zero() ;

			//compute the vortex centre of gravity (in case I want fancy vortex)
			//p[i]->appliedForce += blackHoleCentre - p[i]->position * blackHoleMagnitude * ( 1/p[i]->position.distanceSq(blackHoleCentre) ) ;

			//simms formula (adds another division by the distance)
			p[i]->appliedForce += (blackHoleCentre - p[i]->position) / 
								( (p[i]->position.distanceSq(blackHoleCentre)) * (p[i]->position.distanceSq(blackHoleCentre))// * (p[i]->position.distanceSq(blackHoleCentre))
								) * blackHoleMagnitude ;

			for(int k=0; k < forces.size(); k++)
				p[i]->appliedForce += forces[k];

			}catch (std::exception& e) {
				std::cout << "Element " << i << ": index exceeds vector dimensions." << std::endl;
			}

	}

	void ParticleSystem::ScaleVector(Vector &tempV, Vector &tempA) //inertia?
	{
		tempV *= timestep;
		tempA *= timestep;
	}

	void ParticleSystem::AddVectors(Vector &currX, Vector &currV,Vector &tempV, Vector &tempA)
	{
		currX += tempV;
		currV += tempA;
	}

	void ParticleSystem::update()
	{
		
		for(int i=0; i < p.size(); i++){
			EulerStep(i); //integration; I expect the particles to move
			//VerletStep(i);
			p[i]->update(); // I update ONLY their time (for death), then (below) I check if they really are dead
			p[i]->coolCollDet(planeNormal);
		}
		checkParticlesLife(); //should this routine be here or in update?
	}

	void ParticleSystem::checkParticlesLife()
	{
		for(int i=0; i < p.size(); i++)
			if(p[i]->time>p[i]->ttl) {
				p[i]->reset();
			}
	}

	void ParticleSystem::draw()
	{
		for(int i=0; i < p.size(); i++)		p[i]->draw(Vector(255,0,255)) ;
	}