#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"


int main(int argc, char *argv[]) {
	char *value = "Chicken Dinner";

	unsigned int key = 1337;

	insert(key, value,strlen(value)+1);

	printf("value = %s\n", get(key));


}


