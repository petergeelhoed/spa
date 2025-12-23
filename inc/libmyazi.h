#include <stddef.h>
#include <time.h>
const double RADPI = 57.29577951308237993927;
const double D180 = 180.;
const double SECS_IN_DAY = 86400.;
const double SECS_IN_MINUTE = 60.;
const double DEG_HOUR = 15.;
const double SECS_HOUR = 3600.;
const double MIN_IN_DAY = 1440.;

const size_t BUF_SIZE = 256;
const size_t DATE_BUF_SIZE = 21;

const double mylng = 4.687;
const double mylat = 51.836;
const double myPanAzi = 210;
const double myPanZen = 10;

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

void normalize_vector(double* vec_x, double* vec_y, double* vec_z);
void calcazi(struct azizen* ret);
int epoch_to_iso8601_local(time_t epoch, char* out, size_t out_size);
