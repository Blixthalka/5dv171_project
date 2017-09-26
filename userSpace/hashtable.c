//
// Created by Emil Vannebäck on 2017-09-26.
//

#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *create_entry(unsigned int key, char *value, size_t size);

int store(char *entry);

void insert(unsigned int key, char *value, size_t size) {

	char *entry = create_entry(key, value,size);
	store(entry);
}

char *create_entry(unsigned int key, char *value, size_t size) {
	char* entry = malloc(sizeof(unsigned int)+size);
	memcpy(entry, &key,sizeof(key));
	memcpy(entry+sizeof(key),value,size);
	return entry;
}

int store(char *entry) {
	FILE *fp;

	fp = fopen("/sys/kernel/key_value_store/test", "wb");
	if (!fp) {
		fprintf(stderr, "can not open file");
		return -1;
	}

	fwrite(entry, sizeof(entry), 1, fp);
	fclose(fp);
	return 1;
}

char *get(unsigned int key) {
	FILE *fp;
	char *value = (char *) malloc(1000);
	memcpy(value,&key,sizeof(key));

	unsigned int test_key;
	memcpy(&test_key, value,sizeof(test_key));
	fprintf(stderr, "getting key %u\n",test_key);
	fp = fopen("/sys/kernel/key_value_store/test", "rb");
	if (!fp) {
		fprintf(stderr, "can not open file\n");
	}
	fread(value, 1000, 1, fp);
	fclose(fp);

	return value;

}
