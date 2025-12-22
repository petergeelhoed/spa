#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "libmyazi.h"
#include "parseutil.h"

const double mylng = 6.687;
const double mylat = 51.836;
const int BUF_SIZE = 256;
const double DEG_HOUR = 15.;
const double SECS_HOUR = 3600.;
void print_usage()
{
    printf("usage: date +%%s | azid\n\n-n <lat>\n-e <long>\n-t <gmtoffset "
           "seconds>\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{

    time_t now = time(NULL);
    struct tm localNow = {0};
    localtime_r(&now, &localNow);
    int gmtoff = localNow.tm_gmtoff;

    double lng = mylng;
    double lat = mylat;

    // Panel angles
    double epoch = 0.0;
    int optChar;
    while ((optChar = getopt(argc, argv, "n:e:t:h")) != -1)
    {
        switch (optChar)
        {
        case 'e':
            if (parse_double(optarg, &lng))
            {
                exit(EXIT_FAILURE);
            }
            break;
        case 't':
            if (parse_int10(optarg, &gmtoff))
            {
                exit(EXIT_FAILURE);
            }
            break;
        case 'n':
            if (parse_double(optarg, &lat))
            {
                exit(EXIT_FAILURE);
            }
            break;
        case 'h':
        default:
            print_usage();
        }
    }

    char line[BUF_SIZE];
    size_t lineno = 0;

    double prev_epoch = -1;
    while (fgets(line, sizeof line, stdin) != NULL)
    {
        lineno++;

        // Trim leading whitespace
        char* parseChar = line;
        while (*parseChar && (*parseChar == ' ' || *parseChar == '\t' || *parseChar == '\r' ||
                              *parseChar == '\n'))
        {
            parseChar++;
        }
        if (*parseChar == '\0')
        {
            continue;
        }

        int retVal = parse_double(parseChar, &epoch);
        if (retVal != 0)
        {
            return retVal;
        }

        double prev_azi = -1.;
        double prev_prev_azi = -2.;
        struct azizen azi = {epoch, lng, lat, 0.0, 0.0, 0.0, 0.0, D180, 0.0};
        calcazi(&azi);
        while (prev_prev_azi != azi.azimuth && prev_azi != azi.azimuth)
        {
            double newEpoch =
                (azi.azimuth - D180) / DEG_HOUR * SECS_HOUR * cos(azi.zenith / D180 * M_PI);
            azi.epoch -= newEpoch;

            prev_prev_azi = prev_azi;
            prev_azi = azi.azimuth;
            calcazi(&azi);
        }

        char iso[25];
        if (epoch_to_iso8601_local(azi.epoch, iso, sizeof(iso)) != 0)
        {
            exit(EXIT_FAILURE);
        }

        printf("%s %lf %lf %lf %lf\n",
               iso,
               azi.epoch,
               fmod(azi.secofday + gmtoff, SECS_IN_DAY),
               prev_epoch == -1. ? 0.0 : azi.epoch - prev_epoch,
               azi.azimuth);
        prev_epoch = azi.epoch;
    }
    return 0;
}
