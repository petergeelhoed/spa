#include <time.h>
const double RADPI = 57.29577951308237993927;
const int SECS_IN_DAY = 86400.;
const double D180 = 180.;
const double SECS_IN_MINUTE = 60.;
const double MIN_IN_DAY = 1440.;
struct azizen
{
    double epoch;
    double lng;
    double lat;
    double secofday;
    double zenith;
    double azimuth;
    double cos;
    double panazi;
    double panzen;
};

void normalize_vector(double* x, double* y, double* z);
void calcazi(struct azizen* azi);
int epoch_to_iso8601_local(time_t epoch, char* out, size_t out_size);
