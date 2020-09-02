#pragma once

class C_GameEvent
{
public:
	virtual ~C_GameEvent() { };
	virtual const char* GetName() const = 0;

	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char* szKey = 0) = 0;

	virtual bool GetBool(const char* szKey = 0, bool bDefault = false) = 0;
	virtual int	GetInt(const char* szKey = 0, int nDefault = 0) = 0;
	virtual float GetFloat(const char* szKey = 0, float flDefault = 0.0f) = 0;
	virtual const char* GetString(const char* szKey = 0, const char* szDefault = "") = 0;

	virtual void SetBool(const char* szKey, bool bNew) = 0;
	virtual void SetInt(const char* szKey, int nNew) = 0;
	virtual void SetFloat(const char* szKey, float flNew) = 0;
	virtual void SetString(const char* szKey, const char* szNew) = 0;
};

class C_GameEventListener
{
public:
	virtual ~C_GameEventListener(void) {};

	virtual void FireGameEvent(C_GameEvent* pEvent) = 0;
};

class C_GameEventManager
{
public:
	bool AddListener(C_GameEventListener* pListener, const char* szName, bool ServerSide) {
		typedef bool(__thiscall* FN)(PVOID, C_GameEventListener*, const char*, bool);
		return GetVFunc<FN>(this, 3)(this, pListener, szName, ServerSide);
	}
};

#define GAMEEVENTSMANAGER_ENGINE_INTERFACE "GAMEEVENTSMANAGER002"