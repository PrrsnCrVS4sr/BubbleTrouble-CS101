#include <simplecpp>

/* Bubble Vars */
const int BUBBLE_START_X = 250;
const int BUBBLE_START_Y = 50;
const int BUBBLE_DEFAULT_RADIUS = 10;
const int BUBBLE_RADIUS_THRESHOLD = 10;
const int BUBBLE_DEFAULT_VX = 500;
const int BUBBLE_DEFAULT_VY = 0;
const int gravity = 40;
class Bubble
{
private:
    Circle circle;  // the circle representing the bubble
    double vx, vy;  // velocity in x and y direction
    Color color;    // color of the bubble
    double minRadius = BUBBLE_DEFAULT_RADIUS;

public:
    Bubble(double cx, double cy, double r, double vx_=BUBBLE_DEFAULT_VX, double vy_=BUBBLE_DEFAULT_VY, Color color_=COLOR(0, 0, 255))
    {
        // Bubble constructor
        color = color_;
        circle = Circle(cx, cy, r);
        circle.setColor(color);
        circle.setFill(true);
        vx = vx_;
        vy = vy_;
    }

    void nextStep(double t)
    {
        // move the bubble
        vy = get_vy() + (gravity*t); // updates y velocity due to gravity
        double new_x = circle.getX() + vx*t;
        double new_y = circle.getY() + vy*t;

        if ((vx < 0 && new_x < (0 + circle.getRadius())) // bounce along X direction at left border
            ||
            (vx > 0 && new_x > (WINDOW_X - circle.getRadius()))) // bounce along X direction at right border
        {

            vx = -vx;
            new_x = circle.getX() + vx*t;

        }
        if ((vy > 0 && new_y > ((WINDOW_Y-50) - circle.getRadius()))) // bounce along Y direction at bottom
        {

            vy = -vy;
            new_y = circle.getY() + vy*t;

        }

        circle.moveTo(new_x, new_y);
    }

    //Splits the bubble into two smaller bubbles
    vector<Bubble> Split()
    {   vector<Bubble> newBubbles;
        if(get_radius()>minRadius){

            newBubbles.push_back(Bubble(get_center_x(), get_center_y(), get_radius()/2, -BUBBLE_DEFAULT_VX*1.2, vy, COLOR(255,105,180)));
            newBubbles.push_back(Bubble(get_center_x(), get_center_y(), get_radius()/2, BUBBLE_DEFAULT_VX*1.2, vy, COLOR(255,105,180)));
            for(int i =0; i<2;i++)
            {
                newBubbles[i].setColour();

            }
            return newBubbles;
        }
        return newBubbles;
    }


    void setColour()
    {
        if(get_radius() == 2*BUBBLE_DEFAULT_RADIUS)
        {
            circle.setColor(COLOR(216,191,216));
        }
        if(get_radius()== BUBBLE_DEFAULT_RADIUS)
        {

            circle.setColor(COLOR(255,105,180));
        }

    }
    double get_radius()
    {
        // return the radius of the bubble
        return circle.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bubble
        return circle.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bubble
        return circle.getY();
    }

    double get_vx()
    {
        // return the x velocity of the bubble
        return vx;
    }
    double get_vy()
    {
        // return the y velocity of the bubble
        return vy;
    }
    double get_min_radius()
    {
        return minRadius;

    }
    Color get_color()
    {
        // return the color of the bubble
        return color;
    }
};
