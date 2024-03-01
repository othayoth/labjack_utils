// math library derived from savannah
#ifndef SAVANNAH_MATH
#define SAVANNAH_MATH
#include "types.h"
#include <cmath>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

static inline int sign(int x) {
    return (x > 0) - (x < 0);
}

// scalar
static inline f32 absolute(f32 x) {
	if (x >= 0) {
		return x;
	} else {
		return -x;
	}
}

static inline f32 radians(f32 degrees) {
	return degrees * M_PI / 180.0;
}

static inline f32 degrees(f32 radians) {
	return radians * 180.0 / M_PI;
}

static inline f32 lerp(f32 a, f32 t, f32 b) {
	return ((1.0f - t) * a) + (t * b);
}

static inline f32 clamp(f32 a, f32 min_val, f32 max_val) {
	return fmin(fmax(a, min_val), max_val);
}

// vectors
struct vec2 {
	f32 x;
	f32 y;
};

static inline void print_vec(vec2 a) {
	printf("(x: %f, y: %f)", a.x, a.y);
}

// binary vec2 ops
static inline vec2 operator+(const vec2& a, const vec2& b) {
	return (vec2) {a.x + b.x, a.y + b.y};
}

static inline vec2 operator*(const vec2& a, const vec2& b) {
	return (vec2) {a.x * b.x, a.y * b.y};
}

static inline vec2 operator-(const vec2& a, const vec2& b) {
	return (vec2) {a.x - b.x, a.y - b.y};
}

static inline vec2 operator/(const vec2& a, const vec2& b) {
	return (vec2) {a.x / b.x, a.y / b.y};
}

// binary vec-scalar ops
static inline vec2 operator+(const vec2& a, const f32 b) {
	return (vec2) {a.x + b, a.y + b};
}

static inline vec2 operator*(const vec2& a, const f32 b) {
	return (vec2) {a.x * b, a.y * b};
}

static inline vec2 operator-(const vec2& a, const f32 b) {
	return (vec2) {a.x - b, a.y - b};
}

static inline vec2 operator/(const vec2& a, const f32 b) {
	return (vec2) {a.x / b, a.y / b};
}

// binary scalar-vec2 ops
static inline vec2 operator+(const f32 b, const vec2& a) {
	return (vec2) {b + a.x, b + a.y};
}

static inline vec2 operator*(const f32 b, const vec2& a) {
	return (vec2) {b * a.x, b * a.y};
}

static inline vec2 operator-(const f32 b, const vec2& a) {
	return (vec2) {b - a.x, b - a.y};
}

static inline vec2 operator/(const f32 b, const vec2& a) {
	return (vec2) {b / a.x, b / a.y};
}

// unary vec2 ops
static inline vec2 operator-(const vec2& a) {
	return (vec2) {-a.x, -a.y};
}

static inline vec2& operator+=(vec2& a, const vec2& b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

static inline vec2& operator-=(vec2& a, const vec2& b) {
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

static inline vec2& operator*=(vec2& a, const vec2& b) {
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

static inline vec2& operator/=(vec2& a, const vec2& b) {
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

static inline f32 dot(vec2 a, vec2 b) {
	return a.x * b.x + a.y * b.y;
}

static inline f32 l2_norm_squared(vec2 a) {
	return dot(a, a);
}

static inline f32 l2_norm(vec2 a) {
	return sqrt(l2_norm_squared(a));
}

static inline vec2 normalize(vec2 a) {
	f32 norm = l2_norm(a);
	vec2 a_normed = {a.x / norm, a.y / norm};
	return a_normed;
}

struct vec3 {
	f32 x;
	f32 y;
	f32 z;
};

static inline void print_vec(vec3 a) {
	printf("(x: %f, y: %f, z: %f)", a.x, a.y, a.z);
}

// binary vec3 ops
static inline vec3 operator+(const vec3& a, const vec3& b) {
	return (vec3) {a.x + b.x, a.y + b.y, a.z + b.z};
}

static inline vec3 operator*(const vec3& a, const vec3& b) {
	return (vec3) {a.x * b.x, a.y * b.y, a.z * b.z};
}

static inline vec3 operator-(const vec3& a, const vec3& b) {
	return (vec3) {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline vec3 operator/(const vec3& a, const vec3& b) {
	return (vec3) {a.x / b.x, a.y / b.y, a.z / b.z};
}

// binary vec-scalar ops
static inline vec3 operator+(const vec3& a, const f32 b) {
	return (vec3) {a.x + b, a.y + b, a.z + b};
}

static inline vec3 operator*(const vec3& a, const f32 b) {
	return (vec3) {a.x * b, a.y * b, a.z * b};
}

static inline vec3 operator-(const vec3& a, const f32 b) {
	return (vec3) {a.x - b, a.y - b, a.z - b};
}

static inline vec3 operator/(const vec3& a, const f32 b) {
	return (vec3) {a.x / b, a.y / b, a.z / b};
}

// binary scalar-vec3 ops
static inline vec3 operator+(const f32 b, const vec3& a) {
	return (vec3) {b + a.x, b + a.y, b + a.z};
}

static inline vec3 operator*(const f32 b, const vec3& a) {
	return (vec3) {b * a.x, b * a.y, b * a.z};
}

static inline vec3 operator-(const f32 b, const vec3& a) {
	return (vec3) {b - a.x, b - a.y, b - a.z};
}

static inline vec3 operator/(const f32 b, const vec3& a) {
	return (vec3) {b / a.x, b / a.y, b / a.z};
}

// unary vec3 ops
static inline vec3 operator-(const vec3& a) {
	return (vec3) {-a.x, -a.y, -a.z};
}

static inline vec3& operator+=(vec3& a, const vec3& b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

static inline vec3& operator-=(vec3& a, const vec3& b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

static inline vec3& operator*=(vec3& a, const vec3& b) {
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return a;
}

static inline vec3& operator/=(vec3& a, const vec3& b) {
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return a;
}

static inline vec3 vec3_from_f32(f32 b) {
	return (vec3) {b, b, b};
}

static inline f32 dot(vec3 a, vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline vec3 cross(vec3 a, vec3 b) {
	vec3 v = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
	return v;
}

static inline f32 l2_norm_squared(vec3 a) {
	return dot(a, a);
}

static inline f32 l2_norm(vec3 a) {
	return sqrt(l2_norm_squared(a));
}

static inline vec3 normalize(vec3 a) {
	f32 norm = l2_norm(a);
	vec3 a_normed = {a.x / norm, a.y / norm, a.z / norm};
	return a_normed;
}

static inline vec3 clamp(vec3 a, f32 min_val, f32 max_val) {
	return (vec3) {
		clamp(a.x, min_val, max_val),
		clamp(a.y, min_val, max_val),
		clamp(a.z, min_val, max_val),
	};
}

struct vec4 {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
};

static inline void print_vec(vec4 a) {
	printf("(x: %f, y: %f, z: %f w: %f)", a.x, a.y, a.z, a.w);
}

// binary vec4 ops
static inline vec4 operator+(const vec4& a, const vec4& b) {
	return (vec4) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static inline vec4 operator*(const vec4& a, const vec4& b) {
	return (vec4) {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

static inline vec4 operator-(const vec4& a, const vec4& b) {
	return (vec4) {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static inline vec4 operator/(const vec4& a, const vec4& b) {
	return (vec4) {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

// binary vec4-scalar ops
static inline vec4 operator+(const vec4& a, const f32 b) {
	return (vec4) {a.x + b, a.y + b, a.z + b, a.w + b};
}

static inline vec4 operator*(const vec4& a, const f32 b) {
	return (vec4) {a.x * b, a.y * b, a.z * b, a.w * b};
}

static inline vec4 operator-(const vec4& a, const f32 b) {
	return (vec4) {a.x - b, a.y - b, a.z - b, a.w - b};
}

static inline vec4 operator/(const vec4& a, const f32 b) {
	return (vec4) {a.x / b, a.y / b, a.z / b, a.w / b};
}

// binary scalar-vec4 ops
static inline vec4 operator+(const f32 b, const vec4& a) {
	return (vec4) {b + a.x, b + a.y, b + a.z, b + a.w};
}

static inline vec4 operator*(const f32 b, const vec4& a) {
	return (vec4) {b * a.x, b * a.y, b * a.z, b * a.w};
}

static inline vec4 operator-(const f32 b, const vec4& a) {
	return (vec4) {b - a.x, b - a.y, b - a.z, b - a.w};
}

static inline vec4 operator/(const f32 b, const vec4& a) {
	return (vec4) {b / a.x, b / a.y, b / a.z, b / a.w};
}

// unary vec4 ops
static inline vec4 operator-(const vec4& a) {
	return (vec4) {-a.x, -a.y, -a.z, -a.w};
}

static inline vec4& operator+=(vec4& a, const vec4& b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return a;
}

static inline vec4& operator-=(vec4& a, const vec4& b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return a;
}

static inline vec4& operator*=(vec4& a, const vec4& b) {
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	a.w *= b.w;
	return a;
}

static inline vec4& operator/=(vec4& a, const vec4& b) {
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	a.w /= b.w;
	return a;
}

static inline f32 dot(vec4 a, vec4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline f32 l2_norm_squared(vec4 a) {
	return dot(a, a);
}

static inline f32 l2_norm(vec4 a) {
	return sqrt(l2_norm_squared(a));
}

static inline vec4 normalize(vec4 a) {
	f32 norm = l2_norm(a);
	vec4 a_normed = {a.x / norm, a.y / norm, a.z / norm, a.w / norm};
	return a_normed;
}

static inline vec4 clamp(vec4 a, f32 min_val, f32 max_val) {
	return (vec4) {
		clamp(a.x, min_val, max_val),
		clamp(a.y, min_val, max_val),
		clamp(a.z, min_val, max_val),
		clamp(a.w, min_val, max_val),
	};
}

// quaternions
struct quat {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
};

static inline void print_quat(quat a) {
	printf("(x: %f, y: %f, z: %f, w: %f)\n", a.x, a.y, a.z, a.w);
}


// binary quat ops
static inline quat operator+(const quat& a, const quat& b) {
	return (quat) {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

static inline quat operator*(const quat& a, const quat& b) {
	return (quat) {
		a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x,
		-a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y,
		a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z,
		-a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w
	};
}

static inline quat operator-(const quat& a, const quat& b) {
	return (quat) {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

static inline quat operator/(const quat& a, const quat& b) {
	return (quat) {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

// binary quat-scalar ops
static inline quat operator*(const quat& a, const f32 b) {
	return (quat) {a.x * b, a.y * b, a.z * b, a.w * b};
}

static inline quat operator/(const quat& a, const f32 b) {
	return (quat) {a.x / b, a.y / b, a.z / b, a.w / b};
}

// binary scalar-quat ops
static inline quat operator*(const f32 b, const quat& a) {
	return (quat) {b * a.x, b * a.y, b * a.z, b * a.w};
}

static inline quat operator/(const f32 b, const quat& a) {
	return (quat) {b / a.x, b / a.y, b / a.z, b / a.w};
}

static inline f32 dot(quat a, quat b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline quat inv(quat a) {
	quat conjugate = (quat) {-a.x, -a.y, -a.z, a.w};
	f32 norm = sqrt(dot(a, a));
	f32 norm_squared = norm * norm;
	quat result = conjugate / norm_squared;
	return result;
}

static inline f32 l2_norm_squared(quat a) {
	return dot(a, a);
}

static inline f32 l2_norm(quat a) {
	return sqrt(l2_norm_squared(a));
}

static inline quat normalize(quat a) {
	f32 norm = l2_norm(a);
	quat a_normed = {a.x / norm, a.y / norm, a.z / norm, a.w / norm};
	return a_normed;
}

static inline quat nlerp(quat a, f32 t, quat b) {
	quat result = (quat) {
		lerp(a.x, t, b.x),
		lerp(a.y, t, b.y),
		lerp(a.z, t, b.z),
		lerp(a.w, t, b.w),
	};
	result = normalize(result);
	return result;
}

static inline quat slerp(quat a, f32 t, quat b) {
	f32 cos_theta = dot(a, b);
	f32 theta = acosf(cos_theta);
	f32 w_a = sinf((1.0f - t) * theta);
	f32 w_b = sinf(t * theta);
	f32 sin_theta = sin(theta);
	return ((w_a * (a)) + (w_b * (b))) / (sin_theta);
}

static inline quat quat_affine_rotate(vec3 axis, f32 angle) {
	f32 axis_norm = l2_norm(axis);
	f32 sin_theta = sinf(angle / 2.0f);
	vec3 rotated_axis = ((axis) * sin_theta) / axis_norm;
	quat result = (quat) {
		rotated_axis.x,
		rotated_axis.y,
		rotated_axis.z,
		cosf(angle / 2.0f)
	};
	return result;
}

// matrices
struct mat3 {
	f32 elements[3][3]; // column major
};

static inline void print_mat(mat3 a) {
	printf("mat3x3(");
	for (u32 column = 0; column < 3; column++) {
		printf(
			"(%0.8f, %0.8f, %0.8f)",
			a.elements[column][0],
			a.elements[column][1],
			a.elements[column][2]
		);
		if (column != 2) {
			printf(", ");
		}
	}
	printf(")\n");
}

// binary mat3 ops
static inline mat3 operator*(const mat3& a, const f32 b){
	mat3 result;
	for (u32 column = 0; column < 3; column++) {
		for (u32 row = 0; row < 3; row++) {
			result.elements[column][row] = a.elements[column][row] * b;
		}
	}
	return result;
}

static inline vec3 operator*(const mat3& a, const vec3& b) {
	vec3 result;
	result.x = a.elements[0][0] * b.x + a.elements[1][0] * b.y + a.elements[2][0] * b.z;
	result.y = a.elements[0][1] * b.x + a.elements[1][1] * b.y + a.elements[2][1] * b.z;
	result.z = a.elements[0][2] * b.x + a.elements[1][2] * b.y + a.elements[2][2] * b.z;
	return result;
}

static inline mat3 operator+=(mat3& a, mat3& b) {
	for (u32 column = 0; column < 3; column++) {
		for (u32 row = 0; row < 3; row++) {
			a.elements[column][row] = a.elements[column][row] + b.elements[column][row];
		}
	}
	return a;
}

static inline mat3 mat3_diagonal(vec3 a) {
	mat3 result = {0};
	result.elements[0][0] = a.x;
	result.elements[1][1] = a.y;
	result.elements[2][2] = a.z;
	return result;
}

static inline mat3 mat3_diagonal(f32 a) {
	mat3 result = {0};
	result.elements[0][0] = a;
	result.elements[1][1] = a;
	result.elements[2][2] = a;
	return result;
}

static inline mat3 operator*(const mat3& a, const mat3& b) {
	mat3 result;
	for (u32 column = 0; column < 3; column++) {
		for (u32 row = 0; row < 3; row++) {
			result.elements[column][row] = a.elements[column][row] * b.elements[column][row];
		}
	}
	return result;
}

static inline mat3 transpose(mat3 a) {
	mat3 result;
	for (u32 column = 0; column < 3; column++) {
		for (u32 row = 0; row < 3; row++) {
			result.elements[column][row] = a.elements[row][column];
		}
	}
	return result;
}

static inline mat3 star(vec3 a) {
	mat3 result = {0};
	result.elements[0][1] = a.z;
	result.elements[0][2] = -a.y;
	result.elements[1][0] = -a.z;
	result.elements[1][2] = a.x;  
	result.elements[2][0] = a.y;
	result.elements[2][1] = a.x;
	return result;
}

static inline mat3 column_normalize(mat3 a){
	mat3 result = {0};
	vec3 v_col;
	vec3 v_col_norm;
	for (u32 column = 0; column < 3; column++){
		v_col.x = a.elements[column][0];
		v_col.y = a.elements[column][1];
		v_col.z = a.elements[column][2];
		v_col_norm = normalize(v_col);
		result.elements[column][0] = v_col_norm.x;
		result.elements[column][1] = v_col_norm.y;
		result.elements[column][2] = v_col_norm.z;
	}
	return result;
}

static inline mat3 matmul(mat3 a, mat3 b) {
	mat3 result;
	for (u32 column = 0; column < 3; column++) {
		for (u32 row = 0; row < 3; row++) {
			f32 sum = 0.0;
			for (u32 i = 0; i < 3; i++) {
				sum += a.elements[i][row] * b.elements[column][i];
			}
			result.elements[column][row] = sum;
		}
	}
	return result;
}

static inline mat3 mat3_affine_rotate(vec3 axis, f32 angle) {
	vec3 unit_axis = normalize(axis);
	f32 x = unit_axis.x;
	f32 y = unit_axis.y;
	f32 z = unit_axis.z;
	f32 cos_theta = cosf(angle);
	f32 sin_theta = sinf(angle);
	mat3 result;
	result.elements[0][0] = (x * x * (1 - cos_theta)) + cos_theta;
	result.elements[0][1] = (x * y * (1 - cos_theta)) + (z * sin_theta);
	result.elements[0][2] = (x * z * (1 - cos_theta)) - (y * sin_theta);
	result.elements[1][0] = (y * x * (1 - cos_theta)) - (z * sin_theta);
	result.elements[1][1] = (y * y * (1 - cos_theta)) + cos_theta;
	result.elements[1][2] = (y * z * (1 - cos_theta)) + (x * sin_theta);
	result.elements[2][0] = (z * x * (1 - cos_theta)) + (y * sin_theta);
	result.elements[2][1] = (z * y * (1 - cos_theta)) - (x * sin_theta);
	result.elements[2][2] = (z * z * (1 - cos_theta)) + cos_theta;
	return result;
}

static inline mat3 mat3_from_quat(quat a) {
	mat3 result = {0};
	quat q = normalize(a);
	f32 xx = q.x * q.x;
	f32 yy = q.y * q.y;
	f32 zz = q.z * q.z;
	f32 xy = q.x * q.y;
	f32 xz = q.x * q.z;
	f32 yz = q.y * q.z;
	f32 wx = q.w * q.x;
	f32 wy = q.w * q.y;
	f32 wz = q.w * q.z;
	result.elements[0][0] = 1.0f - (2.0f * (yy + zz));
	result.elements[0][1] = 2.0f * (xy + wz);
	result.elements[0][2] = 2.0f * (xz - wy);
	result.elements[1][0] = 2.0f * (xy - wz);
	result.elements[1][1] = 1.0f - (2.0f * (xx + zz));
	result.elements[1][2] = 2.0f * (yz + wx);
	result.elements[2][0] = 2.0f * (xz + wy);
	result.elements[2][1] = 2.0f * (yz - wx);
	result.elements[2][2] = 1.0f - (2.0f * (xx + yy));
	return result;
}

struct mat4 {
	f32 elements[4][4]; // column major
};

static inline void print_mat(mat4 a) {
	printf("mat4x4(");
	for (u32 column = 0; column < 4; column++) {
		printf(
			"(%0.6f, %0.6f, %0.6f, %0.6f)",
			a.elements[column][0],
			a.elements[column][1],
			a.elements[column][2],
			a.elements[column][3]
		);
		if (column != 3) {
			printf(", ");
		}
	}
	printf(")\n");
}

// binary mat4 ops
static inline mat4 operator+(const mat4& a, const mat4& b) {
	mat4 result;
	for (u32 column = 0; column < 4; column++) {
		for (u32 row = 0; row < 4; row++) {
			result.elements[column][row] = a.elements[column][row] + b.elements[column][row];
		}
	}
	return result;
}

static inline mat4 operator*(const mat4& a, const mat4& b) {
	mat4 result;
	for (u32 column = 0; column < 4; column++) {
		for (u32 row = 0; row < 4; row++) {
			result.elements[column][row] = a.elements[column][row] * b.elements[column][row];
		}
	}
	return result;
}

static inline vec4 operator*(const mat4& a, const vec4& b) {
	vec4 result;
	result.x = a.elements[0][0] * b.x + a.elements[1][0] * b.y + a.elements[2][0] * b.z + a.elements[3][0] * b.w;
	result.y = a.elements[0][1] * b.x + a.elements[1][1] * b.y + a.elements[2][1] * b.z + a.elements[3][1] * b.w;
	result.z = a.elements[0][2] * b.x + a.elements[1][2] * b.y + a.elements[2][2] * b.z + a.elements[3][2] * b.w;
	result.w = a.elements[0][3] * b.x + a.elements[1][3] * b.y + a.elements[2][3] * b.z + a.elements[3][3] * b.w; 	
	return result;
}

static inline mat4 operator-(const mat4& a, const mat4& b) {
	mat4 result;
	for (u32 column = 0; column < 4; column++) {
		for (u32 row = 0; row < 4; row++) {
			result.elements[column][row] = a.elements[column][row] - b.elements[column][row];
		}
	}
	return result;
}

static inline mat4 operator/(const mat4& a, const mat4& b) {
	mat4 result;
	for (u32 column = 0; column < 4; column++) {
		for (u32 row = 0; row < 4; row++) {
			result.elements[column][row] = a.elements[column][row] / b.elements[column][row];
		}
	}
	return result;
}

static inline mat4 matmul(mat4 a, mat4 b) {
	mat4 result;
	for (u32 column = 0; column < 4; column++) {
		for (u32 row = 0; row < 4; row++) {
			f32 sum = 0.0;
			for (u32 i = 0; i < 4; i++) {
				sum += a.elements[i][row] * b.elements[column][i];
			}
			result.elements[column][row] = sum;
		}
	}
	return result;
}

static inline mat4 mat4_diagonal(vec4 a) {
	mat4 result = {0};
	result.elements[0][0] = a.x;
	result.elements[1][1] = a.y;
	result.elements[2][2] = a.y;
	result.elements[3][3] = a.w;
	return result;
}

static inline mat4 mat4_diagonal(f32 a) {
	mat4 result = {0};
	result.elements[0][0] = a;
	result.elements[1][1] = a;
	result.elements[2][2] = a;
	result.elements[3][3] = a;
	return result;
}

static inline mat4 mat4_perspective(f32 fov, f32 aspect_ratio, f32 near, f32 far) {
	mat4 result = {0};
	f32 cotangent = 1.0f / tanf(0.5 * fov);
	result.elements[0][0] = cotangent / aspect_ratio;
	result.elements[1][1] = cotangent;
	result.elements[2][3] = -1.0f;
	result.elements[2][2] = (near + far) / (near - far);
	result.elements[3][2] = (2.0f * near * far) / (near - far);
	return result;
}

static inline mat4 mat4_look_at(vec3 position, vec3 target, vec3 up) {
	mat4 result = mat4_diagonal(1.0f);
	vec3 direction = target - position;
	direction = normalize(direction);
	vec3 right = cross(direction, up);
	right = normalize(right);
	vec3 normalized_up = cross(right, direction);
	
	result.elements[0][0] = right.x;
	result.elements[0][1] = normalized_up.x;
	result.elements[0][2] = -direction.x;
	result.elements[0][3] = 0.0f;
	
	result.elements[1][0] = right.y;
	result.elements[1][1] = normalized_up.y;
	result.elements[1][2] = -direction.y;
	result.elements[1][3] = 0.0f;
	
	result.elements[2][0] = right.z;
	result.elements[2][1] = normalized_up.z;
	result.elements[2][2] = -direction.z;
	result.elements[2][3] = 0.0f;
	
	result.elements[3][0] = -dot(right, position);
	result.elements[3][1] = -dot(normalized_up, position);
	result.elements[3][2] = dot(direction, position);
	result.elements[3][3] = 1.0f;
	
	return result;
}

static inline mat4 mat4_affine_translate(vec3 translate) {
	mat4 result = mat4_diagonal(1.0f);
	result.elements[3][0] = translate.x;
	result.elements[3][1] = translate.y;
	result.elements[3][2] = translate.z;
	return result;
}

static inline mat4 mat4_affine_scale(vec3 scale) {
	mat4 result = mat4_diagonal(1.0f);
	result.elements[0][0] = scale.x;
	result.elements[1][1] = scale.y;
	result.elements[2][2] = scale.z;
	return result;
}

static inline mat4 homogenous(mat3 rotation) {
	mat4 result = {0};
	for (u32 column = 0; column < 3; column++) {
		for (u32 row = 0; row < 3; row++) {
			result.elements[column][row] = rotation.elements[column][row];
		}
	}
	result.elements[3][3] = 1.0f;
	return result;
}

static inline mat4 mat4_from_quat(quat a) {
	mat4 result = {0};
	quat q = normalize(a);
	f32 xx = q.x * q.x;
	f32 yy = q.y * q.y;
	f32 zz = q.z * q.z;
	f32 xy = q.x * q.y;
	f32 xz = q.x * q.z;
	f32 yz = q.y * q.z;
	f32 wx = q.w * q.x;
	f32 wy = q.w * q.y;
	f32 wz = q.w * q.z;
	result.elements[0][0] = 1.0f - (2.0f * (yy + zz));
	result.elements[0][1] = 2.0f * (xy + wz);
	result.elements[0][2] = 2.0f * (xz - wy);
	result.elements[1][0] = 2.0f * (xy - wz);
	result.elements[1][1] = 1.0f - (2.0f * (xx + zz));
	result.elements[1][2] = 2.0f * (yz + wx);
	result.elements[2][0] = 2.0f * (xz + wy);
	result.elements[2][1] = 2.0f * (yz - wx);
	result.elements[2][2] = 1.0f - (2.0f * (xx + yy));
	result.elements[3][3] = 1.0f;
	return result;
}

static inline mat4 mat4_affine_rotate(vec3 axis, f32 angle) {
	mat3 rotation_matrix = mat3_affine_rotate(axis, angle);
	return homogenous(rotation_matrix);
}

#endif // SAVANNAH_MATH
