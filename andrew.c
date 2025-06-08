#include "andrew.h"

static unsigned long long compare = 0, copy = 0;

int compare_xycoord(const void* a, const void* b) {
    Location *l1, *l2;
    l1 = *(Location**)a;
    l2 = *(Location**)b;
    compare++;
    if (l1->lng < l2->lng) return -1;
    compare++;
    if (l1->lng > l2->lng) return 1;
    compare++;
    if (l1->lat <l2->lat) return -1;
    compare++;
    if (l1->lat > l2->lat) return 1;
    return 0;
}

Outline* Andrew(Location** map, int len, char* out) {
    clock_t start = clock();

    qsort(map, len, sizeof(Location*), compare_xycoord);

    Node* header = newNode();
    int listLen = 0;
    insertLast(header, newNodeWithLocation(map[0])); listLen++; copy++;
    insertLast(header, newNodeWithLocation(map[1])); listLen++; copy++;
    for (int i = 2; i < len; i++) {
        if (header->prev->location->lat == map[i]->lat && header->prev->location->lng == map[i]->lng) continue;
        insertLast(header, newNodeWithLocation(map[i])); listLen++; copy++;
        compare++;
        while (listLen >= 3 && counter_clockwise(header->prev->prev->prev->location, header->prev->prev->location, header->prev->location)) {
            free(removeNode(header->prev->prev));
            listLen--;
            compare++;
        }
    }

    insertLast(header, newNodeWithLocation(map[len - 2])); listLen++; copy++;
    for (int i = len -3; i >= 0; i--) {
        if (header->prev->location->lat == map[i]->lat && header->prev->location->lng == map[i]->lng) continue;
        insertLast(header, newNodeWithLocation(map[i])); listLen++; copy++;
        compare++;
        while (listLen >= 3 && counter_clockwise(header->prev->prev->prev->location, header->prev->prev->location, header->prev->location)) {
            free(removeNode(header->prev->prev));
            listLen--;
            compare++;
        }
    }
    free(removeLast(header)); listLen--;

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