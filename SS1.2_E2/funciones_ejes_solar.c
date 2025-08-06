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
	// Configuramos la matriz de proyección
	glMatrixMode(GL_PROJECTION);
	// Cargamos la matriz identidad
	glLoadIdentity();

	// Se configura la proyección ortográfica fijando los límites determinados por distCam
	// Esto evita trabajar con el tamaño por default de la cámara, sino habria que emplear decimales
	// Establezco el near y el far como 1.0 y 4*distCam respectivamente
	// Las coordenadas de la cámara se fijaron para que todos los planetas esten en el espacio de vista 
	glOrtho(-distCam, distCam, -distCam, distCam, 1.0f, 4 * distCam);

	gluLookAt(((float)distCam * (float)sin(alpha) * cos(beta)), // Valores en coordenadas polares de la cámara
		((float)distCam * (float)sin(beta)),
		((float)distCam * cos(alpha) * cos(beta)),
		0.0, 0.0, 0.0,											// Miro al (0,0,0), donde está el Sol
		0.0, 1.0, 0.0);											// Orientación en Y para mirar hacia adelante
}

void myTelescopio(planeta origen, planeta destino) {
	//Configuración de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	// Cargamos la matriz identidad
	glLoadIdentity();

	// Posicion absoluta del planeta origen (Tierra)
	float origenPosX = origen.distancia * cos(origen.angulo_trans * CR);
	float origenPosZ = origen.distancia * sin(origen.angulo_trans * CR);

	// POSICIÓN RELATIVA del planeta destino respecto al origen (Tierra)
	float destinoPosX = destino.distancia * cos(destino.angulo_trans * CR);
	float destinoPosZ = destino.distancia * sin(destino.angulo_trans * CR);

	// Con gluPerspective se configura la matriz de proyección, ancho dividido por alto
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
		// POSICIÓN ABSOLUTA de la Luna (respecto al Sol)
		float xLuna = origenPosX + destinoPosX;
		float zLuna = origenPosZ + destinoPosZ;

		gluLookAt(
			origenPosX,
			0.0,
			origenPosZ,				// posición de la cámara (Tierra)
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
		// Se configura la distancia de la cámara mediante las teclas l y c, de forma que 
		// se aumenta o disminuye la distancia de la cámara en un 20% respectivamente
	case 'l':
		distCam *= 1.2f;
		break;
	case 'c':
		distCam /= 1.2f;
		break;
	case 'a':
		// Se activa la órbita
		orbita = !orbita;
	// En caso de presionar s se apaga el sol o enciende
		break;
	case 's':
		luces = !luces;
		break;
	default:
		break;
	}
	// Si se modifica la distancia de la cámara se vuelve a llamar a la función myCamara
	glutPostRedisplay();
}

// Función de control de las teclas especiales
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
