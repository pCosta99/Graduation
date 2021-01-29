#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

float alfa = 0.0f, beta = 0.5f, g_radius = 100.0f;
float camX, camY, camZ;

int gi = 0;
float gstep = 0.03;

int time_counter, timebase, frame = 0;
float fps;

struct tree{
    float x, y;
};

struct tree tree;

vector<struct tree> trees;

// --- implementation -----------------------------------------------

void spherical2Cartesian() {
	camX = g_radius * cos(beta) * sin(alfa);
	camY = g_radius * sin(beta);
	camZ = g_radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;
	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

// --- Drawings -----------------------------------------------------

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
    srand(time(NULL));
    glPushMatrix();
    glRotatef(270,1,0,0);
    for(int i = 0; i < trees.size(); i++){
        glPushMatrix();
        glColor3f(0.741f, 0.718f, 0.420f);//Brown
        glTranslatef(trees.at(i).x, trees.at(i).y, 0);
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
void genPos(int ntrees, float lo, float hi, float radius){
    srand(time(NULL));
    float rx, ry;

    for(int i = 0; i < ntrees; i++){
        do{
            rx = lo + (rand()) /((RAND_MAX/(hi-lo)));
            ry = lo + (rand()) /((RAND_MAX/(hi-lo)));;
        }while(pow(rx, 2) + pow(ry,2) < pow(radius,2));
        tree.x = rx;
        tree.y = ry;
        trees.push_back(tree);
    }
}

/**
 * Updates global i
 */
void refreshClockMovement(){
    gi++;
}

/**
 * Displays FPS at the window title 
 */ 
void displayFPS(){
    char* title = (char*) malloc(sizeof(char) * 100);
    time_counter = glutGet(GLUT_ELAPSED_TIME);

    frame++;
    if(time_counter - timebase > 1000){
        fps = frame*1000.0/(time_counter-timebase);
        timebase = time_counter;
        frame = 0;
        sprintf(title, "%.2f", fps);

        glutSetWindowTitle(title);
    }
}

// ------------------------------------------------------------------

void renderScene(void) {
    displayFPS();

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		      0.0 , 0.0 , 0.0 ,
		      0.0f, 1.0f, 0.0f);
    
    // draw primitives
    drawTorus();
    drawTeapots(8,2,15.0,-1,-90,0.0,0.0,1.0);
    drawTeapots(16,2,35.0,1,0,1.0,0.0,0.0);
    drawTree();
    refreshClockMovement();

    // draw plane
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();

	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
// put code to process regular keys in here
    switch(c){
        case 'z' : g_radius -= 1.0f;
                   if (g_radius < 1.0f)
                       g_radius = 1.0f;
                   break;
        case 'x' : g_radius += 1.0f; 
                   break;
    }
	spherical2Cartesian();
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {
	switch (key) {
        case GLUT_KEY_RIGHT : alfa -= 0.1; 
                              break;
        case GLUT_KEY_LEFT  : alfa += 0.1; 
                              break;
        case GLUT_KEY_UP    : beta += 0.1f;
                              if (beta > 1.5f)
                                  beta = 1.5f;
                              break;
        case GLUT_KEY_DOWN  : beta -= 0.1f;
                              if (beta < -1.5f)
                                  beta = -1.5f;
                              break;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}


void printInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("'z' and 'x' control the distance from the camera to the origin\n");
}

int main(int argc, char **argv) {
    // init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@Guiao5");
		
    // Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

    // Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

    //  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();
    
    genPos(750,-100,100,50);
    
    // enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
