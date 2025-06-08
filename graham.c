#include "graham.h"

double baseLat, baseLng;
static unsigned long long compare = 0, copy = 0;

int compare_revpolar(const void *a, const void *b) {
	Location *l1, *l2;
	double angle1, angle2;
	l1 = *(Location**)a;
	l2 = *(Location**)b;
    compare++;
	if (l1->lng == l2->lng && l1->lat == l2->lat) return 0;
	angle1 = atan2(l1->lat - baseLat, l1->lng - baseLng);
	angle2 = atan2(l2->lat - baseLat, l2->lng - baseLng);
    compare++;
	if (angle1 > angle2) return -1;
    compare++;
	if (angle1 < angle2) return 1;
    compare++;
	if (l1->lat > l2->lat) return -1;
    compare++;
	if (l1->lat < l2->lat) return 1;
	return 0;
}

Outline* Graham(Location** map, int len, char* out) {
    clock_t start = clock();

    Location *curr = map[0];
    int lowest = 0;
    double lowestLat = curr->lat;
    for (int i = 0; i < len; i++, curr = map[i]) {
        compare++;
        if(curr->lat < lowestLat) {
            copy++;
            lowestLat = curr->lat;
            lowest = i;
        }
    }
    compare++;
    if (lowest != len - 1) {swapElement(map, lowest, len - 1); copy += 3;}

    baseLat = map[len - 1]->lat;
    baseLng = map[len-1]->lng;
    qsort(map, len - 1, sizeof(Location*), compare_revpolar);

    Node* header = newNode();
    int listLen = 0;
    insertLast(header, newNodeWithLocation(map[len - 2])); listLen++; copy++;
    insertLast(header, newNodeWithLocation(map[len - 1])); listLen++; copy++;

    for (int i = 0; i < len - 1; i++) {
        if (header->prev->location->lat == map[i]->lat && header->prev->location->lng == map[i]->lng) continue;
        compare++;
        while (counter_clockwise(header->prev->prev->location, header->prev->location, map[i])) {
            free(removeLast(header));
            listLen--;
            compare++;
        }
        insertLast(header, newNodeWithLocation(map[i])); listLen++; copy++;
    }
    free(removeFirst(header)); listLen--;

    clock_t end = clock();
    output_res(compare, copy, end - start, out);

    Outline* outline = newOutline(listLen);
    for (int i = 0; i < listLen; i++) {
        Node* first = removeFirst(header);
        outline->map[i] = first->location;
        outline->len++;
        free(first);
    }

    while (header->next != header) free(removeFirst(header));
    free(header);

    return outline;
}


//eof