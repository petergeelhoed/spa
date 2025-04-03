#define RADPI 57.29577951308237993927
struct azizen
{
    double secofday;
    double zenith;
    double azimuth;
    double cos;
};

void normalize_vector(double* x, double* y, double* z);
struct azizen calcazi(double epoch, double lng, double lat);
