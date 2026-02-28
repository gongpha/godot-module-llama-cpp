#include "llama_sampler_chain.h"
#include "llama_context.h"
#include "llama_vocab.h"
#include "llama_sampler.h"

#include "core/error/error_macros.h"

#include "thirdparty/llama/include/llama.h"
#include "thirdparty/llama/src/llama-sampler.h"

#include <vector>
#include <string>

LlamaSamplerChainInstance::LlamaSamplerChainInstance() {
	llama_sampler_chain_params p = llama_sampler_chain_default_params();
	chain = llama_sampler_chain_init(p);
}

LlamaSamplerChainInstance::~LlamaSamplerChainInstance() {
	if (chain) {
		llama_sampler_free(chain);
		chain = nullptr;
	}
}

void LlamaSamplerChainInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_valid"), &LlamaSamplerChainInstance::is_valid);

	ClassDB::bind_method(D_METHOD("append_from_chain", "chain"), &LlamaSamplerChainInstance::append_from_chain);
	ClassDB::bind_method(D_METHOD("remove", "idx"), &LlamaSamplerChainInstance::remove);
	ClassDB::bind_method(D_METHOD("clear"), &LlamaSamplerChainInstance::clear);

	ClassDB::bind_method(D_METHOD("size"), &LlamaSamplerChainInstance::size);
	ClassDB::bind_method(D_METHOD("reset"), &LlamaSamplerChainInstance::reset);
	ClassDB::bind_method(D_METHOD("accept", "token"), &LlamaSamplerChainInstance::accept);
	ClassDB::bind_method(D_METHOD("sample", "ctx", "idx"), &LlamaSamplerChainInstance::sample, DEFVAL(-1));

	ClassDB::bind_method(D_METHOD("set_vocab", "vocab"), &LlamaSamplerChainInstance::set_vocab);
	ClassDB::bind_method(D_METHOD("get_vocab"), &LlamaSamplerChainInstance::get_vocab);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "vocab", PROPERTY_HINT_RESOURCE_TYPE, "LlamaVocab"), "set_vocab", "get_vocab");
}

void LlamaSamplerChainInstance::copy_from_chain(const llama_sampler *p_chain) {
	ERR_FAIL_COND(!p_chain);

	if (chain) {
		llama_sampler_free(chain);
	}

	chain = llama_sampler_clone(p_chain);
}

PackedInt32Array LlamaSamplerChainInstance::append_from_chain(const Ref<LlamaSamplerChain> &p_chain) {
	ERR_FAIL_COND_V(!chain, PackedInt32Array());
	ERR_FAIL_COND_V(p_chain.is_null(), PackedInt32Array());

	PackedInt32Array indices;

	for (const Ref<LlamaSampler> &s : p_chain->get_samplers()) {
		if (!s.is_valid()) {
			continue;
		}

		struct llama_sampler *ls = s->_make_sampler(vocab.is_valid() ? vocab->get_ptr() : nullptr);
		if (!ls) {
			continue;
		}

		indices.push_back(llama_sampler_chain_n(chain));
		llama_sampler_chain_add(chain, ls);
	}
	return indices;
}

void LlamaSamplerChainInstance::remove(int32_t idx) {
	ERR_FAIL_COND(!chain);
	ERR_FAIL_INDEX(idx, llama_sampler_chain_n(chain));

	llama_sampler_chain_remove(chain, (size_t)idx);
}

int32_t LlamaSamplerChainInstance::size() const {
	ERR_FAIL_COND_V(!chain, 0);
	return (int32_t)llama_sampler_chain_n(chain);
}

void LlamaSamplerChainInstance::clear() {
	ERR_FAIL_COND(!chain);

	while (llama_sampler_chain_n(chain) > 0) {
		llama_sampler_chain_remove(chain, 0);
	}
}

//
void LlamaSamplerChainInstance::reset() {
	ERR_FAIL_COND(!chain);
	llama_sampler_reset(chain);
}

void LlamaSamplerChainInstance::accept(int32_t token) {
	ERR_FAIL_COND(!chain);
	llama_sampler_accept(chain, (llama_token)token);
}

int32_t LlamaSamplerChainInstance::sample(const Ref<LlamaContext> &ctx_ref, int32_t idx) {
	ERR_FAIL_COND_V(!chain, -1);
	ERR_FAIL_COND_V(ctx_ref.is_null() || !ctx_ref->is_valid(), -1);

	llama_sampler_chain* chainctx = (llama_sampler_chain*)chain->ctx;
	ERR_FAIL_COND_V_MSG(chainctx->samplers.empty(), -1, "Sampler chain is empty");

	llama_context *ctx = ctx_ref->get_native();

	// do not use llama_sampler_sample.
	// because it may throw segmentation fault

	/* from llama_sampler_sample. changed std::vector to LocalVector */
	const auto * logits = llama_get_logits_ith(ctx, idx);

	const llama_model * model = llama_get_model(ctx);
	const llama_vocab * pvocab = llama_model_get_vocab(model);

	const int n_vocab = llama_vocab_n_tokens(pvocab);

	// TODO: do not allocate each time
	LocalVector<llama_token_data> cur; 
	cur.reserve(n_vocab);
	for (llama_token token_id = 0; token_id < n_vocab; token_id++) {
		cur.push_back(llama_token_data{token_id, logits[token_id], 0.0f});
	}

	llama_token_data_array cur_p = {
		/* .data       = */ cur.ptr(),
		/* .size       = */ cur.size(),
		/* .selected   = */ -1,
		/* .sorted     = */ false,
	};

	llama_sampler_apply(chain, &cur_p);
	if (cur_p.selected < 0 || cur_p.selected >= (int32_t) cur_p.size) {
		return -1;
	}
	llama_token token = cur_p.data[cur_p.selected].id;
	llama_sampler_accept(chain, token);

	return (int32_t)token;
	/* llama_sampler_chain_sample */

}

void LlamaSamplerChain::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_samplers", "samplers"), &LlamaSamplerChain::set_samplers_script);
	ClassDB::bind_method(D_METHOD("get_samplers"), &LlamaSamplerChain::get_samplers_script);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "samplers", PROPERTY_HINT_RESOURCE_TYPE, "LlamaSampler"),
		"set_samplers", "get_samplers");
}

void LlamaSamplerChain::set_samplers_script(const TypedArray<Ref<LlamaSampler>> &p_samplers) {
	samplers.resize(p_samplers.size());
	for (int i = 0; i < p_samplers.size(); ++i) {
		samplers[i] = p_samplers[i];
	}
}

TypedArray<Ref<LlamaSampler>> LlamaSamplerChain::get_samplers_script() const {
	TypedArray<Ref<LlamaSampler>> arr;
	const int n = (int)samplers.size();
	arr.resize(n);
	for (int i = 0; i < n; ++i) {
		arr[i] = samplers[i];
	}
	return arr;
}

LlamaSamplerChain::LlamaSamplerChain() {
}

LlamaSamplerChain::~LlamaSamplerChain() {
}