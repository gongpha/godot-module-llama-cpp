#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerPenalties : public LlamaSampler {
	GDCLASS(LlamaSamplerPenalties, LlamaSampler);

	int32_t last_n = 64;
	float repeat = 1.1f;
	float freq = 0.0f;
	float present = 0.0f;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_last_n", "n"), &LlamaSamplerPenalties::set_last_n);
		ClassDB::bind_method(D_METHOD("get_last_n"), &LlamaSamplerPenalties::get_last_n);
		ClassDB::bind_method(D_METHOD("set_repeat", "repeat"), &LlamaSamplerPenalties::set_repeat);
		ClassDB::bind_method(D_METHOD("get_repeat"), &LlamaSamplerPenalties::get_repeat);
		ClassDB::bind_method(D_METHOD("set_freq", "freq"), &LlamaSamplerPenalties::set_freq);
		ClassDB::bind_method(D_METHOD("get_freq"), &LlamaSamplerPenalties::get_freq);
		ClassDB::bind_method(D_METHOD("set_present", "present"), &LlamaSamplerPenalties::set_present);
		ClassDB::bind_method(D_METHOD("get_present"), &LlamaSamplerPenalties::get_present);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "last_n", PROPERTY_HINT_RANGE, "0,100000,1"), "set_last_n", "get_last_n");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "repeat", PROPERTY_HINT_RANGE, "0.0,10.0,0.001"), "set_repeat", "get_repeat");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "freq", PROPERTY_HINT_RANGE, "-10.0,10.0,0.001"), "set_freq", "get_freq");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "present", PROPERTY_HINT_RANGE, "-10.0,10.0,0.001"), "set_present", "get_present");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_penalties(last_n, repeat, freq, present); }

public:
	void set_last_n(int32_t v) { last_n = v; }
	int32_t get_last_n() const { return last_n; }
	void set_repeat(float v) { repeat = v; }
	float get_repeat() const { return repeat; }
	void set_freq(float v) { freq = v; }
	float get_freq() const { return freq; }
	void set_present(float v) { present = v; }
	float get_present() const { return present; }
};
