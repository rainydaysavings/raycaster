#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533

// player position
float px, py, pdx, pdy, pa;

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

    // draw the view the player is facing
    glLineWidth( 3 );
    glBegin( GL_LINES );
      glVertex2i( px, py );
      glVertex2i( px + pdx * 5, py + pdy * 5 );
    glEnd();
  }

int mapX = 8, mapY = 8, mapS = 64;
int map[] =
  {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1
  };

void drawMap2D()
  {
    int x, y, xo, yo;
    for( y = 0; y < mapY; ++y )
      {
        for( x = 0; x < mapX; ++x )
          {
            if( map[y * mapX + x] == 1 )
              {
                glColor3f( 1, 1, 1 );
              }
            else
              {
                glColor3f( 0, 0, 0 );
              }

            xo = x * mapS;
            yo = y * mapS;

            glBegin( GL_QUADS );
              glVertex2i( xo + 1       , yo + 1        );
              glVertex2i( xo + 1       , yo + mapS - 1 );
              glVertex2i( xo + mapS - 1, yo + mapS - 1 );
              glVertex2i( xo + mapS - 1, yo + 1        );
            glEnd();
          }
      }
  }

float dist(float ax, float ay, float bx, float by, float ang)
  {
    return ( sqrt(( bx - ax ) * ( bx - ax ) + ( by - ay ) * ( by - ay )) );
  }

void drawRays2D()
  {
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, disT;
    ra = pa - DR * 70;
    if( ra < 0 ) { ra += 2*PI; }
    if( ra > 2*PI ) { ra -= 2*PI; }

    for (r = 0; r < 140; ++r)
      {
        // CHECK HORIZONTAL LINES
        dof = 0;
        float disH = 1000000000, hx = px, hy = py;
        float aTan = -1 / tan(ra);
        // looking up
        if( ra > PI )
          {
            ry = (((int)py >> 6) << 6) - 0.0001;
            rx = (py - ry) * aTan + px;
            yo = -64;
            xo = -yo * aTan;
          }
        // looking down
        if( ra < PI )
          {
            ry = (((int)py >> 6) << 6) + 64;
            rx = (py - ry) * aTan + px;
            yo = 64;
            xo = -yo * aTan;
          }
        // looking straight left or right
        if( ra == 0 || ra == PI )
          {
            rx = px;
            ry = py;
            dof = 8;
          }

        while( dof < 8 )
          {
            mx = (int)(rx)>>6;
            my = (int)(ry)>>6;
            mp = my * mapX + mx;
            if(mp > 0 && mp < mapX * mapY && map[mp] == 1)
              {
                // Hit wall
                hx = rx;
                hy = ry;
                disH = dist(px, py, hx, hy, ra);
                dof = 8;
              }
            else
              {
                // Next line
                rx += xo;
                ry += yo;
                dof += 1;
              }
          }

        // CHECK VERTICAL LINES
        dof = 0;
        float nTan = -tan(ra);
        float disV = 1000000000, vx = px, vy = py;
        // looking left
        if( ra > P2 && ra < P3 )
          {
            rx = (((int)px >> 6) << 6) - 0.0001;
            ry = (px - rx) * nTan + py;
            xo = -64;
            yo = -xo * nTan;
          }
        // looking right
        if( ra < P2 || ra > P3 )
          {
            rx = (((int)px >> 6) << 6) + 64;
            ry = (px - rx) * nTan + py;
            xo = 64;
            yo = -xo * nTan;
          }
        // looking straight left or right
        if( ra == 0 || ra == PI )
          {
            rx = px;
            ry = py;
            dof = 8;
          }

        while( dof < 8 )
          {
            mx = (int)(rx)>>6;
            my = (int)(ry)>>6;
            mp = my * mapX + mx;
            if(mp > 0 && mp < mapX * mapY && map[mp] == 1)
              {
                // Hit wall
                vx = rx;
                vy = ry;
                disV = dist(px, py, vx, vy, ra);
                dof = 8;
              }
            else
              {
                // Next line
                rx += xo;
                ry += yo;
                dof += 1;
              }
          }

        if( disV < disH )
          {
            rx = vx;
            ry = vy;
            disT = disV;
            glColor3f(0, 0, 0.9);
          }
        else if( disH < disV )
          {
            rx = hx;
            ry = hy;
            disT = disH;
            glColor3f(0, 0, 0.6);
          }

        // Draw 2D rays
        /*
        glLineWidth( 4 );
        glBegin( GL_LINES );
          glVertex2i( px, py );
          glVertex2i( rx, ry );
        glEnd();
        */

        // Draw 3D walls
        float ca = pa - ra;
        if( ca < 0 ) { ca += 2*PI; }
        if( ca > 2*PI ) { ca -= 2*PI; }
        disT = disT * cos(ca);

        float lineH = (mapS * 512) / disT;
        if(lineH > 512) { lineH = 512; }
        float lineO = 256 - lineH / 2;

        glLineWidth( 8 );
        glBegin( GL_LINES );
          glVertex2i( r*8, lineO );
          glVertex2i( r*8, lineH + lineO );
        glEnd();

        ra += DR;
        if( ra < 0 ) { ra += 2*PI; }
        if( ra > 2*PI ) { ra -= 2*PI; }
      }
  }

void display()
  {
    // Clears buffer to the preset values in init
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Here goes everything to be drawn
    // drawMap2D();
    drawRays2D();
    // drawplayer();

    // Since it's double buffered there's a front and back buffer, the back
    // buffer is the one being drawn and the front buffer is the one shown on
    // the screen. This swap promotes the back buffer to the front buffer.
    glutSwapBuffers();
  }

void buttons( unsigned char key, int x, int y )
  {
    if( key == 'a' ) { pa -= 0.1; if(pa < 0) { pa += 2*PI; } pdx = cos(pa)*5; pdy = sin(pa)*5; }
    if( key == 'd' ) { pa += 0.1; if(pa > 2*PI) { pa -= 2*PI; } pdx = cos(pa)*5; pdy = sin(pa)*5; }
    if( key == 'w' ) { px += pdx; py += pdy; }
    if( key == 's' ) { px -= pdx; py -= pdy; }
    glutPostRedisplay();
  }

void init()
  {
    // Preset clear color value
    glClearColor( 0.5, 0.3, 0.3, 0 );
    // The window size
    gluOrtho2D( 0, 1024, 512, 0 );
    // Initial player position
    px = 300; py = 300; pdx = cos(pa)*5; pdy = sin(pa)*5;
  }

// Compile with $ gcc -lglut -lGLU -lGL -lm code.c
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
