#pragma once

#include <vector>
#include <stdexcept>

#include <Math/Common.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

namespace {
	class Matrix {
	protected:
		unsigned int mSize = 3;
		struct MatrixRow {
			Matrix* mat;
			int row = 0;

			MatrixRow(Matrix* mat, int row) : mat(mat), row(row) {};

			inline float& operator[](unsigned int i) {
				if (i > 0 && i <= mat->mSize) return mat->m[row][i];
				throw std::runtime_error("Matrix index out of range");
			}
		};

		void ResizeMatrix(unsigned int newSize = 0) {
			if (newSize) mSize = newSize;
			else newSize = mSize;

			m.resize(mSize);
			m.shrink_to_fit();
			for (auto& vec : m) {
				vec.resize(mSize);
				vec.shrink_to_fit();
			}
		}
	public:
		std::vector<std::vector<float>> m = { {0} };

		// Creates an identity matrix
		Matrix() {
			ResizeMatrix();
		};

		// Creates a matrix using another matrix of any size, starting from the top left corner.
		Matrix(const Matrix& mat) {
			m = mat.m;
			ResizeMatrix();
		}

		Matrix& operator=(const Matrix& other) {
			if (this != &other) {
				m = other.m;
				ResizeMatrix();
			}
			return *this;
		}

		inline MatrixRow operator[](unsigned int i) {
			if (i < 0 || i >= mSize) throw std::runtime_error("Matrix index out of range");
			return MatrixRow(this, i);
		}

		inline Matrix operator*(Matrix& other) {
			if (other.mSize != mSize) throw std::runtime_error("Matrix sizes do not match");
			Matrix result;
			result.ResizeMatrix(mSize);

			for (unsigned int i = 0; i < mSize; i++) {
				for (unsigned int j = 0; j < mSize; j++) {
					result[i][j] = 0;

					for (unsigned int k = 0; k < mSize; k++) {
						result[i][j] += m[i][k] * other[k][j];
					}
				}
			}
			return result;
		}

		inline void operator*=(Matrix& other) {
			if (other.mSize != mSize) throw std::runtime_error("Matrix sizes do not match");
			Matrix result;
			result.ResizeMatrix(mSize);

			for (unsigned int i = 0; i < mSize; i++) {
				for (unsigned int j = 0; j < mSize; j++) {
					result[i][j] = 0;

					for (unsigned int k = 0; k < mSize; k++) {
						result[i][j] += m[i][k] * other[k][j];
					}
				}
			}
			m = result.m;
		}
	};
}

namespace Refraction::Math {
	class Matrix3 : public Matrix {
	private:
		unsigned int mSize = 3;

		// Specialisation to allow flexibility with Vector3f
		struct Matrix3Row : public Matrix::MatrixRow {
			using MatrixRow::MatrixRow; // Use base constructor

			inline void operator=(const Vector3& v) {
				mat->m[row][0] = v.x;
				mat->m[row][1] = v.y;
				mat->m[row][2] = v.z;
			}
			
			inline operator const Vector3() {
				return GetVector();
			}

			inline Vector3 Cross(const Vector3& other) { return GetVector().Cross(other); };
		private:
			Vector3 GetVector() {
				return Vector3(mat->m[row][0], mat->m[row][1], mat->m[row][2]);
			}
		};
	public:
		using Matrix::Matrix;

		Matrix3& operator=(const Matrix3& other) {
			if (this != &other) {
				m = other.m;
				ResizeMatrix();
			}
			return *this;
		}

		inline Matrix3Row operator[](unsigned int i) {
			return Matrix3Row(this, i);
		}

		inline Matrix3 operator*(Matrix3 other) const;
		inline void operator*=(Matrix3 other);
	};

	class Matrix4 : public Matrix {
	private:
		unsigned int mSize = 4;

		// Specialisation to allow flexibility with Vector4f
		struct Matrix4Row : public Matrix::MatrixRow {
			using MatrixRow::MatrixRow; // Use base constructor

			inline void operator=(const Vector4& v) {
				mat->m[row][0] = v.x;
				mat->m[row][1] = v.y;
				mat->m[row][2] = v.z;
			}

			inline operator const Vector4() {
				return GetVector();
			}
		private:
			Vector4 GetVector() {
				return Vector4(mat->m[row][0], mat->m[row][1], mat->m[row][2], mat->m[row][3]);
			}
		};
	public:
		using Matrix::Matrix;

		Matrix4(const Vector3& from, const Vector3& at, const Vector3& up);

		static Matrix4 Perspective(float fovY, float aspectRatio, float zNear, float zFar);

		Matrix4& operator=(const Matrix4& other) {
			if (this != &other) {
				m = other.m;
				ResizeMatrix();
			}
			return *this;
		}

		inline Matrix4Row operator[](unsigned int i) {
			return Matrix4Row(this, i);
		}

		inline Matrix4 operator*(Matrix4 other) const;
		inline void operator*=(Matrix4 other);

		// Translate matrix using a Vector3f
		inline Matrix4 Translate(Vector3 v) {
			Matrix4 result(*this);
			Vector4 row0 = result[0];
			Vector4 row1 = result[1];
			Vector4 row2 = result[2];
			result[3] = Vector4(row0 * v.x + row1 * v.y + row2 * v.z + (*this)[3]);
			return result;
		}
		Matrix4 Rotate(const float& angle, Vector3 axis);
		Matrix4 Scale(Vector3 scale);
	};
}
