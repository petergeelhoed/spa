#include <getopt.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "libmyazi.h"
#include "parseutil.h"

const double HALF = 0.5;
void print_usage()
{
    printf("usage: date +%%s | azid\n\n-n <lat>\n-e <long>\n-t <gmtoffset "
           "seconds>\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
    (void)setlocale(LC_NUMERIC, "C");

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
    double prev_epoch = -1.0;
    while (fgets(line, sizeof line, stdin) != NULL)
    {
        lineno++;

        // Trim leading whitespace
        char* parseChar = line;
        skip_white(&parseChar);
        if (*parseChar == '\0')
        {
            continue;
        }

        int retVal = parse_double(parseChar, &epoch);
        if (retVal != 0)
        {
            return retVal;
        }

        const double epsilon = 1e-7;
        struct azizen azi = {epoch, lng, lat, 0.0, 0.0, 0.0, 0.0, D180, 0.0};
        calcazi(&azi);
        double err_deg = (azi.azimuth - D180);
        double prev_err_deg = 0.0;

        double factor = HALF;
        while (fabs(err_deg) > epsilon) // && !(fabs(prev_err_deg + err_deg) < epsilon))
        {

            double cosz = cos(azi.zenith * M_PI / D180);
            double newEpoch = err_deg * SECS_HOUR * cosz / DEG_HOUR;
            if (fabs(prev_err_deg + err_deg) < epsilon)
            {
                newEpoch *= factor;
                factor *= HALF;
            }
            azi.epoch -= newEpoch;

            calcazi(&azi);
            prev_err_deg = err_deg;
            err_deg = azi.azimuth - D180;
        }

        char iso[DATE_BUF_SIZE];
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
