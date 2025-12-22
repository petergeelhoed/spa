#define RADPI 57.29577951308237993927
const int SECS_IN_DAY = 86400.;
const double D180 = 180.;
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
