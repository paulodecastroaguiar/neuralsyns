//Paulo Aguiar and Ricardo Aguiar, Nov 2007

#ifndef _QUATERNIONS_H_
#define _QUATERNIONS_H_

#include "vec3math.h"


#define PI 3.14159265
#define DELTA_DISPLACEMENT_RAD 0.01
#define MAX_ROTATION_X 2.0
#define MAX_ROTATION_Y 2.0


/// data defining a quaternion
typedef struct quat
{
  Vec3 v;
  float w;

}Quat;



void SumQuat(Quat * q1, Quat* q2, Quat * res);
void ConjugateQuaternion(Quat * q1,Quat * res);
void MultQuat(Quat * q1, Quat* q2, Quat * res);
float NormQuat(Quat * q1);
void NormalizeQuat(Quat * q1, Quat * res);
void Inverse(Quat *q1, Quat * res);
void QuatToMatrix(Quat * q1,float matrixVector[16]);
void RotateQuaternion(Quat * q1,Vec3 position,Vec3 * viewVec, Vec3 axis,float angle,Quat * res);
void InitializeQuaternion(Quat * q1,Vec3 axis,float angle);
void RotateCamera(Vec3 viewVec,Vec3 upVec, Vec3 positionVec, Vec3 axis,float angle);
void SetView(float xDisplacement,float yDisplacement);

#endif
