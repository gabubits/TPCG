/*
Trabalho de computação gráfica
Brinquedo: Carrossel
Nomes: Gabriel Gomes e Wandra Martins
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Declaração da função PosicionaObservador
void PosicionaObservador(void);

// Variaveis para controles de navegação
GLfloat angulo = 0.0f, fAspect;
int interruptorAnimacao = 0;
bool luzDirecionalAtiva = true;
bool luzPontualAtiva = true;
float anguloCamera = 0.0f;
float distanciaCamera = 5.0f;
float alturaCamera = 2.0f;
float rotX = 0.0f, rotY = 0.0f, obsZ = 50.0f;  // Declarando variáveis globais
#define PI 3.1415927

// Funcao responsavel por desenhar um cilindro
void DesenhaCilindro(float raio, float altura, int segmentos)
{
    float anguloCilindro, x, y;
    float anguloIncremento = 2.0f * PI / segmentos;

    // Desenha o corpo do cilindro
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segmentos; i++)
    {
        anguloCilindro = i * anguloIncremento;
        x = cos(anguloCilindro);
        y = sin(anguloCilindro);

        // Normal (aponta para fora, na direção radial)
        glNormal3f(x, y, 0.0f);

        glVertex3f(raio * x, raio * y, 0.0f);
        glVertex3f(raio * x, raio * y, altura);
    }
    glEnd();

    // Desenha a base do cilindro
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal aponta para baixo
    glVertex3f(0.0f, 0.0f, 0.0f);  // Centro da base
    for (int i = 0; i <= segmentos; i++)
    {
        anguloCilindro = i * anguloIncremento;
        x = raio * cos(anguloCilindro);
        y = raio * sin(anguloCilindro);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Desenha o topo do cilindro
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);   // Normal aponta para cima
    glVertex3f(0.0f, 0.0f, altura); // Centro do topo
    for (int i = 0; i <= segmentos; i++)
    {
        anguloCilindro = i * anguloIncremento;
        x = raio * cos(anguloCilindro);
        y = raio * sin(anguloCilindro);
        glVertex3f(x, y, altura);
    }
    glEnd();
}
void DefineLuzPontual(void) {
    GLfloat luzPosicao[] = {0.0f, 1.0f, 0.0f, 1.0f};  // Posiciona a luz na cena
    GLfloat luzAmbient[]  = {0.3f, 0.0f, 0.3f, 1.0f};  // Componente ambiente para reforçar a luz
    GLfloat luzCor[]      = {1.0f, 0.0f, 1.0f, 1.0f};  // Cor roxa intensa
    
    glLightfv(GL_LIGHT1, GL_POSITION, luzPosicao);
    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzCor);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luzCor);
}



// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao(void)
{
    // Luz direcional (Simulando o sol)
    GLfloat direcional_posicao[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat direcional_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};   // Componente ambiente para iluminar mais
    GLfloat direcional_diffuse[]  = {1.0f, 0.9f, 0.7f, 1.0f};   // Tom quente, como a luz do sol
    GLfloat direcional_specular[] = {1.0f, 0.9f, 0.7f, 1.0f};   // Mesma tonalidade para o especular

    glLightfv(GL_LIGHT0, GL_POSITION, direcional_posicao);
    glLightfv(GL_LIGHT0, GL_AMBIENT, direcional_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, direcional_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, direcional_specular);
    
    // Configuração da luz pontual (Roxa)
    DefineLuzPontual();
}


// Função para desenhar o piso horizontal, abaixo do brinquedo.
void DesenhaPiso()
{
    glColor3f(0.0, 0.7, 0.0);
    glLineWidth(1.6f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    glColor3f(0.0, 0.7, 0.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glEnd();
}

// Função que desenha o carrossel
void DesenhaCarrossel()
{
    // Base do carrossel
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glColor3ub(229, 170, 203);
    DesenhaCilindro(2.0, 0.2, 50);
    glPopMatrix();

    // Poste central
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glColor3ub(178, 9, 166);
    DesenhaCilindro(0.1, 4.0, 50);
    glPopMatrix();

    // Teto cônico
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glColor3ub(125, 63, 151);
    glutSolidCone(3, 1.5, 50, 50);
    glPopMatrix();

    // Inicia modelagem hierárquica dos postes em torno da base
    // e dos bancos cônicos
    glPushMatrix();
    glRotatef(angulo, 0.0f, 1.0f, 0.0f);

    for (int i = 0; i < 4; i++) // Desenha 4 cones simulando um banco do carrossel
    {
        glPushMatrix();

        float theta = 2.0f * PI * i / 4; //
        float x = 1.5f * cos(theta);     // Cálculo para determinar o desenho do poste e do banco
        float z = 1.5f * sin(theta);     //

        glTranslatef(x, 0.5f, z);
        glRotatef(i * 90.0f, 0.0f, 1.0f, 0.0f);
        glColor3ub(178, 9, 166);

        glPushMatrix();
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glutSolidCone(0.5, 0.5, 50, 50); // Banco
        glPopMatrix();

        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glColor3ub(178, 9, 166);
        DesenhaCilindro(0.05, 3.5, 50); // Poste
        glPopMatrix();
    }
    glPopMatrix();
}
void DesenhaGramado() {
    glColor3f(0.0, 0.5, 0.0); // Cor verde para o gramado
    glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0); // Normal aponta para cima
        glVertex3f(-10.0, -1.0, -10.0); // Canto inferior esquerdo
        glVertex3f(10.0, -1.0, -10.0);  // Canto inferior direito
        glVertex3f(10.0, -1.0, 10.0);    // Canto superior direito
        glVertex3f(-10.0, -1.0, 10.0);   // Canto superior esquerdo
    glEnd();
}
// Função para desenhar nuvens no céu
// Função para desenhar nuvens no céu, que ficam escuras quando a luz direcional está desligada
void DesenhaNuvens() {
    glPushMatrix();
        // Desabilita a iluminação para que possamos definir a cor manualmente
        glDisable(GL_LIGHTING);
        if (luzDirecionalAtiva) {
            glColor3f(1.0f, 1.0f, 1.0f);  // Nuvens brilhantes (brancas)
        } else {
            glColor3f(0.2f, 0.2f, 0.2f);  // Nuvens escuras
        }
        
        // Nuvem 1
        glPushMatrix();
            glTranslatef(-5.0f, 8.0f, -5.0f);
            glutSolidSphere(0.8, 20, 20);
            glTranslatef(0.8f, 0.2f, 0.0f);
            glutSolidSphere(0.6, 20, 20);
            glTranslatef(-1.6f, 0.0f, 0.0f);
            glutSolidSphere(0.7, 20, 20);
        glPopMatrix();

        // Nuvem 2
        glPushMatrix();
            glTranslatef(4.0f, 10.0f, -6.0f);
            glutSolidSphere(1.0, 20, 20);
            glTranslatef(1.0f, 0.3f, 0.0f);
            glutSolidSphere(0.8, 20, 20);
            glTranslatef(-2.0f, 0.0f, 0.0f);
            glutSolidSphere(0.9, 20, 20);
        glPopMatrix();

        // Nuvem 3
        glPushMatrix();
            glTranslatef(0.0f, 9.0f, 6.0f);
            glutSolidSphere(0.9, 20, 20);
            glTranslatef(1.0f, 0.2f, 0.0f);
            glutSolidSphere(0.7, 20, 20);
            glTranslatef(-2.0f, 0.0f, 0.0f);
            glutSolidSphere(0.8, 20, 20);
        glPopMatrix();

        // Reativa a iluminação para os demais elementos da cena
        glEnable(GL_LIGHTING);
    glPopMatrix();
}



// Função callback de redesenho da janela de visualização
void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DefineIluminacao();
    glMatrixMode(GL_MODELVIEW);
    
    PosicionaObservador(); // Atualiza a posição da câmera

    DesenhaNuvens();      // Adiciona as nuvens ao céu
    DesenhaGramado();     // Desenha o gramado
    DesenhaCarrossel();   // Desenha o carrossel

    glutSwapBuffers();
}


// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Ajusta a posição da câmera com base nas variáveis rotX, rotY e obsZ
    float obsX = obsZ * sin(rotY * PI / 180.0) * cos(rotX * PI / 180.0);
    float obsY = obsZ * sin(rotX * PI / 180.0);
    float obsZFinal = obsZ * cos(rotY * PI / 180.0) * cos(rotX * PI / 180.0);

    gluLookAt(obsX, obsY, obsZFinal, // Posição da câmera
              0.0, 2.0, 0.0,         // Ponto de interesse
              0.0, 1.0, 0.0);        // Vetor "up"
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, fAspect, 0.5, 1000);

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

// Função callback para a animação de rotação do carrossel
void Animacao(int value)
{
    if (interruptorAnimacao) // Se a animação estiver ligada
    {
        angulo += 1.0f;
        if (angulo > 360) // Quando rotaciona em 360 graus, reseta.
            angulo -= 360;

        glutPostRedisplay();
        glutTimerFunc(16, Animacao, 0); // ~60 FPS
    }
}

void TeclasEspeciais(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:   // Tecla de seta para a esquerda
            rotY -= 5.0f;     // Girar a câmera para a esquerda
            break;
        case GLUT_KEY_RIGHT:  // Tecla de seta para a direita
            rotY += 5.0f;     // Girar a câmera para a direita
            break;
        case GLUT_KEY_UP:     // Tecla de seta para cima
            rotX -= 5.0f;     // Girar a câmera para cima
            break;
        case GLUT_KEY_DOWN:   // Tecla de seta para baixo
            rotX += 5.0f;     // Girar a câmera para baixo
            break;
        case GLUT_KEY_F1:    // Tecla F1
            obsZ -= 10.0f;    // Aproxime a câmera
            break;
        case GLUT_KEY_F2:    // Tecla F2
            obsZ += 10.0f;    // Afaste a câmera
            break;
        default:
            break;
    }
    glutPostRedisplay(); // Redesenha a cena
}



// Função responsável por inicializar parâmetros e variáveis
void Inicializa(void)
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Simula a cor azul claro do céu
    glutSpecialFunc(TeclasEspeciais);  // Registra a função de teclas especiais

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);
}

// Função callback que define quais teclas terão efeito
// na interação do usuário.
void GerenciaTeclado(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'e': // Sair do programa
    case 'E':
        exit(0);
        break;

    case 'a': // Ligar/desligar animação
    case 'A':
        interruptorAnimacao = !interruptorAnimacao; // Troca o estado do interruptor
        if (interruptorAnimacao)
            glutTimerFunc(25, Animacao, 0); // Se ligado, inicia a animação
        break;
    case 's': // Ligar/desligar luz direcional
        luzDirecionalAtiva = !luzDirecionalAtiva;
        if (luzDirecionalAtiva)
            glEnable(GL_LIGHT0);  // Ativa luz direcional
        else
            glDisable(GL_LIGHT0); // Desativa luz direcional
        break;
    case 'l': // Ligar/desligar luz pontual
        luzPontualAtiva = !luzPontualAtiva;
        if (luzPontualAtiva)
            glEnable(GL_LIGHT1);  // Ativa luz pontual
        else
            glDisable(GL_LIGHT1); // Desativa luz pontual
        break;
    }

    glutPostRedisplay(); // Atualiza a tela após a modificação
}


// Programa Principal
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Modo de operação do GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Posição inicial da janela
    glutInitWindowPosition(5, 5);

    // Tamanho inicial
    glutInitWindowSize(850, 850);

    // Título da janela
    glutCreateWindow("Carrossel");

    // Registra função de callback de redesenho da janela
    glutDisplayFunc(Desenha);

    // Registra função de callback de redimensionamento
    glutReshapeFunc(AlteraTamanhoJanela);
    
    // Registra função de callback para comandos recebidos do teclado
    glutKeyboardFunc(GerenciaTeclado);

    // Registra função de callback para teclas especiais (setas)
    glutSpecialFunc(TeclasEspeciais);
    
    // Inicialização
    Inicializa();
    
    // Inicia o processamento
    glutMainLoop();
    
    return 0;
}