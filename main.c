#include <stdlib.h>
#include <GL/glut.h>

#define TIMER_ID 1
#define TIMER_INTERVAL 20

float hours;
int animation_ongoing;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int id);

/* 
Radius sunca - 	696,392 km - stavljamo na 100
Radius zemlje - 6371 km - 109 puta manji od sunca - stavljamo na 0.917
Udaljenost zemlje od sunca - 149.600.000 km - prevelika vrednost
    pa cemo da stavimo kao dva puta radius sunca

*/


int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    /* Inicijalizujemo promenljive */
    hours = 0;
    animation_ongoing = 0;

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_timer(int id)
{
    if (TIMER_ID != id)
        return;

    hours += 18;

    glutPostRedisplay();

    if (animation_ongoing) {
        glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    }
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;

    case 'g':
    case 'G':
        /* Pokrecemo animaciju */
        if (!animation_ongoing) {
            animation_ongoing = 1;
            glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
        }
        break;

    case 's':
    case 'S':
        animation_ongoing = 0;
        break;
    }
}

static void on_reshape(int width, int height)
{
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se vidna tacka. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(2, 5, 4, 0, 0, 0, 0, 1, 0);
    gluLookAt(0, 1000, 200, 0, 0, 0, 0, 1, 0);

    // Ugao rotacije Sunca oko svoje ose 
    float sun_rotation = 360*hours/(15*24);

    // Sunce
    glPushMatrix();
        glRotatef(sun_rotation, 0,0,1);
        glColor3f(1,1,0);
        glutWireSphere(100,50,50);
    glPopMatrix();

    // Uglovi rotacije Zemlje oko svoje ose i oko Sunca
    float earth_revolution = 360 * hours / (365 * 24);
    float earth_rotation = 360*hours/24;

    // Zemlja
    glRotatef(earth_revolution, 0,0,1);
    glTranslatef(200,0,0);  
    glPushMatrix();
        glRotatef(earth_rotation, 0,0,1);
        glColor3f(0,0,1);
        glutWireSphere(0.917,50,50);
    glPopMatrix();


    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}