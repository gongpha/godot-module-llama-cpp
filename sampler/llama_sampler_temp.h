#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerTemp : public LlamaSampler {
	GDCLASS(LlamaSamplerTemp, LlamaSampler);

	float temp = 1.0f;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_temp", "temp"), &LlamaSamplerTemp::set_temp);
		ClassDB::bind_method(D_METHOD("get_temp"), &LlamaSamplerTemp::get_temp);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "temp", PROPERTY_HINT_RANGE, "0.0,5.0,0.001"), "set_temp", "get_temp");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_temp(temp); }

public:
	void set_temp(float v) { temp = v; }
	float get_temp() const { return temp; }
};
