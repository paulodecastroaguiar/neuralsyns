//Paulo Aguiar and Ricardo Aguiar, Nov 2007

#include "quaternions.h"


void ConjugateQuaternion(Quat * q1, Quat * res)
{
  res->v.x=-q1->v.x;
  res->v.y=-q1->v.y;
  res->v.z=-q1->v.z;
  res->w=q1->w;

  return;  
}


void SumQuat(Quat * q1, Quat* q2, Quat * res)
{
  res->v.x=q1->v.x+q2->v.x;
  res->v.y=q1->v.y+q2->v.y;
  res->v.z=q1->v.z+q2->v.z;
  res->w=q1->w+q2->w;

  return;  
}


void MultQuat(Quat * q1, Quat* q2, Quat * res)
{
  float A, B, C, D, E, F, G, H;
  
  
  A = (q1->w + q1->v.x)*(q2->w + q2->v.x);
  B = (q1->v.z - q1->v.y)*(q2->v.y - q2->v.z);
  C = (q1->w - q1->v.x)*(q2->v.y + q2->v.z); 
  D = (q1->v.y + q1->v.z)*(q2->w - q2->v.x);
  E = (q1->v.x + q1->v.z)*(q2->v.x + q2->v.y);
  F = (q1->v.x - q1->v.z)*(q2->v.x - q2->v.y);
  G = (q1->w + q1->v.y)*(q2->w - q2->v.z);
  H = (q1->w - q1->v.y)*(q2->w + q2->v.z);
  
  
  res->w = B + (-E - F + G + H) /2;
  res->v.x = A - (E + F + G + H)/2; 
  res->v.y = C + (E - F + G - H)/2; 
  res->v.z = D + (E - F - G + H)/2;

  return;	
}


float NormQuat(Quat * q1)
{
  return sqrt(q1->v.x*q1->v.x+q1->v.y*q1->v.y+q1->v.z*q1->v.z+q1->w*q1->w);
}


void NormalizeQuat(Quat * q1, Quat * res)
{
  float n=NormQuat(q1);
  res->v.x=q1->v.x/n;
  res->v.y=q1->v.y/n;
  res->v.z=q1->v.z/n;
  res->w=q1->w/n;

  return;
}


void Inverse(Quat *q1, Quat * res)
{
  float n=NormQuat(q1);
  res->v.x=-q1->v.x/n;
  res->v.y=-q1->v.y/n;
  res->v.z=-q1->v.z/n;
  res->w=q1->w/n;

  return;
}


void QuatToMatrix(Quat * q1, float matrixVector[16])
{
  float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    
  // calculate coefficients
  x2 = q1->v.x + q1->v.x; y2 = q1->v.y + q1->v.y;
  z2 = q1->v.z + q1->v.z;
  xx = q1->v.x * x2; xy = q1->v.x * y2; xz = q1->v.x * z2;
  yy = q1->v.y * y2; yz = q1->v.y * z2; zz = q1->v.z * z2;
  wx = q1->w * x2; wy = q1->w * y2; wz = q1->w * z2;
  
  matrixVector[0] = 1.0 - (yy + zz);
  matrixVector[1] = xy - wz;
  matrixVector[2] = xz + wy;
  matrixVector[3] = 0.0;
  
  matrixVector[4] = xy + wz; 
  matrixVector[5] = 1.0 - (xx + zz);
  matrixVector[6] = yz - wx; 
  matrixVector[7] = 0.0;
  
  
  matrixVector[8] = xz - wy;
  matrixVector[9] = yz + wx;
  matrixVector[10] = 1.0 - (xx + yy); 
  matrixVector[11] = 0.0;
  
  
  matrixVector[12] = 0; 
  matrixVector[13] = 0;
  matrixVector[14] = 0; 
  matrixVector[15] = 1;
  
  /*
    matrix[0][0] = 1.0 - (yy + zz);
    matrix[1][0] = xy - wz;
    matrix[2][0] = xz + wy;
    matrix[3][0] = 0.0;
    
    matrix[0][1] = xy + wz; 
    matrix[1][1] = 1.0 - (xx + zz);
    matrix[2][1] = yz - wx; 
    matrix[3][1] = 0.0;
    
    
    matrix[0][2] = xz - wy;
    matrix[1][2] = yz + wx;
    matrix[2][2] = 1.0 - (xx + yy); 
    matrix[3][2] = 0.0;
    
    
    matrix[0][3] = 0; 
    matrix[1][3] = 0;
    matrix[2][3] = 0; 
    matrix[3][3] = 1;
  */  

  return;
}


void InitializeQuaternion(Quat * q1, Vec3 axis, float angle)
{
  float angleRad=angle*PI/180;
  float theta=sin(angleRad/2);
  
  q1->w=cos(angleRad/2);
  q1->v.x=theta*axis.x;
  q1->v.y=theta*axis.y;
  q1->v.z=theta*axis.z;

  return;
}


void RotateQuaternion(Quat * q1, Vec3 position, Vec3 * viewVec, Vec3 axis, float angle, Quat * res)
{
  Quat viewVecQuat;
  Quat newRot;
  Quat newRotConj;
  
  float angleRad=angle*PI/180;
  float theta=sin(angleRad/2);
  
  viewVecQuat.v.x=viewVec->x-position.x;
  viewVecQuat.v.y=viewVec->y-position.y;
  viewVecQuat.v.z=viewVec->z-position.z;
  viewVecQuat.w=0;
	
  newRot.w=cos(angleRad/2);
  newRot.v.x=theta*axis.x;
  newRot.v.y=theta*axis.y;
  newRot.v.z=theta*axis.z;
  NormalizeQuat(&newRot,&newRot);  
  
  MultQuat(&newRot,&viewVecQuat,&viewVecQuat);
  ConjugateQuaternion(&newRot,&newRotConj);
  MultQuat(&viewVecQuat,&newRotConj,&viewVecQuat);
  
  viewVec->x=viewVecQuat.v.x+position.x;
  viewVec->y=viewVecQuat.v.y+position.y;
  viewVec->z=viewVecQuat.v.z+position.z;
  
  return;	
}


void SetView(float xDisplacement, float yDisplacement)
{
  static float currentRotationX=0.0;
  static float currentRotationY=0.0;
  
  float xDisplacementRad=xDisplacement*PI/180;
  float yDisplacementRad=yDisplacement*PI/180;
  
  if(fabs(xDisplacementRad)<DELTA_DISPLACEMENT_RAD && fabs(yDisplacementRad)<DELTA_DISPLACEMENT_RAD) {      
		return;
	}
  
  currentRotationX+=xDisplacementRad;
  currentRotationY+=yDisplacementRad;
  
  
  if(currentRotationX>MAX_ROTATION_X) {
		currentRotationX=MAX_ROTATION_X;
		return;
	}

  if(currentRotationX<-MAX_ROTATION_X) {
		currentRotationX=-MAX_ROTATION_X;
		return;
	}
  
  if(currentRotationY>MAX_ROTATION_Y) {
		currentRotationY=MAX_ROTATION_Y;
		return;
	}

  if(currentRotationY<-MAX_ROTATION_Y) {
		currentRotationY=-MAX_ROTATION_Y;
		return;
	}

  return;
}
