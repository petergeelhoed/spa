#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "libmyazi.h"

// Function to calculate the length of a vector
double calculate_length(double vec_x, double vec_y, double vec_z)
{
    return sqrt(vec_x * vec_x + vec_y * vec_y + vec_z * vec_z);
}

// Function to normalize a vector
void normalize_vector(double* vec_x, double* vec_y, double* vec_z)
{
    double length = calculate_length(*vec_x, *vec_y, *vec_z);
    *vec_x /= length;
    *vec_y /= length;
    *vec_z /= length;
}

void calcazi(struct azizen* ret)
{
    // Panel angles
    double panx = sin(ret->panzen) * cos(ret->panazi);
    double pany = sin(ret->panzen) * sin(ret->panazi);
    double panz = cos(ret->panzen);
    normalize_vector(&panx, &pany, &panz);

    double julian = ret->epoch / SECS_IN_DAY;
    double secofday = fmod(ret->epoch, SECS_IN_DAY);
    // NOLINTBEGIN(readability-magic-numbers)
    double julday = julian + 25569 + 2415018.5;
    double julcent = (julday - 2451545) / 36525;
    double geomMeanLong =
        fmod((280.46646 + julcent * (36000.76983 + julcent * 0.0003032)), 360) / RADPI;
    double geomMeanAnom = (357.52911 + julcent * (35999.05029 - 0.0001537 * julcent)) / RADPI;
    double eccentEarth = 0.016708634 - julcent * (0.000042037 + 0.0000001267 * julcent);
    double sunEqofCtr =
        (sin(geomMeanAnom) * (1.914602 - julcent * (0.004817 + 0.000014 * julcent)) +
         sin((2 * geomMeanAnom)) * (0.019993 - 0.000101 * julcent) +
         sin((3 * geomMeanAnom)) * 0.000289) /
        RADPI;
    double SunTrueLong = (geomMeanLong + sunEqofCtr);
    // double SunTrueAnom = (geomMeanAnom + sunEqofCtr);
    //  double sunRadVec = (1.000001018 * (1 - eccentEarth * eccentEarth)) /
    //                     (1 + eccentEarth * cos(SunTrueAnom));
    double sunAppLong =
        SunTrueLong + (-0.00569 - 0.00478 * sin((125.04 - 1934.136 * julcent) / RADPI)) / RADPI;
    double meanOblElip =
        (23 +
         (26 + ((21.448 - julcent * (46.815 + julcent * (0.00059 - julcent * 0.001813)))) / 60) /
             60) /
        RADPI;
    double OblCorr = meanOblElip + (0.00256 * cos((125.04 - 1934.136 * julcent) / RADPI)) / RADPI;
    double sunDecl = asin(sin(OblCorr) * sin(sunAppLong));
    double vary = tan(OblCorr / 2) * tan(OblCorr / 2);
    double eqOfTime = (vary * sin(2 * geomMeanLong) - 2 * eccentEarth * sin(geomMeanAnom) +
                       4 * eccentEarth * vary * sin(geomMeanAnom) * cos(2 * geomMeanLong) -
                       0.5 * vary * vary * sin(4 * geomMeanLong) -
                       1.25 * eccentEarth * eccentEarth * sin(2 * geomMeanAnom)) *
                      4 * RADPI;
    // NOLINTEND(readability-magic-numbers)

    double trueSolTime = fmod(secofday / SECS_IN_MINUTE + eqOfTime + 4 * ret->lng, MIN_IN_DAY);
    double hourangle =
        ((trueSolTime < 0) ? trueSolTime / 4 + D180 : trueSolTime / 4 - D180) / RADPI;
    double SolZenith = acos(sin(ret->lat / RADPI) * sin(sunDecl) +
                            cos(ret->lat / RADPI) * cos(sunDecl) * cos(hourangle));

    double solAzi = D180 / RADPI;
    double acosArg = ((sin(ret->lat / RADPI) * cos(SolZenith)) - sin(sunDecl)) /
                     (cos(ret->lat / RADPI) * sin(SolZenith));
    double aCos = acos(acosArg);
    if (!isnan(aCos))
    {
        solAzi = (hourangle > 0) ? D180 / RADPI + aCos : 180 / RADPI - aCos;
    }
    double solX = sin(SolZenith) * cos(solAzi);
    double solY = sin(SolZenith) * sin(solAzi);
    double solZ = cos(SolZenith);
    normalize_vector(&solX, &solY, &solZ);

    ret->secofday = secofday;
    ret->zenith = 90 - SolZenith * RADPI;
    ret->azimuth = solAzi * RADPI;
    ret->cos = solX * panx + solY * pany + solZ * panz;
}

int epoch_to_iso8601_local(time_t epoch, char* out, size_t out_size)
// int epoch_to_iso8601_utc(time_t epoch, char* out, size_t out_size)
{
    struct tm tm_local;
    if (out == NULL || out_size < 21)
    {
        return -1;
    }
    if (localtime_r(&epoch, &tm_local) == NULL)
    {
        return -2;
    }
    if (strftime(out, out_size, "%Y-%m-%dT%H:%M:%S", &tm_local) == 0)
    {
        return -3;
    }
    return 0;
}
