#pragma once
#include "../../Includes/Includes.h"

class C_GameMovement
{
public:
	virtual			~C_GameMovement(void) {}

	// Process the current movement command
	virtual void	ProcessMovement(C_BaseEntity *pPlayer, C_MoveData *pMove) = 0;
	virtual void	StartTrackPredictionErrors(C_BaseEntity *pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(C_BaseEntity *pPlayer) = 0;
	virtual void	DiffPrint(char const *fmt, ...) = 0;

	// Allows other parts of the engine to find out the normal and ducked player bbox sizes
	virtual Vec3 GetPlayerMins(bool ducked) const = 0;
	virtual Vec3 GetPlayerMaxs(bool ducked) const = 0;
	virtual Vec3 GetPlayerViewOffset(bool ducked) const = 0;
};

#define CLIENT_GAMEMOVEMENT_INTERFACE_VERSION "GameMovement001"