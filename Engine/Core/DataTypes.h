#pragma once
#include <string>
#include <iostream>

using namespace std;

enum ELifeCycleState : uint8_t
{
	Initializing = 0,
	Ready = 1 << 0,			// 0x 0000 0001  = 1
	PendingKill = 1 << 1,	// 0x 0000 0010 = 2

};

class FVector
{
public:
	FVector();
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
	
	// Returns a copy of a custom made Vector
	static FVector CreateVector(float PayloadX, float PayloadY, float PayloadZ) { FVector vec; vec.X = PayloadX; vec.Y = PayloadY; vec.Z = PayloadZ; return vec; };

	//operators ----------------------------

	//vector
	FVector operator=(const FVector a){ X = a.X, Y = a.Y, Z = a.Z; return *this; };

	FVector operator+=(const FVector a){ X+=a.X, Y+=a.Y, Z+=a.Z; return *this; };
	FVector operator-=(const FVector a){ X-=a.X, Y-=a.Y, Z-=a.Z; return *this; };
	FVector operator*=(const FVector a){ X*=a.X, Y*=a.Y, Z*=a.Z; return *this; };
	FVector operator/=(const FVector a){ X/=a.X, Y/=a.Y, Z/=a.Z; return *this; };
	
	FVector operator+(const FVector a) { FVector temp; temp.X = X + a.X, temp.Y = Y + a.Y, temp.Z = Z + a.Z; return temp;};
	FVector operator-(const FVector a) { FVector temp; temp.X = X - a.X, temp.Y = Y - a.Y, temp.Z = Z - a.Z; return temp; };
	FVector operator*(const FVector a) { FVector temp; temp.X = X * a.X, temp.Y = Y * a.Y, temp.Z = Z * a.Z; return temp; };
	FVector operator/(const FVector a) { FVector temp; temp.X = X / a.X, temp.Y = Y / a.Y, temp.Z = Z / a.Z; return temp; };

	//float
	FVector operator=(float a) { X = a, Y = a, Z = a; return *this; };

	FVector operator+=(float a) { X += a, Y += a, Z += a; return *this; };
	FVector operator-=(float a) { X -= a, Y -= a, Z -= a; return *this; };
	FVector operator*=(float a){ X*=a, Y*=a, Z*=a; return *this; };
	FVector operator/=(float a){ X/=a, Y/=a, Z/=a; return *this; };

	FVector operator+(float a){ FVector temp;  temp.X = X + a, temp.Y = Y + a, temp.Z = Z + a; return temp; };
	FVector operator-(float a) { FVector temp;  temp.X = X - a, temp.Y = Y - a, temp.Z = Z - a; return temp; };
	FVector operator*(float a) { FVector temp;  temp.X = X * a, temp.Y = Y * a, temp.Z = Z * a; return temp; };
	FVector operator/(float a) { FVector temp;  temp.X = X / a, temp.Y = Y / a, temp.Z = Z / a; return temp; };

	//bool
	bool operator==(const FVector a){ if( Y == a.Y && Z == a.Z && X == a.X ) return true; return false; }
	bool operator!=(const FVector a){ if( Y != a.Y || Z != a.Z || X != a.X ) return true; return false; }

};

class FTransform
{
public:
	FTransform();
	~FTransform();

	// whether this transform is relative to a parent or not
	bool IsRelative = false;

	void SetLocation(FVector NewLocation) {_Location = NewLocation; }
	void SetRelativeLocation(FVector NewLocation) { _RelativeLocation = NewLocation; }
	void SetRotation(FVector NewRotation) { _Rotation = NewRotation; }
	void SetRelativeRotation(FVector NewRotation) { _RelativeRotation = NewRotation; }
	void SetScale(FVector NewScale) { _Scale = NewScale; }
	void SetRelativeScale(FVector NewScale) { _RelativeScale = NewScale; }


	// Returns a copy of World location
	FVector GetLocation();
	//Returns a copy of the relative Location
	FVector GetRelativeLocation() { return _RelativeLocation; }
	// Returns a copy of World rotation
	FVector GetRotation();
	//Returns a copy of the relative Rotation
	FVector GetRelativeRotation() { return _RelativeRotation; }
	// Returns a copy of World Scale
	FVector GetScale();
	//Returns a copy of the relative Scale
	FVector GetRelativeScale() { return _RelativeScale; }

private:
	FVector _Location, _Rotation, _Scale;
	FVector _RelativeLocation, _RelativeRotation, _RelativeScale;

	FTransform operator=(const FTransform a) { _Location = a._Location, _Rotation = a._Rotation, _Scale = a._Scale, IsRelative = a.IsRelative; return *this; };
};

struct FColor
{
	float R = 0.f, G = 0.f, B = 0.f, A = 1.f;
};