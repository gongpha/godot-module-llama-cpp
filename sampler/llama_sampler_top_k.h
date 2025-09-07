#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerTopK : public LlamaSampler {
	GDCLASS(LlamaSamplerTopK, LlamaSampler);

	int32_t k = 40;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_k", "k"), &LlamaSamplerTopK::set_k);
		ClassDB::bind_method(D_METHOD("get_k"), &LlamaSamplerTopK::get_k);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "k", PROPERTY_HINT_RANGE, "1,100000,1"), "set_k", "get_k");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_top_k(k); }

public:
	void set_k(int32_t p_k) { k = p_k; }
	int32_t get_k() const { return k; }
};
