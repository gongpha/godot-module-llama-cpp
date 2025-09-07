#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerXTC : public LlamaSampler {
	GDCLASS(LlamaSamplerXTC, LlamaSampler);

	float probability = 0.0f;
	float threshold = 0.1f;
	int32_t min_keep = 1;
	uint32_t seed = 0;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_probability", "p"), &LlamaSamplerXTC::set_probability);
		ClassDB::bind_method(D_METHOD("get_probability"), &LlamaSamplerXTC::get_probability);
		ClassDB::bind_method(D_METHOD("set_threshold", "t"), &LlamaSamplerXTC::set_threshold);
		ClassDB::bind_method(D_METHOD("get_threshold"), &LlamaSamplerXTC::get_threshold);
		ClassDB::bind_method(D_METHOD("set_min_keep", "min_keep"), &LlamaSamplerXTC::set_min_keep);
		ClassDB::bind_method(D_METHOD("get_min_keep"), &LlamaSamplerXTC::get_min_keep);
		ClassDB::bind_method(D_METHOD("set_seed", "seed"), &LlamaSamplerXTC::set_seed);
		ClassDB::bind_method(D_METHOD("get_seed"), &LlamaSamplerXTC::get_seed);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "probability", PROPERTY_HINT_RANGE, "0.0,1.0,0.001"), "set_probability", "get_probability");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "threshold", PROPERTY_HINT_RANGE, "0.0,1.0,0.001"), "set_threshold", "get_threshold");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "min_keep", PROPERTY_HINT_RANGE, "0,100000,1"), "set_min_keep", "get_min_keep");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "seed", PROPERTY_HINT_RANGE, "0,4294967295,1"), "set_seed", "get_seed");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_xtc(probability, threshold, (size_t)min_keep, seed); }

public:
	void set_probability(float v) { probability = v; }
	float get_probability() const { return probability; }
	void set_threshold(float v) { threshold = v; }
	float get_threshold() const { return threshold; }
	void set_min_keep(int32_t v) { min_keep = v; }
	int32_t get_min_keep() const { return min_keep; }
	void set_seed(uint32_t v) { seed = v; }
	uint32_t get_seed() const { return seed; }
};
