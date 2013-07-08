//Paulo Aguiar and Ricardo Aguiar, Nov 2007

#include "vec3math.h"


Vec3 MultVec3Float(Vec3 v1, float f1)
{
  Vec3 temp;
  temp.x=v1.x*f1;
  temp.y=v1.y*f1;
  temp.z=v1.z*f1;
  
  return temp;  
}


float DotProduct(Vec3 v1, Vec3 v2)
{
  float temp;
  temp=v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
  return temp;
  
}


Vec3 CrossProduct(Vec3 v, Vec3 u)
{
  Vec3 res;
  res.x=v.y*u.z-u.y*v.z;
  res.y=u.x*v.z-v.x*u.z;
  res.z=v.x*u.y-u.x*v.y;
  return res;
}


Vec3 SubVec(Vec3 v1, Vec3 v2) //subtract vectors
{
  Vec3 res;
  res.x=v1.x-v2.x;
  res.y=v1.y-v2.y;
  res.z=v1.z-v2.z;
  
  return res;
}


Vec3 SumVec(Vec3 v1, Vec3 v2) //sum vectors
{
  Vec3 res;
  res.x=v1.x+v2.x;
  res.y=v1.y+v2.y;
  res.z=v1.z+v2.z;
  
  return res;
}


float NormVec3(Vec3 v)
{
  return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}


Vec3 NormalizeVec3(Vec3 v)
{
  float norm=NormVec3(v);
  Vec3 res;
  
  res.x=v.x/norm;
  res.y=v.y/norm;
  res.z=v.z/norm;

  return res; 
}
