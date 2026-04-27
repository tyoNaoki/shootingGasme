#pragma once
#define _USE_MATH_DEFINES
#include<math.h>
#include<string>
#include<sstream>
#include<exception>
#include<cmath>


//倍精度浮動小数点二次元ベクトルクラス

// イプシロン（許容誤差）を定義  
static const float EPSILON = 1e-5f;

template<typename T> class Vector2D {
public:
	Vector2D() :x(0), y(0) {};
	Vector2D(const Vector2D<T>& obj) { x = obj.x; y = obj.y; }
	template<typename T2>
	Vector2D(const Vector2D<T2>& obj) :x((T)obj.x), y((T)obj.y) {}
	Vector2D(T m_x, T m_y) : x(m_x), y(m_y) {};
	Vector2D<T>& operator =(const Vector2D<T>& obj) { x = obj.x; y = obj.y; return *this; }
	template<typename T2>
	Vector2D<T>& operator =(const Vector2D<T2>& obj) { x = (T)obj.x; y = (T)obj.y; return *this; }
	Vector2D<T> operator+(const Vector2D<T>& in)const {
		Vector2D<T> out(x + in.x, y + in.y);
		return out;
	}
	Vector2D<T>operator-(const Vector2D<T>& in)const {
		Vector2D<T> out(x - in.x, y - in.y);
		return out;
	}
	template<typename U>
	Vector2D<T> operator*(U num)const {
		Vector2D<T> out((T)(x * num), (T)(y * num));
		return out;
	}

	template<typename U>
	Vector2D<T> operator*(Vector2D<U> num)const {
		Vector2D<T> out((T)(x * num.x), (T)(y * num.y));
		return out;
	}

	template<typename U>
	Vector2D<T> operator*=(U num) {
		Vector2D<T> out((T)(x * num), (T)(y * num));
		x = out.x;
		y = out.y;
		return out;
	}

	template<typename U>
	Vector2D<T> operator*=(Vector2D<U> num) {
		Vector2D<T> out((T)(x * num.x), (T)(y * num.y));
		x = out.x;
		y = out.y;
		return out;
	}

	


	template<typename U>
	Vector2D<T> operator/(U num)const {
		if (num == 0) {
			throw std::range_error("Divided by zero.");
		}
		Vector2D<T> out(static_cast<T>(static_cast<double>(x) / num), static_cast<T>(static_cast<double>(y) / num));
		return out;
	}

	template<typename U>
	Vector2D<T> operator/(Vector2D<U>num)const {
		if (num.x == 0 || num.y ==0) {
			throw std::range_error("Divided by zero.");
		}
		Vector2D<T> out(static_cast<T>(static_cast<double>(x) / num.x), static_cast<T>(static_cast<double>(y) / num.y));
		return out;
	}

	template<typename U>
	Vector2D<T> operator/=(U num) {
		if (num == 0) {
			throw std::range_error("Divided by zero.");
		}
		x /= num;
		y /= num;
		return *this;
	}

	template<typename U>
	Vector2D<T> operator/=(Vector2D<U> num) {
		if (num.x == 0) {
			throw std::range_error("Divided by zero.");
		}
		if (num.y == 0) {
			throw std::range_error("Divided by zero.");
		}
		x /= num.x;
		y /= num.y;
		return *this;
	}

	Vector2D<T> operator-()const {
		Vector2D<T> out(-x, -y);
		return out;
	}
	bool operator==(const Vector2D<T>& in)const {
		if (in.x == x && in.y == y) { return true; }
		return false;
	}
	bool operator!=(const Vector2D<T>& in)const {
		return !(*this == in);
	}
	Vector2D<T>& operator+=(const Vector2D<T>& in) {
		x += in.x;
		y += in.y;
		return *this;
	}
	Vector2D<T>& operator-=(const Vector2D<T>& in) {
		x -= in.x;
		y -= in.y;
		return *this;
	}
	Vector2D<T> operator++(int) {
		Vector2D<T> buf = *this;
		++x;
		++y;
		return buf;
	}
	Vector2D<T>& operator++() {
		++x;
		++y;
		return *this;
	}
	Vector2D<T> operator--(int) {
		Vector2D<T> buf = *this;
		--x;
		--y;
		return buf;
	}
	Vector2D<T>& operator--() {
		--x;
		--y;
		return *this;
	}

	//	template <typename T2> operator Vector2D<T2>(); //Vector2Dのキャスト

	Vector2D<int> ToInt(){
		return Vector2D<int>(static_cast<int>(x), static_cast<int>(y));
	}

	void Set(T mx, T my) {
		x = mx; y = my;
	}

	T Dot(const Vector2D<T>& other) const {
		return Vector2D<T>::Dot(*this,other);
	}

	T Cross(Vector2D<T>& other) {
		return Vector2D<T>::Cross(*this,other);
	}

	double Length()const {
		return Length(*this);
	}

	double Length_Square()const{
		return Length_Square(*this);
	}


	const std::string ToString()const {
		using namespace std;
		stringstream is;
		is << "(" << x << "," << y << ")";
		return is.str();
	}

	Vector2D<T> Normalize() const{
		auto length = Length(*this);
		if(length==0.0f){return Vector2D<T>(0,0);}
		return Vector2D<T>(this->x/length,this->y/length);
	}

	Vector2D<T> Perpendicular() const {
		return Vector2D<T>(-y, x); 
	}

	Vector2D<T>NegateVector()const{
		return GetNegateVector(*this);
	}

	Vector2D<T>RotateVector(float degree)const{
		return getRotateVector(*this,degree);
	}

	T x, y; //要素
public:

	static Vector2D<T> ZeroVector(){
		return Vector2D<T>(0,0);
	}

	static T Length(Vector2D<T> v){
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	static T Length_Square(Vector2D<T> v) {
		return pow(v.x, 2) + pow(v.y, 2);
	}

	static T GetDegreesFromRadians(T radians){
		if(radians == 0){
			return 0.0;
		}
		return (T)(radians * 180.0 / M_PI);
	}

	static T GetRadiansFromDegrees(T degrees) {
		if (degrees == 0) {
			return 0.0;
		}
		return (T)(degrees / 180.0 * M_PI);
	}

	static T GetDegreeFromOrigin(Vector2D<T>v){
		double radian = std::atan2(v.y,v.x);
		return static_cast<T>(GetDegreesFromRadians(radian));
	}

	template<typename T>
	static T Dot(Vector2D<T> v1, Vector2D<T> v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	template<typename T>
	static T Cross(Vector2D<T> v1, Vector2D<T> v2) {
		return (T)(v1.x * v2.y - v2.x * v1.y);
	}

	//v2がv1となす角のコサイン値を求める(時計周りが正)
	template<typename T>
	static T GetCosin(Vector2D<T> v1, Vector2D<T> v2) {
		double lengths = Length(v1) * Length(v2);
		double v1v2_dot(Dot(v1, v2));

		if (lengths == 0) return 1.0f; //0除算対策
		return v1v2_dot / lengths;
	}

	//v2がv1となす角のサイン値を求める(時計周りが正)
	template<typename T>
	static T GetSin(Vector2D<T> v1, Vector2D<T> v2) {
		double lengths = Length(v1) * Length(v2);
		double v1v2_cross(Cross(v1, v2));
		if (lengths == 0) return 0.0f; //0除算対策

		return v1v2_cross / lengths;
	}

	static float AngleDiff(float angle1,float angle2){
		if (angle1 < 0) {
			angle1 += 360;
		}
		if(angle2 < 0){
			angle2 += 360;
		}

		float diff = angle1 - angle2;

		if (diff > 180) { 
			diff -= 360;
		}else if (diff < -180) {
			diff += 360; 
		} 
		return diff;
	}
	
	//二つのワールド座標からラジアンを計算する
	static T GetLookAtRadian(const Vector2D<T> p1, const Vector2D<T> p2) {
		Vector2D<T> vector = { p2.x - p1.x, p2.y - p1.y };
		double angle = atan2(vector.y, vector.x); 
		return angle;
	}

	static T GetLookAtRadian(const Vector2D<T> p1) {
		double angle = atan2(p1.y, p1.x);
		return angle;
	}

	//二つのワールド座標から角度を計算する
	static T GetLookAtAngle(const Vector2D<T> p1, const Vector2D<T> p2) {
		double radian = GetLookAtRadian(p1,p2);
		return GetDegreesFromRadians(radian);
	}

	//二つのワールド座標から角度を計算する
	static T GetLookAtAngle(const Vector2D<T> p1) {
		double radian = GetLookAtRadian(p1);
		return GetDegreesFromRadians(radian);
	}

	//v2がv1となす角の角度(度)を求める(時計周りが正)
	static T GetRotationAngle(Vector2D<T> v1, Vector2D<T> v2) {
		double v1v2_rad = (GetRotationRadian(v1, v2));
		double v1v2_deg(v1v2_rad / M_PI * 180);
		return v1v2_deg;
	}

	//ベクトルV1がベクトルV2に一致するためにどのくらいのラジアン（角度）回転させる必要があるか計算する(時計周りが正)
	static T GetRotationRadian(Vector2D<T> v1, Vector2D<T> v2) {
		double v1v2_cos(GetCosin(v1, v2));
		double v1v1_sin(GetSin(v1, v2));
		double v1v2_rad(acos(v1v2_cos));
		if (v1v1_sin < 0) { v1v2_rad = -v1v2_rad; }
		return v1v2_rad;
	}

	//指定した角度方向の単位ベクトルを取得する
	static Vector2D<T> GetUnitVectorByRadian(double rad) {
		return Vector2D<T>((T)std::cos(rad), (T)std::sin(rad));
	}

	//v1のv2に対する正射影ベクトルを求める
	static Vector2D<T> GetOrthogonalProjectionVector(const Vector2D<T>& v1, const Vector2D<T>& v2) {
		return Vector2D<T>::GetParallelUnitVector(v2) * Vector2D<T>::Dot(v1, v2);
	}

	//平行な単位ベクトルを求める(同じ向きのもの)
	static Vector2D<T> GetParallelUnitVector(Vector2D<T> v) {
		double v_length(Length(v));
		if (v_length == 0) { throw std::invalid_argument("tryed to calc unit vector from zero vector"); }
		Vector2D<T>out(v / Length(v));
		return out;
	}

	//垂直単位ベクトルを求める(時計回りに90度回転したもの)
	static Vector2D<T> GetVerticalUnitVector(Vector2D<T> v) {
		Vector2D<T> v_vtcl(-v.y, v.x);
		double v_length(Length(v_vtcl));
		Vector2D<T> out(GetParallelUnitVector(v_vtcl));
		return out;
	}

	static Vector2D<T> GetRoundValue(Vector2D<T> v) {
		return Vector2D<T>((T)round((double)v.x), (T)round((double)v.y));
	}
	
	//v1の四捨五入値を取得
	static Vector2D<T> GetRoundValue(){
		return Vector2D<T>((T)round((double)this->v.x), (T)round((double)this->v.y));
	}

	static Vector2D<T>GetNegateVector(Vector2D<T>vector) {
		vector.x = -vector.x;
		vector.y = -vector.y;
		return vector;
	}

	static Vector2D<T> getRotateVector(Vector2D<T>vec,float degree) {
		float radian = GetRadiansFromDegrees(degree);

		float sin = std::sin(radian);
		float cos = std::cos(radian);

		auto r = Vector2D<T>();
		r.x = vec.x * cos - vec.y * sin;
		r.y = vec.x * sin + vec.y * cos;

		return r;
	}
};

//スカラとの乗算演算子
template<typename S, typename VT>
Vector2D<VT> operator*(S s, const Vector2D<VT>& v) {
	return Vector2D<VT>(v.x * s, v.y * s);
}

//値がほぼ等しいかを比較する関数
template<typename T>
bool nearEqual(T a, T b,float epsilon = EPSILON){ 
	return std::fabs(a - b) < epsilon; 
}

