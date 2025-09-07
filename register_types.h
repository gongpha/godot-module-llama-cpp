#pragma once

#include "modules/register_module_types.h"
#include "core/io/resource_loader.h"

void initialize_llama_module(ModuleInitializationLevel p_level);
void uninitialize_llama_module(ModuleInitializationLevel p_level);

void register_llama_samplers();