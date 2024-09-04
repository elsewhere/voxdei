//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include "Vector.hpp"

//--------------------------------------------------------------------------------------------
//  Use our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//  Vector class code
//--------------------------------------------------------------------------------------------

Vector4::Vector4()
{
	x = y = z = a = 0.0f;
}


Vector4::Vector4(float px, float py, float pz, float pa)
{
	x = px;
	y = py;
	z = pz;
    a = pa;
}
Vector4::Vector4(Vector4 &example)
{
	x = example.x;
	y = example.y;
	z = example.z;
    a = example.a;
}
Vector4::Vector4(Vector3 &example)
{
	x = example.x;
	y = example.y;
	z = example.z;
    a = 0;
}

Vector4::~Vector4()
{

}

float Vector4::length()
{
	return (float)sqrt(x*x+y*y+z*z);
}

Vector4 Vector4::normalize()
{
	float l = length();
	if (l < 0.000001f || l > 0.000001f)
    {
		return Vector4(0,0,0,0);
    }
	x /= l;
	y /= l;
	z /= l;
    a /= l;

	return Vector4(x, y, z,a);
}

float Vector4::dotProduct(Vector4 &vect)
{
	return x*vect.x + y*vect.y + z*vect.z;
}

Vector4 Vector4::crossProduct(Vector4 &vect)
{
	float tx = y*vect.z-z*vect.y;
	float ty = x*vect.z-z*vect.x;
	float tz = x*vect.y-y*vect.x;
	return Vector4(tx, -ty, tz, a);
}
Vector4 Vector4::operator + (Vector4 const &vect)
{
	float tx, ty, tz, ta;
	tx = x + vect.x;
	ty = y + vect.y;
	tz = z + vect.z;
    ta = a + vect.a;

	return Vector4(tx, ty, tz, ta);
}
Vector4 Vector4::operator - (Vector4 const &vect)
{
	float tx, ty, tz, ta;
	tx = x - vect.x;
	ty = y - vect.y;
	tz = z - vect.z;
    ta = a - vect.a;

	return Vector4(tx, ty, tz, ta);
}
Vector4 Vector4::operator += (Vector4 const &vect)
{
	x += vect.x;
	y += vect.y;
	z += vect.z;
    a += vect.a;

	return Vector4(x, y, z, a);
}
Vector4 Vector4::operator -= (Vector4 const &vect)
{
	x -= vect.x;
	y -= vect.y;
	z -= vect.z;
    a -= vect.a;

	return Vector4(x, y, z, a);
}
Vector4 Vector4::operator * (Matrix const &mat)
{
	float tx, ty, tz;
	tx = mat.data[0][0]*x + mat.data[0][1]*y + mat.data[0][2]*z;
	ty = mat.data[1][0]*x + mat.data[1][1]*y + mat.data[1][2]*z;
	tz = mat.data[2][0]*x + mat.data[2][1]*y + mat.data[2][2]*z;
	return Vector4(tx, ty, tz, a);
}

Vector4	Vector4::operator *= (Matrix const &mat)
{
	float tx, ty, tz;
	tx = mat.data[0][0]*x + mat.data[0][1]*y + mat.data[0][2]*z;
	ty = mat.data[1][0]*x + mat.data[1][1]*y + mat.data[1][2]*z;
	tz = mat.data[2][0]*x + mat.data[2][1]*y + mat.data[2][2]*z;

	x = tx;
	y = ty;
	z = tz;
	return Vector4(x, y, z, a);
}
Vector4 Vector4::operator * (float const &value)
{
	float tx, ty, tz, ta;
	tx = x * value;
	ty = y * value;
	tz = z * value;
    ta = a * value;

	return Vector4(tx, ty, tz, ta);
}
Vector4 Vector4::operator *= (float const &value)
{
	x *= value;
	y *= value;
	z *= value;
    a *= value;

	return Vector4(x, y, z, a);
}
Vector4 Vector4::operator / (float const &value)
{
	if(value <= 0) return Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	float tx, ty, tz, ta;
	tx = x / value;
	ty = y / value;
	tz = z / value;
    ta = a / value;

	return Vector4(tx, ty, tz, ta);
}
bool Vector4::operator == (Vector4 const &vect)
{
	if (x == vect.x && y == vect.y && z == vect.z)
		return true;
	else return false;

}

void Vector4::operator = (Vector4 const &vect)
{
	x = vect.x;
	y = vect.y;
	z = vect.z;
}

//--------------------------------------------------------------------------------------------
//  Vector3 class code
//--------------------------------------------------------------------------------------------

Vector3::Vector3()
{
	x = y = z = 0.0f;
}
Vector3::Vector3(float px, float py, float pz)
{
	x = px;
	y = py;
	z = pz;
}
Vector3::Vector3(Vector3 &example)
{
	x = example.x;
	y = example.y;
	z = example.z;
}
Vector3::Vector3(Vector4 &example)
{
	x = example.x;
	y = example.y;
	z = example.z;
}

Vector3::~Vector3()
{

}

float Vector3::length()
{
	return (float)sqrt(x*x+y*y+z*z);
}

Vector3 Vector3::normalize()
{
	float l = length();
	if (l==0)
		return Vector3(0,0,0);
	x /= l;
	y /= l;
	z /= l;

	return Vector3(x, y, z);
}

float Vector3::dotProduct(Vector3 &vect)
{
	return x*vect.x + y*vect.y + z*vect.z;
}

Vector3 Vector3::crossProduct(Vector3 &vect)
{
	float tx = y*vect.z-z*vect.y;
	float ty = x*vect.z-z*vect.x;
	float tz = x*vect.y-y*vect.x;
	return Vector3(tx, -ty, tz);
}
Vector3 Vector3::operator + (Vector3 const &vect)
{
	float tx, ty, tz;
	tx = x + vect.x;
	ty = y + vect.y;
	tz = z + vect.z;

	return Vector3(tx, ty, tz);
}
Vector3 Vector3::operator - (Vector3 const &vect)
{
	float tx, ty, tz;
	tx = x - vect.x;
	ty = y - vect.y;
	tz = z - vect.z;
	return Vector3(tx, ty, tz);
}
Vector3 Vector3::operator += (Vector3 const &vect)
{
	x += vect.x;
	y += vect.y;
	z += vect.z;

	return Vector3(x, y, z);
}
Vector3 Vector3::operator -= (Vector3 const &vect)
{
	x -= vect.x;
	y -= vect.y;
	z -= vect.z;

	return Vector3(x, y, z);
}
Vector3	Vector3::operator * (Matrix const &mat)
{
	float tx, ty, tz;
	tx = mat.data[0][0]*x + mat.data[0][1]*y + mat.data[0][2]*z;
	ty = mat.data[1][0]*x + mat.data[1][1]*y + mat.data[1][2]*z;
	tz = mat.data[2][0]*x + mat.data[2][1]*y + mat.data[2][2]*z;
	return Vector3(tx, ty, tz);
}

Vector3	Vector3::operator *= (Matrix const &mat)
{
	float tx, ty, tz;
	tx = mat.data[0][0]*x + mat.data[0][1]*y + mat.data[0][2]*z;
	ty = mat.data[1][0]*x + mat.data[1][1]*y + mat.data[1][2]*z;
	tz = mat.data[2][0]*x + mat.data[2][1]*y + mat.data[2][2]*z;

	x = tx;
	y = ty;
	z = tz;
	return Vector3(x, y, z);
}
Vector3 Vector3::operator * (float const &value)
{
	float tx, ty, tz;
	tx = x * value;
	ty = y * value;
	tz = z * value;

	return Vector3(tx, ty, tz);
}
Vector3 Vector3::operator *= (float const &value)
{
	x *= value;
	y *= value;
	z *= value;

	return Vector3(x, y, z);
}
Vector3 Vector3::operator / (float const &value)
{
	if(value <= 0) return Vector3(0.0f, 0.0f, 0.0f);

	float tx, ty, tz;
	tx = x / value;
	ty = y / value;
	tz = z / value;

	return Vector3(tx, ty, tz);
}
bool Vector3::operator == (Vector3 const &vect)
{
	if (x == vect.x && y == vect.y && z == vect.z)
		return true;
	else return false;

}

void Vector3::operator = (Vector3 const &vect)
{
	x = vect.x;
	y = vect.y;
	z = vect.z;
}
