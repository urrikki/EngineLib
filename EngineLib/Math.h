#pragma once
#include "framework.h"

typedef int i32;

class  Rect
{
public:
	Rect() {}
	Rect(i32 width, i32 height) : width(width), height(height) {}
	Rect(i32 left, i32 top, i32 width, i32 height) :left(left), top(top), width(width), height(height) {}
	Rect(const  Rect& rect) :left(rect.left), top(rect.top), width(rect.width), height(rect.height) {}

public:
	i32 width = 0, height = 0, left = 0, top = 0;
};

//  Vecteur 2D
class  Vec2
{
public:
	Vec2() {}
	Vec2(float x, float y) :x(x), y(y) {}
	Vec2(const  Vec2& vector) :x(vector.x), y(vector.y) {}

	Vec2 operator *(float num) const
	{
		return Vec2(x * num, y * num);
	}
	Vec2 operator +(const  Vec2& vec) const
	{
		return Vec2(x + vec.x, y + vec.y);
	}
	Vec2 operator -(const  Vec2& vec) const
	{
		return Vec2(x - vec.x, y - vec.y);
	}

public:
	float x = 0.0f, y = 0.0f;
};

//  Vecteur 3D
class  Vec3
{
public:
	Vec3() {}
	Vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	Vec3(const  Vec3& vector) :x(vector.x), y(vector.y), z(vector.z) {}

	static Vec3 lerp(const  Vec3& start, const  Vec3& end, float delta)
	{
		Vec3 v;
		v.x = start.x * (1.0f - delta) + end.x * (delta);
		v.y = start.y * (1.0f - delta) + end.y * (delta);
		v.z = start.z * (1.0f - delta) + end.z * (delta);
		return v;
	}

	Vec3 operator *(float num) const
	{
		return Vec3(x * num, y * num, z * num);
	}

	Vec3 operator +(const  Vec3& vec) const
	{
		return Vec3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vec3 operator -(const  Vec3& vec) const
	{
		return Vec3(x - vec.x, y - vec.y, z - vec.z);
	}

	float length() const
	{
		return (float)sqrt((x * x) + (y * y) + (z * z));
	}

	static Vec3 normalize(const  Vec3& vec)
	{
		Vec3 res;
		float len = (float)sqrt((float)(vec.x * vec.x) + (float)(vec.y * vec.y) + (float)(vec.z * vec.z));
		if (!len)
			return Vec3();

		res.x = vec.x / len;
		res.y = vec.y / len;
		res.z = vec.z / len;

		return res;
	}

	static Vec3 cross(const  Vec3& v1, const  Vec3& v2)
	{
		Vec3 res;
		res.x = (v1.y * v2.z) - (v1.z * v2.y);
		res.y = (v1.z * v2.x) - (v1.x * v2.z);
		res.z = (v1.x * v2.y) - (v1.y * v2.x);
		return res;
	}

public:
	float x = 0.0f, y = 0.0f, z = 0.0f;
};

//  Vecteur 4D
class  Vec4
{
public:
	Vec4() {}
	Vec4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
	Vec4(const  Vec4& vector) :x(vector.x), y(vector.y), z(vector.z), w(vector.w) {}
	Vec4(const  Vec3& vector) :x(vector.x), y(vector.y), z(vector.z), w(1.0) {}

	void cross(Vec4& v1, Vec4& v2, Vec4& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

public:
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};

// Matrice 4x4 =  Identité, translation, mise a l'échelle, rotation, determinant, invertion de matrice
class  Mat
{
public:
	Mat()
	{
		setIdentity();
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(m_mat));
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const  Vec3& translation)
	{
		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;
	}

	void setScale(const  Vec3& scale)
	{
		m_mat[0][0] = (float)scale.x;
		m_mat[1][1] = (float)scale.y;
		m_mat[2][2] = (float)scale.z;
	}

	void setRotationX(float x)
	{
		m_mat[1][1] = (float)cos(x);
		m_mat[1][2] = (float)sin(x);
		m_mat[2][1] = (float)-sin(x);
		m_mat[2][2] = (float)cos(x);
	}

	void setRotationY(float y)
	{
		m_mat[0][0] = (float)cos(y);
		m_mat[0][2] = (float)-sin(y);
		m_mat[2][0] = (float)sin(y);
		m_mat[2][2] = (float)cos(y);
	}

	void setRotationZ(float z)
	{
		m_mat[0][0] = (float)cos(z);
		m_mat[0][1] = (float)sin(z);
		m_mat[1][0] = (float)-sin(z);
		m_mat[1][1] = (float)cos(z);
	}

	float getDeterminant()
	{
		Vec4 minor, v1, v2, v3;
		float det = 0.0f;

		v1 = Vec4(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vec4(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vec4(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z +
			this->m_mat[3][3] * minor.w);
		return det;
	}

	void inverse()
	{
		i32 a = 0, i = 0, j = 0;
		Mat out = {};
		Vec4 v = {}, vec[3] = {};
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->m_mat[j][0]);
					vec[a].y = (this->m_mat[j][1]);
					vec[a].z = (this->m_mat[j][2]);
					vec[a].w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = (float)pow(-1.0f, i) * v.x / det;
			out.m_mat[1][i] = (float)pow(-1.0f, i) * v.y / det;
			out.m_mat[2][i] = (float)pow(-1.0f, i) * v.z / det;
			out.m_mat[3][i] = (float)pow(-1.0f, i) * v.w / det;
		}

		this->setMatrix(out);
	}


	void operator *=(const  Mat& m_matrix)
	{
		Mat out;
		for (i32 i = 0; i < 4; i++)
		{
			for (i32 j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * m_matrix.m_mat[0][j] + m_mat[i][1] * m_matrix.m_mat[1][j] +
					m_mat[i][2] * m_matrix.m_mat[2][j] + m_mat[i][3] * m_matrix.m_mat[3][j];
			}
		}
		setMatrix(out);
	}

	void setMatrix(const  Mat& m_matrix)
	{
		::memcpy(m_mat, m_matrix.m_mat, sizeof(m_mat));
	}

	Vec3 getRightwardDirection()
	{
		return Vec3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	Vec3 getUpDirection()
	{
		return Vec3(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}
	Vec3 getForwardDirection()
	{
		return Vec3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}

	Vec3 getTranslation()
	{
		return Vec3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float yscale = 1.0f / (float)tan(fov / 2.0f);
		float xscale = yscale / aspect;
		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
		m_mat[3][3] = 0.0f;

	}


	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

public:
	float m_mat[4][4] = {};
};