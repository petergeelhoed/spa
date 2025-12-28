#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "libmyazi.h"
#include "parseutil.h"

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

        struct azizen azi = {epoch, lng, lat, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        calcazi(&azi);

        printf("%lf %lf %lf\n", fmod(azi.secofday + gmtoff, SECS_IN_DAY), azi.zenith, azi.azimuth);
    }
    return 0;
}
