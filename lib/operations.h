/*****************************************************************************
* Purpose: Provides a set of functions to facilitate distance handling and
*          other useful operations.
* Last changed: 11/03/2025
* TODO: Add '-lm' when compiling as the last argument of each command in the
*       makefile.
*****************************************************************************/
#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    float latitude;
	float longitude;
} Coordinates;

/*****************************************************************************
* Purpose: Makes a copy of Coordinates.
* Parameters: in: src = Coordinates to copy.
*             out: dest = copy of src.
* Return: ----
*****************************************************************************/
void OP_copyCoords(Coordinates src, Coordinates *dest);

/*****************************************************************************
* Purpose: Find the nearest item between two items based on a reference point.
* Parameters: in: id_a = int representing the ID of the first item to check.
*             in: coords_a = Coordinates of the first item.
*			  in: id_b = int representing the ID of the second item to check.
*			  in: coords_b = Coordinates of the second item.
*			  in: point = Coordinates of the reference point.
* Return: Returns the ID of the nearest item to the reference point.
*****************************************************************************/
int OP_findNearest(int id_a, Coordinates coords_a, int id_b, Coordinates coords_b, Coordinates point);

/*****************************************************************************
* Purpose: Find the items within radius of a reference point.
* Parameters: out: n_found = number of items found.
*             in: ids = list of IDs of the items to find.
*             in: coords = list of Coordinates of the items to find.
*             in: n_items = number of total items.
*			  in: point = Coordinates of the reference point for the search.
*			  in: radius = float representing the radius from the point in km.
* Return: Returns a list of IDs with the items whithin range.
*****************************************************************************/
int * OP_findInRadius(int *n_found, int *ids, Coordinates *coords, int n_items, Coordinates point, float radius);

/*****************************************************************************
* Purpose: Sorts strings in either ascending or descending alphabetical order.
* Parameters: in/out: strings = list of strings to sort.
*             in: n_strings = number of strings to sort.
*             in: asc = boolean to indicate whether to sort in ascending (1)
*			      or descending (0) order.
* Return: A list of integers containing the old positions of the sorted list
*         of strings in the new positions.
*****************************************************************************/
int * OP_sortAlphabetically(char **strings, int n_strings, char asc);

/*****************************************************************************
* Purpose: Checks whether an integer value is in the specified range.
* Parameters: in: value = int representing the value to check.
*             in: low = int representing the lower limit of the range.
*			  in: high = int representing the upper limit of the range.
*			  in: include = boolean to indicate whether to include (1) the
*			      range's limits or not (0).
* Return: Returns 1 if the value is in range, otherwise 0.
*****************************************************************************/
char OP_isInRange(int value, int low, int high, char include);

/*****************************************************************************
* Purpose: Calculates the average rating of a set of 1 to 5 stars ratings.
*          Rates are represented with '*'.
* Parameters: in: ratings = array of strings with the ratings.
*             in: n_ratings = int representing the total number of ratings.
* Return: Returns a float with the average rating.
*****************************************************************************/
float OP_calculateAvgRating(char **ratings, int n_ratings);

#endif
