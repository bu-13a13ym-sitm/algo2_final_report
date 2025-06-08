#include "utils.h"

void output_res(unsigned long long compare, unsigned long long copy, clock_t exe_time, char* out) {
    FILE* fp = fopen(out, "w");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file.");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "compare: %I64u\ncopy: %I64u\nexe_time: %f\n", compare, copy, (double)exe_time / CLOCKS_PER_SEC);

    fclose(fp);

    return;
}

int counter_clockwise(Location *l1, Location *l2, Location *l3) {
	return	(l2->lng - l1->lng) * (l3->lat - l1->lat) - 
		(l2->lat - l1->lat) * (l3->lng - l1->lng) > 0;
}

Location* newLocation(double lat, double lng) {
    Location* location = (Location*)calloc(1, sizeof(Location));
    if (location == NULL) {
        fprintf(stderr, "failed to assign memory in newLocation.\n");
        exit(EXIT_FAILURE);
    }

    location->lat = lat;
    location->lng = lng;

    return location;
}

void clearMap(Location** map, int len) {
    for (int i = 0; i < len; i++) if (map[i] != NULL) free(map[i]);

    return;
}

void swapElement(Location** map, int l1, int l2) {
    Location* tmp = map[l1];
    map[l1] = map[l2];
    map[l2] = tmp;

    return;
}

Node* newNode() {
    Node* node = (Node*)calloc(1, sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "failed to assign memory in initList.\n");
        exit(EXIT_FAILURE);
    }

    node->location = NULL;
    node->prev = node->next = node;

    return node;
}

Node* newNodeWithLocation(Location* location) {
    Node* node = newNode();
    node->location = location;

    return node;
}

void insertLast(Node* header, Node* node) {
    node->prev = header->prev;
    node->next = header;
    header->prev->next = node;
    header->prev = node;

    return;
}

Node* removeNode(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

Node* removeFirst(Node* header) {
    Node* first = header->next;
    header->next = first->next;
    first->next->prev = header;
    first->prev = NULL;
    first->next = NULL;

    return first;
}

Node* removeLast(Node* header) {
    Node* last = header->prev;
    header->prev = last->prev;
    last->prev->next = header;
    last->prev = NULL;
    last->next = NULL;

    return last;
}

Outline* newOutline(int len) {
    Outline* outline = (Outline*)calloc(1, sizeof(Outline));
    if (outline == NULL) {
        fprintf(stderr, "failed to assign memory in newOutline.\n");
        exit(EXIT_FAILURE);
    }

    outline->map = (Location**)calloc(len, sizeof(Location*));
    if (outline->map == NULL) {
        fprintf(stderr, "failed to assign memory in newOutline.\n");
        exit(EXIT_FAILURE);
    }

    outline->len = 0;

    return outline;
}

void clearOutline(Outline* outline) {
    if (outline->map != NULL) free(outline->map);
    if (outline != NULL) free(outline);

    return;
}