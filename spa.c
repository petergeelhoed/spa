#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double RADPI=57.29577951308237993927;
int main(int argc, char **argv) 
{
    int c;
    float lng = 4.687;
    float lat = 51.836;
    long int epoch = 0;
    float timezone =2;
    while ((c = getopt (argc, argv, "z")) != -1)
        switch (c)
        {
            case 'z':
                printf("z\n");
                break;
            default:
                abort();
        }
   int ret ;
    while ( fscanf(stdin,"%ld", &epoch) != EOF)
    {

    double julian = epoch/86400.;
    int secofday = epoch%86400;
    double julday = julian + 25569 + 2415018.5;
    //printf("%lf\n",julday);
    double julcent = (julday - 2451545)/36525;
    //printf("%lf\n",julcent);
    int modhelper = (int)(280.46646+julcent*(36000.76983 + julcent*0.0003032))/360.;
    double geomMeanLong = (280.46646+julcent*(36000.76983 + julcent*0.0003032))-modhelper*360;
    //printf("%lf\n",geomMeanLong );
    double geomMeanAnom = 357.52911+julcent*(35999.05029-0.0001537*julcent);
    //printf("%lf\n",geomMeanAnom );
    double eccentEarth = 0.016708634-julcent*(0.000042037+0.0000001267*julcent);
    //printf("%lf\n",eccentEarth);
    double sunEqofCtr = sin(geomMeanAnom/RADPI)*(1.914602-julcent*(0.004817+0.000014*julcent))+sin((2*geomMeanAnom/RADPI))*(0.019993-0.000101*julcent)+sin((3*geomMeanAnom/RADPI))*0.000289;
    //printf("%lf %lf\n",sunEqofCtr, geomMeanAnom/57.29577951308237993927);
    double SunTrueLong =geomMeanLong + sunEqofCtr;
    //printf("%lf\n",SunTrueLong);
    double SunTrueAnom =geomMeanAnom + sunEqofCtr;
    //printf("%lf\n",SunTrueAnom);
    double sunRadVec = (1.000001018*(1-eccentEarth*eccentEarth))/(1+eccentEarth*cos(SunTrueAnom/RADPI));
    //printf("%lf\n",sunRadVec);
    double sunAppLong = SunTrueLong-0.00569-0.00478*sin((125.04-1934.136*julcent)/RADPI);
    //printf("%lf\n",sunAppLong);
    double meanOblElip = 23+(26+((21.448-julcent*(46.815+julcent*(0.00059-julcent*0.001813))))/60)/60;
    //printf("%lf\n",meanOblElip);
    double OblCorr = meanOblElip+0.00256*cos((125.04-1934.136*julcent)/RADPI);
    //printf("%lf\n",OblCorr);
    double sunRtAsc = atan2( cos((OblCorr/RADPI))*sin((sunAppLong/RADPI)) , cos((sunAppLong/RADPI)))*RADPI;
    //printf("%lf\n",sunRtAsc);
    double sunDecl = (asin(sin((OblCorr/RADPI))*sin((sunAppLong/RADPI))))*RADPI;
    //printf("%lf\n",sunDecl);
    double vary = tan(OblCorr/RADPI/2)*tan(OblCorr/RADPI/2);
    //printf("%lf\n",vary);
    double eqOfTime =
        (vary*sin(2*(geomMeanLong)/RADPI)-2*eccentEarth*sin((geomMeanAnom)/RADPI)+4*eccentEarth*vary*sin((geomMeanAnom)/RADPI)*cos(2*(geomMeanLong)/RADPI)-0.5*vary*vary*sin(4*(geomMeanLong)/RADPI)-1.25*eccentEarth*eccentEarth*sin(2*(geomMeanAnom)/RADPI))*4*RADPI;
    //printf("%lf\n",eqOfTime);

    double trueSolTime = ((double)secofday/60+eqOfTime+4*lng);
    modhelper = (int)(trueSolTime/1440);
    trueSolTime-=modhelper*1440;
    //printf("%lf\n",trueSolTime);
    double hourangle = (trueSolTime<0)?  trueSolTime/4+180: trueSolTime/4-180;
    //printf("%lf\n",hourangle);
double SolZenith = (acos(sin((lat)/RADPI)*sin((sunDecl)/RADPI)+cos((lat)/RADPI)*cos((sunDecl)/RADPI)*cos((hourangle)/RADPI)))*RADPI;

    //printf("%lf\n",90-SolZenith);
    double solAzi = 
        (hourangle > 0 )?
                (acos(((sin((lat)/RADPI)*cos((SolZenith)/RADPI))-sin((sunDecl)/RADPI))/(cos((lat)/RADPI)*sin((SolZenith)/RADPI))))*RADPI+180:
                180-(acos(((sin((lat)/RADPI)*cos((SolZenith)/RADPI))-sin((sunDecl)/RADPI))/(cos((lat)/RADPI)*sin((SolZenith)/RADPI))))*RADPI;
   //printf("%lf\n",solAzi);
     printf("%ld %lf %lf\n",(secofday+(int)(timezone*3600))%86400,90-SolZenith,solAzi);
            }
}
