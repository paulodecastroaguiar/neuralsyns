//************************************************************************
//*                                                                      *
//*                            G R A P H I C S                           *
//*                                                                      *
//************************************************************************

/**
 * \file graphics.c
 * \author Paulo Aguiar, with help from Ricardo Aguiar
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "general.h"
#include "models.h"
#include "graphics.h"
#include "quaternions.h"
#include "populations.h"
#include "neurons.h"
#include "synapses.h"
#include "electrodes.h"

extern struct POPULATION *population;
extern unsigned int total_populations;
extern struct NEURON *neuron;
extern unsigned long total_neurons;
extern unsigned long total_synapses;
extern struct CONNECTION **connection;

extern struct FLAGS flags;

extern struct GRAPHICS graphics;

extern double t;
extern unsigned long sim_step;

extern gsl_rng *rng;		//pointer to the global random number generator


int neuron_repres_style;
char *neuron_repres_style_string;
short flag_schematic_representation;

double frames_fps = 0.0;

GLuint **displaylist[MAX_DL_STEPS];   //precompiled OpenGL commands to draw fdomains (would fit appropriately in FDOMAIN_GLOBAL but it's safer here)
GLuint spike_DisplayList;



// Light values and coordinates
GLfloat  lightPos[] = { 0.0f, 0.0f, 100.0f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f};
GLfloat  spotDir[] = { 0.0f, 0.0f, -1.0f };

// Rotation amounts
static GLfloat light_xRot = 0.0f;
static GLfloat light_yRot = 0.0f;

GLfloat colorFog[] = { 0.2f, 0.2f, 0.2f, 1.0f };

GLUquadricObj *quadric;


//Quaternion Cam Control Variables
Vec3 eyeVec = { 0.0, 0.0, 1.0 };
Vec3 centerVec = { 0.0f, 0.0f, 0.0f };
Vec3 upVec = { 0.0, 1.0, 0.0 };
Quat cameraQuat;
Quat eyeVecQuat;
Quat eyeVecQuatX;


//neurons volume
float min_x, max_x, min_y, max_y, min_z, max_z, d_model;


//------------------------------------------------------------------------

/**
 * Prepares graphical interface and all graphical variables
 */

void Prepare_Graphics(int argc, char *argv[])
{
	unsigned int pop, i;
  unsigned long nrn;
  double x, y, z;
  GLdouble sizes[2];
  GLdouble granularity;

  Vec3 axis = { 0.0, 1.0, 0.0 };


  InitializeQuaternion( &cameraQuat, axis, 40.0);


	for(i=0; i<MAX_DL_STEPS; i++) {		
		displaylist[i] = calloc( total_populations, sizeof( GLuint * ) );
		if( displaylist[i] == NULL ) {
			printf("\n\tERROR: unable to allocate memory for graphical display lists.");
			printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
			Free_Mem();
			exit(-1);
		}		
		for( pop = 0; pop < total_populations; pop++) {
			displaylist[i][pop] = calloc( population[pop].n_fdomains, sizeof( GLuint ) ) ;
			if( displaylist[i][pop] == NULL ) {
				printf("\n\tERROR: unable to allocate memory for graphical display lists.");
				printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
				Free_Mem();
				exit(-1);
			}
		}
	}


  glutInit(&argc, argv);


  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  glutCreateWindow("NeuralSyns");

  neuron_repres_style = SCHEME_STYLE;
  neuron_repres_style_string = "Schematic Representation";


  // OPENGL CALLBACKS
  glutDisplayFunc(Update_Graphics);	
  glutIdleFunc(Update_Graphics);
  glutReshapeFunc(Window_Resize);
  glutMouseFunc(Mouse_Buttons);
  glutMotionFunc(Mouse_Motion);
  glutPassiveMotionFunc(Mouse_Passive_Motion);
  glutKeyboardFunc(Process_Normal_Keys);
  glutSpecialFunc(Process_Special_Keys);


  // CREATE NEURONS REPRESENTATIONS
  quadric = gluNewQuadric();
  Create_DisplayLists(graphics.scale_factor, 20, 20);

  
  glGetDoublev(GL_POINT_SIZE_RANGE, sizes);
  glGetDoublev(GL_POINT_SIZE_GRANULARITY, &granularity);
  graphics.dot_size = 2.0; //the above variables are not used because a size of 2 will be available (almost) for sure
  glPointSize(graphics.dot_size);


  Create_Voltage2Color_Palette();
  Create_Frequency2Color_Palette();


  // ACTIVATE LIGHT
  initLighting();

  // ACTIVATE FOG
  //setupFog(0.01, colorFog, GL_EXP, 1, 10);

  CreateMenus();

  glEnable(GL_DEPTH_TEST);	//glDisable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  //Transparency. The proper way to deal with transparency would be:
  // 1) disable depth test;
  // 2) draw all in z order (something demanding to do in a network with 1000's of neurons...);
  // 3) enable depth test
  // Neurons however are drawn without z order (the draw order is the id order... sorry about this!)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



  // ANTIALISING STUFF
  //glEnable(GL_POINT_SMOOTH);
  //glEnable(GL_LINE_SMOOTH);
  //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


  // get network box size
  min_x = neuron[0].position.x;  max_x = neuron[0].position.x;
  min_y = neuron[0].position.y;  max_y = neuron[0].position.y;
  min_z = neuron[0].position.z;  max_z = neuron[0].position.z;

  for (nrn = 0; nrn < total_neurons; nrn++) {
    x = neuron[nrn].position.x;
		y = neuron[nrn].position.y;
    z = neuron[nrn].position.z;
    if (x > max_x) {      max_x = x;    }
    if (x < min_x) {      min_x = x;    }
    if (y > max_y) {      max_y = y;    }
    if (y < min_y) {      min_y = y;    }
    if (z > max_z) {      max_z = z;    }
    if (z < min_z) {      min_z = z;    }
  }

  // get model max length (model inside a xy box)
  d_model = max_x - min_x;
  if( max_y - min_y > d_model ) {    d_model = max_y - min_y;  }


  // fill the graphics info structure with its actual settings
  graphics.Buttons[0] = 0;
  graphics.Buttons[1] = 0;
  graphics.Buttons[2] = 0;

  graphics.origin_x = 0.5 * (min_x + max_x);
  graphics.origin_y = 0.5 * (min_y + max_y);
  graphics.origin_z = 0.5 * (min_z + max_z);

  lightPos[0] = graphics.origin_x;
  lightPos[1] = graphics.origin_y;
  lightPos[2] = d_model / ( 2.0 * tan( 0.5 * FOV_XY * PI / 180.0) ) + max_z;
	if( lightPos[2] < 200.0 ) {
    lightPos[2] = 200.0;
  }

  eyeVec.x = graphics.origin_x;
  eyeVec.y = graphics.origin_y;
  eyeVec.z = graphics.origin_z + 2.0 * ( max_z - min_z ) + d_model + 100.0;

  centerVec.x = graphics.origin_x;
  centerVec.y = graphics.origin_y;
  centerVec.z = graphics.origin_z;

  graphics.lastx = 0;
  graphics.lasty = 0;

  graphics.neuron_size = NEURON_BALL_RADIUS;
  graphics.spike_size = SPIKE_BALL_RADIUS;

  graphics.close_graphics = 0;	
	if( flags.graphics == 2 ) {
		graphics.pause_graphics = 1;
	}
	else {
		graphics.pause_graphics = 0;
	}
  graphics.step1_graphics = 0;

  graphics.help = 0;
  graphics.target_info = 0;
  graphics.popcolorcode = 0;
  graphics.target_nrn = -1;
	graphics.pick_target = 0;
  graphics.display_light = 0;

  graphics.population_dimx = (int *) malloc( total_populations * sizeof(int));
  graphics.population_dimy = (int *) malloc( total_populations * sizeof(int));
  if ( graphics.population_dimx == NULL || graphics.population_dimy == NULL ) {
    printf("\n\tERROR: unable to allocate memory for data regarding the Schematic Representation.");
    printf("\n\tERROR occured in %s: %d\n\n", __FILE__, __LINE__);
		Free_Mem();
		exit(-1);
  }


  return;
}





//------------------------------------------------------------------------

/**
 * Updates graphics;
 * Be careful about everything you write here; performance is paramount!
 */

void Update_Graphics(void)
{
  float transfMatrix[16];


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	QuatToMatrix(&cameraQuat, transfMatrix);

  glLoadIdentity();

  //gluLookAt(centerVec.x, centerVec.y, centerVec.z, eyeVec.x, eyeVec.y, eyeVec.z, upVec.x, upVec.y, upVec.z);
  gluLookAt( eyeVec.x, eyeVec.y, eyeVec.z, centerVec.x, centerVec.y, centerVec.z, upVec.x, upVec.y, upVec.z);


  if( neuron_repres_style != SCHEME_STYLE ) {

    glPushMatrix();

    // Rotate coordinate system
    glRotatef(light_xRot, 0.0f, 1.0f, 0.0f);
    glRotatef(light_yRot, 1.0f, 0.0f, 0.0f);
    
    // Specify new position and direction in rotated coords.
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
    
    // Light source graphical representation
    if( graphics.display_light == 1) {
      glColor3f(0.0,0.0,1.0);      
      glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
      glutSolidCone(50.0f,50.0f,15,15);    
      glColor3f(1.0,1.0,1.0);    
      glutSolidSphere(30.0f, 15, 15);
    }

    glPopMatrix();
  }


  //update graphical representation of NEURONS
  switch (neuron_repres_style) {

  case DOT_STYLE:
		if( graphics.pick_target == 1 ) {
			drawPickingMode();
			graphics.target_nrn = processPick();
		}
		else {
			Draw_Dots();
		}
    flag_schematic_representation = 0;
    break;

  case SPHERE_STYLE:
		if( graphics.pick_target == 1 ) {
			drawPickingMode();
			graphics.target_nrn = processPick();
		}
		else {
			Draw_Neurons();
		}
    flag_schematic_representation = 0;
    break;

  case SCHEME_STYLE:
    //to improve performance, the schematic representation is drawn inside the Update_Orthographic_View_Info() 
    flag_schematic_representation = 1;
    break;
  }


  //Draw spikes: if you are really keen in consistently using the graphical visualization, you can perform this operation Update_Spike to save a repeated cicle over all synapses
  if( graphics.draw_spikes == 1 && neuron_repres_style != SCHEME_STYLE && graphics.pick_target != 1 ) {
    Draw_Spikes();
  }


	//Draw target connections
	if( graphics.target_nrn > -1 &&  graphics.target_nrn < total_neurons ) {
		Draw_OutputPlexus( graphics.target_nrn );
		Draw_InputPlexus( graphics.target_nrn );
	}

  //Draw sample connections
  if( graphics.draw_network_connections == 1 && neuron_repres_style != SCHEME_STYLE && graphics.pick_target != 1 ) {
    Draw_Sample_Connections();
  }

  //Draw all connections
  if( graphics.draw_network_connections == 2 && neuron_repres_style != SCHEME_STYLE && graphics.pick_target != 1 ) {
    Draw_All_Connections();
  }




  glDisable(GL_DEPTH_TEST);

  Update_Orthographic_View_Info();

  glEnable(GL_DEPTH_TEST);

	if( graphics.pick_target == 0 ) {
		glutSwapBuffers();
	}
	
	graphics.pick_target = 0;


  return;
}





//------------------------------------------------------------------------

/**
 * Creates and compiles glutSolidSphere (soma), gluCylinder (dendrites), 
 * gluCylinder (axonal plexus) and glutSolidSphere(spike) in display lists
 * \param scalefactor scale factor from [um] to pixels
 * \param slices number of subdivisions around the z axis of the sphere. 
 * \param stacks number of subdivisions along the z axis oh the sphere
 */

void Create_DisplayLists(GLdouble scalefactor, GLint slices, GLint stacks)
{
  unsigned int i, pop, fdm, parent_fdm = 0;

  GLint n_slices, n_stacks;
  GLfloat r, h, x0, x1, y0, y1, z0, z1, lx, ly, lz, theta_x, theta_y, h_xOz;

  GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 100.0 }; //intensity of the specular component, [0,128]


  //go through all fdomains in all populations and create the display list for the fdomain 3D representation
  for( pop = 0; pop < total_populations; pop++) {
		for( fdm = 0; fdm < population[pop].n_fdomains; fdm++) {
			
			//produce display lists for MAX_DL_STEPS levels of detail
			for( i = 0; i < MAX_DL_STEPS; i++) {
				
				displaylist[i][pop][fdm] = glGenLists(1);
				
				n_slices = slices - i;
				if (n_slices < 4) {
					n_slices = 4;
				}
				n_stacks = n_slices;
				
				r = population[pop].fdomain[fdm].position.r;

				// FDM == 0
				if( fdm == 0 ) {
					/* cylinder
					h = r;

					//start display list
					glNewList( displaylist[i][pop][fdm], GL_COMPILE );

					glShadeModel(GL_SMOOTH);		  
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);		  

					gluQuadricOrientation( quadric, GLU_INSIDE );
					gluDisk( quadric, 0.0f, r, n_slices, 1 );
					gluQuadricOrientation( quadric, GLU_OUTSIDE );
					gluCylinder( quadric, r, r, 2.0f*h, n_slices, n_stacks );
					glTranslatef( 0.0f, 0.0f, 2.0f*h );
					gluDisk(quadric, 0.0f, r, n_slices, 1);
					glTranslatef( 0.0f, 0.0f, -h );

					glEndList();
					//end display list
					*/

					//start display list
					glNewList( displaylist[i][pop][fdm], GL_COMPILE );

					glShadeModel(GL_SMOOTH);		  
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);		  

					glutSolidSphere(r, n_slices, n_stacks );

					glEndList();
					//end display list
				}
				// FDM > 0
				else {
					parent_fdm = population[pop].fdomain[fdm].parent_fdomain;

					x0 = population[pop].fdomain[parent_fdm].position.x;
					x1 = population[pop].fdomain[fdm].position.x;
					y0 = population[pop].fdomain[parent_fdm].position.y;
					y1 = population[pop].fdomain[fdm].position.y;
					z0 = population[pop].fdomain[parent_fdm].position.z;
					z1 = population[pop].fdomain[fdm].position.z;
					lx = x1 - x0;
					ly = y1 - y0;
					lz = z1 - z0;
					h = sqrt( lx*lx + ly*ly + lz*lz );
					h_xOz = sqrt( lx*lx + lz*lz );
					theta_x = -atan2( ly, h_xOz ) * 180.0 / PI ;
					theta_y = atan2( lx, lz ) * 180.0 / PI;

					//start display list
					glNewList( displaylist[i][pop][fdm], GL_COMPILE );
					glShadeModel(GL_SMOOTH);					
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
					glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);					

					glTranslatef( x0, y0, z0);
					glRotatef( theta_y, 0.0f, 1.0f, 0.0f); //rotate theta_y around ey
					glRotatef( theta_x, 1.0f, 0.0f, 0.0f); //rotate theta_x around ex

					gluQuadricOrientation( quadric, GLU_INSIDE );
					gluDisk(quadric, 0.0, r, n_slices, 1);
					gluQuadricOrientation( quadric, GLU_OUTSIDE );
					gluCylinder(quadric, r, r, h, n_slices, n_stacks);
					glTranslatef(0.0, 0.0, h);
					gluDisk(quadric, 0.0, r, n_slices, 1);

					glEndList();
					//end display list
		  
				}				
			}
			
			
		}
  }


  //SPIKE
  spike_DisplayList = glGenLists(1);

	//start display list
  glNewList(spike_DisplayList, GL_COMPILE);
  glShadeModel(GL_SMOOTH);
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  
  glutSolidSphere(SPIKE_BALL_RADIUS, 6, 6);
    
  glEndList();
	//end display list
	

  return;
}





//------------------------------------------------------------------------

/**
 * Draws all neurons as colored dots
 */

void Draw_Dots(void)
{
  unsigned int pop;
  unsigned long nrn;
  struct RGBAColor color;

  glBegin(GL_POINTS);

  for (pop = 0; pop < total_populations; pop++) {

    for (nrn = population[pop].first_neuron; nrn < population[pop].first_neuron + population[pop].n_neurons; nrn++) {

			if( graphics.popcolorcode == 0 ) {
				color = Voltage2Color( nrn, 0 ); //fdm=0
			}
			else {
				color = Population2Color(neuron[nrn].population);
			}
			glColor4f(color.r, color.g, color.b, color.a);
			glVertex3f(neuron[nrn].position.x, neuron[nrn].position.y, neuron[nrn].position.z);
    }

  }

  glEnd();

  return;
}





//------------------------------------------------------------------------

/**
 * Draws a NEURON with membrane potential color code
 */

void Draw_Neurons(void)
{
  unsigned int pop, fdm;
  unsigned long nrn;
  int detail_reduction_level;
  double dist, d_x, d_y, d_z;
  struct RGBAColor color;

  for (nrn = 0; nrn < total_neurons; nrn++) {

    //calculate distance between neuron and camera
    d_x = neuron[nrn].position.x - eyeVec.x;
    d_y = neuron[nrn].position.y - eyeVec.y;
    d_z = neuron[nrn].position.z - eyeVec.z;
    dist = sqrt( d_x * d_x + d_y * d_y + d_z * d_z );  
    if( dist < 1.0) {    dist = 1.0;  }
    if( dist > MINIMUM_DETAIL_DISTANCE) {    dist = MINIMUM_DETAIL_DISTANCE;  }
    
    //define the detail level to draw the neuron; the factor used is for the defined FOV
    detail_reduction_level = (int) ( (MAX_DL_STEPS - 1) * dist / MINIMUM_DETAIL_DISTANCE );
    
		glPushMatrix();

    glTranslatef(neuron[nrn].position.x, neuron[nrn].position.y, neuron[nrn].position.z);

		pop = neuron[nrn].population;

    //soma
		if( graphics.popcolorcode == 0 ) {
			color = Voltage2Color( nrn, 0 ); //fdm=0
		}
		else {
			color = Population2Color(neuron[nrn].population);
		}
    glColor4f(color.r, color.g, color.b, color.a);
		glCallList( displaylist[detail_reduction_level][pop][0] );

		//dendrites / functional domains
    for (fdm = 1; fdm < population[neuron[nrn].population].n_fdomains; fdm++) {
			if( graphics.popcolorcode == 0 ) {
				color = Voltage2Color( nrn, fdm );
			}
			else {
				color = Population2Color(neuron[nrn].population);
			}
      glColor4f(color.r, color.g, color.b, color.a);
			glPushMatrix();				
			glCallList( displaylist[detail_reduction_level][pop][fdm] );                  
			glPopMatrix();
    }    
    
    glPopMatrix();
		

  }

  return;
}





//------------------------------------------------------------------------

/**
 * Draws a schematic diagram of the network state
 */

void Draw_Schematic(void)
{
  unsigned int pop, length;;
  unsigned long nrn;
  float dist, dp, r;
  struct RGBAColor color;

  dp = graphics.window_size_w / (total_populations + 1) / (total_populations + 1);
  dist = graphics.dist_factor * graphics.window_size_w;
  r = 0.2 * graphics.window_size_h / ( graphics.n_lines + 3);

  glShadeModel(GL_FLAT);
  
  if( graphics.n_lines < 1) {
    graphics.n_lines = 1;	}	//shouldn't be needed... but it is
  length = 0;
  
  for( pop=0; pop<total_populations; pop++ ) {    
    for( nrn=0; nrn<population[pop].n_neurons; nrn++ ) {
      
      glPushMatrix();      
      glTranslatef( (pop + 1) * dp +  dist * ( length + ((int) ( nrn / graphics.n_lines)) ), dp + dist * (nrn % graphics.n_lines), 0);      
      color = Voltage2Color( nrn + population[pop].first_neuron, 0 ); //fdm=0      
      glColor3f(color.r, color.g, color.b);      
      gluDisk(quadric, 0.0, r , 8, 1);
            
      if( flags.enable_rate_estimation == 1) {	
				glTranslatef( r + r, 0, 0);				
				color = Frequency2Color( neuron[nrn + population[pop].first_neuron ].firing_rate );				
				glColor3f(color.r, color.g, color.b);	
				glRectf(-r, -r, 0.0, r);	
      }
      
      glPopMatrix();
      
    }    
    length += graphics.population_dimx[pop];
  }

  return;
}





//------------------------------------------------------------------------

/**
 * Draws connections of the first unit on each population
 */

void Draw_Sample_Connections(void)
{
  unsigned int pop;

  for (pop = 0; pop < total_populations; pop++) {
    if (population[pop].n_neurons > 0) {
      //choose 1st NEURON from the population
      Draw_OutputPlexus(population[pop].first_neuron);
    }
  }

  return;
}




//------------------------------------------------------------------------

/**
 * Draws lines joining all NEURONS that are synapticaly connected
 */

void Draw_All_Connections(void)
{
  unsigned long nrn;

  for (nrn = 0; nrn < total_neurons; nrn++) {
    Draw_OutputPlexus(nrn);
  }

  //Draw_Synapses();

  return;

}





//------------------------------------------------------------------------

/**
 * Draws lines joining all output of a given NEURON
 * \param nrn_pre neuron structure id
 */

void Draw_OutputPlexus(long nrn_pre)
{
  unsigned long syn, nrn_post;
	unsigned int pop_post, fdm_post, parent_fdm_post;
  float dx, dy, dz, alpha;

	if( graphics.draw_network_connections == 2 ) {
		alpha = 0.1;	}
	else {
		alpha = 0.5;	}


  if (neuron[nrn_pre].n_synapses > 0) {	
		for (syn = 0; syn < neuron[nrn_pre].n_synapses; syn++) {
		
			nrn_post = neuron[nrn_pre].synapse[syn].target_neuron;
			pop_post = neuron[nrn_post].population;
			fdm_post = connection[ neuron[nrn_pre].population ][ pop_post ].fdomain;
			parent_fdm_post = population[ pop_post ].fdomain[fdm_post].parent_fdomain;

			if( parent_fdm_post == -1 ) { //fdm00 condition
				dx = population[ pop_post ].fdomain[fdm_post].position.x;
				dy = population[ pop_post ].fdomain[fdm_post].position.y;
				dz = population[ pop_post ].fdomain[fdm_post].position.z;
			}
			else {
				dx = 0.5*( population[ pop_post ].fdomain[ fdm_post ].position.x + population[ pop_post ].fdomain[parent_fdm_post].position.x );
				dy = 0.5*( population[ pop_post ].fdomain[ fdm_post ].position.y + population[ pop_post ].fdomain[parent_fdm_post].position.y );
				dz = 0.5*( population[ pop_post ].fdomain[ fdm_post ].position.z + population[ pop_post ].fdomain[parent_fdm_post].position.z );
			}

			glBegin(GL_LINES);
      
			glColor4f(1.0, 0.25, 0.25, alpha);
			glVertex3f(neuron[nrn_pre].position.x, neuron[nrn_pre].position.y, neuron[nrn_pre].position.z);
			glColor4f(1.0, 0.5, 0.5, alpha);
			glVertex3f(neuron[nrn_post].position.x + dx, neuron[nrn_post].position.y + dy, neuron[nrn_post].position.z + dz);
      
			glEnd();
      
		}      
	}  

	return;
}





//------------------------------------------------------------------------

/**
 * Draws lines joining all inputs of a given NEURON. This is an heavy function as it goes through all synapses to find the input connections.
 * \param nrn_post neuron structure id
 */

void Draw_InputPlexus(long nrn_post)
{
  unsigned long syn, nrn_pre;
	unsigned int pop_post, fdm_post, parent_fdm_post;
  float dx, dy, dz, alpha;

	pop_post = neuron[nrn_post].population;

	if( graphics.draw_network_connections == 2 ) {
		alpha = 0.1;	}
	else {
		alpha = 0.5;	}

	for( nrn_pre = 0; nrn_pre < total_neurons; nrn_pre++ ) {
		if (neuron[nrn_pre].n_synapses > 0) {	
			for (syn = 0; syn < neuron[nrn_pre].n_synapses; syn++) {

				if( neuron[nrn_pre].synapse[syn].target_neuron == nrn_post ) {
				
					fdm_post = connection[ neuron[nrn_pre].population ][ pop_post ].fdomain;
					parent_fdm_post = population[ pop_post ].fdomain[fdm_post].parent_fdomain;

					if( parent_fdm_post == -1 ) { //fdm00 condition
						dx = population[ pop_post ].fdomain[fdm_post].position.x;
						dy = population[ pop_post ].fdomain[fdm_post].position.y;
						dz = population[ pop_post ].fdomain[fdm_post].position.z;
					}
					else {
						dx = 0.5*(population[pop_post].fdomain[fdm_post].position.x + population[pop_post].fdomain[parent_fdm_post].position.x );
						dy = 0.5*(population[pop_post].fdomain[fdm_post].position.y + population[pop_post].fdomain[parent_fdm_post].position.y );
						dz = 0.5*(population[pop_post].fdomain[fdm_post].position.z + population[pop_post].fdomain[parent_fdm_post].position.z );
					}

					glBegin(GL_LINES);
					
					glColor4f(0.25, 0.25, 1.0, alpha);
					glVertex3f(neuron[nrn_pre].position.x, neuron[nrn_pre].position.y, neuron[nrn_pre].position.z);
					glColor4f(0.5, 0.5, 1.0, alpha);
					glVertex3f(neuron[nrn_post].position.x + dx, neuron[nrn_post].position.y + dy, neuron[nrn_post].position.z + dz);
					
					glEnd();

				}
				
			}      
		} 

	} 
		
	return;
}





//------------------------------------------------------------------------

/**
 * Represents each SYNAPSE
 */

void Draw_Synapses(void)
{
	//hum... let's not exagerate okay...
  return;
}





//------------------------------------------------------------------------

/**
 * Draws Spikes in the path between the intervening Neurons
 * Do not worry too much about "optimization" here... it is useless :)
 * -> if you want optimization do not use this at all!
 */

void Draw_Spikes(void)
{
  unsigned int pop_post, fdm_post, parent_fdm_post, spk, index;
  unsigned long nrn_pre, nrn_post, syn;
  double f, x, y, z, x0, y0, z0, x1, y1, z1, dx, dy, dz;
  GLfloat color[] = {1.0, 0.0, 0.0, 1.0};


  for (nrn_pre = 0; nrn_pre < total_neurons; nrn_pre++) {

		//color could be dependent on the popularion
		glColor4fv(color);

		for (syn = 0; syn < neuron[nrn_pre].n_synapses; syn++) {	
			if (neuron[nrn_pre].synapse[syn].spikes_cue > 0 && neuron[nrn_pre].synapse[syn].delay > 0.0) {
		
				nrn_post = neuron[nrn_pre].synapse[syn].target_neuron;
				pop_post = neuron[nrn_post].population;
				fdm_post = connection[ neuron[nrn_pre].population ][ pop_post ].fdomain;
				parent_fdm_post = population[ pop_post ].fdomain[fdm_post].parent_fdomain;

				if( parent_fdm_post == -1 ) { //fdm00 condition
					dx = population[ pop_post ].fdomain[ fdm_post ].position.x;
					dy = population[ pop_post ].fdomain[ fdm_post ].position.y;
					dz = population[ pop_post ].fdomain[ fdm_post ].position.z;
				}
				else {
					dx = 0.5*( population[ pop_post ].fdomain[ fdm_post ].position.x + population[ pop_post ].fdomain[ parent_fdm_post ].position.x );
					dy = 0.5*( population[ pop_post ].fdomain[ fdm_post ].position.y + population[ pop_post ].fdomain[ parent_fdm_post ].position.y );
					dz = 0.5*( population[ pop_post ].fdomain[ fdm_post ].position.z + population[ pop_post ].fdomain[ parent_fdm_post ].position.z );
				}				
				

				x0 = neuron[ nrn_pre ].position.x;
				y0 = neuron[ nrn_pre ].position.y;
				z0 = neuron[ nrn_pre ].position.z;

				x1 = neuron[ nrn_post ].position.x + dx;
				y1 = neuron[ nrn_post ].position.y + dy;
				z1 = neuron[ nrn_post ].position.z + dz;


				for (spk = 0; spk < neuron[nrn_pre].synapse[syn].spikes_cue; spk++) {					
					index = (N_SPIKE_TIMES + neuron[nrn_pre].last_spike_time_index - spk) % N_SPIKE_TIMES;					
					f = (t - neuron[nrn_pre].spike_time[index]) / neuron[nrn_pre].synapse[syn].delay;
					if( f < 0.0 ) { f = 0.0; }					
					x = x0 + f * (x1 - x0);
					y = y0 + f * (y1 - y0);
					z = z0 + f * (z1 - z0);
					
					glPushMatrix();
					
					glTranslatef(x,y,z);
					glCallList(spike_DisplayList);
					
					glPopMatrix();
					
				}	//for spk
			}	//if cue		
		} //for syn		
  } //for nrn_pre

  return;
}





//------------------------------------------------------------------------

/**
 * updates text information provided in the graphical window
 */

void Update_Orthographic_View_Info(void)
{
  char msg[80];
  unsigned int pop, line, fdm, offset;
  unsigned long nrn;

  int frames_time_t;
  static int frames_passed = 0;
  static int frames_oldtime = 0; 


  glDisable(GL_LIGHTING);

  //update frames-per-second
  frames_passed++;
  frames_time_t = glutGet(GLUT_ELAPSED_TIME);

  if (frames_time_t - frames_oldtime > 1000) {
    frames_fps = frames_passed * 1000.0 / (frames_time_t-frames_oldtime);
    frames_oldtime = frames_time_t;		
    frames_passed = 0;
  }

  if( flag_schematic_representation == 0 ) {  
    glColor3f(0.0, 1.0, 1.0);  
    for(pop=0; pop<total_populations; pop++) {
      nrn = population[pop].first_neuron;
      glRasterPos3f( neuron[nrn].position.x - graphics.neuron_size, neuron[nrn].position.y - graphics.neuron_size, neuron[nrn].position.z + graphics.neuron_size);
      glutBitmapString( GLUT_BITMAP_8_BY_13, (unsigned char *) population[pop].label );
    }
  }
  
  SwitchToOrthographicView(graphics.window_size_w, graphics.window_size_h);

  glLoadIdentity();

  //time
  glColor4f(1.0, 1.0, 1.0, 1.0);
	if( graphics.pause_graphics == 0 ) {
		sprintf(msg, "time = %.3f ms [%ld*dt] - FPS:%.1f", t, sim_step, frames_fps);
	}
	else {
		sprintf(msg, "time = %.3f ms [%ld*dt] PAUSED - FPS:%.1f", t, sim_step, frames_fps);
	}
  WriteOnScreen(msg, 10, graphics.window_size_h - 10);

	//focus
  glColor4f(1.0, 0.0, 0.0, 1.0);
	if( graphics.target_nrn < 0 ) {
		sprintf(msg, "focus: center"); }
	else {
		sprintf(msg, "focus: nrn %ld", graphics.target_nrn); }
  WriteOnScreen(msg, graphics.window_size_w - 18*8, graphics.window_size_h - 25);

  for(pop=0; pop<total_populations; pop++) {
    glColor4f(0.0, 1.0, 1.0, 1.0);
    if( flags.enable_rate_estimation == 0 ) {
      sprintf(msg, "%s: AL=%.1f%%; spks=%ld", population[pop].label, population[pop].activity_level, population[pop].total_spikes );
      WriteOnScreen(msg, 10, graphics.window_size_h - 25 - 15 * pop);
    }
    else {
      sprintf(msg, "%s: AL=%.1f%%; r=%.1f %.1f; spks=%ld", population[pop].label, population[pop].activity_level, population[pop].mean_firing_rate, population[pop].stdev_firing_rate, population[pop].total_spikes );
      WriteOnScreen(msg, 10, graphics.window_size_h - 25 - 15 * pop);
    }
  }

  line = 25 + 15 * total_populations;

  //help & info
  glColor4f(1.0, 0.0, 0.0, 1.0);
  WriteOnScreen("H:help, I:info", graphics.window_size_w - 18*8, graphics.window_size_h - 10);

  if(graphics.help == 1 && graphics.window_size_h > line + 270) { //270= 15 (px) * 18 (lines)
    glColor4f(1.0, 1.0, 0.0, 1.0);
		offset = 15;

    WriteOnScreen("Esc: force quit", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("q: clean quit", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("p: pause simulation", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("s: step dt and pause", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("f: estimate firing rates", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("x: toggle between Pop and V colour code", 10, graphics.window_size_h - line - offset);		offset+=15;

    WriteOnScreen("h: toggle key list", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("i: toggle target info", 10, graphics.window_size_h - line - offset);		offset+=15;

    WriteOnScreen("Mouse LeftButton: pick neuron target", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("Mouse MiddleButton: display menu", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("Mouse RightButton: rotate camera", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("Mouse RightButton + Shift: light source", 10, graphics.window_size_h - line - offset);		offset+=15;

    WriteOnScreen("cursor, PgUp/PgDown: navigate", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("navigation key + Ctrl: warp", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("Home key: reset view", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("Insert key: focus on target", 10, graphics.window_size_h - line - offset);		offset+=15;

    WriteOnScreen("c: toggle sample connections", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("C: toggle all connections", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("k: toggle spikes", 10, graphics.window_size_h - line - offset);		offset+=15;
    WriteOnScreen("a: toggle anti-aliasing", 10, graphics.window_size_h - line - offset);		offset+=15;
	}
	
  //help & info are mutually exclusive	
  if(graphics.target_info == 1 && graphics.target_nrn > -1 && graphics.target_nrn < total_neurons && graphics.window_size_h > line + 270) { //270= 15 (px) * 18 (lines)
    glColor4f(1.0, 1.0, 0.0, 1.0);
		offset = 15;

		nrn = graphics.target_nrn;

		sprintf(msg, "Information regarding neuron %ld", nrn);
    WriteOnScreen(msg, 10, graphics.window_size_h - line - offset);		offset+=15;

		sprintf(msg, "population: %s", population[ neuron[nrn].population ].label);
    WriteOnScreen(msg, 10, graphics.window_size_h - line - offset);		offset+=15;

		sprintf(msg, "established synapses (active): %ld (%ld)", neuron[nrn].n_synapses, neuron[nrn].n_active_synapses );
    WriteOnScreen(msg, 10, graphics.window_size_h - line - offset);		offset+=15;

		sprintf(msg, "estimated firing rate [Hz]: %.1f", neuron[nrn].firing_rate );
    WriteOnScreen(msg, 10, graphics.window_size_h - line - offset);		offset+=15;

		sprintf(msg, "membrane potential [mV]" );
    WriteOnScreen(msg, 10, graphics.window_size_h - line - offset);		offset+=15;
		for( fdm = 0; fdm < neuron[nrn].n_fdomains; fdm++ ) {
			sprintf(msg, "- %s: %.3f", population[ neuron[nrn].population ].fdomain[fdm].label, neuron[nrn].fdomain[fdm].V[0] );
			WriteOnScreen(msg, 10, graphics.window_size_h - line - offset);		offset+=15;		
		}


	}



  if( flag_schematic_representation == 1 ) {  
    Draw_Schematic();
  }
	
	//Re-setup a perspective projection  
  glMatrixMode(GL_PROJECTION); // set the current matrix to GL_PROJECTION
  glPopMatrix();               // restore previous settings
  glMatrixMode(GL_MODELVIEW);  // get back to GL_MODELVIEW matrix
	
  glEnable(GL_LIGHTING);

  return;
}





//------------------------------------------------------------------------

/**
 * writes a message in the display window with bitmap characters
 * \param msg message to be written
 * \param x message's x position in windows coordinates
 * \param y message's y position in windows coordinates
 */

void WriteOnScreen(char *msg, int x, int y)
{
  glRasterPos2i(x, y);

  glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *) msg);

  return;
}





//------------------------------------------------------------------------

/**
 * Creates a lookup table to convert voltages to RGB color indexes
 */

void Create_Voltage2Color_Palette(void)
{
  unsigned int i;
  float alpha;

  alpha = 0.85;

  //hyperpolarized color code
  for (i = 0; i < VOLTAGE_COLORS / 2; i++) {
    graphics.voltage2color_palette[i].r = i * 2.0 / VOLTAGE_COLORS;
    graphics.voltage2color_palette[i].g = i * 2.0 / VOLTAGE_COLORS;
    graphics.voltage2color_palette[i].b = 1.0;
    graphics.voltage2color_palette[i].a = alpha;
  }

  //subthreshold color code
  for (i = VOLTAGE_COLORS / 2; i < VOLTAGE_COLORS - 1; i++) {
    graphics.voltage2color_palette[i].r = 1.0;
    graphics.voltage2color_palette[i].g = 2.0 - i * 2.0 / VOLTAGE_COLORS;
    graphics.voltage2color_palette[i].b = 2.0 - i * 2.0 / VOLTAGE_COLORS;
    graphics.voltage2color_palette[i].a = alpha;
  }

  //threshold color code
  graphics.voltage2color_palette[VOLTAGE_COLORS - 1].r = 1.0;
  graphics.voltage2color_palette[VOLTAGE_COLORS - 1].g = 1.0;
  graphics.voltage2color_palette[VOLTAGE_COLORS - 1].b = 0.0;
  graphics.voltage2color_palette[VOLTAGE_COLORS - 1].a = alpha;


  return;
}





//------------------------------------------------------------------------

/**
 * Creates color from present voltage using color code
 */

struct RGBAColor Voltage2Color(long nrn, int fdm)
{
  unsigned int i;
	double V_rest = -70.0;    //parameters used to set the borders of the color gradients
	double V_thresh = -50.0;  //parameters used to set the borders of the color gradients
	double V = neuron[nrn].fdomain[fdm].V[0];

  //threshold color code; override membrane potential contition to account for electrodes and models where V is always below V_thresh
  if (V > V_thresh || ( fdm == 0 && t - neuron[nrn].spike_time[ neuron[nrn].last_spike_time_index ] < 2.0 ) ) {
    return graphics.voltage2color_palette[VOLTAGE_COLORS - 1];
  }

  //subthreshold color code
  if (V > V_rest) {

    i = VOLTAGE_COLORS / 2 + (int) (VOLTAGE_COLORS / 2 * (V - V_rest) / (V_thresh - V_rest));
    if (i > VOLTAGE_COLORS - 1) {
      i = VOLTAGE_COLORS - 1;
    }

    return graphics.voltage2color_palette[i];
  }
  //hyperpolarized color code
  else {

    i = (int) (VOLTAGE_COLORS / 2 * (V - 0.0) / (V_rest - 0.0));
    if (i < 0) {
      i = 0;
    }
    if (i > FREQUENCY_COLORS - 1) {
      i = FREQUENCY_COLORS - 1;
    }

    return graphics.voltage2color_palette[i];
  }

}





//------------------------------------------------------------------------

/**
 * Creates color from population id
 */

struct RGBAColor Population2Color(int pop)
{
	float xh, i, f;
	float r=1.0, g=1.0, b=1.0, a=0.85;
	struct RGBAColor color;

	//this is the same as a HSV2RGB conversion with S=V=1.0 and H depends on pop value
	xh = 6.0 * pop / total_populations;
	i = (float)floor((double)xh);    /* i = greatest integer <= h    */
	f = xh - i;                     /* f = fractional part of h     */
	
	switch ((int) i) {
	case 0:
		r = 1.0;
		g = f;
		b = 0.0;
		break;
	case 1:
		r = 1.0 - f;
		g = 1.0;
		b = 0.0;
		break;
	case 2:
		r = 0.0;
		g = 1.0;
		b = f;
		break;
	case 3:
		r = 0.0;
		g = 1.0 - f;
		b = 1.0;
		break;
	case 4:
		r = f;
		g = 0.0;
		b = 1.0;
		break;
	case 5:
		r = 1.0;
		g = 0.0;
		b = 1.0 - f;
		break;
	}
	
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	
	return color;
}





//------------------------------------------------------------------------

/**
 * Creates a lookup table to convert frequencies to RGB color indexes
 */

void Create_Frequency2Color_Palette(void)
{
  unsigned int i;
  float alpha;

  alpha = 1.0;

  //below FREQUENCY_THRESH
  for (i = 0; i < FREQUENCY_COLORS / 2; i++) {
    graphics.frequency2color_palette[i].r = ( 2.0 * i / FREQUENCY_COLORS );
    graphics.frequency2color_palette[i].g = ( 2.0 * i / FREQUENCY_COLORS );
    graphics.frequency2color_palette[i].b = 1.0;
    graphics.frequency2color_palette[i].a = alpha;
  }

  //above FREQUENCY_THRESH
  for (i = FREQUENCY_COLORS / 2; i < FREQUENCY_COLORS; i++) {
    graphics.frequency2color_palette[i].r = 1,0;
    graphics.frequency2color_palette[i].g = 1.0 - ( 2.0 * i / FREQUENCY_COLORS - 1.0);
    graphics.frequency2color_palette[i].b = 1.0 - ( 2.0 * i / FREQUENCY_COLORS - 1.0);
    graphics.frequency2color_palette[i].a = alpha;
  }


  return;
}





//------------------------------------------------------------------------

/**
 * Creates color code for present frequency
 */

struct RGBAColor Frequency2Color(double f)
{
  unsigned int i;

  //below FREQUENCY_THRESH
  if (f < FREQUENCY_THRESH) {

    i = (int) (FREQUENCY_COLORS / 2.0 * f / FREQUENCY_THRESH);

    return graphics.frequency2color_palette[i];
  }
  //above FREQUENCY_THRESH
  else {

    i = FREQUENCY_COLORS / 2 + (int) (0.5 * FREQUENCY_COLORS * ( f -  FREQUENCY_THRESH ) / ( FREQUENCY_MAX -  FREQUENCY_THRESH ) ) ;

    if (i > FREQUENCY_COLORS - 1) {
      i = FREQUENCY_COLORS - 1;
    }

    return graphics.frequency2color_palette[i];
  }
}





//------------------------------------------------------------------------

/**
 * Takes care of window resising
 */

void Window_Resize(int w, int h)
{
  unsigned int pop, total_columns = 0;
  double ratio;

  graphics.window_size_w = w;
  graphics.window_size_h = h;


  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (h == 0) {    h = 1;  }
  ratio = 1.0 * w / h;

  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set the correct perspective.
  gluPerspective(FOV_XY, ratio, 1.0, FAR_CLIPING_FRAME);	//gluPerspective(FOV in yzplane,ratio,near clipping plane, far cli. pl.)
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //set the population array sizes for the schematic representation
   graphics.n_lines = (int) (sqrt( total_neurons / ratio ));

  //prevent a divide by zero, when window is too short
  if ( graphics.n_lines < 1) {     graphics.n_lines = 1;  }

  for( pop=0; pop<total_populations; pop++ ) {

    if ( population[pop].n_neurons >  graphics.n_lines ) {

      graphics.population_dimy[pop] =  graphics.n_lines;

      if( population[pop].n_neurons %  graphics.n_lines == 0 ) {
				graphics.population_dimx[pop] = population[pop].n_neurons /  graphics.n_lines; }
      else {
				graphics.population_dimx[pop] = 1 + (int) (population[pop].n_neurons /  graphics.n_lines); }
    }
    else {
      graphics.population_dimy[pop] = population[pop].n_neurons;
      graphics.population_dimx[pop] = 1;
    }
		

    total_columns += graphics.population_dimx[pop];
  }
  
  graphics.dist_factor = 1.0 * total_populations / (total_populations + 1) / total_columns;


  return;
}





//------------------------------------------------------------------------

/**
 * Handles control of normal keyboard keys
 */

void Process_Normal_Keys(unsigned char key, int x, int y)
{
  static short antialiasing_state = 0;

  switch (key) {

  case 27: //Esc
    Free_Mem();
    exit(0);
    break;
  
  case 'q':
  case 'Q':		
    flags.finish_earlier = 1;
    graphics.close_graphics = 1;
    break;
 
  case 'p':
  case 'P':
    graphics.pause_graphics = (graphics.pause_graphics + 1) % 2;
    break;

  case 's':
  case 'S':
    if( graphics.pause_graphics == 1 ) {
      graphics.step1_graphics = 1;
      graphics.pause_graphics = 0;
    }
    else {
      graphics.pause_graphics = 1;
    }
    break;

  case 'c':
    graphics.draw_network_connections = (graphics.draw_network_connections + 1) % 2;
    break;

  case 'C':		
    graphics.draw_network_connections = 2 * ((graphics.draw_network_connections/2 + 1) % 2);
    break;

  case 'k':		
    graphics.draw_spikes = (graphics.draw_spikes + 1) % 2;
    break;

  case 'f':		
    flags.enable_rate_estimation = (flags.enable_rate_estimation + 1) % 2;
    break;

  case 'a':		
  case 'A':		
    // ANTIALISING STUFF
    antialiasing_state = (antialiasing_state + 1) % 2;
    if( antialiasing_state == 1 ) {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }
    else {
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
      glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);      
    }
    break;

  case 'h':
  case 'H':
    graphics.help = (graphics.help + 1) % 2;
	  graphics.target_info = 0;
    break;

  case 'i':
  case 'I':
    graphics.target_info = (graphics.target_info + 1) % 2;
	  graphics.help = 0;
    break;

  case 'x':
  case 'X':
    graphics.popcolorcode = (graphics.popcolorcode + 1) % 2;
    break;

  }

  return;
}





//------------------------------------------------------------------------

/**
 * Handles control of special keyboard keys
 */

void Process_Special_Keys(int key, int x, int y)
{
  Process_Camera_Movement(key);

  return;
}





//------------------------------------------------------------------------

/**
 * Process Camera movement
 */

void Process_Camera_Movement(int key)
{
  float step_linear;
  Vec3 viewDir, deltaVec, center2eyeVec;
	Vec3 xAxis = { 1.0, 0.0, 0.0 };
	Vec3 yAxis = { 0.0, 1.0, 0.0 };


  center2eyeVec = SubVec(eyeVec, centerVec);
  step_linear  = CAMERA_TRANSLATION_STEP;

  if(glutGetModifiers() == GLUT_ACTIVE_CTRL) {
    step_linear  = CAMERA_TRANSLATION_STEP * 50.0;
  }


  switch(key) {

  case GLUT_KEY_INSERT:
		if( graphics.target_nrn > -1 && graphics.target_nrn < total_neurons ) {
			centerVec.x = neuron[graphics.target_nrn].position.x;
			centerVec.y = neuron[graphics.target_nrn].position.y;
			centerVec.z = neuron[graphics.target_nrn].position.z;
		}
		break;

  case GLUT_KEY_PAGE_UP:
    viewDir = NormalizeVec3( center2eyeVec );
		deltaVec = MultVec3Float( viewDir, -step_linear );
		if( NormVec3(center2eyeVec) >  NormVec3(deltaVec) ) {
			eyeVec = SumVec( eyeVec, deltaVec );
		}
    break;
  
  case GLUT_KEY_PAGE_DOWN:
		viewDir = NormalizeVec3( center2eyeVec );
		deltaVec = MultVec3Float( viewDir, step_linear );
		eyeVec = SumVec( eyeVec, deltaVec );
		break;
		
  case GLUT_KEY_LEFT:
		deltaVec  = MultVec3Float( xAxis, step_linear );
		//centerVec = SumVec(centerVec, deltaVec);
    eyeVec    = SumVec(eyeVec, deltaVec);
    break;
  
  case GLUT_KEY_RIGHT:
		deltaVec  = MultVec3Float( xAxis, -step_linear );
		//centerVec = SumVec(centerVec, deltaVec);
    eyeVec    = SumVec(eyeVec, deltaVec);
    break;
  
  case GLUT_KEY_UP:
		deltaVec  = MultVec3Float( yAxis, step_linear );
		//centerVec = SumVec(centerVec, deltaVec);
    eyeVec    = SumVec(eyeVec, deltaVec);
		break;
  
  case GLUT_KEY_DOWN:
		deltaVec  = MultVec3Float( yAxis, -step_linear );
		//centerVec = SumVec(centerVec, deltaVec);
    eyeVec    = SumVec(eyeVec, deltaVec);
    break;

  case GLUT_KEY_HOME:
		eyeVec.x = graphics.origin_x;
		eyeVec.y = graphics.origin_y;
		eyeVec.z = graphics.origin_z + 2.0 * ( max_z - min_z ) + 200.0;
		
		centerVec.x = graphics.origin_x;
		centerVec.y = graphics.origin_y;
		centerVec.z = graphics.origin_z;

    upVec.x = 0.0;
    upVec.y = 1.0;
    upVec.z = 0.0;

    glutPostRedisplay();

    break;
  
  }  

  return;
}





//------------------------------------------------------------------------

/**
 * Handles mouse motion
 */

void Mouse_Motion(int x, int y)
{
  int diffx, diffy;
  float speed = 0.1;
  Vec3 verticalAxis={0.0,1.0,0.0};
  Vec3 newAxis;                   

  diffx = x - graphics.lastx;
  diffy = y - graphics.lasty;
  
  graphics.lastx = x;
  graphics.lasty = y;
  

  //MOVE LIGHT SOURCE
  if (graphics.Buttons[2] && glutGetModifiers() == GLUT_ACTIVE_SHIFT ) {

    light_yRot += 0.2 * diffy;
    light_xRot += 0.2 * diffx;
    
    light_yRot = fmod( light_yRot, 360.0);
    light_xRot = fmod( light_xRot, 360.0);
    
  }

  //MOVE CAMERA
  if (graphics.Buttons[2] && glutGetModifiers() != GLUT_ACTIVE_SHIFT ) {

    if(diffx!=0) { 
      graphics.rotx -= (float) speed * diffx;
      RotateQuaternion(&eyeVecQuatX,centerVec,&eyeVec,verticalAxis,graphics.rotx,&eyeVecQuatX);
      graphics.rotx=0;
    }
    
    if(diffy!=0) {	
      graphics.roty -= (float) speed * diffy;
      newAxis = CrossProduct( SubVec(eyeVec,centerVec), upVec);
      newAxis=NormalizeVec3(newAxis);
      RotateQuaternion(&eyeVecQuatX,centerVec,&eyeVec,newAxis,graphics.roty,&eyeVecQuat);
      graphics.roty=0;
    }
    
  }
  
  glutPostRedisplay();

  return;
}





//------------------------------------------------------------------------

/**
 * Handles passive mouse motion - without buttons beeing pressed
 */
void Mouse_Passive_Motion(int x, int y)
{
  graphics.lastx = x;
  graphics.lasty = y;

  return;
}





//------------------------------------------------------------------------

/**
 * Handles mouse buttons
 */

void Mouse_Buttons(int b, int s, int x, int y)
{
  graphics.lastx = x;
  graphics.lasty = y;

  switch (b) {
  case GLUT_LEFT_BUTTON:
		if( s == GLUT_UP ) {
			graphics.pick_target = 1;
		}
    graphics.Buttons[0] = ((GLUT_DOWN == s) ? 1 : 0);
    break;

  case GLUT_MIDDLE_BUTTON:
    graphics.Buttons[1] = ((GLUT_DOWN == s) ? 1 : 0);
    break;

  case GLUT_RIGHT_BUTTON:
    graphics.Buttons[2] = ((GLUT_DOWN == s) ? 1 : 0);
    if(s == GLUT_DOWN) {
      graphics.display_light = 1;
    }
    else {
      graphics.display_light = 0;
    }
    break;

  default:
    break;
  }

  glutPostRedisplay();

  return;
}





//------------------------------------------------------------------------

/**
 * Creates a glut menu and attach it to right mouse button
 */

void CreateMenus(void)
{
  int menu;

  menu = glutCreateMenu(callBackMenu);

  glutAddMenuEntry("DOT STYLE", DOT_STYLE);
  glutAddMenuEntry("SPHERE STYLE", SPHERE_STYLE);
  glutAddMenuEntry("SCHEMATIC STYLE", SCHEME_STYLE);
  glutAttachMenu(GLUT_MIDDLE_BUTTON);
}





//------------------------------------------------------------------------

/**
 * Callback function for mouse middle button menu
 * \param value menu option
 */

void callBackMenu(int value)
{
  switch (value) {
  case DOT_STYLE:
    neuron_repres_style = DOT_STYLE;
    neuron_repres_style_string = "Dot Style";
    break;
  case SPHERE_STYLE:
    neuron_repres_style = SPHERE_STYLE;
    neuron_repres_style_string = "3D Detailed Representation";
    break;
  case SCHEME_STYLE:
    neuron_repres_style = SCHEME_STYLE;
    neuron_repres_style_string = "Model Diagram";
    break;

  }
  glutPostRedisplay();

  return;
}





//------------------------------------------------------------------------

/**
 * Setup for light source 0
 */

void initLighting(void)
{
  glEnable(GL_DEPTH_TEST);      // Hidden surface removal
  glFrontFace(GL_CCW);          // Counter clock-wise polygons face out
  glEnable(GL_CULL_FACE);       // Do not try to display the back sides
  
  // Enable lighting
  glEnable(GL_LIGHTING);
  
  //This gives a constant ambient light independently of the light source
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
  

  glLightfv(GL_LIGHT0,GL_DIFFUSE,ambientLight);
  glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
  glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
  

  glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,90.0f);
  
  glEnable(GL_LIGHT0);
  
  glEnable(GL_COLOR_MATERIAL);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  

  glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
  glMateriali(GL_FRONT, GL_SHININESS, 64);
  
  
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

  return;
}





//------------------------------------------------------------------------

/**
 * Setup gl_fog characteristics
 * \param density fog's density
 * \param color fog's color
 * \param type fog's equation->GL_LINEAR, GL_EXP, GL_EXP2
 * \param start fog's begin point
 * \param end fog's end point
 */

void setupFog(GLfloat density, GLfloat * color, GLint type, GLfloat start, GLfloat end)
{
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, type);
  glFogf(GL_FOG_DENSITY, density);
  glFogfv(GL_FOG_COLOR, color);
  glFogf(GL_FOG_START, start);
  glFogf(GL_FOG_END, end);
  glHint(GL_FOG_HINT, GL_NICEST);

  return;
}





//------------------------------------------------------------------------

/**
 * Setup of a 2D ortographic projection
 */

void SwitchToOrthographicView(int w, int h)
{

  // switch to projection mode
  glMatrixMode(GL_PROJECTION);
  // save previous matrix which contains the settings for the perspective projection
  glPushMatrix();
  // reset matrix
  glLoadIdentity();
  // set a 2D orthographic projection
  gluOrtho2D(0, w, 0, h);
  // do not invert the y axis, down is negative
  glScalef(1, 1, 1);
  // do not move the origin from the bottom left corner
  // to the upper left corner
  glTranslatef(0, 0, 0);
  glMatrixMode(GL_MODELVIEW);

  return;
}




//------------------------------------------------------------------------

/**
 * Draw function used in the "pick neuron" mode. This is a simplification of Draw_Neurons()
 */

void drawPickingMode()
{
  unsigned int pop, bitstep;
  unsigned long nrn, id;
  unsigned int n_slices, n_stacks;
	float r;
	GLuint R, G, B;


	glDisable (GL_BLEND);
	glDisable (GL_DITHER);
	//glDisable (GL_FOG);
	glDisable (GL_LIGHTING);
	//glDisable (GL_TEXTURE_1D); 
	//glDisable (GL_TEXTURE_2D);
	//glDisable (GL_TEXTURE_3D);

	glShadeModel (GL_FLAT);

	n_slices = 16;
	n_stacks = 1;

	bitstep = 8 * ( sizeof(int) - 1 );

	//assuming 7-bits/color !!!

  for (nrn = 0; nrn < total_neurons; nrn++) {

		id = nrn + 1; //+1 is used in the inverse corversion to put nrn_id = -1 in the background
		// why didn't I use 256?: because it wasn't working and I didn't understand why (sign bit?)
		R = id % 128;
		G = (( id - R ) / 128 )  % 128;
		B = ( id - R - G * 128) / (128 * 128);


		R = R << bitstep; //openGL only uses higher significance bits; this moves the used byte to the highest byte in an integer (which usually is 4 bytes; but it's given by sizeof(int))
		G = G << bitstep;
		B = B << bitstep;

		glColor3ui(R, G, B); 		//this supports a total of 2,097,152 neurons... 

		glPushMatrix();

    glTranslatef(neuron[nrn].position.x, neuron[nrn].position.y, neuron[nrn].position.z);

		pop = neuron[nrn].population;
		r = population[pop].fdomain[0].position.r;

    //soma
		gluQuadricOrientation( quadric, GLU_INSIDE );
		gluDisk( quadric, 0.0f, r, n_slices, 1 );
		gluQuadricOrientation( quadric, GLU_OUTSIDE );
		gluCylinder( quadric, r, r, 2.0f*r, n_slices, n_stacks );
		glTranslatef( 0.0f, 0.0f, 2.0f*r );
		gluDisk(quadric, 0.0f, r, n_slices, 1);
		glTranslatef( 0.0f, 0.0f, -r );
    
    glPopMatrix();

  }

	glEnable (GL_BLEND);
	glEnable (GL_DITHER);
	//glEnable (GL_FOG);
	glEnable (GL_LIGHTING);
	//glDisable (GL_TEXTURE_1D); 
	//glDisable (GL_TEXTURE_2D);
	//glDisable (GL_TEXTURE_3D);

	glShadeModel (GL_SMOOTH);


	return;
}





//------------------------------------------------------------------------

/**
 * Function that reads cursor location and returns the NEURON id of the selected neuron
 */
   
long processPick(void)
{
	long nrn_id;
	GLint viewport[4];
	GLubyte pixel[3];


	glGetIntegerv(GL_VIEWPORT, viewport);

	//void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels); 
	glReadPixels( graphics.lastx, viewport[3] - graphics.lasty, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel );

	//convert pixel color (in RGB) to neuron id (long)
	nrn_id = pixel[0] + 128 * pixel[1] + 128 * 128 * pixel[2];
	nrn_id = nrn_id - 1; //removes the added 1 (black background gets nrn_id=-1);
	
	return nrn_id;
}





//------------------------------------------------------------------------

/**
 * Frees memory used is graphics related date
 */

void Free_GraphicsMem(void)
{
  unsigned int i, pop, fdm;

	for(i=0; i<MAX_DL_STEPS; i++) {		
		for( pop = 0; pop < total_populations; pop++) {
			for( fdm = 0; fdm < population[pop].n_fdomains; fdm++) {
				glDeleteLists( displaylist[i][pop][fdm] , 1 );
			}    			
			free( displaylist[i][pop] );
		}
		free( displaylist[i] );
	}
	
	glDeleteLists( spike_DisplayList, 1);    

  return;
}
