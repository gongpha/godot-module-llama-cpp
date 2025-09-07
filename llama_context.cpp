#include "llama_context.h"
#include "llama_model_instance.h"

#include "core/error/error_macros.h"
#include "core/string/print_string.h"
#include "core/math/math_defs.h"

#include <string>
#include <vector>

#include "thirdparty/llama/include/llama.h"

void LlamaContext::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create", "model", "n_ctx", "n_threads"), &LlamaContext::create, DEFVAL(4096), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("destroy"), &LlamaContext::destroy);
	ClassDB::bind_method(D_METHOD("is_valid"), &LlamaContext::is_valid);

	// Low-level API
	ClassDB::bind_method(D_METHOD("decode", "tokens"), &LlamaContext::decode);
	ClassDB::bind_method(D_METHOD("reset"), &LlamaContext::reset);
	// Sampling via LlamaSamplerChainInstance (separate class)
}

LlamaContext::LlamaContext() {}
LlamaContext::~LlamaContext() { destroy(); }

Error LlamaContext::create(LlamaModelInstance *p_model, int32_t p_n_ctx, int32_t p_n_threads) {
	destroy();
	ERR_FAIL_COND_V(p_model == nullptr, ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(!p_model->is_ready(), ERR_UNCONFIGURED);
	model_id = p_model->get_instance_id();

	llama_context_params params = llama_context_default_params();
	params.n_ctx = p_n_ctx;
	params.n_threads = p_n_threads > 0 ? p_n_threads : params.n_threads;

	// Prefer the non-deprecated initializer
	ctx = llama_init_from_model(p_model->get_model_ptr(), params);
	if (!ctx) {
		ERR_PRINT("LlamaContext: failed to create context");
		model_id = ObjectID();
		return ERR_CANT_CREATE;
	}
	return OK;
}

void LlamaContext::destroy() {
	if (ctx) {
		llama_free(ctx);
		ctx = nullptr;
	}
	model_id = ObjectID();
}

Error LlamaContext::decode(const PackedInt32Array &p_tokens) {
	ERR_FAIL_COND_V(ctx == nullptr, ERR_UNCONFIGURED);
	if (p_tokens.is_empty()) return OK;

	std::vector<llama_token> tmp((size_t)p_tokens.size());
	for (int i = 0; i < p_tokens.size(); ++i) tmp[(size_t)i] = (llama_token)p_tokens[i];

	int rc = llama_decode(ctx, llama_batch_get_one(tmp.data(), (int)tmp.size()));
	return rc == 0 ? OK : ERR_CANT_ACQUIRE_RESOURCE;
}

void LlamaContext::reset() {
	if (!ctx) return;
	llama_memory_clear(llama_get_memory(ctx), true);
}


