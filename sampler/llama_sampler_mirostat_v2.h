#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerMirostatV2 : public LlamaSampler {
	GDCLASS(LlamaSamplerMirostatV2, LlamaSampler);

	uint32_t seed = 0;
	float tau = 5.0f;
	float eta = 0.1f;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_seed", "seed"), &LlamaSamplerMirostatV2::set_seed);
		ClassDB::bind_method(D_METHOD("get_seed"), &LlamaSamplerMirostatV2::get_seed);
		ClassDB::bind_method(D_METHOD("set_tau", "tau"), &LlamaSamplerMirostatV2::set_tau);
		ClassDB::bind_method(D_METHOD("get_tau"), &LlamaSamplerMirostatV2::get_tau);
		ClassDB::bind_method(D_METHOD("set_eta", "eta"), &LlamaSamplerMirostatV2::set_eta);
		ClassDB::bind_method(D_METHOD("get_eta"), &LlamaSamplerMirostatV2::get_eta);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "seed", PROPERTY_HINT_RANGE, "0,4294967295,1"), "set_seed", "get_seed");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tau", PROPERTY_HINT_RANGE, "0.0,50.0,0.001"), "set_tau", "get_tau");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eta", PROPERTY_HINT_RANGE, "0.0,10.0,0.001"), "set_eta", "get_eta");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_mirostat_v2(seed, tau, eta); }

public:
	void set_seed(uint32_t v) { seed = v; }
	uint32_t get_seed() const { return seed; }
	void set_tau(float v) { tau = v; }
	float get_tau() const { return tau; }
	void set_eta(float v) { eta = v; }
	float get_eta() const { return eta; }
};
