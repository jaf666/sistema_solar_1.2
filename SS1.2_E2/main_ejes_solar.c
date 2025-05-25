#include <windows.h>
#include <glut.h>
#include <GL/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "defini.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PLAN 11

extern void myCamara();
extern void myTelescopio(planeta origen, planeta destino);
extern void myTeclasEspeciales(int cursor, int x, int y);
extern int myEsfera();
void dibujaPlaneta(planeta p);


// Variables de iluminación
GLfloat Ambient[] = { .0f, .0f, .0f, 1.0f };
GLfloat Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat SpecRef[] = { .5f, .5f, .5f, .5f };
GLfloat Specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Variables de definicion de la posicion del foco y direccion de iluminacion
// La posicion en el Sol
GLfloat luzPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
// La direccion de la luz
GLfloat spotDir[] = { 1.0f, 1.0f, 1.0f };
int W_WIDTH = 1000;
int W_HEIGHT = 1000;
int cubo;
int cuboScale = 10;
boolean orbita = 1;
boolean luces = 1;
planeta* planetasPtr;

/* Definición de los planetas
Argumentos:
	1. GLfloat distancia;
	2. GLfloat velocidad_trans;
	3. GLfloat angulo_trans;
	4. GLfloat velocidad_rot;
	5. GLfloat angulo_rot;
	6. GLint tamano;
	7. GLint listarender;
	8. char* nombre;
	9. int textura;

	A continuación se definen los planetas del sistema solar, con sus respectivas características
*/
planeta sol = { 0, 0, 0, 10, 0, 200, 0, "Sol", 0 };
planeta mercurio = { 250, 10, 0, 10, 0, 50, 0, "Mercurio", 0 };
planeta venus = { 500, 12, 0, 10, 0, 60, 0, "Venus", 0 };
planeta tierra = { 800,  5, 0, 10, 0, 80, 0, "Tierra", 0 };
planeta luna = { 140, 20, 0, 10, 0, 20, 0, "Luna", 0 };
planeta iss = { 100, 10, 0, 10, 0, 8,  0, "Iss", 0 };
planeta marte = { 1200, 8, 0, 10, 0, 30, 0, "Marte", 0 };
planeta jupiter = { 1500, 4, 0, 10, 0, 150, 0, "Jupiter", 0 };
planeta saturno = { 2000, 6, 0, 10, 0, 120, 0, "Saturno", 0 };
planeta urano = { 2500, 5, 0, 10, 0, 100, 0, "Urano", 0 };
planeta neptuno = { 3000, 9, 0, 10, 0, 90, 0, "Neptuno", 0 };


// Variable que almacenara el valor de la camara, que variara en funcion de la tecla pulsada y 
// por defecto se encuentra en la camara de la Voyager
camara = 1;
// Indice de la textura empleado para asociar la textura a cada planeta
GLuint textura;


// Parte de las luces

void initLuces(){

	// Definimos las iluminaciones
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, luzPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

	// Efectos de foco
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 179.0f);

	// Activamos las luces
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// Definimos el seguimiento del color como propiedad luminosa de los materiales, esto hara que multiplique el color por el color de la luz
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Definimos las propiedades del brillo metalico, atenua el efecto de la luz especular, marcando mas el dia y la noche
	//glMaterialfv(GL_FRONT, GL_SPECULAR, SpecRef);
}

// Genera un índice para la textura, hace un bind a la textura 2D y le mete los parametros
// vistos, de repetirse en S y en T, de filtrado lineal para que sea mas pequeño y mayor
// y despues se lee el archivo de la textura.
// La asignacion de parametros una vez se hace el bind se hizo aqui.
int myCargaTexturas(char* nombre) {
	// Se genera un índice para las texturas
	glGenTextures(1, &textura);
	// Se hace un bind a la textura 2D
	glBindTexture(GL_TEXTURE_2D, textura); // A partir de este momento todas las operaciones tendrán efecto sobre este objeto de texturas GL_TEXTURE_2D

	// Se aplican parametros de repeticion de textura 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Se aplican parametros de filtro lineal
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	// Se lee el archivo de la textura
	unsigned char* data = stbi_load(nombre, &width, &height, &nrChannels, 0);
	// Dependiento del numero de canales, se asigna el formato de la textura, en este caso RGB (Numero de canales 3)
	// Parametros: textura en 2D, el nivel de detalle, componente de color, ancho y alto de la textura, no tiene borde, formato de la textura (RGB) y el tipo de dato (unsigned char)
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		printf("Error al cargar la textura %s\n", nombre);
	}
	stbi_image_free(data);
	return textura;
}

void initTextures() {
	// Definimos las texturas
	sol.textura = myCargaTexturas("../Texturas/sol.png");
	mercurio.textura = myCargaTexturas("../Texturas/Mercurio.png");
	venus.textura = myCargaTexturas("../Texturas/Venus.png");
	tierra.textura = myCargaTexturas("../Texturas/tierra.png");
	marte.textura = myCargaTexturas("../Texturas/Marte.png");
	jupiter.textura = myCargaTexturas("../Texturas/Jupiter.png");
	saturno.textura = myCargaTexturas("../Texturas/Saturno.png");
	urano.textura = myCargaTexturas("../Texturas/Urano.png");
	neptuno.textura = myCargaTexturas("../Texturas/Neptuno.png");
	luna.textura = myCargaTexturas("../Texturas/Luna.png");
	iss.textura = myCargaTexturas("../Texturas/Iss.jpg");
}

// Funcion encargada de dibujar el sistema solar, incluyendo los modos de visualización de la cámara y el telescopio
void myDisplay() {
	// Se limpian los buffers de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, W_WIDTH, W_HEIGHT);

	// Se coloca la cámara en la posición adecuada, por defecto esta en Voyager
	switch (camara)
	{
	case 1: // Vista de la camara
		myCamara();
		break;
	case 2: // Vista del telescopio
		myTelescopio(tierra, sol);
		break;
		// Miro a mercurio, se acerca y se aleja
	case 3:
		myTelescopio(tierra, mercurio);
		break;
	case 4:
		myTelescopio(tierra, venus);
		break;
	case 5:
		myTelescopio(tierra, marte);
		break;
	case 6: myTelescopio(tierra, jupiter);
		break;
	case 7: myTelescopio(tierra, saturno);
		break;
	case 8: myTelescopio(tierra, urano);
		break;
	case 9: myTelescopio(tierra, neptuno);
		break;
	case 10: myTelescopio(tierra, luna);
		break;
	case 11: myTelescopio(tierra, iss);
		break;
	case 12: myTelescopio(luna, tierra);
		break;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

	glColor3f(1.0f, 1.0f, 1.0f);

	// Se dibujan todos los planetas. Dependiendo de la variable luces, se apaga o enciende el sol
	if (luces) {
	glDisable(GL_LIGHTING);
	dibujaPlaneta(sol);
	glEnable(GL_LIGHTING);
	} else{
	dibujaPlaneta(sol);
	}
	dibujaPlaneta(mercurio);
	dibujaPlaneta(venus);
	dibujaPlaneta(tierra);
	dibujaPlaneta(marte);
	dibujaPlaneta(jupiter);
	dibujaPlaneta(saturno);
	dibujaPlaneta(urano);
	dibujaPlaneta(neptuno);


	glFlush();
	glutSwapBuffers();
}

// Creamos las orbitas dependiendo de la distancia al sol
// Se escogio un slide de 100 segmentos
void myOrbita(float radius) {
	if (orbita == 1) {
		glBegin(GL_LINE_LOOP);
		glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
		for (int a = 0; a < 360; a += 360 / 100) {
			double heading = a * 3.1415926535897932384626433832795 / 180;
			glVertex3f(cos(heading) * radius, 0, sin(heading) * radius);
		}
		glEnd();
		glFlush();
	}
}

// Funcion que se encarga de dibujar un planeta pasado como argumento
void dibujaPlaneta(planeta p) {
	// Se comienza guardando la posicion del Sol
	glPushMatrix();
	myOrbita(p.distancia);
 	// Se traslada el planeta a la posicion dada por su distancia y angulo. Y es igual a 0 porque gira en el eje X, Z
	// Esta funcion movera el planeta a su posicion adecuada en funcion de la distancia a su padre y su angulo de traslacion, dado por el coseno y seno
	// en los ejes X y Z respectivamente, que determinan la posicion del planeta en la orbita
	glTranslatef(p.distancia * cos(p.angulo_trans * CR), 0.0, p.distancia * sin(p.angulo_trans * CR));
	// Se hace un push ya que el planeta hijo no hereadara la rotacion del padre ni su tamaño, pero si la posicion del padre
	glPushMatrix();
		glRotatef(p.angulo_rot, 0, 1, 0);
		glScalef(p.tamano, p.tamano, p.tamano);
		glBindTexture(GL_TEXTURE_2D, p.textura);
		// Inicializo la geometria del planeta mediante la llamada a la lista de renderizado a traves de su indice
		glCallList(myEsfera());
		glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	// Si el planeta es la Tierra, dibujar la Luna y la ISS
	if (strcmp(p.nombre, "Tierra") == 0) {
		glPushMatrix();
			myOrbita(luna.distancia);
			// Dibujo la luna
			glTranslatef(luna.distancia * cos(luna.angulo_trans * CR), 0.0, luna.distancia * sin(luna.angulo_trans * CR));
				glPushMatrix();
					glRotatef(luna.angulo_rot, 0, 1, 0);
					glScalef(luna.tamano, luna.tamano, luna.tamano);
					glBindTexture(GL_TEXTURE_2D, luna.textura);
					glCallList(myEsfera());
					glBindTexture(GL_TEXTURE_2D, 0);
				glPopMatrix();
		glPopMatrix();
		// Dibujo la ISS
		glPushMatrix();
			myOrbita(iss.distancia);
			glTranslatef(iss.distancia * cos(iss.angulo_trans * CR), 0.0, iss.distancia * sin(iss.angulo_trans * CR));
				glPushMatrix();
					glRotatef(iss.angulo_rot, 0, 1, 0);
					glScalef(iss.tamano, iss.tamano, iss.tamano);
					glBindTexture(GL_TEXTURE_2D, iss.textura);
					glCallList(myEsfera());
					glBindTexture(GL_TEXTURE_2D, 0);
				glPopMatrix();
		glPopMatrix();
	}
	// Se elimina de la pila la posicion del Sol
	glPopMatrix();
}

// Funcion para girar los planetas, de forma que se actualiza el angulo de traslacion y rotacion
// cada vez que se llama a la funcion. Se actualiza el angulo de traslacion y rotacion sumando
// la velocidad de traslacion y rotacion respectivamente
void _movPlaneta(planeta* obj) {
	obj->angulo_trans += obj->velocidad_trans;
	if (obj->angulo_trans > 360) obj->angulo_trans -= 360;

	obj->angulo_rot += obj->velocidad_rot;
	if (obj->angulo_rot > 360) obj->angulo_rot -= 360;
}

// Funcion que se encarga de llamar a la funcion _movPlaneta para cada planeta a traves de una lista de planetas
void myMovimiento() {
	planeta* planetas[] = { &sol, &mercurio, &venus, &tierra, &luna, &iss, &marte, &jupiter, &saturno, &urano, &neptuno };

	for (int i = 0; i < sizeof(planetas) / sizeof(planetas[0]); i++) {
		_movPlaneta(planetas[i]);
	}

	glutPostRedisplay();
	// La funcion myMovimiento se llama cada MYTIEMPO milisegundos con prioridad 10
	glutTimerFunc(MYTIEMPO, myMovimiento, 10);
}

// Funcion encargada de cambiar la camara en funcion de las teclas especiales
void onMenu(int option) {
	if (option >= 1 && option <= 12) {
		camara = option;
		glutPostRedisplay();
	}
}

// Funcion que crea el menu de opciones dada una lista de nombres de planetas
void myMenu(void) {
	int menuFondo;
	menuFondo = glutCreateMenu(onMenu);

	char* nombresMenu[] = { "Voyayer", "Sol", "Mercurio", "Venus", "Marte", "Jupiter", "Saturno", "Urano", "Neptuno", "Luna", "ISS", "Luna->Tierra"};
	for (int i = 0; i < 12; i++) {
		glutAddMenuEntry(nombresMenu[i], i + 1);
	}

	// Cuando se hace click derecho salta el menú desde el cual se podrá modificar la vista.
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void openGLInit(void) {
	// Limpio el buffer de profundidad
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);		// Habilitar la profundidad (hacer depth test)
	glEnable(GL_CULL_FACE);			// No dibujar las caras traseras
	glEnable(GL_BACK);			    // Habilitar las caras traseras
	glEnable(GL_NORMALIZE);			// Normalizar las caras

	glEnable(GL_TEXTURE_2D);		// Habilitar texturas
}

void changeSize(GLint newWidth, GLint newHeight) {
	W_HEIGHT = newHeight;
	W_WIDTH = newWidth;
	glViewport(0, 0, newWidth, newHeight);
	onMenu(camara);
}

int main(int argc, char** argv) {
	planeta planetas[PLAN] = { sol, mercurio, venus, tierra, luna, iss, marte, jupiter, saturno, urano, neptuno };
	planetasPtr = planetas;

	// Se inicializa glut
	glutInit(&argc, argv);
	// Inicializo el modo de visualización con doble buffer y colores RGBA
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	// Inicializo la ventana con un tamaño determinado por los parámetros (width, height)
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	// Inicializo la ventana en la posición determinada por los parámetros (x,y) 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sistema Solar");

	openGLInit();

	glutSpecialFunc(myTeclasEspeciales);
	glutKeyboardFunc(myTeclado);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(changeSize);
	
	// Llamo a la funcion que inicializa las luces
	initLuces();
	
	myMovimiento();

	myMenu();
	initTextures(planetas);
	
	myCamara();
	// Empieza el bucle principal
	glutMainLoop();
	return 0;
}