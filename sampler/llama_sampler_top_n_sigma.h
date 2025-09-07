#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerTopNSigma : public LlamaSampler {
	GDCLASS(LlamaSamplerTopNSigma, LlamaSampler);

	float n = 1.0f;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_n", "n"), &LlamaSamplerTopNSigma::set_n);
		ClassDB::bind_method(D_METHOD("get_n"), &LlamaSamplerTopNSigma::get_n);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "n", PROPERTY_HINT_RANGE, "0.0,100.0,0.001"), "set_n", "get_n");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_top_n_sigma(n); }

public:
	void set_n(float v) { n = v; }
	float get_n() const { return n; }
};
