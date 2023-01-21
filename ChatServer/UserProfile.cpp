#include "pch.h"
#include "UserProfile.h"

UserProfile::~UserProfile()
{
	cout << "~UserProfile " << userId << ' ' << name << endl;
}
