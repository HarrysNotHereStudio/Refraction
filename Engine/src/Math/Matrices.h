#pragma once

#include <vector>
#include <stdexcept>
#include <string>

#include <Math/Common.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

namespace {
	class Matrix {
	protected:
		struct MatrixRow {
			Matrix* mat;
			size_t row = 0;

			MatrixRow(Matrix* mat, size_t row) : mat(mat), row(row) {};

			inline float& operator[](size_t i) {
				if (i >= 0 && i < mat->m.size()) return mat->m[row][i];
				throw std::runtime_error("Matrix index out of range");
			}
		};

		void ResizeMatrix(size_t newSize = 0) {
			if (newSize == 0) newSize = m.size();

			m.resize(newSize);
			m.shrink_to_fit();
			for (auto& vec : m) {
				vec.resize(m.size());
				vec.shrink_to_fit();
			}
		}
	public:
		std::vector<std::vector<float>> m = { {0} };

		// Creates an identity matrix
		Matrix(size_t size) {
			ResizeMatrix(size);
			for (size_t i = 0; i < m.size(); i++) {
				for (size_t j = 0; j < m.size(); j++) {
					if (i == j) m[i][j] = 1;
				}
			}
		};

		// Creates a matrix with an initialiser value
		Matrix(size_t size, float init) {
			ResizeMatrix(size);
			for (size_t i = 0; i < m.size(); i++) {
				for (size_t j = 0; j < m.size(); j++) {
					if(i == j) m[i][j] = init;
				}
			}
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

		inline MatrixRow operator[](size_t i) {
			if (i >= 0 && i < m.size()) return MatrixRow(this, i);
			throw std::runtime_error("Matrix index out of range");
		}

		inline Matrix operator*(Matrix& other) {
			if (other.m.size() != m.size()) throw std::runtime_error("Matrix sizes do not match");
			Matrix result(m.size());

			for (size_t i = 0; i < m.size(); i++) {
				for (size_t j = 0; j < m.size(); j++) {
					result[i][j] = 0;

					for (size_t k = 0; k < m.size(); k++) {
						result[i][j] += m[i][k] * other[k][j];
					}
				}
			}
			return result;
		}

		inline void operator*=(Matrix& other) { m = ((*this) * other).m; }

		inline Matrix Transpose() {
			Matrix result(m.size());

			for (size_t i = 0; i < m.size(); i++) {
				for (size_t j = 0; j < m.size(); j++) {
					result[i][j] = m[j][i];
				}
			}
			return result;
		}

		inline std::string ToString(bool pretty = true) const {
			std::string out;
			if (pretty) {
				for (size_t row = 0; row < m.size(); row++) {
					out += "{ ";
					for (size_t col = 0; col < m.size(); col++) {
						if (col > 0) out += ", ";
						out += std::to_string(m[row][col]);
					}
					out += " }\n";
				}
			} else {
				for (size_t row = 0; row < m.size(); row++) {
					if (row > 0) out += ", ";
					out += "{ ";
					for (size_t col = 0; col < m.size(); col++) {
						if (col > 0) out += ", ";
						out += std::to_string(m[row][col]);
					}
					out += " }";
				}
			}
			return out;
		};
	};
}

namespace Refraction::Math {
	class Quaternion;

	class Matrix3 : public Matrix {
	private:
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
		Matrix3() : Matrix(3) {}
		Matrix3(float init) : Matrix(3, init) {}

		Matrix3& operator=(const Matrix3& other) {
			if (this != &other) {
				m = other.m;
				ResizeMatrix();
			}
			return *this;
		}

		inline Matrix3Row operator[](size_t i) {
			return Matrix3Row(this, i);
		}

		inline Matrix3 operator*(Matrix3 other) const {
			if (other.m.size() != m.size()) throw std::runtime_error("Matrix sizes do not match");
			Matrix3 result;
			result.ResizeMatrix(m.size());

			for (size_t i = 0; i < m.size(); i++) {
				for (size_t j = 0; j < m.size(); j++) {
					result[i][j] = 0;

					for (size_t k = 0; k < m.size(); k++) {
						result[i][j] += m[i][k] * other[k][j];
					}
				}
			}
			return result;
		}
		inline void operator*=(Matrix3 other) {
			m = ((*this) * other).m;
		}
	};

	class Matrix4 : public Matrix {
	private:
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

			inline void operator*=(const Vector4& v) {
				mat->m[row][0] *= v.x;
				mat->m[row][1] *= v.y;
				mat->m[row][2] *= v.z;
				mat->m[row][3] *= v.w;
			}
			inline Vector4 operator*(const Vector4& v) {
				return Vector4(mat->m[row][0] * v.x, mat->m[row][1] * v.y, mat->m[row][2] * v.z, mat->m[row][3] * v.w);
			}
		private:
			Vector4 GetVector() {
				return Vector4(mat->m[row][0], mat->m[row][1], mat->m[row][2], mat->m[row][3]);
			}
		};
	public:
		Matrix4() : Matrix(4) {}
		Matrix4(float init) : Matrix(4, init) {}
		Matrix4(Vector4 r0, Vector4 r1, Vector4 r2, Vector4 r3) : Matrix(4) {
			(*this)[0] = r0;
			(*this)[1] = r1;
			(*this)[2] = r2;
			(*this)[3] = r3;
		}

		static Matrix4 LookAt(const Vector3& from, const Vector3& at, const Vector3& up);
		static Matrix4 Perspective(float fovY, float aspectRatio, float zNear, float zFar);
		static Matrix4 FromTranslation(const Vector3& translation);
		static Matrix4 FromRotationX(const float& angle);
		static Matrix4 FromRotationY(const float& angle);
		static Matrix4 FromRotationZ(const float& angle);
		static Matrix4 FromRotation(const Quaternion& rotation);
		static Matrix4 FromRotationZYX(const Quaternion& rotation);
		static Matrix4 FromScale(const Vector3& scale);

		Matrix4& operator=(const Matrix4& other) {
			if (this != &other) {
				m = other.m;
				ResizeMatrix();
			}
			return *this;
		}

		inline Matrix4Row operator[](size_t i) {
			return Matrix4Row(this, i);
		}

		inline Matrix4 operator*(Matrix4 other) const {
			if (other.m.size() != m.size()) throw std::runtime_error("Matrix sizes do not match");
			Matrix4 result;
			result.ResizeMatrix(m.size());

			for (size_t i = 0; i < m.size(); i++) {
				for (size_t j = 0; j < m.size(); j++) {
					result[i][j] = 0;

					for (size_t k = 0; k < m.size(); k++) {
						result[i][j] += m[i][k] * other[k][j];
					}
				}
			}
			return result;
		}
		inline void operator*=(Matrix4 other) {
			m = ((*this) * other).m;
		}

		// Translate matrix using a Vector3f
		inline Matrix4 Translate(Vector3 v) {
			Matrix4 result(*this);
			result[3] = (*this)[0] * v.x + (*this)[1] * v.y + (*this)[2] * v.z + (*this)[3];
			return result;
		}

		// Rotate using a Quaternion
		Matrix4 Rotate(Quaternion quat);
		Matrix4 Scale(Vector3 scale);

		Matrix4 Inverse();
	};
}
