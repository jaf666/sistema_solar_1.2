#include "texturas.h"
#include "defini.h"
#include <stdio.h>
#include "stb_image.h"
#include <stdarg.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Genera un índice para la textura, hace un bind a la textura 2D y le mete los parametros
// vistos, de repetirse en S y en T, de filtrado lineal para que sea mas pequeño y mayor
// y despues se lee el archivo de la textura.
// La asignacion de parametros una vez se hace el bind se hizo aqui.
int myCargaTexturas(char* nombre) {
	// Indice de la textura empleado para asociar la textura a cada planeta
	GLuint textura;
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
	sol.textura = myCargaTexturas(TEXTURA_SOL);
	mercurio.textura = myCargaTexturas(TEXTURA_MERCURIO);
	venus.textura = myCargaTexturas(TEXTURA_VENUS);
	tierra.textura = myCargaTexturas(TEXTURA_TIERRA);
	marte.textura = myCargaTexturas(TEXTURA_MARTE);
	jupiter.textura = myCargaTexturas(TEXTURA_JUPITER);
	saturno.textura = myCargaTexturas(TEXTURA_SATURNO);
	urano.textura = myCargaTexturas(TEXTURA_URANO);
	neptuno.textura = myCargaTexturas(TEXTURA_NEPTUNO);
	luna.textura = myCargaTexturas(TEXTURA_LUNA);
	iss.textura = myCargaTexturas(TEXTURA_ISS);
}