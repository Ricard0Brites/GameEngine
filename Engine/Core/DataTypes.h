#pragma once
#include <string>
#include <iostream>
#include <mutex>

#define ENUM_CLASS_FLAGS(EnumType)\
using UnderlyingTypeOf_##EnumType = std::underlying_type_t<EnumType>;\
inline EnumType operator & (const EnumType& A, const EnumType& B) { return static_cast<EnumType>((static_cast<UnderlyingTypeOf_##EnumType>(A) & static_cast<UnderlyingTypeOf_##EnumType>(B))); }\
inline EnumType operator | (const EnumType& A, const EnumType& B) { return static_cast<EnumType>((static_cast<UnderlyingTypeOf_##EnumType>(A) | static_cast<UnderlyingTypeOf_##EnumType>(B))); }\
inline EnumType operator ^ (const EnumType& A, const EnumType& B) { return static_cast<EnumType>((static_cast<UnderlyingTypeOf_##EnumType>(A) ^ static_cast<UnderlyingTypeOf_##EnumType>(B))); }\
inline EnumType operator ~ (const EnumType& A) { return static_cast<EnumType>(~static_cast<UnderlyingTypeOf_##EnumType>(A)); }\
inline bool operator == (const EnumType& A, const UnderlyingTypeOf_##EnumType B) { return (static_cast<UnderlyingTypeOf_##EnumType>(A) == B); }\
inline bool operator != (const EnumType& A, const UnderlyingTypeOf_##EnumType B) { return (static_cast<UnderlyingTypeOf_##EnumType>(A) != B); }

enum class ELifeCycleState : uint8_t
{
	Uninitialized = 0,
	Initializing = 1 << 0,	// 0x 0000 0001 = 1
	Ready = 1 << 1,			// 0x 0000 0010 = 2
	PendingKill = 1 << 2,	// 0x 0000 0100 = 4

}; ENUM_CLASS_FLAGS(ELifeCycleState);

struct FVector
{
private:
	mutable std::recursive_mutex _Mutex;

	float X = 0,
		  Y = 0,
		  Z = 0;
public:
	// Constructors
	FVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
	~FVector() = default;

	FVector(const FVector& other)
	{
		std::lock_guard<std::recursive_mutex> lock(other._Mutex);
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}

	// Getters
	float GetX() const { std::lock_guard<std::recursive_mutex> lock(_Mutex); return X; }
	float GetY() const { std::lock_guard<std::recursive_mutex> lock(_Mutex); return Y; }
	float GetZ() const { std::lock_guard<std::recursive_mutex> lock(_Mutex); return Z; }
	void Get(float& outX, float& outY, float& outZ) const
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		outX = X;
		outY = Y;
		outZ = Z;
	}

	// Setters
	void SetX(float val) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X = val; }
	void SetY(float val) { std::lock_guard<std::recursive_mutex> lock(_Mutex); Y = val; }
	void SetZ(float val) { std::lock_guard<std::recursive_mutex> lock(_Mutex); Z = val; }
	void Set(float x, float y, float z) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X = x; Y = y; Z = z; }


	// Utility Methods
	FVector UpVector();
	FVector ForwardVector();
	FVector RighVector();
	FVector Zero();
	void Fill(float Payload);
	std::string ToString() const;

	// Operators
	FVector& operator=(const FVector& in)
	{
		if (this == &in) return *this;
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X = in.X; Y = in.Y; Z = in.Z;
		return *this;
	}

	FVector& operator=(float in)
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		X = in; Y = in; Z = in;
		return *this;
	}

	// Compound Assignment
	FVector& operator+=(const FVector& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X += in.X; Y += in.Y; Z += in.Z;
		return *this;
	}

	FVector& operator-=(const FVector& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X -= in.X; Y -= in.Y; Z -= in.Z;
		return *this;
	}

	FVector& operator*=(const FVector& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X *= in.X; Y *= in.Y; Z *= in.Z;
		return *this;
	}

	FVector& operator/=(const FVector& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X /= in.X; Y /= in.Y; Z /= in.Z;
		return *this;
	}

	// Binary Operators
	FVector operator+(const FVector& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector(X + in.X, Y + in.Y, Z + in.Z);
	}

	FVector operator-(const FVector& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector(X - in.X, Y - in.Y, Z - in.Z);
	}

	FVector operator*(const FVector& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector(X * in.X, Y * in.Y, Z * in.Z);
	}

	FVector operator/(const FVector& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector(X / in.X, Y / in.Y, Z / in.Z);
	}
	
	// Comparison
	bool operator==(const FVector& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return (X == in.X && Y == in.Y && Z == in.Z);
	}

	bool operator!=(const FVector& in) const
	{
		return !(*this == in);
	}
};

struct FVector2
{
private:
	mutable std::recursive_mutex _Mutex;

	float X = 0, Y = 0;
public:
	// Constructors
	FVector2(float x = 0, float y = 0, float z = 0) : X(x), Y(y) {}
	~FVector2() = default;

	FVector2(const FVector2& other)
	{
		std::lock_guard<std::recursive_mutex> lock(other._Mutex);
		X = other.X;
		Y = other.Y;
	}

	// Getters
	float GetX() const { std::lock_guard<std::recursive_mutex> lock(_Mutex); return X; }
	float GetY() const { std::lock_guard<std::recursive_mutex> lock(_Mutex); return Y; }
	void Get(float& outX, float& outY) const
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		outX = X;
		outY = Y;
	}

	// Setters
	void SetX(float val) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X = val; }
	void SetY(float val) { std::lock_guard<std::recursive_mutex> lock(_Mutex); Y = val; }
	void Set(float x, float y) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X = x; Y = y; }


	// Utility Methods
	FVector2 Zero();
	void Fill(float Payload);
	std::string ToString() const;

	// Operators
	FVector2& operator=(const FVector2& in)
	{
		if (this == &in) return *this;
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X = in.X; Y = in.Y;
		return *this;
	}

	FVector2& operator=(float in)
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		X = in; Y = in;
		return *this;
	}

	// Compound Assignment
	FVector2& operator+=(const FVector2& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X += in.X; Y += in.Y;
		return *this;
	}

	FVector2& operator-=(const FVector2& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X -= in.X; Y -= in.Y;
		return *this;
	}

	FVector2& operator*=(const FVector2& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X *= in.X; Y *= in.Y;
		return *this;
	}

	FVector2& operator/=(const FVector2& in)
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		X /= in.X; Y /= in.Y;
		return *this;
	}

	// Binary Operators
	FVector2 operator+(const FVector2& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector2(X + in.X, Y + in.Y);
	}

	FVector2 operator-(const FVector2& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector2(X - in.X, Y - in.Y);
	}

	FVector2 operator*(const FVector2& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector2(X * in.X, Y * in.Y);
	}

	FVector2 operator/(const FVector2& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return FVector2(X / in.X, Y / in.Y);
	}

	// Comparison
	bool operator==(const FVector2& in) const
	{
		std::lock(_Mutex, in._Mutex);
		std::lock_guard<std::recursive_mutex> this_lock(_Mutex, std::adopt_lock);
		std::lock_guard<std::recursive_mutex> in_lock(in._Mutex, std::adopt_lock);
		return (X == in.X && Y == in.Y);
	}

	bool operator!=(const FVector2& in) const
	{
		return !(*this == in);
	}
};

struct FTransform
{
private:
	mutable std::mutex _locationMutex;
	mutable std::mutex _rotationMutex;
	mutable std::mutex _scaleMutex;

	FVector _Location, _Rotation, _Scale;

public:
	FTransform& operator=(const FTransform& in) 
	{
		std::lock(_locationMutex, _rotationMutex, _scaleMutex);
		std::lock_guard<std::mutex> ll(_locationMutex, std::adopt_lock);
		std::lock_guard<std::mutex> lr(_rotationMutex, std::adopt_lock);
		std::lock_guard<std::mutex> ls(_scaleMutex, std::adopt_lock);

		_Location = in._Location;
		_Rotation = in._Rotation;
		_Scale = in._Scale; 
		IsRelative = in.IsRelative;
		return *this; 
	};
	FTransform();
	~FTransform();
	
	FTransform(const FTransform& in)
    {
        std::lock(in._locationMutex, in._rotationMutex, in._scaleMutex);
        std::lock_guard<std::mutex> ol(in._locationMutex, std::adopt_lock);
        std::lock_guard<std::mutex> orot(in._rotationMutex, std::adopt_lock);
        std::lock_guard<std::mutex> os(in._scaleMutex, std::adopt_lock);

        _Location = in._Location;
        _Rotation = in._Rotation;
        _Scale = in._Scale;
        IsRelative = in.IsRelative;
    }

	// whether this transform is relative to a parent or not
	bool IsRelative = false;

	void SetLocation(FVector NewLocation) { std::lock_guard<std::mutex> lock(_locationMutex); _Location = NewLocation; }
	void SetRotation(FVector NewRotation) { std::lock_guard<std::mutex> lock(_rotationMutex); _Rotation = NewRotation; }
	void SetScale(FVector NewScale) { std::lock_guard<std::mutex> lock(_scaleMutex); _Scale = NewScale; }


	// Returns a copy of World location
	FVector GetLocation();
	// Returns a copy of World rotation
	FVector GetRotation();
	// Returns a copy of World Scale
	FVector GetScale();

};

struct FColor
{
	float R = 0.f, G = 0.f, B = 0.f, A = 1.f;
};