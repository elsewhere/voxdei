#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include <math.h>
#include "matrix.hpp"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------


	class Vector3;
	class Vector4;

//--------------------------------------------------------------------------------------------
//  Vector class
//--------------------------------------------------------------------------------------------

	class Vector4
	{
		friend class Matrix;	

		public:
			Vector4();
			Vector4(float px, float py, float pz, float pa);
			Vector4(Vector4 &example);
			Vector4(Vector3 &example);
			~Vector4();

			Vector4 crossProduct(Vector4 &vect);
			float dotProduct(Vector4 &vect);


			Vector4 operator + (Vector4 const &vect);
			Vector4 operator - (Vector4 const &vect);
			Vector4 operator += (Vector4 const &vect);
			Vector4 operator -= (Vector4 const &vect);

			Vector4 operator * (Matrix const &mat);
			Vector4 operator *= (Matrix const &mat);
			Vector4 operator * (const float &value);
			Vector4 operator *= (float const &value);
			Vector4 operator / (const float &value);

			bool operator == (Vector4 const &vect);
			void operator = (Vector4 const &vect);
			
			Vector4 normalize();
			float length();

			float x, y, z;

			float a; // tämän ei pitäisi olla tässä! läst minute häk
		private:
	};

	class Vector3
	{
		friend class Matrix;	

		public:
			Vector3();
			Vector3(float px, float py, float pz);
			Vector3(Vector3 &example);
			Vector3(Vector4 &example);
			~Vector3();

			Vector3 crossProduct(Vector3 &vect);
			float dotProduct(Vector3 &vect);

			Vector3 operator + (Vector3 const &vect);
			Vector3 operator - (Vector3 const &vect);
			Vector3 operator += (Vector3 const &vect);
			Vector3 operator -= (Vector3 const &vect);

			Vector3 operator * (Matrix const &mat);
			Vector3 operator *= (Matrix const &mat);
			Vector3 operator * (const float &value);
			Vector3 operator *= (float const &value);
			Vector3 operator / (const float &value);

			bool operator == (Vector3 const &vect);
			void operator = (Vector3 const &vect);
			
			Vector3 normalize();
			float length();

			float x, y, z;
			
		private:
	};

#endif