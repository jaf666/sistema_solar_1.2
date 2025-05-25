// Fichero de definiciones que contiene la estructura que define cada planeta, así como los prototipos
// de las funciones relacionadas con el movimiento de la cámara y la gestión de las teclas especiales


// Defino la estructura del planeta
typedef struct {
	GLfloat distancia;			// Distancia del planeta a su padre (Sol o Tierra en el caso de la Luna y el ISS)
	GLfloat velocidad_trans;	// Velocidad de traslación alrededor de su padre
	GLfloat angulo_trans;		// Angulo girado
	GLfloat velocidad_rot;		// Velocidad de rotación sobre su eje
	GLfloat angulo_rot;			// Angulo de rotación
	GLint tamano;				// Tamaño del planeta
	GLint listarender;			// Lista de renderizado (esfera)
	char* nombre;				// Nombre del planeta (para identificar luna y iss al dibujarlas y en el modo telescopio)
	int textura;				// Índice a la textura del planeta
} planeta;

// Convertir grados a radianes (PI / 180)
#define CR 0.0175
// Ajustar la velocidad con la que se mueven los planetas
#define MYTIEMPO 41
#define TELE 0

//////////////////////////////
// Prototipos de funciones //
/////////////////////////////

// Funcion para simular la camara
void myCamara();
// Funcion para atender a los eventos de teclado
void myTeclado(unsigned char trans, int x, int y);
// Funcion para atender a los eventos de teclado especiales
void myTeclasEspeciales(int cursor, int x, int y);
// Funcion para modificar la camara
void myTelescopio(planeta origen, planeta destino);

