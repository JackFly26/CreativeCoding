#include "function.h"
 
uint8_t r[HEIGHT][WIDTH], g[HEIGHT][WIDTH], b[HEIGHT][WIDTH];
long double zoom = 1.0;

void
draw_frame(int framecount)
{
    int i;
    long double cx, cy, zx, zy, zx2, zy2;
    long double xmin = -2.25 * zoom;
    long double xmax = 2.25 * zoom;
    long double ymin = -2.25 * zoom;
    long double ymax = 2.25 * zoom;
    long double dx = (xmax - xmin) / WIDTH;
    long double dy = (ymax - ymin) / HEIGHT;

    zoom *= 0.975;

    //long double centerx = 0;
    //long double centery = 0;
    //long double centerx = -1.76651223650935682;
    //long double centery = 0.04171432271082462;
    //long double centerx = -0.7473354651820724;
    //long double centery = -0.10030992828748005;
    //long double centerx = -1.256148879595297;
    //long double centery = 0.3994106258544796;
    long double centerx = -1.76651223950935682;
    long double centery = 0.04171432271082462;
    //long double centerx = -1.77810334274064037110522326038852639499207961414628307584575173232969154440;
    //long double centery = 0.00767394242121339392672671947893471774958985018535019684946671264012302378;

    for (int y = 0; y < HEIGHT; y++)
    {
        cy = centery + ymax - y * dy;
        for (int x = 0; x < WIDTH; x++)
        {
            cx = centerx + xmin + x * dx;
            zx = 0.0;
            zy = 0.0;
            zx2 = zx * zx;
            zy2 = zy * zy;

            for (i = 0; i < ITERATIONMAX && ((zx2 + zy2) < ESCAPE); i++)
            {
                zy = 2 * zx * zy + cy;
                zx = zx2 - zy2 + cx;
                zx2 = zx * zx;
                zy2 = zy * zy;
            }
            if (i == ITERATIONMAX)
            {
                r[y][x] = 0;
                g[y][x] = 0;
                b[y][x] = 0;
            }
            else 
            {
                //double z = zx + zy;
                //int smooth = 360 * log2(1.75 + i - log2(log2(z))) / log2((double)ITERATIONMAX);
                //smooth = (smooth + framecount) % 360;
                
                int output = (i * 5) + framecount % 360;

                struct HSV data = { output, 0.75, 0.75 };
                struct RGB value = HSVToRGB(data);

                //fprintf( stderr,  "%d\n\n", smooth);

                r[y][x] = value.R;
                g[y][x] = value.G;
                b[y][x] = value.B;

            }
        }
    }
}

void
output_frame(void)
{
    fprintf (stdout, "P6 %d %d %d\n", WIDTH, HEIGHT, 255);
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            fwrite (&r[y][x], sizeof(uint8_t), 1, stdout);
            fwrite (&g[y][x], sizeof(uint8_t), 1, stdout);
            fwrite (&b[y][x], sizeof(uint8_t), 1, stdout);
        }
    }
    fflush(stdout);
}
 
int
main()
{
    for (int i = 0; i < 500; i++)
    {
        draw_frame(i);
        output_frame();
    }
}