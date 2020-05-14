#include "Serializable.h"

#include <iostream>
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
		memcpy(tmp, (void*)&name, sizeof(name));
		tmp += sizeof(name);
		memcpy(tmp, (void*)&x, sizeof(x));
		tmp += sizeof(x);
		memcpy(tmp, (void*)&y, sizeof(y));
    }

    int from_bin(char * data)
    {
		char* tmp = _data;
		memcpy((void*)&name, tmp, sizeof(name));
		tmp += sizeof(name);
		memcpy((void*)&x, tmp, sizeof(x));
		tmp += sizeof(x);
		memcpy((void*)&y, tmp, sizeof(y));
        return 0;
    }

	char * getdata() {
		return _data;
	}

	int32_t getsize() {
		return _size;
	}


public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    //Serializar y escribir one_w en un fichero
	one_w.to_bin();

	// WIP
	FILE *file = fopen("one_w.bin","wb");
	fwrite(one_w.getdata(), sizeof(one_w.getdata()), 1, file);

    //Leer el fichero en un buffer y "deserializar" en one_r
	one_w.from_bin(one_w.getdata());

	char* buffer;
	fread(buffer, sizeof(one_w.getdata()), 1, file);

    //Mostrar el contenido one_r
	printf("REAL: %s %d %d\n", one_w.name, one_w.x, one_w.y);
	printf("DE FICHERO: %s\n", buffer);
	
    return 0;
}