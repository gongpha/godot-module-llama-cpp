#pragma once

#include "core/object/ref_counted.h"
#include "core/variant/typed_array.h"
#include "core/string/ustring.h"

#include "../llama_model_instance.h"
#include "../llama_context.h"

#include "common/sampling.h"

class LlamaCommonSamplerParams;

class LlamaCommonSampler : public RefCounted {
	GDCLASS(LlamaCommonSampler, RefCounted);

	common_sampler *gsmpl;

	ObjectID model_instance_id;

protected:
	static void _bind_methods();

public:
	bool is_valid() const;

	void init(const LlamaModelInstance *p_model, const Ref<LlamaCommonSamplerParams> &p_params);
	void free();
	void reset();
	void accept(int32_t token, bool accept_grammar = true);
	int32_t sample(const Ref<LlamaContext> &ctx, int32_t idx = -1, bool grammar_first = false);
	Ref<LlamaCommonSampler> clone();
	uint32_t get_seed() const;
	Dictionary get_candidates(bool do_sort = false) const;
	int32_t last() const;
	String print() const;
	String prev_str(const Ref<LlamaContext> &ctx, int32_t n);

	LlamaCommonSampler();
	~LlamaCommonSampler();
};
