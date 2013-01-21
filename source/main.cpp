#include <nds.h>
#include <stdio.h>
#include <math.h>

// OpenGL stuff
#include "matrix4x4f.h"
#include "vector3f.h"
#include "gldefines.h"

// The models
#include "gisele.h"
#include "spider.h"
#include "dragon.h"
#include "wu.h"
#include "box.h"
#include "teapot.h"

enum {
    SPIDER,
    DRAGON,
    GISELE,
    WU,
    BOX,
    TEAPOT,
    NUM_MODELS
};

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------

int angle = 0;
int nXDiff = 0;
int nYDiff = 0;

float Znear = 0.01,
      Zfar  = 3000.0f;

int selectedModel = 0;
int drawMode = 0;

int W = 256, H = 192;

struct Camera {
    float eye[3];
    float center[3];
    float up[3];
} C;

#define DRAW_MODEL(m) drawModel(__ ## m ## _vertex,__ ## m ## _normal,__ ## m ## _texture,__ ## m ## _face,__ ## m ## _nfaces)
#define LOOK_AT_MODEL(m) lookAtModel(__ ## m ## _max, __ ## m ## _min)

static const char* getModelName()
{
    static const char* models[] = {"Spider", "Dragon", "Gisele", "Bull", "Box","Teapot"};
    return models[selectedModel];
}

static void drawModel(v16 v[][3], int n[], v16 t[][2], int faces[][3][3], unsigned int nfaces )
{

    if (drawMode == 0)
    {
        glBegin(GL_TRIANGLES);
        for (unsigned int i=0; i < nfaces; i++) {

            glNormal(n[faces[i][0][2]]);
            glVertex3v16v(v[faces[i][0][0]]);

            glNormal(n[faces[i][1][2]]);
            glVertex3v16v(v[faces[i][1][0]]);

            glNormal(n[faces[i][2][2]]);
            glVertex3v16v(v[faces[i][2][0]]);
        }
        glEnd();
    }

    if (drawMode == 1)
    {
        glBegin(GL_TRIANGLES);
        for (unsigned int i=0; i < nfaces; i++) {

            //1
            glNormal(n[faces[i][0][2]]);
            glVertex3v16v(v[faces[i][0][0]]);
            //2
            glNormal(n[faces[i][1][2]]);
            glVertex3v16v(v[faces[i][1][0]]);
            //2
            glNormal(n[faces[i][1][2]]);
            glVertex3v16v(v[faces[i][1][0]]);
            
            //1
            glNormal(n[faces[i][0][2]]);
            glVertex3v16v(v[faces[i][0][0]]);
            //3
            glNormal(n[faces[i][2][2]]);
            glVertex3v16v(v[faces[i][2][0]]);
            //3
            glNormal(n[faces[i][2][2]]);
            glVertex3v16v(v[faces[i][2][0]]);
            
            //3
            glNormal(n[faces[i][2][2]]);
            glVertex3v16v(v[faces[i][2][0]]);
            //2
            glNormal(n[faces[i][1][2]]);
            glVertex3v16v(v[faces[i][1][0]]);
            //2
            glNormal(n[faces[i][1][2]]);
            glVertex3v16v(v[faces[i][1][0]]);
            
        }
        glEnd();
    }

}

static void lookAtModel(float *max, float *min)
{
    float mc[] = { (max[0] + min[0])/2 , (max[1] + min[1])/2 , (max[2] + min[2])/2 };

    float alfa = 70/2 * M_PI/180;
    float fovy = ((max[1] - min[1])/2) / tan( alfa ) ;

    float beta = atan( (float)W/H * tan(alfa) );

    float fovx = ((max[0] - min[0])/2) / tan( beta )  ;

    float newZ = fovy > fovx ? fovy : fovx;

    C.eye[0] = mc[0], C.eye[1] = mc[1], C.eye[2] = max[2]+newZ,
    C.center[0] = mc[0], C.center[1] = mc[1], C.center[2] = mc[2],
    C.up[0]= 0, C.up[1] = 1, C.up[2] = 0;
}

static void lookAtSelectedModel()
{
    switch(selectedModel)
    {
        case DRAGON:
            LOOK_AT_MODEL(dragon);
        break;
        case GISELE:
            LOOK_AT_MODEL(gisele);
        break;
        case BOX:
            LOOK_AT_MODEL(box);
        break;
        case TEAPOT:
            LOOK_AT_MODEL(teapot);
        break;
        case SPIDER:
            LOOK_AT_MODEL(spider);
        break;
        case WU:
            LOOK_AT_MODEL(wu);
        break;
    }
}

static void renderScene(void)
{

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(C.eye[0],C.eye[1],C.eye[2],
              C.center[0],C.center[1],C.center[2],
              C.up[0],C.up[1],C.up[2]);

    glRotatef32i(angle,inttov16(0),inttov16(1),inttov16(0));

    switch(selectedModel)
    {
        case DRAGON:
            DRAW_MODEL(dragon);
        break;
        case GISELE:
            DRAW_MODEL(gisele);
        break;
        case BOX:
            DRAW_MODEL(box);
        break;
        case TEAPOT:
            DRAW_MODEL(teapot);
        break;
        case SPIDER:
            DRAW_MODEL(spider);
        break;
        case WU:
            DRAW_MODEL(wu);
        break;
    }

}

static void setup()
{
    consoleDemoInit();

    //put 3D on top
    lcdMainOnTop();

    //set mode 0, enable BG0 and set it to 3D
    videoSetMode(MODE_0_3D);

    // initialize gl
    glInit();

    // enable antialiasing
    glEnable(GL_ANTIALIAS);

    // setup the rear plane
    glClearColor(0,0,0,31); // BG must be opaque for AA to work
    glClearPolyID(63); // BG must have a unique polygon ID for AA to work
    glClearDepth(0x7FFF);

    // Set our viewport to be the same size as the screen
    glViewport(0,0,255,191);

    glColor3b(255,255,0);

    glLight(0, RGB15(0,31,0) , 0,               floattov10(-1.0),      0);

    //need to set up some material properties since DS does not have them set by default
    glMaterialf(GL_AMBIENT, RGB15(5,5,5));
    glMaterialf(GL_DIFFUSE, RGB15(15,15,15));
    glMaterialf(GL_SPECULAR, RGB15(8,8,8));
    glMaterialf(GL_EMISSION, RGB15(5,5,5));

    //ds uses a table for shinyness..this generates a half-assed one
    glMaterialShinyness();

    //ds specific, several attributes can be set here   
    glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)W / H, Znear, Zfar);

    lookAtSelectedModel();
}

static void rotateCameraX(float rot)
{
    C.eye[0] += rot/1000;
    C.eye[1] += 0*rot/1000;
    C.eye[2] += 0*rot/1000;
}

static void rotateCameraY(float rot)
{
    C.eye[0] += 0*rot/1000;
    C.eye[1] += rot/1000;
    C.eye[2] += 0*rot/1000;
}

static void rotateCameraZ(float rot)
{
    C.eye[0] += 0*rot/1000;
    C.eye[1] += 0*rot/1000;
    C.eye[2] += rot/1000;
}

static void updateConsole()
{
    consoleClear();

    iprintf("\n  \x1b[36mLots of models\n  Draw mode: %s\n  Model: %s\n\n",
        drawMode ? "LINES" : "TRIANGLES", getModelName());
}

static void receiveInput()
{
    bool modelChanged = false;

    scanKeys();

    u16 pressed = keysDown();
    u16 held = keysHeld();

    if (held & KEY_LEFT)  rotateCameraY(50);
    if (held & KEY_RIGHT) rotateCameraY(-50);
    if (held & KEY_UP)    rotateCameraX(50);
    if (held & KEY_DOWN)  rotateCameraX(-50);
    if (held & KEY_X)     rotateCameraZ(30);

    if (pressed & KEY_L)
    {
        selectedModel--; 
        modelChanged = true;
    }

    if (pressed & KEY_R)
    {
        selectedModel++;
        modelChanged = true;
    }

    if (modelChanged)
    {
        if (selectedModel >= NUM_MODELS)
            selectedModel = NUM_MODELS-1;

        if (selectedModel < 0)
            selectedModel = 0;

        updateConsole();
        lookAtSelectedModel();
    }

    if (pressed & KEY_A) drawMode = !drawMode;

    if (pressed & KEY_START) lookAtSelectedModel();

}

int main()
{
    setup();

    updateConsole();

    while(1) {
        renderScene();
        receiveInput();

        glFlush(0);
        swiWaitForVBlank();
    }

    return 0;
}
