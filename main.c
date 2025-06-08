#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "graham.h"
#include "andrew.h"

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: main file-name");
        exit(EXIT_FAILURE);
    }

    char fname[256];
    strncpy(fname, argv[1], sizeof(fname) - 1);
    FILE* fp = fopen(fname, "r");

    int lines = 0;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp)) lines++;
    lines--;
    rewind(fp);
    fgets(buffer, sizeof(buffer), fp);

    Location** map = (Location**)calloc(lines, sizeof(Location*));
    if (map == NULL) {
        fprintf(stderr, "failed to assign memory in main.\n");
        exit(EXIT_FAILURE);
    }
    
    int len = 0;
    for (int i = 0; i < lines; i++) {
        fgets(buffer, sizeof(buffer), fp);
        buffer[strcspn(buffer, "\r\n")] = '\0';
        char* fields[4];
        char* token = strtok(buffer, ",");
        int j = 0;
        while (token && j < 4) {
            fields[j++] = token;
            token = strtok(NULL, ",");
        }

        if (j == 4) {
            map[i] = (Location*)calloc(1, sizeof(Location));
            if (map[i] == NULL) {
                fprintf(stderr, "failed to assign memory in main.\n");
                exit(EXIT_FAILURE);
            }

            map[i]->lat = atof(fields[2]);
            map[i]->lng = atof(fields[3]);
            len++;
        }
    }

    fclose(fp);
    
    printf("Graham algorithm start.\n");
    Outline* outline = Graham(map, len, "graham.csv");
    printf("Graham algorithm ended.\n");
    fp = fopen("graham_outline.csv", "w");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file in main.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "lat,lng\n");
    for (int i = 0; i < outline->len; i++) fprintf(fp, "%f,%f\n", outline->map[i]->lat, outline->map[i]->lng);
    fclose(fp);
    clearOutline(outline);

    printf("Andrew algorithm start.\n");
    outline = Andrew(map, len, "andrew.csv");
    printf("Graham algorithm start.\n");
    fp = fopen("andrew_outline.csv", "w");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file in main.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "lat,lng\n");
    for (int i = 0; i < outline->len; i++) fprintf(fp, "%f,%f\n", outline->map[i]->lat, outline->map[i]->lng);
    fclose(fp);
    clearOutline(outline);

    clearMap(map, len);

    return 0;
}


//eof