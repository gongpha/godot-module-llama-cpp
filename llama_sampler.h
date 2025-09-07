#pragma once

#include "core/io/resource.h"

struct llama_sampler;
struct llama_vocab;

class LlamaSampler : public Resource {
	GDCLASS(LlamaSampler, Resource);

	friend class LlamaSamplerChainInstance;

protected:
	static void _bind_methods();
	virtual struct llama_sampler* _make_sampler(const llama_vocab *vocab) = 0;

public:
	LlamaSampler() {}
	~LlamaSampler() {}
};