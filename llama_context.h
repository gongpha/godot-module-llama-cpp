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

	uint32_t get_n_ctx() const;
	uint32_t get_n_batch() const;
	uint32_t get_n_ubatch() const;
	uint32_t get_n_seq_max() const;

	// memory
	void mem_clear();
	void mem_seq_rm(int32_t p_seq_id = -1, int32_t p_p0 = -1, int32_t p_p1 = -1);
	void mem_seq_cp(int32_t p_seq_id_src, int32_t p_seq_id_dst, int32_t p_p0 = -1, int32_t p_p1 = -1);
	void mem_seq_keep(int32_t p_seq_id);
	void mem_seq_add(int32_t p_seq_id, int32_t p_p0 = -1, int32_t p_p1 = -1, int32_t p_delta = 0);
	void mem_seq_div(int32_t p_seq_id, int32_t p_p0 = -1, int32_t p_p1 = -1, int p_d = 2);
	int32_t mem_seq_pos_min(int32_t p_seq_id);
	int32_t mem_seq_pos_max(int32_t p_seq_id);
	bool mem_can_shift();

	// Clear KV cache/state (reset sequence)
	void reset();

	// Sampling is provided via LlamaSamplerChainInstance; no single-step sampler here

	LlamaContext();
	~LlamaContext();
};
