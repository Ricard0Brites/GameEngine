#include "DataTypes.h"

SVector::SVector()
{
}

SVector::~SVector()
{

}

SVector SVector::UpVector(bool ApplyToSelf)
{
	if(!ApplyToSelf) return SVector::CreateVector(0,0,1);

	*this = SVector::CreateVector(0,0,1);
	return *this;
}

SVector SVector::ForwardVector(bool ApplyToSelf)
{
	if (!ApplyToSelf) return SVector::CreateVector(0, 1, 0);

	*this = SVector::CreateVector(0, 1, 0);
	return *this;
}

SVector SVector::RighVector(bool ApplyToSelf)
{
	if (!ApplyToSelf) return SVector::CreateVector(1, 0, 0);

	*this = SVector::CreateVector(1, 0, 0);
	return *this;
}

SVector SVector::Zero(bool ApplyToSelf)
{
	if (!ApplyToSelf) return SVector::CreateVector(0, 0, 0);

	*this = SVector::CreateVector(0, 0, 0);
	return *this;
}

void SVector::Fill(float Payload)
{
	*this = Payload;
}


string SVector::ToString()
{
	string StringToReturn = "";

	StringToReturn += "X=" + to_string(X);
	StringToReturn += "Y=" + to_string(Y);
	StringToReturn += "Z=" + to_string(Z);

	return StringToReturn;
}

// Transform ---------------------------------------------------

STransform::STransform()
{
	_Location = 0;
	_Rotation = 0;
	_Scale = 1;
}

STransform::~STransform()
{
}

SVector STransform::GetLocation()
{	
	return _Location;
}

SVector STransform::GetRotation()
{
	 return _Rotation;
}

SVector STransform::GetScale()
{
	return _Scale;
}