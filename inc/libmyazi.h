#define RADPI 57.29577951308237993927
struct azizen
{
    double epoch;
    double lng;
    double lat;
    double secofday;
    double zenith;
    double azimuth;
    double cos;
};

void normalize_vector(double* x, double* y, double* z);
void calcazi(struct azizen* azi);
