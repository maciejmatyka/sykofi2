#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>

using namespace std;

const double xmin = -2.25;
const double xmax = 2.15;//15;
const double ymin = 1;
const double ymax = 6;
const double dx = (xmax-xmin);
const double dy = (ymax-ymin);

const int W=int(dx*280);
const int H=int(dy*280);

int pixels[W][H]={0};


//vector<float> X;
//vector<float> Y;

void plot(double x, double y)
{
    int mx = int((x+dx/2)*W/dx);        // -1.25 : 1.25
    int my = int((y)*H/dy);                // 0 : 6
    if(mx<W&&mx>0&&my>0&&my<H)
    {
        pixels[ mx ][ my ] = 1;
    }

}

void saveppm()
{
    ofstream file("pix.ppm");
    int c;
    file << "P3" << endl;
    file << "# dkfjsdljf" << endl;
    file << W << " " << H << endl;
    file << 255 << endl;

    for(int j=0; j<H; j++)
    {
        for(int i=0; i<W; i++)

        {
            c = 255-pixels[i][H-j] * 255;
            file << c << " " << c << " " << c << " ";
        }
        file << endl;
    }

    file.close();
}

// two stressed pilots
int main()
{
    double x,y;
    double x0,y0;


    mt19937 mt_rand(time(0));


    srand(time(NULL));

    // J. Kudrewicz, p. 24
    // a11 a12 a21 a22 c1 c2 s J
    // choinka
    /*double a1[4][8]=    {    {-0.67, -0.02, -0.18, 0.81, 0.00, 1.02, 0.8530, 0.546},
                        { 0.40, 0.40, -0.10, 0.40, -0.04, 0.06, 0.6217, 0.200},
                        {-0.40, -0.40, -0.10, 0.40, 0.04, 0.06, 0.6217, 0.200},
                        {-0.10, 0.00, 0.44, 0.44, 0.00, -0.14, 0.6263, 0.044}
                    };*/
    // paproc
    double a1[4][8]=    {    {0.240, -0.007, 0.007, 0.015, 0.000, 0, 0.004},
                        {0.220, -0.330, 0.360, 0.100, 0.540, 0, 0.149},
                        {0.140, -0.360, -0.380, -0.100, 1.400, 0, 0.160},
                        {0.800, 0.100, -0.100, 0.800, 1.600, 0, 0.687}
                    };

    // iterate
    x0=0.1;
    y0=0.6;

    long int step=0;
    int i;
    const long int ITERATIONS = 1000000000;

    //cout << mt_rand() << endl;
    //return 0;

    while(step++ < ITERATIONS)
    {
        //i = rand()%4;       // 0-3
        i = mt_rand()%4;
        x = a1[i][0] * x0 + a1[i][1] * y0 + a1[i][4];
        y = a1[i][2] * x0 + a1[i][3] * y0 + a1[i][5];
        plot(x,y);
        x0=x; y0=y;

        if(!(step % (ITERATIONS/50)))
            cout << "*";

    }
    cout << step << endl;
    saveppm();

    return 0;
}