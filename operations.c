/*****************************************************************************
* Purpose: Provides a set of functions to facilitate distance handling and
*          other useful operations.
* Last changed: 11/03/2025
*****************************************************************************/
#include "operations.h"

/*****************************************************************************
* Purpose: Makes a copy of Coordinates.
* Parameters: in: src = Coordinates to copy.
*             out: dest = copy of src.
* Return: ----
*****************************************************************************/
void OP_copyCoords(Coordinates src, Coordinates *dest) {
	dest = &src;
}

/*****************************************************************************
* Purpose: Calculates the distance in km between two points based on their
*          coordinates. You can assume there are NO errors in this function.
* Parameters: in: a = Coordinates of one point.
*             in: b = Coordinates of the other point.
* Return: Returns the distance in km between a and b.
*****************************************************************************/
long calculateDistance(Coordinates a, Coordinates b) {
	double rad_a = a.latitude * M_PI / 180, rad_b = b.latitude * M_PI / 180;
	double d_lat = (b.latitude - a.latitude) * M_PI / 180;
	double d_lon = (b.longitude - a.longitude) * M_PI / 180;
	double h = 0.0, c = 0.0;
    long d = 0.0;

	h = sin(d_lat / 2) * sin(d_lat / 2) + cos(rad_a) * cos(rad_b) * sin(d_lon / 2) * sin(d_lon / 2);
	c = 2 * atan2(sqrt(h), sqrt(1 - h));
	c /= 1000;
	d = 6371 * c;

	return (d);
}

/*****************************************************************************
* Purpose: Find the nearest item between two items based on a reference point.
* Parameters: in: id_a = int representing the ID of the first item to check.
*             in: coords_a = Coordinates of the first item.
*			  in: id_b = int representing the ID of the second item to check.
*			  in: coords_b = Coordinates of the second item.
*			  in: point = Coordinates of the reference point.
* Return: Returns the ID of the nearest item to the reference point.
*****************************************************************************/
int OP_findNearest(int id_a, Coordinates coords_a, int id_b, Coordinates coords_b, Coordinates point) {
	if (calculateDistance(coords_a, point) < calculateDistance(coords_b, point)) {
	    return (id_b);
	} else {
	    return (id_a);
	}
}

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
int * OP_findInRadius(int *n_found, int *ids, Coordinates *coords, int n_items, Coordinates point, float radius) {
    int *found = NULL;
	int i = 0, j = 0;
	long aux = 0;

	for (i = 0; i < n_items; i++) {
	    aux = calculateDistance(coords[i], point);

		if (radius >= aux) {
			found = (int *) realloc (found, sizeof(int) * (j + 1));
			found[j] = ids[i];
			j++;
		}
	}

	*n_found = i;

	return (found);
}

/*****************************************************************************
* Purpose: Swaps two integers.
* Parameters: in/out: a = an integer.
*             in/out: b = another integer.
* Return: ----
*****************************************************************************/
void swapInt(int *a, int *b) {
    int tmp = *a;

	*a = *b;
	*b = tmp;
}

/*****************************************************************************
* Purpose: Swaps two strings.
* Parameters: in/out: a = a string.
*             in/out: b = another string.
* Return: Returns 1 if there was an error allocating memory, otherwise 0.
*****************************************************************************/
char swapStr(char *a, char *b) {
	char *tmp = (char *) malloc(sizeof(char) * (strlen(a) + 1));

	if (NULL == tmp) {
		return (1); // Error allocating memory
	}
				
	strcpy(tmp, b);
	a = (char *) realloc(a, sizeof(char) * (strlen(b) + 1));

	if (NULL == a) {
		return (1); // Error allocating memory
	}

	strcpy(a, b);
	b = (char *) realloc(b, sizeof(char) * (strlen(a) + 1));
	
	if (NULL == b) {
		return (1); // Error allocating memory
	}
	
	strcpy(b, tmp);

	return (0);
}

/*****************************************************************************
* Purpose: Sorts strings in ascending alphabetical order.
* Parameters: in/out: strings = array of strings to sort.
*             in: n = int representing the number of strings in the array.
* Return: Returns an array of integers representing the new positions of the
*         sorted strings compared to the original positions.
*****************************************************************************/
int * sortAlphabeticallyAsc(char **strings, int n) {
    int *positions = (int *) malloc(sizeof(int) * n);
	char swap = 1;
	int i = 0;
	
	while (i < n) {
	    positions[i] = i;
		i++;
	}

	while (swap) {
		swap = 0;
		
		while (i < n) {
		    if (0 < strcmp(strings[i], strings[i - 1])) {
				if (swapStr(strings[i - 1], strings[i])) {
				    free(positions);
					return (NULL);
				}

				swapInt(&positions[i - 1], &positions[i]);
				swap = 1;
			}
		}

		i++;
	}

	return (positions);
}

/*****************************************************************************
* Purpose: Sorts strings in descending alphabetical order.
* Parameters: in/out: strings = array of strings to sort.
*             in: n = int representing the number of strings in the array.
* Return: Returns an array of integers representing the new positions of the
*         sorted strings compared to the original positions.
*****************************************************************************/
int * sortAlphabeticallyDesc(char **strings, int n) {
    int *positions = (int *) malloc(sizeof(int) * n);
	char swap = 1;
	int i = 0;
	
	while (i < n) {
	    positions[i] = i;
		i++;
	}

	while (swap) {
		swap = 0;
		
		while (i < n) {
		    if (0 > strcmp(strings[i], strings[i - 1])) {
				if (swapStr(strings[i - 1], strings[i])) {
				    free(positions);
					return (NULL);
				}

				swapInt(&positions[i - 1], &positions[i]);
				swap = 1;
			}
		}

		i++;
	}
	
	return (positions);
}

/*****************************************************************************
* Purpose: Sorts strings in either ascending or descending alphabetical order.
* Parameters: in/out: strings = list of strings to sort.
*             in: n_strings = number of strings to sort.
*             in: asc = boolean to indicate whether to sort in ascending (1)
*			      or descending (0) order.
* Return: A list of integers containing the old positions of the sorted list
*         of strings in the new positions.
*****************************************************************************/
int * OP_sortAlphabetically(char **strings, int n_strings, char asc) {
    int *positions = NULL;

	if (asc) {
	    positions = sortAlphabeticallyAsc(strings, n_strings);
	} else {
	    positions = sortAlphabeticallyDesc(strings, n_strings);
	}

	return (positions);
}

/*****************************************************************************
* Purpose: Checks whether an integer value is in the specified range.
* Parameters: in: value = int representing the value to check.
*             in: low = int representing the lower limit of the range.
*			  in: high = int representing the upper limit of the range.
*			  in: include = boolean to indicate whether to include (1) the
*			      range's limits or not (0).
* Return: Returns 1 if the value is in range, otherwise 0.
*****************************************************************************/
char OP_isInRange(int value, int low, int high, char include) {
    if (include) {
	    return (low > value || high < value);
	} else {
	    return (low >= value || high <= value);
	}
}

/*****************************************************************************
* Purpose: Calculates the average rating of a set of 1 to 5 stars ratings.
*          Rates are represented with '*'.
* Parameters: in: ratings = array of strings with the ratings.
*             in: n_ratings = int representing the total number of ratings.
* Return: Returns a float with the average rating.
*****************************************************************************/
float OP_calculateAvgRating(char **ratings, int n_ratings) {
    float avg = 0.0;
	int i = 0, j = 0;

	for (i = 0; i < n_ratings; i++) {
	    while ('\0' != ratings[j][i]) {
		    if ('*' == ratings[j][i]) {
			    avg++;
			}
			
			j++;
		}
	}

	return (avg);
}
