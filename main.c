/**************************************************************************************************
** Program:       The triangle program calculates the area of a triangle
** Author:        Roger Hill
** Date:          1/09/2020
** Description:   the triangle function that calculates the area of a triangle using the
**                Heron formula
**************************************************************************************************/

#include <stdio.h>
#include "triangle.h"  // fixed with including the file in double quotes.

int main() {

    double x = 5.0;
    double y = 6.0;
    double z = 7.0;

    triangle(&x, &y, &z);

  return 0;
}
