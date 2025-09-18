#pragma once

#include "core/io/resource.h"
#include "core/variant/typed_array.h"
#include "core/string/ustring.h"

class LlamaContext;
class LlamaSampler;
class LlamaSamplerChain;
class LlamaVocab;
struct llama_sampler;

class LlamaSamplerChainInstance : public RefCounted {
	GDCLASS(LlamaSamplerChainInstance, RefCounted);

	llama_sampler *chain = nullptr;

	Ref<LlamaVocab> vocab;

protected:
	static void _bind_methods();

public:
	bool is_valid() const { return chain != nullptr; }

	void copy_from_chain(const llama_sampler *p_chain);
	PackedInt32Array append_from_chain(const Ref<LlamaSamplerChain> &p_chain);

	void remove(int32_t idx);
	int32_t size() const;
	void clear();

	void accept(int32_t token);

	void reset();

	int32_t sample(const Ref<LlamaContext> &ctx, int32_t idx = -1);

	void set_vocab(const Ref<LlamaVocab> &p_vocab) { vocab = p_vocab; }
	Ref<LlamaVocab> get_vocab() const { return vocab; }

	LlamaSamplerChainInstance();
	~LlamaSamplerChainInstance();
};

class LlamaSamplerChain : public Resource {
	GDCLASS(LlamaSamplerChain, Resource);

	LocalVector<Ref<LlamaSampler>> samplers;

protected:
	static void _bind_methods();

public:
	const LocalVector<Ref<LlamaSampler>> &get_samplers() { return samplers; }

	void set_samplers_script(const TypedArray<Ref<LlamaSampler>> &p_samplers);
	TypedArray<Ref<LlamaSampler>> get_samplers_script() const;

	LlamaSamplerChain();
	~LlamaSamplerChain();
};