#pragma once

#include "../Interfaces/Interfaces.h"

//TODO: Rewrite

//Made by Emily and some other dude
class C_NetVars
{
	struct node;
	using map_type = std::unordered_map<std::string, std::shared_ptr<node>>;

	struct node {
		node(DWORD offset) : offset(offset) {}
		map_type nodes;
		DWORD offset;
	};

	map_type nodes;

public:
	void Init();

private:
	void populate_nodes(class RecvTable *recv_table, map_type *map);

	DWORD get_offset_recursive(map_type &map, int acc, const char *name) {
		return acc + map[name]->offset;
	}

	template<typename ...args_t>
	DWORD get_offset_recursive(map_type &map, int acc, const char *name, args_t ...args) {
		const auto &node = map[name];
		return get_offset_recursive(node->nodes, acc + node->offset, args...);
	}

public:
	template<typename ...args_t>
	DWORD get_offset(const char *name, args_t ...args) {
		const auto &node = nodes[name];
		return get_offset_recursive(node->nodes, node->offset, args...);
	}
};

extern C_NetVars gNetVars;

template<typename T>
class CDynamicNetvar
{
	DWORD off;

public:
	template<typename... args_t>
	CDynamicNetvar(args_t... a) {
		off = gNetVars.get_offset(a...);
	}

	template<typename... args_t>
	CDynamicNetvar(int offset, args_t... a) {
		off = gNetVars.get_offset(a...) + offset;
	}

	T GetValue(PVOID base) {
		return *reinterpret_cast<T *>((DWORD)base + (DWORD)off);
	}

	void SetValue(PVOID base, T val) {
		*reinterpret_cast<T *>((DWORD)(base)+((DWORD)(off))) = val;
	}
};

#define DYNVAR(name, type, ...) static CDynamicNetvar<type>  ##name( __VA_ARGS__ )
#define DYNVAR_RETURN(type, base, ...) DYNVAR(n, type, __VA_ARGS__); return n.GetValue(base)
#define DYNVAR_SET(type, base, value, ...) DYNVAR(n, type, __VA_ARGS__); n.SetValue(base,value)