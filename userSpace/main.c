#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"


int main(int argc, char *argv[]) {
	char *value = "Chicken Dinner";
	unsigned int key = 1337;

	insert(key, value);

	printf("value = %d\n", get(key));


}


