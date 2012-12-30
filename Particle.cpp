//#include <stdlib.h>
#include "Particle.h"

#ifdef _WIN32
#include <glut.h> // GLUT
#endif

#ifdef __gnu_linux__ // or __linux__ as well
#include <GL/glut.h> // GLUT
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// because of http://social.msdn.microsoft.com/Forums/en/vcgeneral/thread/05f6bd7c-5543-4b72-bd6d-3bc9bd6b78be
#undef DIVISION

#ifdef _WIN32
#define DIVISION 10 
#endif

#ifdef __gnu_linux__ // or __linux__ as well
#define DIVISION 1000000
#endif

    Particle::Particle(Vector3f location):
        size(2.0),
        startPos(location),
        previousPos(location),
        position(location),
        velocity(vZero),
        appliedForce(vZero),  // nobody starts with any force, not even gravity force
        speed(0),
        damping(0.01),
        ttl(100),
        time(0),
        colour(Vector3f(0,255,0))
	{
        ttl += (int)rand() / DIVISION ;

        velocity = Vector3f(
            (int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3 ,
            (int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3 ,
            (int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3
        ) * 0.1;

	}

    void Particle::draw()
	{
		glPushMatrix(); //position in the world

            //drawing spheres makes it expensive
            //glutSolidSphere(size/8, 20, 20); //size = 0.1

            //drawing vertices is quicker
            glPointSize(size);
            glColor3f(colour[0],colour[1],colour[2]);
            glBegin(GL_POINTS);
                glVertex3f(position[0],position[1],position[2]);
            glEnd();

		glPopMatrix(); //position in the world
	}


    void Particle::addPosition(Vector3f offsetLocation)
    {
        position += offsetLocation;
    }

    void Particle::setPosition(Vector3f newLocation)
    {
        position = newLocation;
    }

    Vector3f Particle::getPosition()
    {
        return position;
    }

    void Particle::setVelocity(Vector3f newVelocity)
    {
        velocity = newVelocity;
    }

    Vector3f Particle::getVelocity()
    {
        return velocity;
    }

    void Particle::setForce(Vector3f newForce)
    {
        appliedForce = newForce;
    }

    Vector3f Particle::getForce()
    {
        return appliedForce;
    }


    void Particle::reset()
	{
		position = startPos;
        velocity = vZero;
		time = 0;
		size = 2.0;
        velocity = Vector3f(
			(int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3 ,
			(int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3 , 
			(int)rand()%2 == 0 ? (float)rand() / RAND_MAX *3 : -(float)rand() / RAND_MAX *3
		) * 0.1;
        colour = Vector3f(0,255,0);
	}

    void Particle::update(Plane collisionPlane)
	{
		
		//Euler integration
		//velocity += appliedForce;
		//position += velocity;//ok

		time++; 
		//NB who should call the check for life? here or particlesystem?
        //size -= 0.001;  //EVOLUTION

        float step = (255+time)/ttl;
        colour.x += step ; //EVOLUTION (based on ttl)
        colour.y -= step ; //EVOLUTION (based on ttl)

        //from http://en.wikipedia.org/wiki/B%C3%A9zier_curve. Doesn't work as good as my method
        /*float tee = time/ttl;
        colour.x = (1-tee)*0.1 + tee*255;
        colour.y = 255 - colour.x;*/

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
        if( -25<position.x && position.x<25 && -25<position.z && position.z<25){

//            std::cout << collisionPlane.origin << "  "  <<  position << std::endl;
//            if (position.y < -8.5) {
//                std::cout << position.y << std::endl;
//                //exit(0);
//            }

            if((position-collisionPlane.origin).dotProduct(collisionPlane.normal) < 0.01 ){

                //position.y = 0;
                //more general:
                float correction = -((position - collisionPlane.origin).dotProduct(collisionPlane.normal));
                position.y += correction ;


                velocity = ( collisionPlane.normal * -(velocity.dotProduct(collisionPlane.normal)) );
                velocity *= 0.7;//0.5 //velocity damping


                /*Vector velocityN, velocityT;

                velocityN = ( planeNormal * -(velocity.dotProduct(planeNormal)) );
                velocityT = velocity - velocityN ;
                velocity = velocityT + velocityN*0.7; */ //should be minus, but velocityN has already it in itself

                if(Vector3f(0,velocity.y,0).length()<0.05){
                    velocity.y=0; // i cant zero x and z
                }
            }
        }

        //was in ParticleSystem, moved here.
        if(time>ttl) {
            reset();
        }

	}

    //deprecated. done in the update function
//    void Particle::coolCollDet(Vector3f planeNormal)
//	{
//		//Xn dot X = d is the vector equation of the plane. d is ...??


//		if( -25<position.x && position.x<25 && -25<position.z && position.z<25){

//            if((position-vZero).dotProduct(planeNormal) < 0.01 ){ //assumption: 0,0,0 is always part of the plane (the "floor" plane has always y=0)

//                //position.y = 0;
//                //more general:
//                float correction = -((position - vZero).dotProduct(planeNormal));
//                position.y += correction ;


//                velocity = ( planeNormal * -(velocity.dotProduct(planeNormal)) );
//                velocity *= 0.7;//0.5 //velocity damping


//                /*Vector velocityN, velocityT;

//                velocityN = ( planeNormal * -(velocity.dotProduct(planeNormal)) );
//                velocityT = velocity - velocityN ;
//                velocity = velocityT + velocityN*0.7; */ //should be minus, but velocityN has already it in itself

//                if(Vector3f(0,velocity.y,0).length()<0.05){
//                    velocity.y=0; // i cant zero x and z
//                }
//            }
//        }
//	}

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

