/// @fn Math.h
/// @brief É}ÉX
/// @date 2015/10/02
/// @author syuki nishida
#pragma once

#include <math.h>

namespace snlib {

const float PI = 3.141592654f;

/// @struct Vector2
struct Vector2{
public:
  inline Vector2() {}
  inline Vector2(float* p) : x(p[0]), y(p[1]) {}
  inline Vector2(float x, float y) : x(x), y(y) {}

  inline operator float* () { return &x; }
  inline operator const float* () { return &x; }

  inline Vector2& operator += (const Vector2& a) { x+=a.x; y+=a.y; return *this; }
  inline Vector2& operator -= (const Vector2& a) { x-=a.x; y-=a.y; return *this; }
  inline Vector2& operator *= (float a) { x *= a; y *= a; return *this; }
  inline Vector2& operator /= (float a) { x /= a; y /= a; return *this; }

  inline Vector2 operator + () const { return *this; }
  inline Vector2 operator - () const { Vector2 r(-x, -y); return r; }

  inline Vector2 operator + (const Vector2& a) const{ return Vector2(x+a.x,y+a.y); }
  inline Vector2 operator - (const Vector2& a) const{ return Vector2(x-a.x,y-a.y); }
  inline Vector2 operator * (float a) const { return Vector2(x * a, y * a); }
  inline Vector2 operator / (float a) const { return Vector2(x / a, y / a); }

  inline friend Vector2 operator*(float f,const Vector2& v)
  { return Vector2(f*v.x, f*v.y); }

  inline bool operator == (const Vector2& a) const { return x == a.x && y == a.y; }
  inline bool operator != (const Vector2& a) const { return x != a.x && y != a.y; }

  float x, y;
};

/// @struct Vector3
struct Vector3 {
public:
  inline Vector3() {}
  inline Vector3(const float* p) : x(p[0]), y(p[1]), z(p[2]) {}
  inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

  inline operator float* () { return &x; }
  inline operator const float* () const { return &x; }

  inline Vector3& operator += (const Vector3& a){x+=a.x;y+=a.y;z+=a.z;return *this;}
  inline Vector3& operator -= (const Vector3& a){x-=a.x;y-=a.y;z-=a.z;return *this;}
  inline Vector3& operator *= (float a) { x *= a; y *= a; z *= a; return *this; }
  inline Vector3& operator /= (float a) { x /= a; y /= a; z /= a; return *this; }

  inline Vector3 operator + () const { return *this; }
  inline Vector3 operator - () const { return Vector3(-x, -y, -z); }

  inline Vector3 operator+(const Vector3&a)const{return Vector3(x+a.x,y+a.y,z+a.z);}
  inline Vector3 operator-(const Vector3&a)const{return Vector3(x-a.x,y-a.y,z-a.z);}
  inline Vector3 operator * (float a) const{ return Vector3(x*a, y*a, z*a); }
  inline Vector3 operator / (float a) const{ return Vector3(x/a, y/a, z/a); }

  inline friend Vector3 operator * (float f, const struct Vector3& v)
  { return Vector3(v.x * f, v.y * f, v.z * f); }

  inline bool operator == (const Vector3& a) const{ return x==a.x&&y==a.y&&z==a.z; }
  inline bool operator != (const Vector3& a) const{ return x!=a.x&&y!=a.y&&z!=a.z; }

  inline float Length() {
    return sqrt(x * x + y * y + z * z);
  }

  inline Vector3& Normalize() {
    *this *= 1.f / Length();
    return *this;
  }

  inline Vector3& Cross(Vector3 l, Vector3 r) {
    x = (l.y * r.z - l.z * r.y);
    y = (l.z * r.x - l.x * r.z);
    z = (l.x * r.y - l.y * r.x);
    return *this;
  }

  inline float Dot(Vector3 l, Vector3 r) {
    return l.x * r.x + l.y * r.y + l.z * r.z;
  }

  float x, y, z;
};

struct Vector4 {
public:
  inline Vector4() {}
  inline Vector4(const float* p) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) {}
  inline Vector4(const Vector3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w){}
  inline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  inline operator float* () { return &x; }
  inline operator const float* () const { return &x; }

  inline Vector4& operator += (const Vector4& v)
  { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
  inline Vector4& operator -= (const Vector4& v)
  { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
  inline Vector4& operator *= (float f)
  { x *= f; y *= f; z *= f; w *= f; return *this; }
  inline Vector4& operator /= (float f)
  { x /= f; y /= f; z /= f; w /= f; return *this; }

  inline Vector4 operator + () const { return *this; }
  inline Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }

  inline Vector4 operator + (const Vector4& v) const
  { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
  inline Vector4 operator - (const Vector4& v) const
  { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
  inline Vector4 operator * (float f) const
  { return Vector4(x * f, y * f, z * f, w * f); }
  inline Vector4 operator / (float f) const
  { return Vector4(x / f, y / f, z / f, w / f); }

  inline friend Vector4 operator * (float f, const Vector4& v)
  { return Vector4(v.x * f, v.y * f, v.z * f, v.w * f); }

  inline bool operator == (const Vector4& v) const
  { return x == v.x && y == v.y && z == v.z && w == v.w; }
  inline bool operator != (const Vector4& v) const
  { return x != v.x && y != v.y && z != v.z && w != v.w; }

  float x, y, z, w;
};

struct Matrix{
public:
  inline Matrix() {}
  inline Matrix(const float* p) :
    _11(p[0]),  _12(p[1]),  _13(p[2]),  _14(p[3]),
    _21(p[4]),  _22(p[5]),  _23(p[6]),  _24(p[7]),
    _31(p[8]),  _32(p[9]),  _33(p[10]), _34(p[11]),
    _41(p[12]), _42(p[13]), _43(p[14]), _44(p[15]) {}
  inline Matrix(float _11, float _12, float _13, float _14,
    float _21, float _22, float _23, float _24,
    float _31, float _32, float _33, float _34,
    float _41, float _42, float _43, float _44) :
    _11(_11), _12(_12), _13(_13), _14(_14),
    _21(_21), _22(_22), _23(_23), _24(_24),
    _31(_31), _32(_32), _33(_33), _34(_34),
    _41(_41), _42(_42), _43(_43), _44(_44) {}

  inline float& operator () (unsigned int row, unsigned int col)
  { return m[row][col]; }
  inline float operator () (unsigned int row, unsigned int col) const
  { return m[row][col]; }

  inline operator float* () { return &_11; }
  inline operator const float* () const { return &_11; }

  inline Matrix& operator *= (const Matrix& m) {
    Matrix b(*this);
    _11 = b._11 * m._11 + b._12 * m._21 + b._13 * m._31 + b._14 * m._41;
    _12 = b._11 * m._12 + b._12 * m._22 + b._13 * m._32 + b._14 * m._42;
    _13 = b._11 * m._13 + b._12 * m._23 + b._13 * m._33 + b._14 * m._43;
    _14 = b._11 * m._14 + b._12 * m._24 + b._13 * m._34 + b._14 * m._44;
    _21 = b._21 * m._11 + b._22 * m._21 + b._23 * m._31 + b._24 * m._41;
    _22 = b._21 * m._12 + b._22 * m._22 + b._23 * m._32 + b._24 * m._42;
    _23 = b._21 * m._13 + b._22 * m._23 + b._23 * m._33 + b._24 * m._43;
    _24 = b._21 * m._14 + b._22 * m._24 + b._23 * m._34 + b._24 * m._44;
    _31 = b._31 * m._11 + b._32 * m._21 + b._33 * m._31 + b._34 * m._41;
    _32 = b._31 * m._12 + b._32 * m._22 + b._33 * m._32 + b._34 * m._42;
    _33 = b._31 * m._13 + b._32 * m._23 + b._33 * m._33 + b._34 * m._43;
    _34 = b._31 * m._14 + b._32 * m._24 + b._33 * m._34 + b._34 * m._44;
    _41 = b._41 * m._11 + b._42 * m._21 + b._43 * m._31 + b._44 * m._41;
    _42 = b._41 * m._12 + b._42 * m._22 + b._43 * m._32 + b._44 * m._42;
    _43 = b._41 * m._13 + b._42 * m._23 + b._43 * m._33 + b._44 * m._43;
    _44 = b._41 * m._14 + b._42 * m._24 + b._43 * m._34 + b._44 * m._44;
    return *this;}
  inline Matrix& operator += (const Matrix& m) {
    _11 += m._11; _12 += m._12; _13 += m._13; _14 += m._14;
    _21 += m._21; _22 += m._22; _23 += m._23; _24 += m._24;
    _31 += m._31; _32 += m._32; _33 += m._33; _34 += m._34;
    _41 += m._41; _42 += m._42; _43 += m._43; _44 += m._44;
    return *this; }
  inline Matrix& operator -= (const Matrix& m) {
    _11 -= m._11; _12 -= m._12; _13 -= m._13; _14 -= m._14;
    _21 -= m._21; _22 -= m._22; _23 -= m._23; _24 -= m._24;
    _31 -= m._31; _32 -= m._32; _33 -= m._33; _34 -= m._34;
    _41 -= m._41; _42 -= m._42; _43 -= m._43; _44 -= m._44;
    return *this; }
  inline Matrix& operator *= (float f) {
    _11 *= f; _12 *= f; _13 *= f; _14 *= f;
    _21 *= f; _22 *= f; _23 *= f; _24 *= f;
    _31 *= f; _32 *= f; _33 *= f; _34 *= f;
    _41 *= f; _42 *= f; _43 *= f; _44 *= f;
    return *this; }
  inline Matrix& operator /= (float f) {
    _11 /= f; _12 /= f; _13 /= f; _14 /= f;
    _21 /= f; _22 /= f; _23 /= f; _24 /= f;
    _31 /= f; _32 /= f; _33 /= f; _34 /= f;
    _41 /= f; _42 /= f; _43 /= f; _44 /= f;
    return *this;}

  inline Matrix operator + () const { return *this; }
  inline Matrix operator - () const { return Matrix(
    -_11, -_12, -_13, -_14,
    -_21, -_22, -_23, -_24,
    -_31, -_32, -_33, -_34,
    -_41, -_42, -_43, -_44); }

  inline Matrix operator * (const Matrix& m) const { return Matrix(
    _11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41,
    _11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42,
    _11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43,
    _11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44,
    _21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41,
    _21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42,
    _21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43,
    _21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44,
    _31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41,
    _31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42,
    _31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43,
    _31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44,
    _41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41,
    _41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42,
    _41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43,
    _41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44); }
  inline Matrix operator + (const Matrix& m) const { return Matrix(
    _11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14,
    _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
    _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
    _41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44); }
  inline Matrix operator - (const Matrix& m) const{ return Matrix(
    _11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14,
    _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
    _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
    _41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44); }
  inline Matrix operator * (float f) const { return Matrix(
    _11 * f, _12 * f, _13 * f, _14 * f,
    _21 * f, _22 * f, _23 * f, _24 * f,
    _31 * f, _32 * f, _33 * f, _34 * f,
    _41 * f, _42 * f, _43 * f, _44 * f); }
  inline Matrix operator / (float f) const{ return Matrix(
    _11 / f, _12 / f, _13 / f, _14 / f,
    _21 / f, _22 / f, _23 / f, _24 / f,
    _31 / f, _32 / f, _33 / f, _34 / f,
    _41 / f, _42 / f, _43 / f, _44 / f); }

  inline friend Matrix operator * (float f, const Matrix& m){ return Matrix(
    m._11 * f, m._12 * f, m._13 * f, m._14 * f,
    m._21 * f, m._22 * f, m._23 * f, m._24 * f,
    m._31 * f, m._32 * f, m._33 * f, m._34 * f,
    m._41 * f, m._42 * f, m._43 * f, m._44 * f); }

  inline bool operator != (const Matrix& m) const { return 
    _11 != m._11 && _12 != m._12 && _13 != m._13 && _14 != m._14 &&
    _21 != m._21 && _22 != m._22 && _23 != m._23 && _24 != m._24 &&
    _31 != m._31 && _32 != m._32 && _33 != m._33 && _34 != m._34 &&
    _41 != m._41 && _42 != m._42 && _43 != m._43 && _44 != m._44; }
  inline bool operator == (const Matrix& m) const { return 
    _11 == m._11 && _12 == m._12 && _13 == m._13 && _14 == m._14 &&
    _21 == m._21 && _22 == m._22 && _23 == m._23 && _24 == m._24 &&
    _31 == m._31 && _32 == m._32 && _33 == m._33 && _34 == m._34 &&
    _41 == m._41 && _42 == m._42 && _43 == m._43 && _44 == m._44; }

  inline Matrix& SetIdentity() {
    _11 = 1; _12 = 0; _13 = 0; _14 = 0;
    _21 = 0; _22 = 1; _23 = 0; _24 = 0;
    _31 = 0; _32 = 0; _33 = 1; _34 = 0;
    _41 = 0; _42 = 0; _43 = 0; _44 = 1;
    return *this;
  }

  inline Matrix& SetTranslate(Vector3 pos) {
    SetTranslate(pos.x, pos.y, pos.z);
  }

  inline Matrix& SetTranslate(float x, float y, float z) {
    _11 = 1; _12 = 0; _13 = 0; _14 = 0;
    _21 = 0; _22 = 1; _23 = 0; _24 = 0;
    _31 = 0; _32 = 0; _33 = 1; _34 = 0;
    _41 = x; _42 = y; _43 = z; _44 = 1;
    return *this;
  }

  inline Matrix& SetScale(float s) {
    return SetScale(s, s, s);
  }

  inline Matrix& SetScale(Vector3 v) {
    return SetScale(v.x, v.y, v.z);
  }

  inline Matrix& SetScale(float x, float y, float z) {
    _11 = x; _12 = 0; _13 = 0; _14 = 0;
    _21 = 0; _22 = y; _23 = 0; _24 = 0;
    _31 = 0; _32 = 0; _33 = z; _34 = 0;
    _41 = 0; _42 = 0; _43 = 0; _44 = 1;
    return *this;
  }

  inline Matrix& SetView(Vector3 eye, Vector3 at, Vector3 up) {
    Vector3 z = (at - eye).Normalize();
    Vector3 x = Vector3().Cross(up, z).Normalize();
    Vector3 y = Vector3().Cross(z, x);
    _11 = x.x;                    _12 = y.x;                    _13 = z.x;                    _14 = 0;
    _21 = x.y;                    _22 = y.y;                    _23 = z.y;                    _24 = 0;
    _31 = x.z;                    _32 = y.z;                    _33 = z.z;                    _34 = 0;
    _41 = -Vector3().Dot(x, eye); _42 = -Vector3().Dot(y, eye); _43 = -Vector3().Dot(z, eye); _44 = 1;
    return *this;
  }

  inline Matrix PerthL(float w, float h, float zn, float zf) {
    _11 = 2 * zn / w; _12 = 0;          _13 = 0;                 _14 = 0;
    _21 = 0;          _22 = 2 * zn / h; _23 = 0;                 _24 = 0;
    _31 = 0;          _32 = 0;          _33 = zf / (zf - zn);    _34 = 1;
    _41 = 0;          _42 = 0;          _43 = zn*zf / (zn - zf); _44 = 0;
    return *this;
  }

  inline Matrix PerthFovL(float fovY, float aspect, float zn, float zf) {
    float h = 1.f / tanf(fovY / 2.f);
    float w = h / aspect;
    _11 = w; _12 = 0; _13 = 0;                    _14 = 0;
    _21 = 0; _22 = h; _23 = 0;                    _24 = 0;
    _31 = 0; _32 = 0; _33 = zf / (zf - zn);       _34 = 1;
    _41 = 0; _42 = 0; _43 = -zn * zf / (zf - zn); _44 = 0;
    return *this;
  }

  inline Matrix& OrthoL(float w, float h, float zn, float zf) {
    _11 = 2 / w; _12 = 0;     _13 = 0;             _14 = 0;
    _21 = 0;     _22 = 2 / h; _23 = 0;             _24 = 0;
    _31 = 0;     _32 = 0;     _33 = 1 / (zf - zn); _34 = 0;
    _41 = 0;     _42 = 0;     _43 = zn / (zn - zf);_44 = 1;
    return *this;
  }

  inline Matrix& RotateY(float angle) {
    Matrix work;
    work.SetIdentity();

    work._11 = work._33 = cosf(angle);
    work._13 = work._31 = sinf(angle);
    work._13 *= -1;

    *this = *this * work;

    return *this;
  }

  union {
    struct {
      float _11, _12, _13, _14;
      float _21, _22, _23, _24;
      float _31, _32, _33, _34;
      float _41, _42, _43, _44;
    };
    float m[4][4];
  };
};

struct Quaternion {
public:
  inline Quaternion() {}
  inline Quaternion(const float* p) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) {}
  inline Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

  inline operator float* () { return &x; }
  inline operator const float* () const { return &x; }

  inline Quaternion& operator += (const Quaternion& q)
  { x += q.x; y += q.y; z += q.z; w += q.w; return *this; }
  inline Quaternion& operator -= (const Quaternion& q)
  { x -= q.x; y -= q.y; z -= q.z; w -= q.w; return *this; }
  inline Quaternion& operator *= (const Quaternion& q) {
    Quaternion b(*this);
    x = z * q.y - y * q.z + x * q.w + w * q.x;
    y = x * q.z - z * q.x + y * q.w + w * q.y;
    z = y * q.x - x * q.y + z * q.w + w * q.z;
    w = w * q.w - x * q.x - y * q.y - z * q.z;
    return *this; }

  inline Quaternion& operator *= (float f)
  { x *= f; y *= f; z *= f; w *= f; return *this; }
  inline Quaternion& operator /= (float f)
  { x /= f; y /= f; z /= f; w /= f; return *this; }

  inline Quaternion  operator + () const { return *this; }
  inline Quaternion  operator - () const { return Quaternion(-x, -y, -z, -w); }

  inline Quaternion operator + (const Quaternion& q) const
  { return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w); }
  inline Quaternion operator - (const Quaternion& q) const
  { return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w); }
  inline Quaternion operator * (const Quaternion& q) const
  { return Quaternion(
    z * q.y - y * q.z + x * q.w + w * q.x,
    x * q.z - z * q.x + y * q.w + w * q.y,
    y * q.x - x * q.y + z * q.w + w * q.z,
    w * q.w - x * q.x - y * q.y - z * q.z); }
  inline Quaternion operator * (float f) const
  { return Quaternion(x * f, y * f, z * f, w * f); }
  inline Quaternion operator / (float f) const
  { return Quaternion(x / f, y / f, z / f, w / f); }

  inline friend Quaternion operator * (float f, const Quaternion& q)
  { return Quaternion(q.x*f, q.y*f, q.z*f, q.w*f); }

  inline bool operator == (const Quaternion& q) const
  { return x == q.x && y == q.y && z == q.z && w == q.w; }
  inline bool operator != (const Quaternion& q) const
  { return x != q.x && y != q.y && z != q.z && w != q.w; }

  float x, y, z, w;
};

} // namespace snlib

//EOF