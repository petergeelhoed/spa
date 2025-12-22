#define _POSIX_C_SOURCE 200809L
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "libmyazi.h"
const int SECS_IN_DAY = 86400;
// Function to print usage information
void print_usage()
{
    printf("usage: date +%%s | azid\n\n-n <lat>\n-e <long>\n-t <gmtoffset "
           "seconds>\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{

    time_t t = time(NULL);
    struct tm lt = {0};
    localtime_r(&t, &lt);
    int gmtoff = lt.tm_gmtoff;

    float lng = 4.687;
    float lat = 51.836;

    // Panel angles
    float panazi = 210 / RADPI;
    float panzen = 10 / RADPI;

    double px = sin(panzen) * cos(panazi);
    double py = sin(panzen) * sin(panazi);
    double pz = cos(panzen);
    normalize_vector(&px, &py, &pz);

    double epoch = 0.0;
    int c;
    while ((c = getopt(argc, argv, "n:e:t:h")) != -1)
    {
        switch (c)
        {
        case 'e':
            lng = atof(optarg);
            break;
        case 't':
            gmtoff = atoi(optarg);
            break;
        case 'n':
            lat = atof(optarg);
            break;
        case 'h':
        default:
            print_usage();
        }
    }

    while (fscanf(stdin, "%lf", &epoch) != EOF)
    {
        struct azizen azi = {epoch, lng, lat, 0.0, 0.0, 0.0, 0.0};
        calcazi(&azi);

        printf("%lf %lf %lf %lf \n",
               fmod(azi.secofday + gmtoff, SECS_IN_DAY),
               azi.zenith,
               azi.azimuth,
               azi.cos);
    }
    return 0;
}
