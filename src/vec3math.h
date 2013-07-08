//Paulo Aguiar and Ricardo Aguiar, Nov 2007

#ifndef _VEC3MATH_H_
#define _VEC3MATH_H_


#include <math.h>


/// standard 3D vector
typedef struct vec3
{
  float x,y,z;
}Vec3;

Vec3 CrossProduct(Vec3 v, Vec3 u);
Vec3 NormalizeVec3(Vec3 v);
Vec3 SubVec(Vec3 v1, Vec3 v2);
Vec3 SumVec(Vec3 v1, Vec3 v2);
Vec3 MultVec3Float(Vec3 v1, float f1);
float NormVec3(Vec3 v);
float DotProduct(Vec3 v1, Vec3 v2);

#endif
