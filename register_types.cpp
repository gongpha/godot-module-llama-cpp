#include "register_types.h"
#include "core/object/class_db.h"
#include "core/io/resource_loader.h"
#include "core/config/engine.h"
#include "llama_context.h"
#include "llama_model_instance.h"
#include "llama_vocab.h"
#include "llama_sampler_chain.h"
#include "llama_sampler.h"
#include "llama_cpp.h"
#include "llama_gguf.h"
#include "register_types.h"

static LlamaCPP *singleton = nullptr;
static Ref<ResourceFormatLoaderLlamaGGUF> resource_loader_gguf;

void initialize_llama_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		singleton = memnew(LlamaCPP);
		Engine::get_singleton()->add_singleton(Engine::Singleton("LlamaCPP", singleton, "LlamaCPP"));
		ClassDB::register_class<LlamaCPP>();
		ClassDB::register_class<LlamaModelInstance>();
		ClassDB::register_class<LlamaContext>();
		ClassDB::register_class<LlamaVocab>();

		ClassDB::register_class<LlamaSamplerChainInstance>();
		ClassDB::register_class<LlamaSamplerChain>();
		ClassDB::register_abstract_class<LlamaSampler>();
		register_llama_samplers();
		
		ClassDB::register_class<LlamaGGUF>();

		resource_loader_gguf = memnew(ResourceFormatLoaderLlamaGGUF);
		ResourceLoader::add_resource_format_loader(resource_loader_gguf);
	}
}

void uninitialize_llama_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ResourceLoader::remove_resource_format_loader(resource_loader_gguf);
		resource_loader_gguf.unref();

		if (Engine::get_singleton()->has_singleton("LlamaCPP")) {
			Engine::get_singleton()->remove_singleton("LlamaCPP");
			if (singleton) {
				memdelete(singleton);
				singleton = nullptr;
			}
		}
	}
}
