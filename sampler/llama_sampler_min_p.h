#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerMinP : public LlamaSampler {
	GDCLASS(LlamaSamplerMinP, LlamaSampler);

	float p = 0.05f;
	int32_t min_keep = 1;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_p", "p"), &LlamaSamplerMinP::set_p);
		ClassDB::bind_method(D_METHOD("get_p"), &LlamaSamplerMinP::get_p);
		ClassDB::bind_method(D_METHOD("set_min_keep", "min_keep"), &LlamaSamplerMinP::set_min_keep);
		ClassDB::bind_method(D_METHOD("get_min_keep"), &LlamaSamplerMinP::get_min_keep);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "p", PROPERTY_HINT_RANGE, "0.0,1.0,0.001"), "set_p", "get_p");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "min_keep", PROPERTY_HINT_RANGE, "0,100000,1"), "set_min_keep", "get_min_keep");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_min_p(p, (size_t)min_keep); }

public:
	void set_p(float v) { p = v; }
	float get_p() const { return p; }
	void set_min_keep(int32_t v) { min_keep = v; }
	int32_t get_min_keep() const { return min_keep; }
};
