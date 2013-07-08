//************************************************************************
//*                                                                      *
//*                            G R A P H I C S                           *
//*                                                                      *
//************************************************************************

#if defined(GRAPHICS_H)
#else
#define GRAPHICS_H


#include <GL/freeglut.h>


//definitions and structures

#define GRAPHICS_BORDER 40

#define FOV_XY 45.0                         ///< field of view in the xy plane (degrees)

#define CAMERA_TRANSLATION_STEP 1.0         ///< in world units [um]
#define CAMERA_ROTATION_STEP 1.0            ///< in radians

#define FAR_CLIPING_FRAME 100000.0          ///< in world units [um]

#define DOT_STYLE 0
#define SPHERE_STYLE 1
#define SCHEME_STYLE 2

#define MAX_DL_STEPS 16		                  ///< different resolutions in the display lists
#define MINIMUM_DETAIL_DISTANCE 1.0e4       ///< distance after which neurons are rendered at minimum detail

#define SPIKE_BALL_RADIUS 1.0               ///< size of sphere representing a spike [um]
#define NEURON_BALL_RADIUS 1.0              ///< size of sphere representing a neuron in the schematic view [um]

#define VOLTAGE_COLORS 256	                ///< always use an even number
#define FREQUENCY_COLORS 256                ///< always use an even number
#define FREQUENCY_THRESH 20.0	              ///< defines the threshold used in the color code: there is a color transition at this value [Hz]
#define FREQUENCY_MAX 50.0	                ///< defines the frequency at which the color representation saturates [Hz]


/// holds a RGBA color
struct RGBAColor
{
  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;
};


/// holds all graphics related data 
struct GRAPHICS
{
  int window_size_w;     //width  of the visualization window, in pixels
  int window_size_h;     //height of the visualization window, in pixels

  GLdouble scale_factor; //scale factor, from um to pixels
  GLdouble dot_size;     //dot size for the neuron's dot representation 
  GLdouble spike_size;   //size of the spike representation
  GLdouble neuron_size;  //size of the spike representation
	
  short close_graphics;	 //flag to signal when to close the graphical window
  short pause_graphics;	 //flag to signal when to pause the graphical window
  short step1_graphics;	 //flag to signal when to advance a single step from a pause condition

  short help;            //flag to toggle onscreen help
	short target_info;     //flag to toggle onscreen info regarding targeted neuron
	short popcolorcode;    //flag to toggle between population and potential color code
	long target_nrn;       //targeted neuron id
	short pick_target;     //flag to signal if a pick occurred (mouse left button click)
	short display_light;	 //flag to signal if the light source is drawn or not in the scene

  short draw_spikes;
  short draw_network_connections;

  int *population_dimx;  //vector with the number of horizontal units for each population, in the schematic representation
  int *population_dimy;  //vector with the number of  vertical  units for each population, in the schematic representation
  int n_lines;           //number of lines used in schematic representation to draw the population arrays
  float dist_factor;     //total_populations / ((total_populations + 1) * Sum(n_columns))

  float rotx;            //camera yaw
  float roty;            //camera pitch
  
  float origin_x;
  float origin_y;
  float origin_z;

  int lastx;
  int lasty;

  unsigned char Buttons[3];

  struct RGBAColor voltage2color_palette[VOLTAGE_COLORS];
  struct RGBAColor frequency2color_palette[FREQUENCY_COLORS];
};





//function prototypes
void Prepare_Graphics(int argc, char *argv[]);
void initLighting(void);
void setupFog(GLfloat density, GLfloat * color, GLint type, GLfloat start, GLfloat end);

void Update_Graphics(void);

void Window_Resize(int w, int h);

void Create_DisplayLists(GLdouble scalefactor, GLint slices, GLint stacks);

void Draw_Dots(void);
void Draw_Schematic(void);

void Draw_Neurons(void);
void Draw_OutputPlexus(long nrn);
void Draw_InputPlexus(long nrn);
void Draw_Synapses(void);
void Draw_Spikes(void);

void Draw_Sample_Connections(void);
void Draw_All_Connections(void);

void Create_Voltage2Color_Palette(void);
struct RGBAColor Voltage2Color(long nrn, int fdm);
void Create_Frequency2Color_Palette(void);
struct RGBAColor Frequency2Color(double f);
struct RGBAColor Population2Color(int pop);

void Process_Normal_Keys(unsigned char key, int x, int y);
void Process_Special_Keys(int key, int x, int y);

void Process_Camera_Movement(int key);

void Mouse_Buttons(int b, int s, int x, int y);
void Mouse_Motion(int x, int y);
void Mouse_Passive_Motion(int x, int y);

void Update_Orthographic_View_Info(void);
void WriteOnScreen(char *msg, int x, int y);

void SwitchToOrthographicView(int w, int h);

void CreateMenus(void);
void callBackMenu(int value);

void drawPickingMode(void);
long processPick(void);

void Free_GraphicsMem(void);



#endif
