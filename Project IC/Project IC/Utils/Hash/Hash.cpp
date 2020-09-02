#include "Hash.h"

//The magic of number 33, i.e. why it works better than many other constants,
//prime or not, has never been adequately explained by anyone.
int Hash::String(const char* szOrg) {
	int iHash = 5381;

	while (int iStr = *szOrg++) iHash = iHash * 33 + iStr;

	return iHash;
}

bool Hash::m_bIsHealth(int nHash) {
	switch (nHash) {
	case MedKitSmall:
	case MedKitMedium:
	case MedKitLarge:
	case MedKitSmallBday:
	case MedKitMediumBday:
	case MedKitLargeBday:
	case MedKitSmallHalloween:
	case MedKitMediumHalloween:
	case MedKitLargeHalloween:
	case MedKitPlate:
	case MedKitSteak:
	case MedKitLargeMushRoom:
		return true;
	default: 
		return false;
	}
}

bool Hash::m_bIsAmmo(int nHash) {
	switch (nHash) {
	case AmmoSmall:
	case AmmoMedium:
	case AmmoLarge:
	case AmmoMediumBday:
	case AmmoLargeBday:
		return true;
	default:
		return false;
	}
}