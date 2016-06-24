
#ifndef GUID_DEF_HPP__EX__
#define GUID_DEF_HPP__EX__

#define MAKE_ENTITY_GUID(entity_type, seed_value, auto_index)	(((uint64)entity_type & 0x00000000000000FF) << 56) | (((uint64)seed_value & 0x00000000000000FF) << 48) | (auto_index & 0xFFFFFFFFFFFF)

#define SWITCH_GUID_TYPE(entity_type, guid)		((((uint64)entity_type & 0x00000000000000FF) << 56) | (guid & 0x00FFFFFFFFFFFFFF))

#endif
