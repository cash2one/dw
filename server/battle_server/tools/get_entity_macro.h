
#ifndef get_entity_macro_hpp__
#define get_entity_macro_hpp__

#define Get_Entity_From_Pool(guid, entity_ins, entity_type)	\
	entity_type * entity_ins = (entity_type *)m_plugin_cfg.pool->get(guid);

#define Get_Entity_Summoner(guid)	\
	Get_Entity_From_Pool(guid, summoner, Entity::summoner);
	

#endif