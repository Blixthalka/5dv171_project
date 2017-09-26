//
// Created by Emil Vannebäck on 2017-09-26.
//

#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>


char* create_entry(unsigned  int key, char* value);
int store(char* entry);

void insert(unsigned int key, char* value){

    char* entry = create_entry(key, value);
    store(entry);
}

char* create_entry(unsigned int key, char* value){
   return "test";
}

int store(char* entry){
    FILE *fp;

    fp = fopen("/sys/kernel/key_value_store/test","wb");
    if(!fp){
        fprintf(stderr, "can not open file");
        return -1;
    }

    fwrite(entry, sizeof(entry), 1, fp);

}

char* get(unsigned int key) {
    FILE *fp;
    char* value = (char*) malloc(1000);
    if(!fp){
        fprintf(stderr, "can not open file");
    }

    fp = fopen("/sys/kernel/key_value_store/test","rb");

    fread(value,sizeof(value),1,fp);

    return value;

}