/*
Trabalho de computação gráfica
Brinquedo: Carrossel
Nomes: Gabriel Gomes e Wandra Martins

Teclas adicionais: F1, F2, Z e X, descritas na funçaõ de definição dos comando do teclado.
Além das teclas requisitadas no trabalho.

Para compilar: gcc trabalho.c -o trab -lglut -lGLU -lGL -lm
Para executar: ./trab
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
float rotX = 30.0f, rotY = 45.0f, obsZ = 30.0f; // Declarando variáveis globais
float velocidadeGiro = 1.0f;
#define PI 3.1415927

// Funcao responsavel por desenhar um cilindro.
// raio é o raio da base e do topo do cilindro;
// altura é a altura do cilindro no eixo Z;
// segmentos é o número de segmentos para aproximar a forma circular do cilindro
void DesenhaCilindro(float raio, float altura, int segmentos)
{
    // Ângulo atual para calcular x e y
    float anguloCilindro;

    // Ponto (x, y) na circunferencia do círculo
    float x, y;

    // Incremento angular entre cada segmento
    float anguloIncremento = 2.0f * PI / segmentos;

    // Desenha o corpo do cilindro (com quadriláteros)
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segmentos; i++)
    {
        // Calcula o ângulo atual
        anguloCilindro = i * anguloIncremento;

        // Calcula o ponto (x, y) atual
        x = cos(anguloCilindro);
        y = sin(anguloCilindro);

        // Normal (aponta para fora, na direção radial)
        glNormal3f(x, y, 0.0f);

        // Define um vértice na base do cilindro
        glVertex3f(raio * x, raio * y, 0.0f);

        // Define o outro vértice no topo do cilindro
        glVertex3f(raio * x, raio * y, altura);
    }
    glEnd();

    // Desenha a base do cilindro (com leque de triângulos)
    glBegin(GL_TRIANGLE_FAN);

    // Normal aponta para baixo
    glNormal3f(0.0f, 0.0f, -1.0f);

    // Centro da base
    glVertex3f(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= segmentos; i++)
    {
        // Calcula o ângulo atual
        anguloCilindro = i * anguloIncremento;

        // Calcula o ponto (x, y) atual
        x = raio * cos(anguloCilindro);
        y = raio * sin(anguloCilindro);

        // Vértice na borda da base
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Desenha o topo do cilindro (com leque de triângulos)
    glBegin(GL_TRIANGLE_FAN);

    // Normal aponta para cima
    glNormal3f(0.0f, 0.0f, 1.0f);

    // Centro do topo
    glVertex3f(0.0f, 0.0f, altura);

    for (int i = 0; i <= segmentos; i++)
    {
        // Calcula o ângulo atual
        anguloCilindro = i * anguloIncremento;
        // Calcula o ponto (x, y) atual
        x = raio * cos(anguloCilindro);
        y = raio * sin(anguloCilindro);

        // Vértice na borda do topo
        glVertex3f(x, y, altura);
    }
    glEnd();
}

// Função responsável pela especificação dos parâmetros da luz pontual (embaixo do brinquedo)
void DefineLuzPontual(void)
{
    GLfloat luzPosicao[] = {0.0f, 1.0f, 0.0f, 1.0f};  // Posiciona a luz na cena
    GLfloat luzAmbiente[] = {0.3f, 0.0f, 0.3f, 1.0f}; // Componente ambiente para reforçar a luz
    GLfloat luzCor[] = {1.0f, 0.0f, 1.0f, 1.0f};      // Cor roxa intensa

    glLightfv(GL_LIGHT1, GL_POSITION, luzPosicao); // Configuração da posição
    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente); // Configuração da luz ambiente
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzCor);      // Configuração da cor da luz (difusa)
}

// Função responsável pela especificação dos parâmetros da luz direcional (Sol)
void DefineLuzDirecional(void)
{

    GLfloat luzPosicao[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat luzAmbiente[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Componente ambiente para iluminar mais
    GLfloat luzCor[] = {1.0f, 0.9f, 0.7f, 1.0f};      // Tom quente, como a luz do sol

    GLfloat especular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Mesma tonalidade para o especular
    GLfloat shininess[] = {30.0f};                  // Brilho do material (baixo para especularidade suave)

    glLightfv(GL_LIGHT0, GL_POSITION, luzPosicao);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzCor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular);

    glMaterialfv(GL_FRONT, GL_SPECULAR, especular);  // Configuração para a luz especular na frontal
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess); // Configuração do brilho da superfície
}

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao(void)
{
    // Configuração da luz direcional
    DefineLuzDirecional();

    // Configuração da luz pontual (Roxa)
    DefineLuzPontual();
}

// Função para desenhar o piso horizontal, abaixo do brinquedo.
void DesenhaPiso()
{
    glColor3f(0.0, 0.7, 0.0); // Linha Verde escuro
    glLineWidth(1.6f);

    glBegin(GL_LINE_LOOP);        // Desenho de loop de linhas
    glVertex3f(-1.0, -1.0, 1.0);  // Canto inferior esquerdo (frente)
    glVertex3f(1.0, -1.0, 1.0);   // Canto inferior direito (frente)
    glVertex3f(1.0, -1.0, -1.0);  // Canto inferior direito (atrás)
    glVertex3f(-1.0, -1.0, -1.0); // Canto inferior esquerdo (atrás)
    glEnd();

    glBegin(GL_QUADS);            // Desenho de um quadrilátero
    glNormal3f(0.0, 0.0, 1.0);    // Normal do piso, apontada para cima
    glColor3f(0.0, 0.7, 0.0);     // Piso Verde escuro
    glVertex3f(-1.0, -1.0, 1.0);  // Canto inferior esquerdo (frente)
    glVertex3f(1.0, -1.0, 1.0);   // Canto inferior direito (frente)
    glVertex3f(1.0, -1.0, -1.0);  // Canto inferior direito (atrás)
    glVertex3f(-1.0, -1.0, -1.0); // Canto inferior esquerdo (atrás)
    glEnd();
}

// Função que desenha o carrossel
void DesenhaCarrossel()
{
    // Base do carrossel
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f); // Move um pouco para baixo (eiro y)
    glRotatef(-90.0, 1.0, 0.0, 0.0); // Rotaciona em -90 graus no eixo x (sentido horário)
    glColor3ub(229, 170, 203);       // Cor rosa usando parâmetros RGB
    DesenhaCilindro(5.0, 0.2, 50);
    glPopMatrix();

    // Poste central
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f); // Desenha na mesma posição central da base
    glRotatef(-90.0, 1.0, 0.0, 0.0); // Rotaciona -90 graus no eixo x(sentido horário)
    glColor3ub(178, 9, 166);         // Cor roxa usando RGB
    DesenhaCilindro(0.4, 4.0, 50);
    glPopMatrix();

    // Teto cônico
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);  // Move pra cima (eixo y)
    glRotatef(-90.0, 1.0, 0.0, 0.0); // Rotaciona -90 graus no eixo x (sentido horário)
    glColor3ub(125, 63, 151);        // Cor roxa usando RGB
    glutSolidCone(5, 3, 50, 50);
    glPopMatrix();

    // Inicia modelagem hierárquica dos postes em torno da base
    // e dos bancos cônicos
    glPushMatrix();
    glRotatef(angulo, 0.0f, 1.0f, 0.0f); // Eixo de rotação no eixo y

    for (int i = 0; i < 12; i++) // Desenha 4 cones simulando um banco do carrossel
    {
        glPushMatrix();

        float theta = 2.0f * PI * i / 12; //
        float x = 4.1f * cos(theta);      // Cálculo para determinar o desenho do poste e do banco
        float z = 4.1f * sin(theta);      //

        glTranslatef(x, 0.5f, z);
        glRotatef(i * 90.0f, 0.0f, 1.0f, 0.0f);
        glColor3ub(178, 9, 166);

        glPushMatrix();                  // Para não influenciar o desenho do poste
        glRotatef(90.0, 1.0, 0.0, 0.0);  // Desenha o cone de "cabeça para baixo"
        glutSolidCone(0.5, 0.5, 50, 50); // Banco
        glPopMatrix();

        glRotatef(-90.0, 1.0, 0.0, 0.0); // Necessário porque o cilindro é desenhado no eixo z
                                         // Então, rotaciona para que ele fique em pé
        glColor3ub(178, 9, 166);
        DesenhaCilindro(0.05, 3.5, 50); // Poste
        glPopMatrix();
    }
    glPopMatrix();
}
void DesenhaGramado()
{
    glColor3f(0.0, 0.5, 0.0); // Cor verde para o gramado
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);      // Normal aponta para cima
    glVertex3f(-10.0, -1.0, -10.0); // Canto inferior esquerdo
    glVertex3f(10.0, -1.0, -10.0);  // Canto inferior direito
    glVertex3f(10.0, -1.0, 10.0);   // Canto superior direito
    glVertex3f(-10.0, -1.0, 10.0);  // Canto superior esquerdo
    glEnd();
}

// Função para desenhar nuvens no céu, que ficam escuras quando a luz direcional está desligada
// Desenha várias esferar de cor branca juntas para simular uma nuvem
void DesenhaNuvens()
{
    glPushMatrix();
    // Desabilita a iluminação para que possamos definir a cor manualmente
    glDisable(GL_LIGHTING);
    if (luzDirecionalAtiva)
    {
        glColor3f(1.0f, 1.0f, 1.0f); // Nuvens brilhantes (brancas)
    }
    else
    {
        glColor3f(0.2f, 0.2f, 0.2f); // Nuvens escuras
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
void Desenha(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DefineIluminacao(); // Define parâmetros de iluminação
    glMatrixMode(GL_MODELVIEW);

    PosicionaObservador(); // Posiciona a cãmera

    DesenhaNuvens();    // Adiciona as nuvens ao céu
    DesenhaGramado();   // Desenha o gramado
    DesenhaCarrossel(); // Desenha o carrossel

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
        angulo += velocidadeGiro; // Aumenta a rotação de acordo com a velocidade de giro
        if (angulo > 360)         // Quando rotaciona em 360 graus, reseta.
            angulo -= 360;

        glutPostRedisplay();            // Redesenha a cena
        glutTimerFunc(16, Animacao, 0); // Animação exibida em torno de 60FPS.
    }
}

// Teclas especiais que controlam o programa
void TeclasEspeciais(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: // Tecla de seta para a esquerda
        rotY -= 5.0f;   // Girar a câmera para a esquerda
        break;
    case GLUT_KEY_RIGHT: // Tecla de seta para a direita
        rotY += 5.0f;    // Girar a câmera para a direita
        break;
    case GLUT_KEY_UP: // Tecla de seta para cima
        rotX -= 5.0f; // Girar a câmera para cima
        break;
    case GLUT_KEY_DOWN: // Tecla de seta para baixo
        rotX += 5.0f;   // Girar a câmera para baixo
        break;
    case GLUT_KEY_F1:  // Tecla F1
        obsZ -= 10.0f; // Aproxime a câmera
        break;
    case GLUT_KEY_F2:  // Tecla F2
        obsZ += 10.0f; // Afaste a câmera
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

    glEnable(GL_COLOR_MATERIAL); // Ativa o uso de cores dos materiais
    glEnable(GL_LIGHTING);       // Ativa a iluminação
    glEnable(GL_LIGHT0);         // Ativa a fonte de luz no índice 0
    glEnable(GL_DEPTH_TEST);     // Ativa o teste de profundidade

    glShadeModel(GL_SMOOTH); // Modelo de sombreamento de Gouraud
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
    case 'S':
        luzDirecionalAtiva = !luzDirecionalAtiva;
        if (luzDirecionalAtiva)
            glEnable(GL_LIGHT0); // Ativa luz direcional
        else
            glDisable(GL_LIGHT0); // Desativa luz direcional
        break;
    case 'l': // Ligar/desligar luz pontual
    case 'L':
        luzPontualAtiva = !luzPontualAtiva;
        if (luzPontualAtiva)
            glEnable(GL_LIGHT1); // Ativa luz pontual
        else
            glDisable(GL_LIGHT1); // Desativa luz pontual
        break;
    case 'z': // Desacelerar a velocidade de giro
    case 'Z':
        velocidadeGiro -= 0.1f;
        if (velocidadeGiro < 0.0f)
            velocidadeGiro = 0.0f;
        break;
    case 'x': // Acelerar a velocidade de giro
    case 'X':
        velocidadeGiro += 0.1f;
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