/**************************************************************************************************
** The triangle function calculates the area of a triangle using Heron's formula
** Parameters: pointers to the x, y, z variables
** Returns:  None.
** Citation:  https://en.wikipedia.org/wiki/Heron%27s_formula
**************************************************************************************************/

#include "triangle.h"  

void triangle(double *a, double *b, double *c) {

    double semiP = 0.5 * (*a + *b + *c);

    double semiPMinusA = semiP - *a;
    double semiPMinusB = semiP - *b;
    double semiPMinusC = semiP - *c;

    double area = sqrt(semiP * semiPMinusA * semiPMinusB * semiPMinusC);

    printf("The Area of the triangle is: %f\n", area);

}









