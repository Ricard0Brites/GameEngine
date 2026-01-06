#include "DataTypes.h"

FVector::FVector()
{
}

FVector::~FVector()
{

}

FVector FVector::UpVector(bool ApplyToSelf)
{
	if(!ApplyToSelf) return FVector::CreateVector(0,0,1);

	*this = FVector::CreateVector(0,0,1);
	return *this;
}

FVector FVector::ForwardVector(bool ApplyToSelf)
{
	if (!ApplyToSelf) return FVector::CreateVector(0, 1, 0);

	*this = FVector::CreateVector(0, 1, 0);
	return *this;
}

FVector FVector::RighVector(bool ApplyToSelf)
{
	if (!ApplyToSelf) return FVector::CreateVector(1, 0, 0);

	*this = FVector::CreateVector(1, 0, 0);
	return *this;
}

FVector FVector::Zero(bool ApplyToSelf)
{
	if (!ApplyToSelf) return FVector::CreateVector(0, 0, 0);

	*this = FVector::CreateVector(0, 0, 0);
	return *this;
}

void FVector::Fill(float Payload)
{
	*this = Payload;
}


string FVector::ToString()
{
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
	return _Location;
}

FVector FTransform::GetRotation()
{
	 return _Rotation;
}

FVector FTransform::GetScale()
{
	return _Scale;
}