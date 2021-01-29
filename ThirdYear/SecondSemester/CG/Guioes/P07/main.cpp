#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <GL/glew.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std;


float camX = 0, camY = 0, camZ = 0;
float lookX = 0, lookZ = 0;
int startX, startY, tracking = 0;

int alpha = 45, beta = 0, r = 50;
float drawAlfa = 0.0f, drawBeta = 0.5f;
float g_radius = 100.0f;

float widthAux, heightAux;

int gi = 0;
float gstep = 0.03;

int tw, th; //terrain width and height
unsigned char* imageData;

GLuint vertices, verticeCount;

struct tree{
    float x, y;
};

struct tree tree;

vector<struct tree> trees;

float terrain_height(int i, int j){
	float ret = imageData[j + i*tw];

    return ret * (60.0/255.0);
}

float hf(float x, float z){
	int x1,x2,z1,z2;
	float height_xz, h_x1_z, h_x2_z,fx,fz;

	x1 = floor(x) ; x2 = x1 + 1;
	z1 = floor(z) ; z2 = z1 + 1;

	fz = z - z1;
	fx = x - x2;

	h_x1_z = terrain_height(x1,z1) * (1 - fz) + terrain_height(x1,z2) * fz;
	h_x2_z = terrain_height(x2,z1) * (1 - fz) + terrain_height(x2,z2) * fz;

	height_xz = h_x1_z * (1 - fx) + h_x2_z * fx;

	return height_xz;
}

/* --- Drawings ------------------------------------------------------------- */

/**
 * Draws a torus in plane's center
 */
void drawTorus(){
    glPushMatrix();
    glColor3f(0.9f, 0.0f, 0.9f);
    glutSolidTorus(1,2,100,100);
    glPopMatrix();
}

/**
 * Draws both inner and outter teapots based on given radius and rotation
 *
 * The difference between the inner and outter teapots, besides their radius, is that
 * the inner teapot has (-90 + degree) rotation angle and the outter teapot has (degree)
 * rotation angle, both around (0,1,0) vector
 * The value sign defines the direction of rotation (-1 || 1)
 *
 */
void drawTeapots(float nr_teapots, float size, float radius, int sign, float rotation, float r, float g, float b){
    float dx = 2 * M_PI / (float)nr_teapots;
    float alpha, degree, grot;
    glPushMatrix();
    glColor3f(r,g,b);
    for(int i = 0; i < nr_teapots; i++){
        glPushMatrix();
        alpha  = dx * i;
        degree = alpha * 180 / M_PI;
        grot = gi * gstep;
        glTranslatef(radius * sin(alpha + sign * grot),size,radius * cos(alpha + sign * grot));
        glRotated(rotation + degree + sign * grot * 180 / M_PI,0,1,0);
        glutSolidTeapot(size);
        glPopMatrix();
    }
    glPopMatrix();
}

/**
 * Draw trees based on the coordinates generated by the function (genPos).
 *
 * Reads the coordinates contained in trees vector and translates the two
 * solid cones that forms a tree to that position.
 *
 */
void drawTree(){
    float radius = 0.5;
    float height = 8;
		float x,y;
    srand(time(NULL));
    glPushMatrix();
		glRotatef(-90,1,0,0);
    for(int i = 0; i < trees.size(); i++){
				x = trees.at(i).x;
				y = trees.at(i).y;
        glPushMatrix();
        glColor3f(0.741f, 0.718f, 0.420f);//Brown
        glTranslatef(x,y,hf(x+widthAux,y+heightAux));
        glutSolidCone(radius, height, 30, 10);
        glColor3f(0.0f, 0.392f, 0.0f);//Forest Green
        glTranslatef(0,0,height/2.5);
        glutSolidCone(radius*6,height,30,10);
        glPopMatrix();
    }
    glPopMatrix();
}

/**
 * Generates trees positions based on pseudo-random numbers
 *
 * The trees need to be out of the central circle. Its used
 * the circle equation to verify if the generated positions are in that
 * area or not. If the generated positions are in the area of the circle
 * they are rejected, otherwise, they are stored in the trees vector.
 *
 */
void genPos(int ntrees, float width, float height, float radius){
    srand(time(NULL));
    float rx, ry;

    for(int i = 0; i < ntrees; i++){
        do{
            rx = -width + (rand() / (RAND_MAX/(width * 2)));
            ry = -height + (rand() / (RAND_MAX/(height * 2)));
        }while(pow(rx, 2) + pow(ry,2) < pow(radius,2));
        tree.x = rx;
        tree.y = ry;
        trees.push_back(tree);
    }
}

void refreshClockMovement(){
    gi++;
}

/* -------------------------------------------------------------------------- */

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
  glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawTerrain() {
  // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glColor3f(255,255, 255);
    for(int i = 0; i < th - 1; i++){
        //printf("%d %d\n", tw*2*i, tw*2);
        glDrawArrays(GL_TRIANGLE_STRIP, tw*2*i,tw*2);
    }
}

void renderScene(void) {
	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLoadIdentity();
	gluLookAt(camX, 2 + hf(camX,camZ), camZ,
		      	camX + sin(alpha),2 + hf(camX,camZ),camZ + cos(alpha),
			  		0.0f,1.0f,0.0f);

	drawTerrain();
	drawTorus();
	drawTeapots(8,2,15.0,-1,-90,0.0,0.0,1.0);
	drawTeapots(16,2,35.0,1,0,1.0,0.0,0.0);
	drawTree();
	refreshClockMovement();
	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char key, int xx, int yy) {
	// put code to process regular keys in here
	switch (key) {
	case 'x' : camX += 1.0;
					   break;
	case 'X' : camX -= 1.0;
					   break;
	case 'z' : camZ += 1.0;
				     break;
	case 'Z' : camZ -= 1.0;
					   break;
	case 'a' : alpha += 1.0;
					   break;
	case 'd' : alpha -= 1.0;
						 break;
	case 'b' :
						 break;
	case 'f' :
					   break;
	}
}

void init() {
	// 	Load the height map "terreno.jpg"
	//----------------------------------------------------------------------------
	ilInit();
	unsigned int ImgId;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring)"terreno.jpg");
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	float width, height;
	width = tw - 1;
	height = th - 1;

	vector<float> p;
	float z_coord, x_coord; // the coordinates to put in the vertex vector

    /* Draw strips here, the outer cycle moves through the Z axis */
    for (int i = 0; i <= height; i++) {
        z_coord = -(height/2) + i;
        /* Iterate through the X axis here */
        for (int j = 0; j <= width; j++) {
            x_coord = -(width/2) + j;
            // Vertex 0
            p.push_back(x_coord);
            p.push_back(terrain_height(i, j));
            p.push_back(z_coord);
            // Vertex 1
            p.push_back(x_coord);
            p.push_back(terrain_height(i+1, j));
            p.push_back(z_coord + 1);
        }
    }
	verticeCount = p.size();

	// criar o VBO
	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);
	GLuint buffers[1];

	glGenBuffers(1, &vertices);

	// copiar o vector para a memória gráfica
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData( GL_ARRAY_BUFFER, sizeof(float) * p.size(), p.data(), GL_STATIC_DRAW);

	// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {
	GLenum err = glewInit();
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);

	init();
	widthAux = (tw - 1)/2;
	heightAux = (th - 1)/2;
	genPos(750,widthAux,heightAux,50);

	// enter GLUT's main cycle
	glutMainLoop();

	return 0;
}