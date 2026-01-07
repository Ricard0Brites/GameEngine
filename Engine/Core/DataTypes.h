#pragma once
#include <string>
#include <iostream>
#include <mutex>

using namespace std;

enum ELifeCycleState : uint8_t
{
	Initializing = 0,
	Ready = 1 << 0,			// 0x 0000 0001  = 1
	PendingKill = 1 << 1,	// 0x 0000 0010 = 2

};

class FVector
{
private:
	mutable std::recursive_mutex _Mutex;
public:
	FVector(const FVector& other)
	{
		std::lock_guard<std::recursive_mutex> lock(other._Mutex);
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}
	FVector(float x = 0, float y = 0, float z = 0);
	~FVector();

	float X = 0,
		  Y = 0,
	 	  Z = 0;

	//Returns a copy of an up vector (0,0,1) and , optionally, applies to self
	FVector UpVector(bool ApplyToSelf);
	//Returns a copy of a forward vector (0,1,0) and , optionally, applies to self
	FVector ForwardVector(bool ApplyToSelf);
	//Returns a copy of a right vector (1,0,0) and , optionally, applies to self
	FVector RighVector(bool ApplyToSelf);
	//Returns a copy of a zero vector (0,0,0) and , optionally, applies to self
	FVector Zero(bool ApplyToSelf);
	//Fills the vector with the provided float
	void Fill(float Payload);
	//Converts a vector to a string
	string ToString();

	//operators ----------------------------

	//vector
	FVector& operator=(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X = in.X; Y = in.Y; Z = in.Z; return *this; };

	FVector& operator+=(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X += in.X; Y += in.Y; Z += in.Z; return *this; };
	FVector& operator-=(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X -= in.X; Y -= in.Y; Z -= in.Z; return *this; };
	FVector& operator*=(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X *= in.X; Y *= in.Y; Z *= in.Z; return *this; };
	FVector& operator/=(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X /= in.X; Y /= in.Y; Z /= in.Z; return *this; };
	
	FVector operator+(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X + in.X, Y + in.Y, Z + in.Z);};
	FVector operator-(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X - in.X, Y - in.Y, Z - in.Z);};
	FVector operator*(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X * in.X, Y * in.Y, Z * in.Z);};
	FVector operator/(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X / in.X, Y / in.Y, Z / in.Z);};

	//float
	FVector& operator=(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X = in; Y = in; Z = in; return *this; };

	FVector& operator+=(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X += in; Y += in; Z += in; return *this; };
	FVector& operator-=(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X -= in; Y -= in; Z -= in; return *this; };
	FVector& operator*=(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X *= in; Y *= in; Z *= in; return *this; };
	FVector& operator/=(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); X /= in; Y /= in; Z /= in; return *this; };

	FVector operator+(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X + in, Y + in, Z + in); };
	FVector operator-(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X - in, Y - in, Z - in); };
	FVector operator*(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X * in, Y * in, Z * in); };
	FVector operator/(float in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); return FVector(X / in, Y / in, Z / in); };

	//bool
	bool operator==(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); if( Y == in.Y && Z == in.Z && X == in.X ) return true; return false; }
	bool operator!=(const FVector& in) { std::lock_guard<std::recursive_mutex> lock(_Mutex); if( Y != in.Y || Z != in.Z || X != in.X ) return true; return false; }
};

class FTransform
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