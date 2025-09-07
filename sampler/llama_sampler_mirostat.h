#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerMirostat : public LlamaSampler {
	GDCLASS(LlamaSamplerMirostat, LlamaSampler);

	int32_t n_vocab = 0; // should be set from vocab
	uint32_t seed = 0;
	float tau = 5.0f;
	float eta = 0.1f;
	int32_t m = 100;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_n_vocab", "n_vocab"), &LlamaSamplerMirostat::set_n_vocab);
		ClassDB::bind_method(D_METHOD("get_n_vocab"), &LlamaSamplerMirostat::get_n_vocab);
		ClassDB::bind_method(D_METHOD("set_seed", "seed"), &LlamaSamplerMirostat::set_seed);
		ClassDB::bind_method(D_METHOD("get_seed"), &LlamaSamplerMirostat::get_seed);
		ClassDB::bind_method(D_METHOD("set_tau", "tau"), &LlamaSamplerMirostat::set_tau);
		ClassDB::bind_method(D_METHOD("get_tau"), &LlamaSamplerMirostat::get_tau);
		ClassDB::bind_method(D_METHOD("set_eta", "eta"), &LlamaSamplerMirostat::set_eta);
		ClassDB::bind_method(D_METHOD("get_eta"), &LlamaSamplerMirostat::get_eta);
		ClassDB::bind_method(D_METHOD("set_m", "m"), &LlamaSamplerMirostat::set_m);
		ClassDB::bind_method(D_METHOD("get_m"), &LlamaSamplerMirostat::get_m);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "n_vocab", PROPERTY_HINT_RANGE, "0,500000,1"), "set_n_vocab", "get_n_vocab");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "seed", PROPERTY_HINT_RANGE, "0,4294967295,1"), "set_seed", "get_seed");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tau", PROPERTY_HINT_RANGE, "0.0,50.0,0.001"), "set_tau", "get_tau");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eta", PROPERTY_HINT_RANGE, "0.0,10.0,0.001"), "set_eta", "get_eta");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "m", PROPERTY_HINT_RANGE, "1,100000,1"), "set_m", "get_m");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_mirostat(n_vocab, seed, tau, eta, m); }

public:
	void set_n_vocab(int32_t v) { n_vocab = v; }
	int32_t get_n_vocab() const { return n_vocab; }
	void set_seed(uint32_t v) { seed = v; }
	uint32_t get_seed() const { return seed; }
	void set_tau(float v) { tau = v; }
	float get_tau() const { return tau; }
	void set_eta(float v) { eta = v; }
	float get_eta() const { return eta; }
	void set_m(int32_t v) { m = v; }
	int32_t get_m() const { return m; }
};
