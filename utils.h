#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    double lat, lng;
} Location;

typedef struct _Node Node;
typedef struct _Node {
    Location* location;
    Node *prev, *next;
} Node;

typedef struct {
    Location** map;
    int len;
} Outline;

void output_res(unsigned long long, unsigned long long, clock_t, char*);
int counter_clockwise(Location*, Location*, Location*);
Location* newLocation(double, double);
void clearMap(Location**, int);
void swapElement(Location**, int, int);
Node* newNode();
Node* newNodeWithLocation(Location*);
void insertLast(Node*, Node*);
Node* removeNode(Node*);
Node* removeFirst(Node*);
Node* removeLast(Node*);
Outline* newOutline(int);
void clearOutline(Outline*);

#endif