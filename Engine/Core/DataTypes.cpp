#include "DataTypes.h"

FVector FVector::UpVector()
{
	return FVector(0, 0, 1);
}

FVector FVector::ForwardVector()
{
	return FVector(0, 1, 0);

}

FVector FVector::RighVector()
{
	return FVector(1, 0, 0);
}

FVector FVector::Zero()
{
	return FVector(0, 0, 0);
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

void FVector::Fill(float Payload)
{
	std::lock_guard<std::recursive_mutex> lock(_Mutex);
	*this = Payload;
}

std::string FVector::ToString() const
{
	std::lock_guard<std::recursive_mutex> lock(_Mutex);
	std::string StringToReturn = "";

	StringToReturn += "X=" + std::to_string(X);
	StringToReturn += "Y=" + std::to_string(Y);
	StringToReturn += "Z=" + std::to_string(Z);

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

// Vector 2D

FVector2 FVector2::Zero()
{
	return FVector2(0, 0);
}

void FVector2::Fill(float Payload)
{
	X = Payload;
	Y = Payload;
}

std::string FVector2::ToString() const
{
	std::lock_guard<std::recursive_mutex> lock(_Mutex);
	std::string StringToReturn = "";

	StringToReturn += "X=" + std::to_string(X);
	StringToReturn += "Y=" + std::to_string(Y);

	return StringToReturn;
}
