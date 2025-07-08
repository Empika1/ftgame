#include "register_types.h"

#include "render.h"
#include "sim_adapter.h"

void initialize_ftmodules_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    ClassDB::register_class<FTRender>();

    ClassDB::register_class<FTBlock>();
    ClassDB::register_class<FTRect>();
    ClassDB::register_class<FTBackend>();
    ClassDB::register_class<FTDesign>();
}

void uninitialize_ftmodules_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    // Nothing to do here in this example.
}
