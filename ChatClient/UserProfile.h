#pragma once
class UserProfile
{
public:
	uint64 userId = 0;
	string name;
};

extern shared_ptr<UserProfile> GUser;