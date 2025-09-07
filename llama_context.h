#pragma once

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/variant.h"

class LlamaModelInstance;
struct llama_context;

class LlamaContext : public RefCounted {
	GDCLASS(LlamaContext, RefCounted);

	ObjectID model_id = ObjectID();
	llama_context *ctx = nullptr;

protected:
	static void _bind_methods();

public:
	Error create(LlamaModelInstance *p_model, int32_t p_n_ctx = 4096, int32_t p_n_threads = 0);
	void destroy();

	bool is_valid() const { return ctx != nullptr; }

	llama_context *get_native() const { return ctx; }

	Error decode(const PackedInt32Array &p_tokens);

	// Clear KV cache/state (reset sequence)
	void reset();

	// Sampling is provided via LlamaSamplerChainInstance; no single-step sampler here

	LlamaContext();
	~LlamaContext();
};
