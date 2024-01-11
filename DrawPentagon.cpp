#include "DrawPentagon.h"

float DrawPentagon::toRadians(float angle) {
    return angle * (M_PI/180) ;
}

/*
    Uses the polar coordinate system to draw 5 points with the coordinates 
    (side_length, 72 deg * CurrentPoint)
    then converts it into Cartesian
*/

void DrawPentagon::draw(float side_length)
{

    glBegin(GL_POLYGON);

    float x, y;

    for (int i = 1; i <= 5; i++) {

        float angle = toRadians(72 * i);

        x = side_length * cos(angle);
        y = side_length * sin(angle);

        glVertex2f(x, y);

    }
    
    glEnd();

}