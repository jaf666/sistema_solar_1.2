#include <stdio.h>
#include <math.h>
#include "glut.h"
#include <windows.h>
#include <glu.h>
#include <gl.h>
#include "defini.h"

#define INCREMENTO .01
#define PI 3.14159265358979323846

float alpha = 0.0;
float beta = 0.0;
float distCam = 1000;

extern boolean orbita;
extern boolean luces;

void myCamara() {
	// Configuramos la matriz de proyecci�n
	glMatrixMode(GL_PROJECTION);
	// Cargamos la matriz identidad
	glLoadIdentity();

	// Se configura la proyecci�n ortogr�fica fijando los l�mites determinados por distCam
	// Esto evita trabajar con el tama�o por default de la c�mara, sino habria que emplear decimales
	// Establezco el near y el far como 1.0 y 4*distCam respectivamente
	// Las coordenadas de la c�mara se fijaron para que todos los planetas esten en el espacio de vista 
	glOrtho(-distCam, distCam, -distCam, distCam, 1.0f, 4 * distCam);

	gluLookAt(((float)distCam * (float)sin(alpha) * cos(beta)), // Valores en coordenadas polares de la c�mara
		((float)distCam * (float)sin(beta)),
		((float)distCam * cos(alpha) * cos(beta)),
		0.0, 0.0, 0.0,											// Miro al (0,0,0), donde est� el Sol
		0.0, 1.0, 0.0);											// Orientaci�n en Y para mirar hacia adelante
}

void myTelescopio(planeta origen, planeta destino) {
	//Configuraci�n de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	// Cargamos la matriz identidad
	glLoadIdentity();

	// Posicion absoluta del planeta origen (Tierra)
	float origenPosX = origen.distancia * cos(origen.angulo_trans * CR);
	float origenPosZ = origen.distancia * sin(origen.angulo_trans * CR);

	// POSICI�N RELATIVA del planeta destino respecto al origen (Tierra)
	float destinoPosX = destino.distancia * cos(destino.angulo_trans * CR);
	float destinoPosZ = destino.distancia * sin(destino.angulo_trans * CR);

	// Con gluPerspective se configura la matriz de proyecci�n, ancho dividido por alto
	gluPerspective(45.0, 1.0, 1.0, 9000.0);

	if (strcmp(destino.nombre, "Luna") != 0 && strcmp(destino.nombre, "Iss") != 0) {
		gluLookAt(
			origenPosX,
			0,
			origenPosZ,
			destinoPosX,
			0,
			destinoPosZ,
			0, 1, 0
		);
	}
	if (strcmp(origen.nombre, "Tierra") == 0 && (strcmp(destino.nombre, "Luna") == 0 || (strcmp(destino.nombre, "Iss") == 0))) {
		// POSICI�N ABSOLUTA de la Luna (respecto al Sol)
		float xLuna = origenPosX + destinoPosX;
		float zLuna = origenPosZ + destinoPosZ;

		gluLookAt(
			origenPosX,
			0.0,
			origenPosZ,				// posici�n de la c�mara (Tierra)
			xLuna,
			0.0,
			zLuna,					// hacia donde mira (Luna)
			0.0, 1.0, 0.0           // up vector
		);

	}
}

void myTeclado(unsigned char tras, int x, int y)
{
	switch (tolower(tras)) {
		// Se configura la distancia de la c�mara mediante las teclas l y c, de forma que 
		// se aumenta o disminuye la distancia de la c�mara en un 20% respectivamente
	case 'l':
		distCam *= 1.2f;
		break;
	case 'c':
		distCam /= 1.2f;
		break;
	case 'a':
		// Se activa la �rbita
		orbita = !orbita;
	// En caso de presionar s se apaga el sol o enciende
		break;
	case 's':
		luces = !luces;
		break;
	default:
		break;
	}
	// Si se modifica la distancia de la c�mara se vuelve a llamar a la funci�n myCamara
	glutPostRedisplay();
}

// Funci�n de control de las teclas especiales
void myTeclasEspeciales(int cursor, int x, int y)
{
	switch (tolower(cursor))
	{
		//Giros:
	case GLUT_KEY_UP:
		beta += INCREMENTO;
		break;
	case GLUT_KEY_DOWN:
		beta -= INCREMENTO;
		break;
	case GLUT_KEY_RIGHT:
		alpha -= INCREMENTO;
		break;
	case GLUT_KEY_LEFT:
		alpha += INCREMENTO;
		break;
	default:
		break;
	}

	if (alpha >= PI * 2.0 && alpha <= 0) alpha = 0;
	if (beta >= PI * 2.0 && beta <= 0) beta = 0; //hay que repasarlo para evitar el salto


	glutPostRedisplay();
}
