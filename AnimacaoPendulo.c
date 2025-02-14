#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

// Variáveis para controles de navegação
GLfloat angle, fAspect;


float angleP = 0; // rotação angulo
float T = 0; //tempo global

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao (void)
{
	GLfloat luzAmbiente[4]={0.25,0.25,0.25,1.0}; 
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor" 
	GLfloat posicaoLuz0[4]={0.0, 80.0, 0.0, 1.0};
	GLfloat posicaoLuz1[4]={0.0, -80.0, 0.0, 1.0}; 

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz0 );   

	// Define os parâmetros da luz de número 1
	glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz1 ); 
}
void DesenhaPiso(){
   glColor3f(0.0,0.7,0.0);
   glLineWidth(1.6f);
   glBegin(GL_LINE_LOOP);	
		glVertex3f(50.0, -10.0, 50.0);
		glVertex3f(50.0, -10.0, -50.0);
		glVertex3f(-50.0, -10.0, -50.0);
		glVertex3f(-50.0, -10.0, 50.0);
	glEnd();

	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);	
		glColor3f(0.0,0.7,0.0);
		glVertex3f(50.0, -10.0, 50.0);
		glVertex3f(50.0, -10.0, -50.0);
		glVertex3f(-50.0, -10.0, -50.0);
		glVertex3f(-50.0, -10.0, 50.0);
    glEnd();
}
// Função que desenha um cubo cujas cores dos vértices
// mostram como é o espaço RGB
void DesenhaCubo(void)
{
	// Desenhas as linhas das "bordas" do cubo
	glColor3f(0.0f, 0.0f, 0.0f); 
	glLineWidth(1.6f);
	glBegin(GL_LINE_LOOP);	// frontal
		glVertex3f(10.0, 10.0, 10.0);
		glVertex3f(-10.0, 10.0, 10.0);
		glVertex3f(-10.0, -10.0, 10.0);
		glVertex3f(10.0, -10.0, 10.0);
	glEnd();
	glBegin(GL_LINE_LOOP);	//  posterior
		glVertex3f(10.0, 10.0, -10.0);
		glVertex3f(10.0, -10.0, -10.0);
		glVertex3f(-10.0, -10.0, -10.0); 
		glVertex3f(-10.0, 10.0, -10.0);
	glEnd();
	glBegin(GL_LINES);	//  laterais
		glVertex3f(-10.0, 10.0, -10.0);
		glVertex3f(-10.0, 10.0, 10.0); 
		glVertex3f(-10.0, -10.0, -10.0);
		glVertex3f(-10.0, -10.0, 10.0);     
		glVertex3f(10.0, 10.0, -10.0);
		glVertex3f(10.0, 10.0, 10.0); 
		glVertex3f(10.0, -10.0, -10.0);
		glVertex3f(10.0, -10.0, 10.0);  
	glEnd();
 
	// Desenha as faces do cubo preenchidas
	// Face frontal
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);	// Normal da face
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(10.0, 10.0, 10.0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-10.0, 10.0, 10.0);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-10.0, -10.0, 10.0);
		glColor3f(1.0f, 0.0f, 0.0f);  
		glVertex3f(10.0, -10.0, 10.0);
	// Face posterior
     	glNormal3f(0.0, 0.0, -1.0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(10.0, 10.0, -10.0);
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(10.0, -10.0, -10.0);
		glColor3f(0.0f, 0.0f, 1.0f);  
		glVertex3f(-10.0, -10.0, -10.0);
		glColor3f(0.0f, 1.0f, 1.0f);  
		glVertex3f(-10.0, 10.0, -10.0);
	// Face lateral esquerda
		glNormal3f(-1.0, 0.0, 0.0);
		glColor3f(0.0f, 1.0f, 0.0f);  
		glVertex3f(-10.0, 10.0, 10.0);
		glColor3f(0.0f, 1.0f, 1.0f);  
		glVertex3f(-10.0, 10.0, -10.0);
		glColor3f(0.0f, 0.0f, 1.0f);  
		glVertex3f(-10.0, -10.0, -10.0);
		glColor3f(0.0f, 0.0f, 0.0f);  
		glVertex3f(-10.0, -10.0, 10.0);
	// Face lateral direita
		glNormal3f(1.0, 0.0, 0.0);
		glColor3f(1.0f, 1.0f, 0.0f);  
		glVertex3f(10.0, 10.0, 10.0);
		glColor3f(1.0f, 0.0f, 0.0f);  
		glVertex3f(10.0, -10.0, 10.0);
		glColor3f(1.0f, 0.0f, 1.0f);  
		glVertex3f(10.0, -10.0, -10.0);
		glColor3f(1.0f, 1.0f, 1.0f);  
		glVertex3f(10.0, 10.0, -10.0);
	// Face superior
		glNormal3f(0.0, 1.0, 0.0);
		glColor3f(0.0f, 1.0f, 1.0f);  
		glVertex3f(-10.0, 10.0, -10.0);
		glColor3f(0.0f, 1.0f, 0.0f);  
		glVertex3f(-10.0, 10.0, 10.0);
		glColor3f(1.0f, 1.0f, 0.0f);  
		glVertex3f(10.0, 10.0, 10.0);
		glColor3f(1.0f, 1.0f, 1.0f);  
		glVertex3f(10.0, 10.0, -10.0);
	// Face inferior
		glNormal3f(0.0, -1.0, 0.0);
		glColor3f(0.0f, 0.0f, 1.0f);  
		glVertex3f(-10.0, -10.0, -10.0);
		glColor3f(1.0f, 0.0f, 1.0f);  
		glVertex3f(10.0, -10.0, -10.0);
		glColor3f(1.0f, 0.0f, 0.0f);  
		glVertex3f(10.0, -10.0, 10.0);
		glColor3f(0.0f, 0.0f, 0.0f);  
		glVertex3f(-10.0, -10.0, 10.0);
	glEnd();
}

// Função callback de redesenho da janela de visualização
void Desenha(void)
{
	// Limpa a janela de visualização com a cor  
	// de fundo definida previamente
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// Chama a função que especifica os parâmetros de iluminação
   DefineIluminacao();

  glPushMatrix();   
    glRotated(45,0,1,0);	
    glRotated(-15,0,0,1);
	DesenhaPiso();
  glPopMatrix();


  glPushMatrix();
      glTranslated( 0, 10, 0 );
      glRotated(angleP, 0, 0, 1 );
      glTranslated( 0, -10, 0 );
      glPushMatrix();
        glRotated(45,0,1,0);
	    glRotated(-15,0,0,1);
        glTranslated(0,-5.0, 0);
	    glScalef(0.3,0.3,0.3);
	    DesenhaCubo();
    glPopMatrix();
    glPushMatrix();
        glRotated(45,0,1,0);
        glRotated(-15,0,0,1);
	    glTranslated(0, 5, 0);
  	    glScalef(0.1,1.5,0.1);
	    DesenhaCubo();
        glPopMatrix();
  glPopMatrix();
 

	// Executa os comandos OpenGL
	glutSwapBuffers();
}

void TimerFunction( int value ){

  T++;
  if( T == 180 ) T = 0; // movimento periodico
  angleP = sin(T*(3.1417)/90)*45; // calculo do angulo

  glutPostRedisplay();
  glutTimerFunc( 33, TimerFunction, 1);
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();
	DefineIluminacao();
	// Posiciona e orienta o observador	
	gluLookAt(40,20,-120,0,0,0,0,1,0);

}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva 
	// (angulo, aspecto, zMin, zMax)
	gluPerspective(angle,fAspect,0.5,1000);
    //utiliza para verificar se as posições dos objetos estão corretas.
	//glOrtho(-50.0,50.0, -50.0,50.0, 1.0,150.0);
    
	PosicionaObservador();
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica as dimensões da viewport
	glViewport(0, 0, w, h);
 
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{   
	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita a luz de número 1
	glEnable(GL_LIGHT1);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);
     
	// Inicializa a variável que especifica o ângulo da projeção
	// perspectiva
	angle=45;  
	glutTimerFunc(33, TimerFunction, 1 ); // 33 ms 
	
}

// Programa Principal 
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
 
	// Especifica a posição inicial da janela GLUT
	glutInitWindowPosition(5,5); 

	// Especifica o tamanho inicial em pixels da janela GLUT
	glutInitWindowSize(850,850); 
 
	// Cria a janela passando como argumento o título da mesma
	glutCreateWindow("Cubo RGB");
 
	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(Desenha);
  
	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(AlteraTamanhoJanela);

	// Chama a função responsável por fazer as inicializações 
	Inicializa();

	glutTimerFunc(33, TimerFunction,1);
 
	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();
 
	return 0;
}