#include "defini.h"

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