//OGL2 utilities
//contains some (GLOBAL <-- to be changed) definitions of variables as well as handy methods
//THIS FILE NEEDS TO BE LEFT OUT OF THE COMPILATION 


//TODO: check if cstring included already..
#include <cstring>

/**********MATERIALS*********/

/**********MATERIAL DEFINITION*********/
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
/*****END OF MATERIAL DEFINITION****/

enum {jade,ruby,gold,grey,plain};

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
        case plain:
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, plain_ambient);
	        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plain_diffuse);
	        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plain_specular);
	        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, plain_shininess);
			break;
	}
}

/*****END OF MATERIALS****/

/*****WRITE ON SCREEN*****/
void print_bitmap_string(void* font, char* s)
{
   if (s && strlen(s)) {
      while (*s) {
         glutBitmapCharacter(font, *s);
         s++;
      }
   }
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
/*****END OF WRITE ON SCREEN*****/

/*****MISC**********/
GLuint		texture[7]; //cube has 6 faces
//GLuint		bodyTexture; //but the array must be filled otherwise it is not going to work! screws the textures

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



void cubeWorld()
{
	//TUTORIAL textured cube (not a skybox?)
	
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
	glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0); glVertex3f( -50, -0.1f, -50 );
        glTexCoord2f(0, 1); glVertex3f( -50, -0.1f,  50 );
        glTexCoord2f(1, 1); glVertex3f(  50, -0.1f,  50 );
        glTexCoord2f(1, 0); glVertex3f(  50, -0.1f, -50 );
    glEnd();

    // Restore enable bits and matrix
    glPopAttrib();
    glPopMatrix();
}

/*********END MISC************/

