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

// New common chat wrappers
#include "common/llama_common.h"
#include "common/llama_common_chat.h"
#include "common/llama_common_chat_message.h"
#include "common/llama_common_chat_messages.h"
#include "common/llama_common_chat_parameters.h"
#include "common/llama_common_chat_templates.h"
#include "common/llama_common_chat_templates_input.h"
#include "common/llama_common_chat_tool.h"
#include "common/llama_common_sampler_params.h"
#include "common/llama_common_sampler.h"

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

		ClassDB::register_class<LlamaCommon>();
		ClassDB::register_class<LlamaCommonGrammarTrigger>();
		ClassDB::register_class<LlamaCommonChat>();
		ClassDB::register_class<LlamaCommonChatSyntax>();
		ClassDB::register_class<LlamaCommonChatTool>();
		ClassDB::register_class<LlamaCommonChatToolCall>();
		ClassDB::register_class<LlamaCommonChatMessage>();
		ClassDB::register_class<LlamaCommonChatMessageDiff>();
		ClassDB::register_class<LlamaCommonChatMessages>();
		ClassDB::register_class<LlamaCommonChatParameters>();
		ClassDB::register_class<LlamaCommonChatTemplatesInput>();
		ClassDB::register_class<LlamaCommonChatTemplates>();
		ClassDB::register_class<LlamaCommonSamplerParams>();
		ClassDB::register_class<LlamaCommonSampler>();

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
