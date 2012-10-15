#include <stdlib.h>
#include <GL/glut.h>// GLUT
#include "Particle.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


#undef DIVISION

#ifdef _WIN32
#define DIVISION 10
#endif

#ifdef __gnu_linux__ // or __linux__ as well
#define DIVISION 100000
#endif

	Particle::Particle(Vector location)
	{
		size = 2.0;
		startPos = previousPos = position = location;
		velocity = Vector(0,0,0);
		appliedForce = Vector(0,0,0); // nobody starts with gravity force
		speed = 0;
		damping = 0.01;
		ttl = 100 + (int)rand() / DIVISION ;
		time = 0;
	}

	void Particle::draw(Vector color)
	{
		glPushMatrix(); //position in the world

			//global transformations
			glTranslatef(position[0],position[1],position[2]);
			//glRotatef(rotation,0,1,0);
			glScalef(1,1,1);

			glPushMatrix();
			//glutSolidSphere(size/8, 20, 20); //size = 0.1

				glPointSize(size);
				glColor3f(color[0],color[1],color[2]);//doesn't work (neither with [1] nor with .y)
				glBegin(GL_POINTS);
					glVertex3f(position[0],position[1],position[2]);
				glEnd();

			glPopMatrix();

		glPopMatrix(); //position in the world
	}

	void Particle::setPosition(Vector location)
	{
		position = location;
	}


	Vector Particle::getPosition()
	{
		return position;
	}

	void Particle::forceAccumulate(std::vector<Vector> forces) //moved
	{
		appliedForce.zero();
		for(unsigned int i = 0; i < forces.size(); i++)
			appliedForce += forces[i];
	}


	void Particle::reset()
	{
		position = startPos;
		velocity = Vector(0,0,0);
		time = 0;
		size = 2.0;
		velocity = Vector(
			(int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3 ,
			(int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3 , 
			(int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3
		) * 0.1;
	}

	void Particle::update()
	{
		
		//Euler integration
		//velocity += appliedForce;
		//position += velocity;//ok

		time++; 
		//NB who should call the check for life? here or particlesystem?
		size -= 0.001;  //EVOLUTION


		//velocity *= 0.9; //damping; There is no friction (air / inertia from land movement)

		//collision detection with the floor
		//if(position.y < 0) {
		//	position.y = 0; 
		//	velocity.y = -velocity.y;
		//	velocity *= 0.7;//0.5

		//	if(Vector(0,velocity.y,0).length()<0.05){ //trick.. only vertical velocity
		//		velocity.y=0; // i cant zero x and z
		//	}
		//}


		//cool collision detection
		//method called by particleSystem

	}

	void Particle::coolCollDet(Vector planeNormal)
	{
		//Xn dot X = d is the vector equation of the plane. d is ...??


		if( -25<position.x && position.x<25 && -25<position.z && position.z<25){

		if((position-Vector(0,0,0)).dotProduct(planeNormal) < 0.01 ){ //assumption: 0,0,0 is always part of the plane (the "floor" plane has always y=0)
			
			//position.y = 0;
			//more general:
			float correction = -((position - Vector(0,0,0)).dotProduct(planeNormal));
			position.y += correction ;


			velocity = ( planeNormal * -(velocity.dotProduct(planeNormal)) );
			velocity *= 0.7;//0.5 //velocity damping


			/*Vector velocityN, velocityT;
			
			velocityN = ( planeNormal * -(velocity.dotProduct(planeNormal)) );
			velocityT = velocity - velocityN ;
			velocity = velocityT + velocityN*0.7; */ //should be minus, but velocityN has already it in itself
			
			if(Vector(0,velocity.y,0).length()<0.05){
				velocity.y=0; // i cant zero x and z
			}
		}}
	}

	//void Particle::addImpulse(Vector carVelocity) //original
	//{
	//	carVelocity.normalize();
	//	/*float angle = carVelocity.dotProduct(Vector(0,0,1));
	//	Vector temp = carVelocity.getRotated(0,angle,0).getRotated(70,0,0).getRotated(0,-angle,0);*/
	//	carVelocity.y += sin(90*PI/180); //1.5
	//	carVelocity.normalize();
	//	carVelocity *= 1; //impulseStrength;
	//	velocity = carVelocity;
	//}

	//void Particle::update() //original
	//{

	//	//velocity -= Vector(0,0.01,0); //gravity;
	//	velocity -= Vector(0,0.09,0);//ok 0.15

	//	position += velocity;//ok
	//	//velocity *= 0.9; //damping; There is no friction (air / inertia from land movement)

	//	
	//	//collision detection with the floor
	//	if(position.y < 0) {
	//		position.y = 0; 
	//		velocity.y = -velocity.y;
	//		velocity *= 0.7;//0.5
	//		//if(velocity.length()<0.05){// cant use length anymore because now i have components on x and z too (which are not going to be null)
	//		if(Vector(0,velocity.y,0).length()<0.05){ //trick.. only vertical velocity
	//			//velocity.zero(); //something strange happens
	//			velocity.y=0; // i cant zero x and z because now it is wandering
	//		}
	//	}
	//}

