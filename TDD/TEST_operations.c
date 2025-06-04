#include "../lib/operations.h"
#include <stdio.h>

void TEST_OP_copyCoords()
{
    Coordinates src = {41.38879, 2.15899};
    Coordinates dest = {0.0, 0.0};
    OP_copyCoords(src, &dest);
    if (src.latitude == dest.latitude && src.longitude == dest.longitude)
        printf("TEST_OP_copyCoords: PASSED\n");
    else
        printf("TEST_OP_copyCoords: FAILED\n");
}

void TEST_OP_findNearest()
{
    Coordinates a = {41.38879, 2.15899};
    Coordinates b = {40.41678, -3.70379};
    Coordinates position1 = {41.0, 2.0};
    Coordinates position2 = {40.5, -3.5};
    int id = OP_findNearest(1, a, 2, b, position1);
    if (id == 1)
        printf("TEST_OP_findNearest: PASSED\n");
    else
        printf("TEST_OP_findNearest: FAILED (got %d, expected 1)\n", id);

    id = OP_findNearest(1, a, 2, b, position2);
    if (id == 2)
        printf("TEST_OP_findNearest: PASSED\n");
    else
        printf("TEST_OP_findNearest: FAILED (got %d, expected 2)\n", id);
}

void TEST_OP_findInRadius()
{
    int ids[3] = {1, 2, 3};
    Coordinates coords[3] = {
        {41.38879, 2.15899},
        {40.41678, -3.70379},
        {41.0, 2.0}
    };
    Coordinates position1 = {41.0, 2.0};
    Coordinates position2 = {40.5, -3.5};
    int n_found = 0;
    int *found = OP_findInRadius(&n_found, ids, coords, 3, position1, 50.0);
    if (found && n_found >= 1 && found[0] == 1)
        printf("TEST_OP_findInRadius: PASSED\n");
    else
        printf("TEST_OP_findInRadius: FAILED\n");
    free(found);

    found = OP_findInRadius(&n_found, ids, coords, 3, position1, 10000);
    if (found && n_found == 3)
        printf("TEST_OP_findInRadius: PASSED\n");
    else
        printf("TEST_OP_findInRadius: FAILED\n");
    free(found);

    found = OP_findInRadius(&n_found, ids, coords, 3, position2, 50.0);
    if (found && n_found >= 1 && found[0] == 2)
        printf("TEST_OP_findInRadius: PASSED\n");
    else
        printf("TEST_OP_findInRadius: FAILED\n");
    free(found);
}

void TEST_OP_sortAlphabetically()
{
    char *arr[3];
    arr[0] = strdup("banana");
    arr[1] = strdup("apple");
    arr[2] = strdup("cherry");

    int *pos = OP_sortAlphabetically(arr, 3, 1);
    printf("Sorting ascending: %s, %s, %s\n", arr[0], arr[1], arr[2]);
    free(pos);

    pos = OP_sortAlphabetically(arr, 3, 0);
    printf("Sorting descending: %s, %s, %s\n", arr[0], arr[1], arr[2]);
    free(pos);
    
    free(arr[0]);
    free(arr[1]);
    free(arr[2]);
}

void TEST_OP_isInRange()
{
    if (OP_isInRange(5, 1, 10, 1) && !OP_isInRange(0, 1, 10, 0) && OP_isInRange(1, 1, 10, 1) && !OP_isInRange(10, 1, 10, 0))
        printf("TEST_OP_isInRange: PASSED\n");
    else
        printf("TEST_OP_isInRange: FAILED\n");
}

void TEST_OP_calculateAvgRating()
{
    char *ratings[3] = {"***", "****", "**"};
    float avg = OP_calculateAvgRating(ratings, 3);
    if (avg == 3.0)
        printf("TEST_OP_calculateAvgRating: PASSED\n");
    else
        printf("TEST_OP_calculateAvgRating: FAILED (got %.2f, expected 3.00)\n", avg);
}

int main()
{
    TEST_OP_copyCoords();
    TEST_OP_findNearest();
    TEST_OP_findInRadius();
    TEST_OP_sortAlphabetically();
    TEST_OP_isInRange();
    TEST_OP_calculateAvgRating();
    return 0;
}