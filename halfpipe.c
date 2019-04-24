///
/// halfpipe.c
///
///
///
///   how to build:   gcc halfpipe.c -lm -o halfpipe
///
///   generate G-code to cut a part of a halfpipe with a ball endmill
///    Note: as of now this only creates the paths for the  halfpipe outline
///
///        |             |
///         \           /
///          `.       .
///             -----


#define VERSION "102"


#include <stdio.h>
#include <math.h>

//#define PI 3.14159265



static double 
d2r( int d )
{
   // convert degree to radian
   return (d / 180.0) * ((double) M_PI);
}

double dBallEndMilliH = 12.50;       // 12.50 mm  units in hundreds

double dHalfpipe      = 32.00;       // 32.00 mm   diameter of halfpipe
double lHalfpipe      = 26.00;       // 26.00 mm   length of halfpipe
double zcutHalfpipe   =  4.00;       //  4.00 mm   depth of halfpipe   (start of cuts is always at 0mm)

//int xOffset        = 0000;       //  0.00 mm   x offset to center of halfpipe
//int yOffset       = -2000;       // 13.00 mm   y offset to center of halfpipe
//int zCenter        = 0000;       //  0.00 mm   y offset to center of halfpipe    !! not needed as long at start cuts is 0mm depth

double hpCenter_x     =   0.00;       //   0.00 mm   x offset to center of halfpipe
double hpCenter_y     = -20.00;     // -20.00 mm   y offset to center of halfpipe
double hpCenter_z     =  12.00;      // -12.00 mm = 32-4 =  z axis center = hpD - cutdepth    !! calculated on the fly 

double speedHorizontal = 200.0;
double speedVertical   =  50.0;

#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

static int helpMode = 0;
static int verbose = 0;
static int testMode = 0;

void
printUsage( int hmode ) 
{
   //if ( hmode <= 1 ) {
      printf(" halfpipe r%s\n", VERSION );
      printf( "\nusage:\n\n" );
      
      printf( "  halfpipe -b 12.50 -d 32.00 -l 26.00 -x 13.0 -y 0    // ballEndmill 12.5mm\n" );
   //}
   //else {
   //   printf( "  qiflash idtversion\n" );
   //   printf( "\n" );
   //}      
   printf( "\n" );
}

int
getCLopts( int argc, char *argv[] )
{
   int option;
   int nopcounter = 0;; 
   
   //Specifying the expected options
   while ((option = getopt(argc, argv,"vtVhb:d:l:x:y:z:")) != -1) {
         switch (option) {
            
            case 'b' :
               dBallEndMilliH = atof(optarg);
               if (verbose) printf( "ballend mill diameter = %.2lf\n", dBallEndMilliH );
               break;

            case 'd' :
               dHalfpipe = atof(optarg);
               if (verbose) printf( "halfpipe diameter = %.2lf\n", dHalfpipe );
               break;
            case 'l' :
               lHalfpipe = atof(optarg);
               if (verbose) printf( "lhalfpipe = %.2lf\n", lHalfpipe );
               break;
               
            case 'x' :
               hpCenter_x = atof(optarg);
               if (verbose) printf( "halfpipe xoffset = %.2lf\n", hpCenter_x );
               break;
            case 'y' :
               hpCenter_y = atof(optarg);
               if (verbose) printf( "halfpipe yoffset = %.2lf\n", hpCenter_y );
               break;
            case 'z' :
               zcutHalfpipe = atof(optarg);
               if (verbose) printf( "halfpipe zcut = %.2lf\n", zcutHalfpipe );
               break;
               
            case 'v' :
               verbose++;
               break;
            case 't' :
               testMode++;
               break;
            case 'V' :
               printf("halfpipe r%s\n", VERSION );
               exit(0);
               break;
            case 'h' :
               helpMode++;
               printUsage( helpMode );
               break;
               
            default:
               printUsage( 0 );
               printf( "unsupported argument - option=%c %s:\n", option, optarg);
               exit(EXIT_FAILURE);
         }
   }    //while

   
   if (verbose) printf( "CLopts bem=%.2lf v=%d \n", dBallEndMilliH, verbose );
   if ( helpMode ) exit(0);

   return 0;
}

#define CUT_HEIGHT 0.5      // cutting height not used right now
#define CUT_ANGLE  15       // 15 degree    for 4-5 mm cut 33 degree should be enough
                            // for now we do 7angles        : 45, 30, 15, 0, -15, -30, -45
                            // resulting in 4 cut heights at: 45, 30, 15, 0     where we cut both sides of the halfpipe ( square move )


/*
 *     ballendmill is set z height 0.00mm
 *     cutting depth is  -4 mm for now
 *     z center of halfpipe is   BEM diameter - cutting depth = 12.5 - 4 = 8.5mm
 *     
 */

int
main( int argc, char **argv )
{
   int rc;
   double  ysin, zcos;
   int initial = 1;
   
   verbose = 0;
   
   if ( argc > 1 ) {
      rc = getCLopts( argc, argv );
      //printf("getclopts rc = %d\n", rc );
      if ( rc < 0 ) return rc;
   }
   
   double hpRadius  = dHalfpipe/2;
   double bemRadius = dBallEndMilliH/2;
   double cutRadius = (dHalfpipe - dBallEndMilliH) /2;         // calculate cutting radius
   
#if 0  
   double hpc_x =  (double)xOffset/100;                      // calc coordinates of halfpipe center 
   double hpc_y =  (double)yOffset/100;
   double hpc_z =  ((double)dHalfpipe/200) - ((double)zcutHalfpipe/100);     // 
#else
   // hpCenter_x       !! as given on the comandline
   // hpCenter_y
   hpCenter_z = hpRadius - zcutHalfpipe;     // 
#endif

   printf("(--- halfpipe D=%.2lf L=%.2f zcut=%.2f  center x/y/z= %.2lf %.2lf %.2lf  hpR=%.2lf  bemR=%.2lf  cutR=%.2lf )\n",
          dHalfpipe, lHalfpipe, zcutHalfpipe, 
          hpCenter_x,hpCenter_y,hpCenter_z, hpRadius, bemRadius, cutRadius );


   //for ( int i = 90; i >= -90; i -= 15) {    // use for testing
   //for ( int i = 90; i >= -90; i -= 5) {    // use for testing
   //for ( int i = 45; i >= -45; i -= 15) {
   for ( int i = 45; i >= 0; i -= 15) {       // every 15 degrees
   //for ( int i = 50; i >= 0; i -= 5) {       //every 5 degrees
      
      ysin = sin( d2r(i) );
      zcos = cos( d2r(i) );
   
      double hpcoff_y = cutRadius * ysin;       // calculate y offset from center of halfpipe
      double hpcoff_z = cutRadius * zcos;       // calculate z offset from center of halfpipe
      
      double bem_x = hpCenter_x;
      double bem_y = hpCenter_y + hpcoff_y;
      double bem_z = hpCenter_z - bemRadius - hpcoff_z;
            
      if ( initial ) { 
         initial = 0;
         printf("G0 Z10.000\n" );
         printf("G0 X%.3lf Y%.3lf\n", bem_x,bem_y );
         printf("G0 Z5.000\n" );
      }
      
      printf("(halfpipe path %02d degrees: ysin/zcos=%.3lf/%.3lf  hpcoff_yz= %.2lf %.2lf", i, ysin,zcos, hpcoff_y,hpcoff_z );
      printf("  bemx/y/z= %.2lf %.2lf %.2lf", bem_x,bem_y,bem_z );
      printf(")\n");
      
      // lower the endmill
      printf("G1 X%.3lf Y%.3lf Z%.3lf F%.3lf\n", bem_x,bem_y,bem_z, speedVertical );
      
      // make a complete square move
      if ( testMode == 0 ) {
         printf("G1 X%.3lf Y%.3lf Z%.3lf F%.3lf\n", bem_x+lHalfpipe, bem_y,              bem_z, speedHorizontal );
         printf("G1 X%.3lf Y%.3lf Z%.3lf F%.3lf\n", bem_x+lHalfpipe, bem_y-(hpcoff_y*2), bem_z, speedHorizontal );
         printf("G1 X%.3lf Y%.3lf Z%.3lf F%.3lf\n", bem_x,                 bem_y-(hpcoff_y*2), bem_z, speedHorizontal );
         printf("G1 X%.3lf Y%.3lf Z%.3lf F%.3lf\n", bem_x,                 bem_y,              bem_z, speedHorizontal );
      }
      
   }
  
   printf("G0 Z10.000\n" );
   
   return(0);
}


