#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerTempExt : public LlamaSampler {
	GDCLASS(LlamaSamplerTempExt, LlamaSampler);

	float temp = 1.0f;
	float delta = 0.0f;
	float exponent = 1.0f;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_temp", "temp"), &LlamaSamplerTempExt::set_temp);
		ClassDB::bind_method(D_METHOD("get_temp"), &LlamaSamplerTempExt::get_temp);
		ClassDB::bind_method(D_METHOD("set_delta", "delta"), &LlamaSamplerTempExt::set_delta);
		ClassDB::bind_method(D_METHOD("get_delta"), &LlamaSamplerTempExt::get_delta);
		ClassDB::bind_method(D_METHOD("set_exponent", "exponent"), &LlamaSamplerTempExt::set_exponent);
		ClassDB::bind_method(D_METHOD("get_exponent"), &LlamaSamplerTempExt::get_exponent);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "temp", PROPERTY_HINT_RANGE, "0.0,5.0,0.001"), "set_temp", "get_temp");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "delta", PROPERTY_HINT_RANGE, "0.0,5.0,0.001"), "set_delta", "get_delta");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "exponent", PROPERTY_HINT_RANGE, "0.0,5.0,0.001"), "set_exponent", "get_exponent");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_temp_ext(temp, delta, exponent); }

public:
	void set_temp(float v) { temp = v; }
	float get_temp() const { return temp; }
	void set_delta(float v) { delta = v; }
	float get_delta() const { return delta; }
	void set_exponent(float v) { exponent = v; }
	float get_exponent() const { return exponent; }
};
