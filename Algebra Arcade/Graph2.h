#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "gcalc.h"
#include <cstdlib>
#include <ctime>

using namespace sf;

/// sets the windows 720 pixels wide and 560  pixels long
/// starts from the upper left and then down and to the right
unsigned wWidth = 720, wHeight = 560, margin = 60;

template <typename T>
std::string tostr(const T& t) {
   std::ostringstream os;
   os<<t;
   return os.str();
}

void initXVals(float& xstart, float& xstop, float& xmesh) {
    std::cout << "\nEnter a starting value for x: ";
    std::cin >> xstart;
    std::cout << "\nEnter a stopping value for x: ";
    std::cin >> xstop;
    std::cout << "\nEnter the number of function values: ";
    std::cin >> xmesh;
}

std::string getFunction() {
    std::string fstr;
    //std::cout << "\nEnter the function string you want to evaluate in terms of x: ";
    getline(std::cin,fstr);
    fstr += ";";
    //std::cout << "\nfstr = " << fstr;
    return fstr;
}
//------------------------------------------------------------------------------
// Function curve
// plot f() in the range [0:100) with (0,0) at (20,150)
// using 1000 points; scale x values *50, scale y values *50

class Function : public Drawable {
public:
    Function( float begiN, float enD, Vector2f s, const int n, float xS,
              float yS, std::string str);
    Vertex* vertices;// = new Vertex[number_of_points];
    void draw(RenderTarget& target, RenderStates states) const;
    int getNoP() { return number_of_points; };
    ~Function() { free(vertices); vertices = NULL; } // DON'T FORGET TO FREE UP SPACE!
private:
    float startx;
    float stopx;
    Vector2f start;
    const int number_of_points;
    float xscale;
    float yscale;
};

Function::Function(float begiN, float enD, Vector2f s, const int n,
                   float xS, float yS, std::string fstrg)
  : startx(begiN), stopx(enD), start(s), number_of_points(n+1),
    xscale(xS), yscale(yS)
{
    vertices = new Vertex[number_of_points+1];
    float deltaX = 10./(number_of_points-1), x = -5, xg = startx, y{0};
    float deltaXg =  (stopx-startx)/(number_of_points-1);
    //initialize x as xstart in the names table.
    std::string xstring = "let x = " + tostr(-5.) + ";";
    //calc(xstring);
    std::cout << std::endl << "x = " << calc(xstring);

    //std::string fstrg = getFunction();
    //std::cout << "\ncalc(fstrg) = " << calc(fstrg);
    std::cout << fstrg << '\n';
    vertices[0] = Vertex(Vector2f(start.x,start.y-yscale*calc(fstrg)));

    //loop with increments
    //std::cout << "\nnumber of points = " <<number_of_points << '\n';
    for(int i = 1; i <= number_of_points; ++i) {
        //std::cout << "\ni = " << i << '\n';
        x += deltaX;
        xstring = "x = " + tostr(x) + ";";
        //std::cout<<"\nxstring = " << xstring << '\n';
        calc(xstring);
        //std::cout << "x = " << calc(xstring) << '\n';
        xg += deltaXg;

        y = calc(fstrg);
        //std::cout << "y = " << y << '\n' << '\n';
        vertices[i] = Vertex(Vector2f(xg,start.y-yscale*y));
        //std::cout << "\nvertices[" << i << "].position.x = " << vertices[i].position.x << ", "
        //          << "vertices[" << i << "].position.y = " << vertices[i].position.y << '\n';
    }
}

void Function::draw(RenderTarget& target, RenderStates states) const {
    target.draw(vertices, number_of_points, LinesStrip, states);
}

//------------------------------------------------------------------------------

class Axis : public Drawable {
public:

    Axis(int d, Vertex xy, int l, int n=0);
    //std::vector<Vertex> vertices;
    Vertex vertices[22];

    void draw(RenderTarget& target, RenderStates states) const;
private:
    //enum Orientation { x, y };
    Vertex start;
    int length;
    int number_of_notches;

};

Axis::Axis(int d, Vertex xy, int l, int n)
{
    switch (d)
    {
    case 0:  //Axis::x:
    {
        xy.color= Color(255,255, 255);
        vertices[0] = xy;
        vertices[1] = Vertex(Vector2f(xy.position.x+l,xy.position.y),Color(255,255,255));
        // axis line
        if (1<n)  // add notches
        {
            int deltaX = l/n;
            int x = xy.position.x+deltaX;  // x-coord of first notch
            for (int i = 0; i<2*n; i+=2)
            {
                vertices[2+i] =  Vertex(Vector2f(x,xy.position.y),Color(255,255,255));
                vertices [3+i] = Vertex(Vector2f(x,xy.position.y-10),Color(255,255,255));
                x += deltaX;
            }
        }
//        // label under the line
//        label.move(length/3,xy.y+20);
        break;
    }
    case 1:  //Axis::y:
    {
        xy.color= Color(255,255,255);
        vertices[0] = xy;
        vertices[1] = Vertex(Vector2f(xy.position.x,xy.position.y-l),Color(255,255,255));

        if (1<n)        // add notches
        {
            int deltaY = l/n;
            int y = xy.position.y-deltaY;  // x-coord of first notch
            for (int i = 0; i<2*n; i+=2)
            {
                vertices[2+i] = Vertex(Vector2f(xy.position.x,y),Color(255,255,255));
                vertices[3+i] = Vertex(Vector2f(xy.position.x+10,y),Color(255,255,255));
                y -= deltaY;
            }
        }
        break;
    }
    }
}

void Axis::draw(RenderTarget& target, RenderStates states) const {
    target.draw(vertices, 22, Lines, states);
}
