#pragma once

#include "llama_sampler_common.h"

class LlamaSamplerGreedy : public LlamaSampler {
	GDCLASS(LlamaSamplerGreedy, LlamaSampler);

protected:
	static void _bind_methods() {}
	struct llama_sampler* _make_sampler(const llama_vocab *vocab) override { return llama_sampler_init_greedy(); }
};
