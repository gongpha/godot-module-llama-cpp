#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerInfill : public LlamaSampler {
	GDCLASS(LlamaSamplerInfill, LlamaSampler);

protected:
	static void _bind_methods() {
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "vocab", PROPERTY_HINT_RESOURCE_TYPE, "LlamaVocab"), "set_vocab", "get_vocab");
	}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override {
		ERR_FAIL_COND_V_MSG(vocab == nullptr, nullptr, "requries a valid LlamaVocab to create the sampler");
		return llama_sampler_init_infill(vocab);
	}
};
