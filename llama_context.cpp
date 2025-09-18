#include "llama_context.h"
#include "llama_model_instance.h"

#include "core/error/error_macros.h"
#include "core/string/print_string.h"
#include "core/math/math_defs.h"

#include <string>

#include "thirdparty/llama/include/llama.h"

void LlamaContext::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create", "model", "n_ctx", "n_threads"), &LlamaContext::create, DEFVAL(4096), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("destroy"), &LlamaContext::destroy);
	ClassDB::bind_method(D_METHOD("is_valid"), &LlamaContext::is_valid);

	ClassDB::bind_method(D_METHOD("encode", "tokens"), static_cast<Error (LlamaContext::*)(PackedInt32Array)>(&LlamaContext::encode));
	ClassDB::bind_method(D_METHOD("decode", "tokens"), &LlamaContext::decode);
	ClassDB::bind_method(D_METHOD("reset"), &LlamaContext::reset);

	ClassDB::bind_method(D_METHOD("get_n_ctx"), &LlamaContext::get_n_ctx);
	ClassDB::bind_method(D_METHOD("get_n_batch"), &LlamaContext::get_n_batch);
	ClassDB::bind_method(D_METHOD("get_n_ubatch"), &LlamaContext::get_n_ubatch);
	ClassDB::bind_method(D_METHOD("get_n_seq_max"), &LlamaContext::get_n_seq_max);

	// memory
	ClassDB::bind_method(D_METHOD("mem_clear"), &LlamaContext::mem_clear);
	ClassDB::bind_method(D_METHOD("mem_seq_rm", "seq_id", "p0", "p1"), &LlamaContext::mem_seq_rm, DEFVAL(-1), DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("mem_seq_cp", "seq_id_src", "seq_id_dst", "p0", "p1"), &LlamaContext::mem_seq_cp, DEFVAL(-1), DEFVAL(-1), DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("mem_seq_keep", "seq_id"), &LlamaContext::mem_seq_keep);
	ClassDB::bind_method(D_METHOD("mem_seq_add", "seq_id", "p0", "p1", "delta"), &LlamaContext::mem_seq_add, DEFVAL(-1), DEFVAL(-1), DEFVAL(-1), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("mem_seq_div", "seq_id", "p0", "p1", "d"), &LlamaContext::mem_seq_div, DEFVAL(-1), DEFVAL(-1), DEFVAL(-1), DEFVAL(2));
	ClassDB::bind_method(D_METHOD("mem_seq_pos_min", "seq_id"), &LlamaContext::mem_seq_pos_min);
	ClassDB::bind_method(D_METHOD("mem_seq_pos_max", "seq_id"), &LlamaContext::mem_seq_pos_max);
	ClassDB::bind_method(D_METHOD("mem_can_shift"), &LlamaContext::mem_can_shift);
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


Error LlamaContext::encode(PackedInt32Array p_tokens) {
	ERR_FAIL_COND_V(ctx == nullptr, ERR_UNCONFIGURED);
	if (p_tokens.is_empty()) return OK;

	// temporary copy to ensure we have a writable pointer
	llama_token *tokens = memnew_arr(llama_token, p_tokens.size());
	for (int i = 0; i < p_tokens.size(); i++) {
		tokens[i] = p_tokens[i];
	}

	int rc = llama_encode(ctx, llama_batch_get_one(tokens, p_tokens.size()));
	memdelete_arr(tokens);
	return rc == 0 ? OK : ERR_CANT_ACQUIRE_RESOURCE;
}

Error LlamaContext::decode(const PackedInt32Array &p_tokens) {
	ERR_FAIL_COND_V(ctx == nullptr, ERR_UNCONFIGURED);
	if (p_tokens.is_empty()) return OK;

	// temporary copy to ensure we have a writable pointer
	llama_token *tokens = memnew_arr(llama_token, p_tokens.size());
	for (int i = 0; i < p_tokens.size(); i++) {
		tokens[i] = p_tokens[i];
	}

	int rc = llama_decode(ctx, llama_batch_get_one(tokens, p_tokens.size()));
	memdelete_arr(tokens);
	return rc == 0 ? OK : ERR_CANT_ACQUIRE_RESOURCE;
}

uint32_t LlamaContext::get_n_ctx() const {
	ERR_FAIL_COND_V(ctx == nullptr, 0);
	return llama_n_ctx(ctx);
}

uint32_t LlamaContext::get_n_batch() const {
	ERR_FAIL_COND_V(ctx == nullptr, 0);
	return llama_n_batch(ctx);
}

uint32_t LlamaContext::get_n_ubatch() const {
	ERR_FAIL_COND_V(ctx == nullptr, 0);
	return llama_n_ubatch(ctx);
}

uint32_t LlamaContext::get_n_seq_max() const {
	ERR_FAIL_COND_V(ctx == nullptr, 0);
	return llama_n_seq_max(ctx);
}

void LlamaContext::mem_clear() {
	ERR_FAIL_COND(ctx == nullptr);
	llama_memory_clear(llama_get_memory(ctx), true);
}

void LlamaContext::mem_seq_rm(int32_t p_seq_id, int32_t p_p0, int32_t p_p1) {
	ERR_FAIL_COND(ctx == nullptr);
	llama_memory_seq_rm(llama_get_memory(ctx), p_seq_id, p_p0, p_p1);
}

void LlamaContext::mem_seq_cp(int32_t p_seq_id_src, int32_t p_seq_id_dst, int32_t p_p0, int32_t p_p1) {
	ERR_FAIL_COND(ctx == nullptr);
	llama_memory_seq_cp(llama_get_memory(ctx), p_seq_id_src, p_seq_id_dst, p_p0, p_p1);
}

void LlamaContext::mem_seq_keep(int32_t p_seq_id) {
	ERR_FAIL_COND(ctx == nullptr);
	llama_memory_seq_keep(llama_get_memory(ctx), p_seq_id);
}

void LlamaContext::mem_seq_add(int32_t p_seq_id, int32_t p_p0, int32_t p_p1, int32_t p_delta) {
	ERR_FAIL_COND(ctx == nullptr);
	llama_memory_seq_add(llama_get_memory(ctx), p_seq_id, p_p0, p_p1, p_delta);
}

void LlamaContext::mem_seq_div(int32_t p_seq_id, int32_t p_p0, int32_t p_p1, int p_d) {
	ERR_FAIL_COND(ctx == nullptr);
	llama_memory_seq_div(llama_get_memory(ctx), p_seq_id, p_p0, p_p1, p_d);
}

int32_t LlamaContext::mem_seq_pos_min(int32_t p_seq_id) {
	ERR_FAIL_COND_V(ctx == nullptr, -1);
	return llama_memory_seq_pos_min(llama_get_memory(ctx), p_seq_id);
}

int32_t LlamaContext::mem_seq_pos_max(int32_t p_seq_id) {
	ERR_FAIL_COND_V(ctx == nullptr, -1);
	return llama_memory_seq_pos_max(llama_get_memory(ctx), p_seq_id);
}

bool LlamaContext::mem_can_shift() {
	ERR_FAIL_COND_V(ctx == nullptr, false);
	return llama_memory_can_shift(llama_get_memory(ctx));
}

void LlamaContext::reset() {
	if (!ctx) return;
	llama_memory_clear(llama_get_memory(ctx), true);
}


