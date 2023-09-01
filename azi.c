#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// returns second of the day, zenith of the sun, azimuth of the sun, cos(angle of incidence)


double RADPI=57.29577951308237993927;
int main(int argc, char **argv) 
{
    int c;
    float lng = 4.687;
    float lat = 51.836;

    // panels 
    float panazi = 210/RADPI;
    float panzen = 10/RADPI;
    double px = sin(panzen)*cos(panazi);
    double py = sin(panzen)*sin(panazi);
    double pz = cos(panzen);
    double length = sqrt(px*px+pz*pz+py*py);
    px /=length;
    py /=length;
    pz /=length;


    long int epoch = 0;
    float timezone =2;
    while ((c = getopt (argc, argv, "n:e:t:")) != -1)
        switch (c)
        {
            case 'e':
                lng = atof(optarg);
                break;
            case 't':
                timezone = atof(optarg);
                break;
            case 'n':
                lat = atof(optarg);
                break;
            default:
                abort();
        }
    while ( fscanf(stdin,"%ld", &epoch) != EOF)
    {

        double julian = epoch/86400.;
        int secofday = epoch%86400;
        double julday = julian + 25569 + 2415018.5;
        double julcent = (julday - 2451545)/36525;
        int modhelper = (int)(280.46646+julcent*(36000.76983+julcent*0.0003032))/360.;
        double geomMeanLong = ((280.46646+julcent*(36000.76983+julcent*0.0003032))-modhelper*360)/RADPI;
        double geomMeanAnom = (357.52911+julcent*(35999.05029-0.0001537*julcent))/RADPI;
        double eccentEarth = 0.016708634-julcent*(0.000042037+0.0000001267*julcent);
        double sunEqofCtr = (sin(geomMeanAnom)*(1.914602-julcent*(0.004817+0.000014*julcent))
            +sin((2*geomMeanAnom))*(0.019993-0.000101*julcent)
            +sin((3*geomMeanAnom))*0.000289)/RADPI;
        double SunTrueLong = (geomMeanLong+sunEqofCtr);
        double SunTrueAnom = (geomMeanAnom+sunEqofCtr);
        double sunRadVec = (1.000001018*(1-eccentEarth*eccentEarth))/(1+eccentEarth*cos(SunTrueAnom));
        double sunAppLong = SunTrueLong+(-0.00569-0.00478*sin((125.04-1934.136*julcent)/RADPI))/RADPI;
        double meanOblElip = (23+(26+((21.448-julcent*(46.815+julcent*(0.00059-julcent*0.001813))))/60)/60)/RADPI;
        double OblCorr = meanOblElip+(0.00256*cos((125.04-1934.136*julcent)/RADPI))/RADPI;
        double sunDecl = asin(sin(OblCorr)*sin(sunAppLong));
        double vary = tan(OblCorr/2)*tan(OblCorr/2);
        double eqOfTime = (vary*sin(2*geomMeanLong)-2*eccentEarth*sin(geomMeanAnom)+4*eccentEarth*vary*sin(geomMeanAnom)*cos(2*geomMeanLong)-0.5*vary*vary*sin(4*geomMeanLong)-1.25*eccentEarth*eccentEarth*sin(2*geomMeanAnom))*4*RADPI;

        double trueSolTime = (double)secofday/60+eqOfTime+4*lng;
        modhelper = (int)(trueSolTime/1440);
        trueSolTime-=modhelper*1440;
        double hourangle = ((trueSolTime<0)?trueSolTime/4+180:trueSolTime/4-180)/RADPI;
        double SolZenith = (acos(sin(lat/RADPI)*sin(sunDecl)+cos(lat/RADPI)*cos(sunDecl)*cos(hourangle)));

        double solAzi = (hourangle >0)?
            180/RADPI+(acos(((sin(lat/RADPI)*cos(SolZenith))-sin(sunDecl))/(cos(lat/RADPI)*sin(SolZenith)))):
            180/RADPI-(acos(((sin(lat/RADPI)*cos(SolZenith))-sin(sunDecl))/(cos(lat/RADPI)*sin(SolZenith))));
        double sx = sin(SolZenith)*cos(solAzi);
        double sy = sin(SolZenith)*sin(solAzi);
        double sz = cos(SolZenith);
        length = sqrt(px*px+pz*pz+py*py);
        sx /=length;
        sy /=length;
        sz /=length;
       printf("%ld %lf %lf %lf \n",(secofday+(int)(timezone*3600))%86400,
                90-SolZenith*RADPI,solAzi*RADPI,
                sx*px+sy*py+sz*pz);

    }
}
