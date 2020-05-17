#include "Serializable.h"

#include <iostream>
#include <fstream>

#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
	alloc_data(sizeof(name) + sizeof(x) + sizeof(y));
	
	char* tmp = _data;
	
	memcpy(tmp, name, sizeof(name));
	tmp += sizeof(name);

	memcpy(tmp, &x, sizeof(x));
	tmp += sizeof(x);

	memcpy(tmp, &y, sizeof(y));
    }

    int from_bin(char * data)
    {
        char* tmp = data;

	memcpy(name, tmp, sizeof(name));
	tmp += sizeof(name);

	memcpy(&x, tmp, sizeof(x));
	tmp += sizeof(x);
	
        memcpy(&y, tmp, sizeof(y));

	return 0;
    }

public:
    static const size_t MAX_NAME = 80;
    char name[MAX_NAME];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // Serializar y escribir one_w en un fichero
    one_w.to_bin(); 

    int fileWrite = open(one_w.name, O_CREAT | O_WRONLY);
    write(fileWrite, one_w.data(), one_w.size());
    close(fileWrite);

    // Leer el fichero en un buffer y des-serializar en one_r
    char buffer[one_w.size()];
   
    int fileRead = open(one_w.name, O_RDONLY);
    
    read(fileRead, buffer, one_w.size());
    close(fileRead);

    one_r.from_bin(buffer);
    
    //Mostrar el contenido one_r
    printf("REAL: %s %d %d\n\n", one_r.name, one_r.x, one_r.y);

    return 0;
	
	// Pregunta 1: el comando od muestra en consola el archivo binario con otro formato
	// Pregunta 2: la salida es la representación en consola de los datos que se han
	//             serializado en binario en el método to_bin()
}
