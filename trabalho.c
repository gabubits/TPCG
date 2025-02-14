#include <GL/glut.h>
#include <stdlib.h>

// Variáveis para controles de navegação
GLfloat angle, fAspect;

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao(void)
{
    GLfloat luzAmbiente[4] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat luzDifusa[4] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat posicaoLuz0[4] = {5.0f, 5.0f, 0.0f, 1.0f};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz0);
}

// Função callback de redesenho da janela de visualização
void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DefineIluminacao();

    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidCube(1.0f);

    glutSwapBuffers();
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    DefineIluminacao();
    gluLookAt(5.0, 5.0, 3.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.5, 1000);

    PosicionaObservador();
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);

    angle = 45;
}

void GerenciaTeclado(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'e':
    case 'E':
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// Programa Principal
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(850, 850);
    glutCreateWindow("Carrossel");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(GerenciaTeclado);
    Inicializa();
    glutMainLoop();

    return 0;
}