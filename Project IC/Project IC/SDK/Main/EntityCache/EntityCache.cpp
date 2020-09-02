#include "EntityCache.h"

void C_EntityCache::Fill()
{
	C_BaseEntity* _pLocal = gInts.EntityList->GetClientEntity(gInts.Engine->GetLocalPlayer());

	if (_pLocal && _pLocal->IsInValidTeam())
	{
		pLocal = _pLocal;
		pLocalWeapon = pLocal->GetActiveWeapon();

		for (int n = 1; n < gInts.EntityList->GetHighestEntityIndex(); n++)
		{
			C_BaseEntity* pEntity = gInts.EntityList->GetClientEntity(n);

			if (!pEntity || pEntity->IsDormant())
				continue;
		
			switch (pEntity->GetClassId())
			{
				case CTFPlayer:
				{
					Groups[GroupType_t::PLAYERS_ALL].push_back(pEntity);
					Groups[pEntity->GetTeamNum() != pLocal->GetTeamNum() ? GroupType_t::PLAYERS_ENEMIES : GroupType_t::PLAYERS_TEAMMATES].push_back(pEntity);
					break;
				}

				case CObjectSentrygun:
				case CObjectDispenser:
				case CObjectTeleporter:
				{
					Groups[GroupType_t::BUILDINGS_ALL].push_back(pEntity);
					Groups[pEntity->GetTeamNum() != pLocal->GetTeamNum() ? GroupType_t::BUILDINGS_ENEMIES : GroupType_t::BUILDINGS_TEAMMATES].push_back(pEntity);
					break;
				}

				//Not sure about what spells we can airblast
				case CTFProjectile_Arrow:
				case CTFProjectile_BallOfFire:
				case CTFProjectile_Cleaver:
				case CTFProjectile_Jar:
				case CTFProjectile_JarMilk:
				case CTFProjectile_EnergyBall:
				case CTFProjectile_EnergyRing:
				case CTFProjectile_Flare:
				case CTFProjectile_HealingBolt:
				case CTFProjectile_MechanicalArmOrb:
				case CTFProjectile_Rocket:
				case CTFProjectile_SentryRocket:
				case CTFGrenadePipebombProjectile:
				{
					Groups[GroupType_t::PROJECTILES_ALL].push_back(pEntity);
					Groups[pEntity->GetTeamNum() != pLocal->GetTeamNum() ? GroupType_t::PROJECTILES_ENEMIES : GroupType_t::PROJECTILES_TEAMMATES].push_back(pEntity);
				}

				case CBaseAnimating:
				{
					int nHash = Hash::String(pEntity->GetModelName());

					if (Hash::m_bIsAmmo(nHash))
						Groups[GroupType_t::AMMOPACKS_ALL].push_back(pEntity);

					if (Hash::m_bIsHealth(nHash))
						Groups[GroupType_t::HEALTHPACKS_ALL].push_back(pEntity);

					break;
				}

				case CTFAmmoPack:
				{
					Groups[GroupType_t::AMMOPACKS_ALL].push_back(pEntity);
					break;
				}

				default: break;
			}
		}
	}
}

void C_EntityCache::Clear()
{
	pLocal = nullptr;
	pLocalWeapon = nullptr;

	for (auto &group : Groups)
		group.second.clear();
}

const std::vector<C_BaseEntity *> &C_EntityCache::GetGroup(const GroupType_t &group)
{
	return Groups[group];
}

C_EntityCache gEntCache;