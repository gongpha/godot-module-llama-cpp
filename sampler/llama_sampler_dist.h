#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerDist : public LlamaSampler {
	GDCLASS(LlamaSamplerDist, LlamaSampler);

	uint32_t seed = 0;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_seed", "seed"), &LlamaSamplerDist::set_seed);
		ClassDB::bind_method(D_METHOD("get_seed"), &LlamaSamplerDist::get_seed);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "seed", PROPERTY_HINT_RANGE, "0,4294967295,1"), "set_seed", "get_seed");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_dist(seed); }

public:
	void set_seed(uint32_t v) { seed = v; }
	uint32_t get_seed() const { return seed; }
};
