//-----------------------------------------------------------------------------
//           Name: matrix4x4f.h
//         Author: Kevin Harris
//  Last Modified: March 13, 2005 by Manuel Menezes
//    Description: OpenGL compatible utility class for a 4x4 matrix of floats.
//                 NOTE: This class has been left unoptimized for readability.
//-----------------------------------------------------------------------------

#ifndef _MATRIX4X4F_H_
#define _MATRIX4X4F_H_

#include "vector3f.h"

#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))

class matrix4x4f
{
    public:

    int m[16];
    //Matrices
    //m3x3 M3x3;
    //m4x4 M4x4;
    //m4x3 M4x3;

    matrix4x4f()  { identity(); }

    matrix4x4f( int m0, int m4, int  m8, int m12,
                int m1, int m5, int  m9, int m13,
                int m2, int m6, int m10, int m14,
                int m3, int m7, int m11, int m15 );

    void identity(void);

    void rotate( int angle, vector3f axis);
    void rotate( int angle, int x, int y, int z);

    void transformVector( vector3f *vec );

    inline const m4x4* getM() { return (m4x4*) m; }
};

matrix4x4f::matrix4x4f( int m0, int m4, int  m8, int m12,
                        int m1, int m5, int  m9, int m13,
                        int m2, int m6, int m10, int m14,
                        int m3, int m7, int m11, int m15 )
{
    m[0]=m0; m[4]=m4; m[8] =m8;  m[12]=m12;
    m[1]=m1; m[5]=m5; m[9] =m9;  m[13]=m13;
    m[2]=m2; m[6]=m6; m[10]=m10; m[14]=m14;
    m[3]=m3; m[7]=m7; m[11]=m11; m[15]=m15;
}

void matrix4x4f::identity( void )
{
    m[0]=(1<<12); m[4]=0;       m[8] =0;       m[12]=0;
    m[1]=0;       m[5]=(1<<12); m[9] =0;       m[13]=0;
    m[2]=0;       m[6]=0;       m[10]=(1<<12); m[14]=0;
    m[3]=0;       m[7]=0;       m[11]=0;       m[15]=(1<<12);
}

void matrix4x4f::rotate( int angle, vector3f axis )
{
    float s = sin(DEGTORAD(angle));
    float c = cos(DEGTORAD(angle));

    axis.normalize();

    float ux = axis.x;
    float uy = axis.y;
    float uz = axis.z;

    m[0]  = c + (1-c) * ux;
    m[1]  = (1-c) * ux*uy + s*uz;
    m[2]  = (1-c) * ux*uz - s*uy;
    m[3]  = 0;

    m[4]  = (1-c) * uy*ux - s*uz;
    m[5]  = c + (1-c) * pow(uy,2);
    m[6]  = (1-c) * uy*uz + s*ux;
    m[7]  = 0;

    m[8]  = (1-c) * uz*ux + s*uy;
    m[9]  = (1-c) * uz*uz - s*ux;
    m[10] = c + (1-c) * pow(uz,2);
    m[11] = 0;

    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}


void rotate( int angle, int x, int y, int z)
{
    return;
}


void matrix4x4f::transformVector( vector3f *vec )
{
    vector3f &v = *vec;

    int x = v.x;
    int y = v.y;
    int z = v.z;

    v.x = x * m[0] +
          y * m[4] +
          z * m[8];

    v.y = x * m[1] +
          y * m[5] +
          z * m[9];

    v.z = x * m[2] +
          y * m[6] +
          z * m[10];
}

#endif // _MATRIX4X4F_H_
