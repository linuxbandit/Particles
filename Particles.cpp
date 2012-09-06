// Particles.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <windows.h>	// for timeGetTime()
#include <mmsystem.h>	// ditto
#include <iostream>		// I/O
#include <glut.h>// GLUT
#include <time.h>

#include <vector>

#include "ParticleSystem.h"
//#include "Ragno.h"
//#include "Car.h"


//#include "objload.h"

#ifndef PI
#define PI 3.14159265
#endif

#include "Vector.h"

//TENSION
// #include "ParticleSystem.h" AND exclude Particle.h
//


/**********MATERIALS*********/
GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0}; //lol
GLfloat left_light_position[] = {-70,10,-70, 1.0};
GLfloat right_light_position[] = {70,10,70, 1.0};

GLfloat ruby_ambient[]  = {0.1745,	 0.01175,	 0.01175,	 0.55}, 
	    ruby_diffuse[]  = {0.61424,	 0.04136,	 0.04136,	 0.55},
		ruby_specular[] = {0.727811,	 0.626959,	 0.626959,	 0.55}, 
		ruby_shininess  = 76.8;

GLfloat gold_ambient[]  = {0.24725,	 0.1995,	 0.0745,	 1}, 
	    gold_diffuse[]  = {0.75164,	 0.60648,	 0.22648,	 1},
		gold_specular[] = {0.628281,	 0.555802,	 0.366065,	 1}, 
		//gold_specular[] = {0.0,	 0.0,	 0.0,	 1}, 
		gold_shininess  = 51.2;
 
GLfloat jade_ambient[]  = {0.135,	 0.2225,	 0.1575,	 0.95}, 
		//jade_ambient[]  = {0,	 1,	 0,	 0.95}, 
	    jade_diffuse[]  = {0.54,	 0.89,	 0.63,	 0.95},
		jade_specular[] = {0.316228,	 0.316228,	 0.316228,	 0.95}, 
		jade_shininess  = 12.8;

GLfloat grey_ambient[] = {0.01745, 0.01745, 0.01745}, 
		grey_diffuse[] = {0.161424, 0.161424, 0.161424}, 
		grey_specular[] = {0.727811, 0.727811, 0.727811}, 
		grey_shininess = 76.8;

GLfloat plain_ambient[] = {0.01745, 0.01745, 0.01745, 0}, 
		plain_diffuse[] = {1, 1, 1, 1}, 
		plain_specular[] = {0.727811, 0.727811, 0.727811}, 
		plain_shininess = 76.8;
/*****END OF MATERIALS****/

enum {jade,ruby,gold,grey,plain};


//camera views
float zoom = 30.0f;
float rotx = 30.0;
float roty = -220.0f;

//mouse control
float tx = 0;
float ty = 0;
int lastx= 0;
int lasty= 0;
unsigned char Buttons[3] = {0};

//time indicator
int frames=0;

void setupScene();
void updateScene();
void renderScene();
void exitScene();
void keypress(unsigned char key, int x, int y);
void setViewport(int width, int height);

bool		wireframe=true;
int         windowId;
DWORD		lastTickCount;

GLuint		texture[7]; //cube has 6 faces
//GLuint		bodyTexture; //but the array must be filled otherwise it is not going to work! screws the textures


//float pointx=0;//points for the moving leg... IK test..
//float pointz=0;

float progression = 0;
float progressionstep = 0.001;

float cameraRotation;
float cameraTarget;



//Obj::File objFile;

//std::vector<Particle> parts;
std::vector<Vector> forces;

//TENSION
ParticleSystem field; // should call the default constructor! -.-
//

void cameraTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,0,-zoom);
	glTranslatef(tx,ty,0);
	glRotatef(rotx,1,0,0);
	glRotatef(roty-170,0,1,0);
	
}


float* transf(float a,float b,float c){ //DELETE
	float* result=new float[3];
	result[0]=a;
	result[1]=b;
	result[2]=c;
	return result;
}


void print_bitmap_string(void* font, char* s)
{
   if (s && strlen(s)) {
      while (*s) {
         glutBitmapCharacter(font, *s);
         s++;
      }
   }
}


void drawAxes()
{
	glLineWidth(2);

	glColor3f(255,0,0); //RED: x
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(5,0,0);
	glEnd();

	glColor3f(0,0,255); //BLUE: z
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,0,5);
	glEnd();

	glColor3f(0,255,0); //GREEN: y
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,5,0);
	glEnd();

	glLineWidth(1);
}

void drawGrid()
{
	glColor3f(0,0,0); //BLACK: grid
	glBegin(GL_LINES);
	for(int i=-10;i<=10;++i)
	{
		glVertex3f(i,0,-10);
		glVertex3f(i,0,10);

		glVertex3f(10,0,i);
		glVertex3f(-10,0,i);
	}
	glEnd();
}

void fog()
{
	glEnable (GL_FOG); 

	glFogi (GL_FOG_MODE, GL_EXP2);

	GLfloat fogCol[4] = {0.5, 0.5, 0.5, 1.0};
	glFogfv (GL_FOG_COLOR, fogCol); 

	glFogf (GL_FOG_DENSITY, 0.01);

	glHint (GL_FOG_HINT, GL_NICEST); 
}

void setMaterial(int material)
{
	switch (material) {
		case jade:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, jade_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, jade_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, jade_specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, jade_shininess);
			break;
		case ruby:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ruby_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ruby_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ruby_specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, ruby_shininess);
			break;
	}
}

void cubeWorld()
{
	//TUTORIAL texture (not a skybox?)
	
	//NOTE: never mix textures with materials
	// Store the current matrix
    glPushMatrix();
 
    // Reset and transform the matrix.
    //glLoadIdentity(); //mess if uncomment (because it loads the identity without the camera settings)
	//cameraTransform(); //not working :(

    // Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    //glDisable(GL_DEPTH_TEST); //there is no occlusion?!
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
 
    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);
 
 //   // Render the front quad
 //   glBindTexture(GL_TEXTURE_2D, texture[2]);
 //   glBegin(GL_QUADS);
	////glNormal3f(-1, 0, 0); //looks unnecessary... INVESTIGATE.
 //       glTexCoord2f(0, 0); glVertex3f(  50, -0.1f, -50 );
 //       glTexCoord2f(1, 0); glVertex3f( -50, -0.1f, -50 );
 //       glTexCoord2f(1, 1); glVertex3f( -50,  50.1f, -50 );
 //       glTexCoord2f(0, 1); glVertex3f(  50,  50.1f, -50 );
 //   glEnd();
 //
 //   // Render the left quad
 //   glBindTexture(GL_TEXTURE_2D, texture[2]); //  GL knows that I enabled the textures, and it just uses the last binded one
 //   glBegin(GL_QUADS);
	////glNormal3f(1, 0, 0); //looks unnecessary... INVESTIGATE.
 //       glTexCoord2f(0, 0); glVertex3f(  50, -0.1f,  50 );
 //       glTexCoord2f(1, 0); glVertex3f(  50, -0.1f, -50 );
 //       glTexCoord2f(1, 1); glVertex3f(  50,  50.1f, -50 );
 //       glTexCoord2f(0, 1); glVertex3f(  50,  50.1f,  50 );
 //   glEnd();
 //
 //   // Render the back quad
 //   glBindTexture(GL_TEXTURE_2D, texture[2]);
 //   glBegin(GL_QUADS);
	////glNormal3f(0, 0, -1); //looks unnecessary... INVESTIGATE.
 //       glTexCoord2f(0, 0); glVertex3f( -50, -0.1f,  50 );
 //       glTexCoord2f(1, 0); glVertex3f(  50, -0.1f,  50 );
 //       glTexCoord2f(1, 1); glVertex3f(  50,  50.1f,  50 );
 //       glTexCoord2f(0, 1); glVertex3f( -50,  50.1f,  50 );
 //
 //   glEnd();
 //
 //   // Render the right quad
 //   glBindTexture(GL_TEXTURE_2D, texture[2]);
 //   glBegin(GL_QUADS);
	////glNormal3f(0, 0, 1); //looks unnecessary... INVESTIGATE.
 //       glTexCoord2f(0, 0); glVertex3f( -50, -0.1f, -50 );
 //       glTexCoord2f(1, 0); glVertex3f( -50, -0.1f,  50 );
 //       glTexCoord2f(1, 1); glVertex3f( -50,  50.1f,  50 );
 //       glTexCoord2f(0, 1); glVertex3f( -50,  50.1f, -50 );
 //   glEnd();
 //
 //   // Render the top quad
 //   glBindTexture(GL_TEXTURE_2D, texture[1]);
 //   glBegin(GL_QUADS);
	////glNormal3f(0, 1, 0); //looks unnecessary... INVESTIGATE.
 //       glTexCoord2f(0, 1); glVertex3f( -50, 50.1f, -50 );
 //       glTexCoord2f(0, 0); glVertex3f( -50, 50.1f,  50 );
 //       glTexCoord2f(1, 0); glVertex3f(  50, 50.1f,  50 );
 //       glTexCoord2f(1, 1); glVertex3f(  50, 50.1f, -50 );
 //   glEnd();
 
	// Render the bottom quad
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
	glNormal3f(0, 1, 0); //pullula di normali... proprio...
        glTexCoord2f(0, 0); glVertex3f( -50, -0.1f, -50 );
        glTexCoord2f(0, 1); glVertex3f( -50, -0.1f,  50 );
        glTexCoord2f(1, 1); glVertex3f(  50, -0.1f,  50 );
        glTexCoord2f(1, 0); glVertex3f(  50, -0.1f, -50 );
    glEnd();

    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
}

void writeOnScreen(char *message,float x,float y,float window_width,float window_heigth)
{
	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;

	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();

	glLoadIdentity ();

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	
	glLoadIdentity ();
	gluOrtho2D (0.0, window_width, 0.0, window_heigth);
	glRasterPos2f(x, y);
	print_bitmap_string( font_style, message );

	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();

	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
}


void renderScene(){
        
    // Clear framebuffer & depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0,1.0,1.0,1.0); //I actually need to call it only once, so if i put it in setupscene is ok
	cameraTransform();

	glDisable(GL_LIGHTING);

	drawAxes();
	
	drawGrid();
	
	fog();

	cubeWorld();


	//Enable lighting
	glEnable(GL_LIGHTING);


	//WITH = All the light will NOT move with the camera (light fixed even if i rotate)
	//Because i tell at every frame to fix the light at those 2 points
	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);

	setMaterial(jade);
	
	glPushMatrix();
	glTranslatef(field.blackHoleCentre.x,field.blackHoleCentre.y,field.blackHoleCentre.z);
	glutSolidSphere(0.1f,4,4);
	glPopMatrix();

	setMaterial(ruby);



	glEnable(GL_BLEND);

	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glBindTexture(GL_TEXTURE_2D,texture[3]);



	field.update();
	field.draw();
	//std::cout<< field.p[0]->position<<std::endl;//k
	//std::cout<< field.p[0]->velocity<<std::endl;




	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, plain_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plain_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plain_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, plain_shininess);

	glBindTexture(GL_TEXTURE_2D,texture[4]);
	//objFile.Draw();
	//roadroller->drawModel();

	glDisable(GL_TEXTURE_2D);


	/*
	char str[70];
	sprintf(str,"We know black holes resides\n at each galaxy's core");

	writeOnScreen(str,30,30,640,480);
	*/
	
	glutSwapBuffers();
        
}


void updateScene()
{
		frames ++ ;
		// Wait until at least 16ms passed since start of last frame
		// Effectively caps framerate at ~60fps
		while(timeGetTime()-lastTickCount<16);
		lastTickCount=timeGetTime();
    
		/*  Debugs
		 */
		//if(frames%60==1)
		//{
		//
		//	std::cout<<"Spider:"<<std::endl;
		//	std::cout<<spiderRot<<std::endl;
		//	std::cout<<"sin: "<<sin(spiderRot*PI/180)<<std::endl;
		//	std::cout<<"cos: "<<cos(spiderRot*PI/180)<<std::endl<<"---"<<std::endl;
		//	//std::cout<< spiderPosition->x <<" "<< spiderPosition->y <<" "<< spiderPosition->z <<std::endl<<std::endl;
		//	std::cout<< spider->location.x <<" "<< spider->location.y <<" "<< spider->location.z <<std::endl<<std::endl;

		//	std::cout<<"Car:"<<std::endl;
		//	std::cout<<carRot<<std::endl;
		//	std::cout<<"sin: "<<sin(carRot*PI/180)<<std::endl;
		//	std::cout<<"cos: "<<cos(carRot*PI/180)<<std::endl<<"---"<<std::endl;
		//	//std::cout<< carPosition->x <<" "<< carPosition->y <<" "<< carPosition->z <<std::endl<<std::endl;
		//	std::cout<< roadroller->location.x <<" "<< roadroller->location.y <<" "<< roadroller->location.z <<std::endl<<std::endl;
		//}
	

		// ** Do any other updates here ** //

		//	//test on randomness
		//	//float casual = rand();
		//	//bool uno = casual/RAND_MAX > 0.5;
		//	//bool due = (int)casual%2 == 0; // this one test is better
		
		// Draw the next frame
		glutPostRedisplay();

}

void keypress(unsigned char key, int x, int y){
	
	// Test if user pressed ESCAPE (ascii 27)
	// If so, exit the program
    if(key==27){
		exitScene();
	}

	// 'W' key toggles wireframe mode on & off
	/*if(key == 'w' || key == 'W'){
		wireframe=!wireframe;
		if(wireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}*/

	// Other possible keypresses go here
	if(key == 'a'){ 
		field.blackHoleMagnitude -= 0.1;
	}
	if(key == 'd'){ 
		field.blackHoleMagnitude += 0.1;
	}

	if(key == 's'){ 
		field.blackHoleCentre.y-=0.1;
	}
	if(key == 'w'){ 
		field.blackHoleCentre.y+=0.1;
	}


	if(key == 'a'){ // steering
	}

	if(key == 'd'){ // steering
	}
	
	if(key == 's'){ // forward/backward 
	}
	
	if(key == 'w'){ // forward/backward 
	}

	if(key == '5'){ //reset
	}

	if(key == '7'){ // accelerate/decelerate
	}

	if(key == '9'){ // accelerate/decelerate
	}

	
}

void keyreleased(unsigned char key, int x, int y){
	if(key == 'w'){ // 
	}
	if(key == 's'){ // 	
	}

	if(key == 'a'){ // steering
	}

	if(key == 'd'){ // steering
	}
}

//-------------------------------------------------------------------------------
//
void Motion(int x,int y)
{
	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;

	if( Buttons[2] )
	{
		zoom -= (float) 0.15f * diffy;
	}
	else if( Buttons[0] )
	{
		rotx += (float) 0.5f * diffy;
		roty += (float) 0.5f * diffx;		
	}
	else if( Buttons[1] )
	{
		tx += (float) 0.05f * diffx;
		ty -= (float) 0.05f * diffy;
	}
			
}

//-------------------------------------------------------------------------------
//
void Mouse(int b,int s,int x,int y)
{
	lastx=x;
	lasty=y;
	switch(b)
	{
	case GLUT_LEFT_BUTTON:
		Buttons[0] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		Buttons[1] = ((GLUT_DOWN==s)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		Buttons[2] = ((GLUT_DOWN==s)?1:0);
		break;
	default:
		break;		
	}
	
}

void setupScene(){

	std::cout<<"Initializing scene..."<<std::endl;

	std::cout.precision(5);
    
	//Set up Lighting Stuff
	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	
    // Generate GL texture ID & load texture
    
	//glEnable(GL_TEXTURE_2D); //never mix texture with material; therefore I enable and disable it in the mainloop
	
	//glBindTexture(GL_TEXTURE_2D, texture[2]);




	//objFile = Obj::File(); //I don't even have to call delete in this case!
	//objFile.Load("fordfocus128.obj");

	//forces = std::vector<Vector>(2);
	forces.push_back(Vector(-0.01,0.0,0.0));//wind
	forces.push_back(Vector(0.0,0.0,0.0));//not
	forces.push_back(Vector(0.0,-0.09,0.0));//ok 0.15; gravity force
	

	//TENSION
	//field = ParticleSystem(100); //100 particles
	//field(100); //100 particles (wouldn't work anyway)
	field.forces.push_back(Vector(0.0,-0.09,0.0)); //populate with forces (nb: i shouldn't push back; i should substitute the only null force that is there already)
	field.forces.push_back(forces.at(0));

	for(int i=0; i<field.forces.size(); i++){
		std::cout << i << " " << field.forces.at(i) << std::endl;
	}

}

void exitScene()
{

    std::cout<<"Exiting scene..."<<std::endl;

    // Close window
    glutDestroyWindow(windowId);

    // Free any allocated memory

    // Exit program
    std::exit(0);
}

void setViewport(int width, int height) {

    // Work out window ratio, avoid divide-by-zero
    if(height==0) height=1;
	float ratio = float(width)/float(height);

	// Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Fill screen with viewport
	glViewport(0, 0, width, height);

	// Set a 45 degree perspective
	gluPerspective(45, ratio, .1, 1000);

}

int main(int argc, char *argv[]){
        

	srand ( time(NULL) );


    // Initialise OpenGL
    glutInit(&argc, argv); 

    // Set window position, size & create window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(550,50);
    glutInitWindowSize(640,480);
	windowId = glutCreateWindow("IET Physics Lab 1: Particles");
    
    // Set GLUT callback functions
    glutReshapeFunc(setViewport);
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyreleased);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);


	glClearColor(0.5,0.5,0.5,1.0); //I actually need to call it only once, so if i put it in setupscene is ok
    // Setup OpenGL state & scene resources (models, textures etc)
    setupScene();

    // Show window & start update loop
    glutMainLoop();    

    return 0;
    
}
