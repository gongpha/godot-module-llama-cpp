#pragma once

#include "llama_sampler_common.h"
#include "core/variant/typed_array.h"

class LlamaSamplerLogitBias : public LlamaSampler {
	GDCLASS(LlamaSamplerLogitBias, LlamaSampler);

	int32_t n_vocab = 0; // should match model vocab size
	TypedArray<Dictionary> biases; // each dict: {"token": int, "bias": float}

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_n_vocab", "n_vocab"), &LlamaSamplerLogitBias::set_n_vocab);
		ClassDB::bind_method(D_METHOD("get_n_vocab"), &LlamaSamplerLogitBias::get_n_vocab);
		ClassDB::bind_method(D_METHOD("set_biases", "biases"), &LlamaSamplerLogitBias::set_biases);
		ClassDB::bind_method(D_METHOD("get_biases"), &LlamaSamplerLogitBias::get_biases);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "n_vocab", PROPERTY_HINT_RANGE, "0,500000,1"), "set_n_vocab", "get_n_vocab");
		ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "biases", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT, "Dictionary"), "set_biases", "get_biases");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override {
		Vector<llama_logit_bias> arr;
		arr.resize(biases.size());
		for (int i = 0; i < biases.size(); ++i) {
			Dictionary d = biases[i];
			llama_logit_bias lb;
			lb.token = (int32_t)d.get("token", 0);
			lb.bias = (float)(double)d.get("bias", 0.0);
			arr.write[i] = lb;
		}
		return llama_sampler_init_logit_bias(n_vocab, arr.size(), arr.ptr());
	}

public:
	void set_n_vocab(int32_t v) { n_vocab = v; }
	int32_t get_n_vocab() const { return n_vocab; }
	void set_biases(const TypedArray<Dictionary> &v) { biases = v; }
	TypedArray<Dictionary> get_biases() const { return biases; }
};
