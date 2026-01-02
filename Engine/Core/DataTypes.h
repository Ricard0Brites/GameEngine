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

struct Color
{
	float R = 0, G = 0, B = 0, A = 255;
};

class SVector
{
public:
	SVector();
	~SVector();

	float X = 0,
		  Y = 0,
	 	  Z = 0;

	//Returns a copy of an up vector (0,0,1) and , optionally, applies to self
	SVector UpVector(bool ApplyToSelf);
	//Returns a copy of a forward vector (0,1,0) and , optionally, applies to self
	SVector ForwardVector(bool ApplyToSelf);
	//Returns a copy of a right vector (1,0,0) and , optionally, applies to self
	SVector RighVector(bool ApplyToSelf);
	//Returns a copy of a zero vector (0,0,0) and , optionally, applies to self
	SVector Zero(bool ApplyToSelf);
	//Fills the vector with the provided float
	void Fill(float Payload);
	//Converts a vector to a string
	string ToString();
	
	// Returns a copy of a custom made Vector
	static SVector CreateVector(float PayloadX, float PayloadY, float PayloadZ) { SVector vec; vec.X = PayloadX; vec.Y = PayloadY; vec.Z = PayloadZ; return vec; };

	//operators ----------------------------

	//vector
	SVector operator=(const SVector a){ X = a.X, Y = a.Y, Z = a.Z; return *this; };

	SVector operator+=(const SVector a){ X+=a.X, Y+=a.Y, Z+=a.Z; return *this; };
	SVector operator-=(const SVector a){ X-=a.X, Y-=a.Y, Z-=a.Z; return *this; };
	SVector operator*=(const SVector a){ X*=a.X, Y*=a.Y, Z*=a.Z; return *this; };
	SVector operator/=(const SVector a){ X/=a.X, Y/=a.Y, Z/=a.Z; return *this; };
	
	SVector operator+(const SVector a) { SVector temp; temp.X = X + a.X, temp.Y = Y + a.Y, temp.Z = Z + a.Z; return temp;};
	SVector operator-(const SVector a) { SVector temp; temp.X = X - a.X, temp.Y = Y - a.Y, temp.Z = Z - a.Z; return temp; };
	SVector operator*(const SVector a) { SVector temp; temp.X = X * a.X, temp.Y = Y * a.Y, temp.Z = Z * a.Z; return temp; };
	SVector operator/(const SVector a) { SVector temp; temp.X = X / a.X, temp.Y = Y / a.Y, temp.Z = Z / a.Z; return temp; };

	//float
	SVector operator=(float a) { X = a, Y = a, Z = a; return *this; };

	SVector operator+=(float a) { X += a, Y += a, Z += a; return *this; };
	SVector operator-=(float a) { X -= a, Y -= a, Z -= a; return *this; };
	SVector operator*=(float a){ X*=a, Y*=a, Z*=a; return *this; };
	SVector operator/=(float a){ X/=a, Y/=a, Z/=a; return *this; };

	SVector operator+(float a){ SVector temp;  temp.X = X + a, temp.Y = Y + a, temp.Z = Z + a; return temp; };
	SVector operator-(float a) { SVector temp;  temp.X = X - a, temp.Y = Y - a, temp.Z = Z - a; return temp; };
	SVector operator*(float a) { SVector temp;  temp.X = X * a, temp.Y = Y * a, temp.Z = Z * a; return temp; };
	SVector operator/(float a) { SVector temp;  temp.X = X / a, temp.Y = Y / a, temp.Z = Z / a; return temp; };

	//bool
	bool operator==(const SVector a){ if( Y == a.Y && Z == a.Z && X == a.X ) return true; return false; }
	bool operator!=(const SVector a){ if( Y != a.Y || Z != a.Z || X != a.X ) return true; return false; }

};

class STransform
{
public:
	STransform();
	~STransform();

	// whether this transform is relative to a parent or not
	bool IsRelative = false;

	void SetLocation(SVector NewLocation) {_Location = NewLocation; }
	void SetRelativeLocation(SVector NewLocation) { _RelativeLocation = NewLocation; }
	void SetRotation(SVector NewRotation) { _Rotation = NewRotation; }
	void SetRelativeRotation(SVector NewRotation) { _RelativeRotation = NewRotation; }
	void SetScale(SVector NewScale) { _Scale = NewScale; }
	void SetRelativeScale(SVector NewScale) { _RelativeScale = NewScale; }


	// Returns a copy of World location
	SVector GetLocation();
	//Returns a copy of the relative Location
	SVector GetRelativeLocation() { return _RelativeLocation; }
	// Returns a copy of World rotation
	SVector GetRotation();
	//Returns a copy of the relative Rotation
	SVector GetRelativeRotation() { return _RelativeRotation; }
	// Returns a copy of World Scale
	SVector GetScale();
	//Returns a copy of the relative Scale
	SVector GetRelativeScale() { return _RelativeScale; }

private:
	SVector _Location, _Rotation, _Scale;
	SVector _RelativeLocation, _RelativeRotation, _RelativeScale;

	STransform operator=(const STransform a) { _Location = a._Location, _Rotation = a._Rotation, _Scale = a._Scale, IsRelative = a.IsRelative; return *this; };
};

enum InputKeyCodes
{
	// Keyboard Buttons
	K_a,
	K_b, 
	K_c, 
	K_d, 
	K_e, 
	K_f, 
	K_g, 
	K_h, 
	K_i, 
	K_j, 
	K_k, 
	K_l, 
	K_m, 
	K_n, 
	K_o, 
	K_p, 
	K_q, 
	K_r, 
	K_s, 
	K_t, 
	K_u, 
	K_v, 
	K_w, 
	K_x, 
	K_y, 
	K_z,
	
	K_A,
	K_B,
	K_C,
	K_D,
	K_E,
	K_F,
	K_G,
	K_H,
	K_I,
	K_J,
	K_K,
	K_L,
	K_M,
	K_N,
	K_O,
	K_P,
	K_Q,
	K_R,
	K_S,
	K_T,
	K_U,
	K_V,
	K_W,
	K_X,
	K_Y,
	K_Z,


	K_0,
	K_1,
	K_2,
	K_3,
	K_4,
	K_5,
	K_6,
	K_7,
	K_8,
	K_9,

	K_Num0,
	K_Num1,
	K_Num2,
	K_Num3,
	K_Num4,
	K_Num5,
	K_Num6,
	K_Num7,
	K_Num8,
	K_Num9,

	K_Up,
	K_Left,
	K_Down,
	K_Right,
	K_Space,

	// gamepad buttons
	GamepadFaceRight,
	GamepadFaceLeft,
	GamepadFaceBottom,
	GamepadFaceTop,
	GamepadArrowLeft,
	GamepadArrowRight,
	GamepadArrowBottom,
	GamepadArrowTop,
	GamepadLeftStickPress,
	GamepadRightStickPress,
	GamepadSpecialLeft,
	GamepadSpecialRight,
	GamepadLeftButton,
	GamepadRightButton,

	// axis 
	GamepadLeftTrigger,
	GamepadRightTrigger,
	GamepadStickRight,
	GamepadStickLeft,

	// Null
	Null
};