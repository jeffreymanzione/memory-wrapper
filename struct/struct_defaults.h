
#ifndef STRUCT_STRUCT_DEFAULTS_H_
#define STRUCT_STRUCT_DEFAULTS_H_
#include "struct/map.h"
#include "struct/set.h"

void map_init_default(Map *map);
Map *map_create_default();

Set *set_create_default();
void set_init_default(Set *set);

#endif /* STRUCT_STRUCT_DEFAULTS_H_ */