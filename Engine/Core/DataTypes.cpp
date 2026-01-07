#include "DataTypes.h"


FVector::FVector(float x, float y, float z) : 
	X(x),
	Y(y),
	Z(z)
{
}

FVector::~FVector()
{

}

FVector FVector::UpVector(bool ApplyToSelf)
{
	const FVector up(0, 0, 1);
	if (ApplyToSelf)
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		*this = up;
	}
	return up;
}

FVector FVector::ForwardVector(bool ApplyToSelf)
{
	const FVector forward(0, 1, 0);
	if (ApplyToSelf)
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		*this = forward;
	}
	return forward;
}

FVector FVector::RighVector(bool ApplyToSelf)
{
	const FVector right(1, 0, 0);
	if (ApplyToSelf)
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		*this = right;
	}
	return right;
}

FVector FVector::Zero(bool ApplyToSelf)
{
	const FVector zero(0, 0, 0);
	if (ApplyToSelf)
	{
		std::lock_guard<std::recursive_mutex> lock(_Mutex);
		*this = zero;
	}
	return zero;
}

void FVector::Fill(float Payload)
{
	std::lock_guard<std::recursive_mutex> lock(_Mutex);
	*this = Payload;
}

std::string FVector::ToString(char* buffer, size_t bufferSize) const
{
	std::lock_guard<std::recursive_mutex> lock(_Mutex);
	string StringToReturn = "";

	StringToReturn += "X=" + to_string(X);
	StringToReturn += "Y=" + to_string(Y);
	StringToReturn += "Z=" + to_string(Z);

	return StringToReturn;
}

// Transform ---------------------------------------------------

FTransform::FTransform()
{
	_Location = 0;
	_Rotation = 0;
	_Scale = 1;
}

FTransform::~FTransform()
{
}

FVector FTransform::GetLocation()
{	
	std::lock_guard<std::mutex> lock(_locationMutex);
	return _Location;
}

FVector FTransform::GetRotation()
{
	std::lock_guard<std::mutex> lock(_rotationMutex);
	return _Rotation;
}

FVector FTransform::GetScale()
{
	std::lock_guard<std::mutex> lock(_scaleMutex);
	return _Scale;
}