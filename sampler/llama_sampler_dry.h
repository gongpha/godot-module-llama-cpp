#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerDRY : public LlamaSampler {
	GDCLASS(LlamaSamplerDRY, LlamaSampler);

	int32_t n_ctx_train = 2048;
	float multiplier = 0.0f;
	float base = 1.0f;
	int32_t allowed_length = 0;
	int32_t penalty_last_n = 0; // -1 to use n_ctx_train
	PackedStringArray sequence_breakers;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_n_ctx_train", "n"), &LlamaSamplerDRY::set_n_ctx_train);
		ClassDB::bind_method(D_METHOD("get_n_ctx_train"), &LlamaSamplerDRY::get_n_ctx_train);
		ClassDB::bind_method(D_METHOD("set_multiplier", "multiplier"), &LlamaSamplerDRY::set_multiplier);
		ClassDB::bind_method(D_METHOD("get_multiplier"), &LlamaSamplerDRY::get_multiplier);
		ClassDB::bind_method(D_METHOD("set_base", "base"), &LlamaSamplerDRY::set_base);
		ClassDB::bind_method(D_METHOD("get_base"), &LlamaSamplerDRY::get_base);
		ClassDB::bind_method(D_METHOD("set_allowed_length", "len"), &LlamaSamplerDRY::set_allowed_length);
		ClassDB::bind_method(D_METHOD("get_allowed_length"), &LlamaSamplerDRY::get_allowed_length);
		ClassDB::bind_method(D_METHOD("set_penalty_last_n", "n"), &LlamaSamplerDRY::set_penalty_last_n);
		ClassDB::bind_method(D_METHOD("get_penalty_last_n"), &LlamaSamplerDRY::get_penalty_last_n);
		ClassDB::bind_method(D_METHOD("set_sequence_breakers", "arr"), &LlamaSamplerDRY::set_sequence_breakers);
		ClassDB::bind_method(D_METHOD("get_sequence_breakers"), &LlamaSamplerDRY::get_sequence_breakers);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "n_ctx_train", PROPERTY_HINT_RANGE, "1,1048576,1"), "set_n_ctx_train", "get_n_ctx_train");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "multiplier", PROPERTY_HINT_RANGE, "0.0,10.0,0.001"), "set_multiplier", "get_multiplier");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "base", PROPERTY_HINT_RANGE, "1.0,10.0,0.001"), "set_base", "get_base");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "allowed_length", PROPERTY_HINT_RANGE, "0,100000,1"), "set_allowed_length", "get_allowed_length");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "penalty_last_n", PROPERTY_HINT_RANGE, "-1,100000,1"), "set_penalty_last_n", "get_penalty_last_n");
		ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "sequence_breakers"), "set_sequence_breakers", "get_sequence_breakers");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override {
		ERR_FAIL_COND_V_MSG(vocab == nullptr, nullptr, "requries a valid LlamaVocab to create the sampler");

		Vector<const char*> cstrs;
		cstrs.resize(sequence_breakers.size());
		for (int i = 0; i < sequence_breakers.size(); ++i) {
			cstrs.write[i] = sequence_breakers[i].utf8().get_data();
		}
		const char **arr = cstrs.size() ? (const char**)cstrs.ptr() : nullptr;
		size_t count = (size_t)cstrs.size();
		return llama_sampler_init_dry(vocab, n_ctx_train, multiplier, base, allowed_length, penalty_last_n, arr, count);
	}

public:
	void set_n_ctx_train(int32_t v) { n_ctx_train = v; }
	int32_t get_n_ctx_train() const { return n_ctx_train; }
	void set_multiplier(float v) { multiplier = v; }
	float get_multiplier() const { return multiplier; }
	void set_base(float v) { base = v; }
	float get_base() const { return base; }
	void set_allowed_length(int32_t v) { allowed_length = v; }
	int32_t get_allowed_length() const { return allowed_length; }
	void set_penalty_last_n(int32_t v) { penalty_last_n = v; }
	int32_t get_penalty_last_n() const { return penalty_last_n; }
	void set_sequence_breakers(const PackedStringArray &arr) { sequence_breakers = arr; }
	PackedStringArray get_sequence_breakers() const { return sequence_breakers; }
};
