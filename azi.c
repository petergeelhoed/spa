#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define RADPI 57.29577951308237993927

// Function to calculate the length of a vector
double calculate_length(double x, double y, double z)
{
    return sqrt(x * x + y * y + z * z);
}

// Function to normalize a vector
void normalize_vector(double* x, double* y, double* z)
{
    double length = calculate_length(*x, *y, *z);
    *x /= length;
    *y /= length;
    *z /= length;
}

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
        double julian = epoch / 86400.;
        double secofday = fmod(epoch, 86400);
        double julday = julian + 25569 + 2415018.5;
        double julcent = (julday - 2451545) / 36525;
        double geomMeanLong =
            fmod((280.46646 + julcent * (36000.76983 + julcent * 0.0003032)),
                 360) /
            RADPI;
        double geomMeanAnom =
            (357.52911 + julcent * (35999.05029 - 0.0001537 * julcent)) / RADPI;
        double eccentEarth =
            0.016708634 - julcent * (0.000042037 + 0.0000001267 * julcent);
        double sunEqofCtr =
            (sin(geomMeanAnom) *
                 (1.914602 - julcent * (0.004817 + 0.000014 * julcent)) +
             sin((2 * geomMeanAnom)) * (0.019993 - 0.000101 * julcent) +
             sin((3 * geomMeanAnom)) * 0.000289) /
            RADPI;
        double SunTrueLong = (geomMeanLong + sunEqofCtr);
        double SunTrueAnom = (geomMeanAnom + sunEqofCtr);
        double sunRadVec = (1.000001018 * (1 - eccentEarth * eccentEarth)) /
                           (1 + eccentEarth * cos(SunTrueAnom));
        double sunAppLong =
            SunTrueLong +
            (-0.00569 - 0.00478 * sin((125.04 - 1934.136 * julcent) / RADPI)) /
                RADPI;
        double meanOblElip =
            (23 +
             (26 +
              ((21.448 -
                julcent *
                    (46.815 + julcent * (0.00059 - julcent * 0.001813)))) /
                  60) /
                 60) /
            RADPI;
        double OblCorr =
            meanOblElip +
            (0.00256 * cos((125.04 - 1934.136 * julcent) / RADPI)) / RADPI;
        double sunDecl = asin(sin(OblCorr) * sin(sunAppLong));
        double vary = tan(OblCorr / 2) * tan(OblCorr / 2);
        double eqOfTime =
            (vary * sin(2 * geomMeanLong) -
             2 * eccentEarth * sin(geomMeanAnom) +
             4 * eccentEarth * vary * sin(geomMeanAnom) *
                 cos(2 * geomMeanLong) -
             0.5 * vary * vary * sin(4 * geomMeanLong) -
             1.25 * eccentEarth * eccentEarth * sin(2 * geomMeanAnom)) *
            4 * RADPI;

        double trueSolTime = fmod(secofday / 60 + eqOfTime + 4 * lng, 1440);
        double hourangle = ((trueSolTime < 0) ? trueSolTime / 4 + 180
                                              : trueSolTime / 4 - 180) /
                           RADPI;
        double SolZenith =
            acos(sin(lat / RADPI) * sin(sunDecl) +
                 cos(lat / RADPI) * cos(sunDecl) * cos(hourangle));

        double solAzi = 180 / RADPI;
        double acosArg = ((sin(lat / RADPI) * cos(SolZenith)) - sin(sunDecl)) /
                         (cos(lat / RADPI) * sin(SolZenith));
        double aCos = acos(acosArg);
        if (!isnan(aCos))
        {
            solAzi = (hourangle > 0) ? 180 / RADPI + aCos : 180 / RADPI - aCos;
        }
        double sx = sin(SolZenith) * cos(solAzi);
        double sy = sin(SolZenith) * sin(solAzi);
        double sz = cos(SolZenith);
        normalize_vector(&sx, &sy, &sz);

        printf("%lf %lf %lf %lf \n",
               fmod(secofday + gmtoff, 86400),
               90 - SolZenith * RADPI,
               solAzi * RADPI,
               sx * px + sy * py + sz * pz);
    }
}
