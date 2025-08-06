#include "register_types.h"

#include "render.h"
#include "sim_adapter.h"

void initialize_ftmodules_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    ClassDB::register_class<FTRender>();

    ClassDB::register_class<FTRect>();
    ClassDB::register_class<FTBlockSpec>();
    ClassDB::register_class<FTDesignSpec>();
    ClassDB::register_class<FTBlock>();
    ClassDB::register_class<FTJoint>();
    ClassDB::register_class<FTJointStack>();
    ClassDB::register_class<FTDesign>();
    ClassDB::register_class<FTSimSettings>();
    ClassDB::register_class<FTSimState>();
    ClassDB::register_class<FTBackend>();
}

void uninitialize_ftmodules_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    // Nothing to do here in this example.
}
