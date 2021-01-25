#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// player position
float px, py;

void drawplayer()
  {
    // player color
    glColor3f( 1, 0, 0 );
    // player size
    glPointSize( 8 );

    // draw the player in its current position
    glBegin( GL_POINTS );
    glVertex2i( px, py );
    glEnd();
  }

void display()
  {
    // Clears buffer to the preset values in init
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Here goes everything to be drawn
    drawplayer();

    // Since it's double buffered there's a front and back buffer, the back
    // buffer is the one being drawn and the front buffer is the one shown on
    // the screen. This swap promotes the back buffer to the front buffer.
    glutSwapBuffers();
  }

void buttons( unsigned char key, int x, int y )
  {
    if( key == 'a' ) { px -= 5; }
    if( key == 'd' ) { px += 5; }
    if( key == 'w' ) { py -= 5; }
    if( key == 's' ) { py += 5; }
    glutPostRedisplay();
  }

void init()
  {
    // Preset clear color value
    glClearColor( 0.3, 0.3, 0.3, 0 );
    // The window size
    gluOrtho2D( 0, 1024, 512, 0 );
    // Initial player position
    px = 300; py = 300;
  }

// Compile with $ gcc -lglut -lGLU -lGL code.c
int main( int argc, char* argv[] )
  {
    // Initialize the OpenGL Utility Toolkit
    glutInit( &argc, argv );
    // Set the initial window size
    glutInitWindowSize( 1024, 512 );
    // Sets the initial display mode (double buffered and RGBA mode window)
    // Double buffered to reduce graphics flickering
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    // Create window and set title
    glutCreateWindow( "testing" );
    init();
    glutDisplayFunc( display );
    glutKeyboardFunc( buttons );
    // Enters the GLUT event processing loop
    glutMainLoop();
  }
