#pragma once

#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/typed_array.h"
#include "thirdparty/llama/include/llama.h"
class LlamaModelInstance;
struct llama_vocab;

class LlamaVocab : public RefCounted {
	GDCLASS(LlamaVocab, RefCounted);

	const llama_vocab* vocab;

public:

	enum VocabType : int {
		VOCAB_TYPE_NONE   = LLAMA_VOCAB_TYPE_NONE,
		VOCAB_TYPE_SPM    = LLAMA_VOCAB_TYPE_SPM,
		VOCAB_TYPE_BPE    = LLAMA_VOCAB_TYPE_BPE,
		VOCAB_TYPE_WPM    = LLAMA_VOCAB_TYPE_WPM,
		VOCAB_TYPE_UGM    = LLAMA_VOCAB_TYPE_UGM,
		VOCAB_TYPE_RWKV   = LLAMA_VOCAB_TYPE_RWKV,
		VOCAB_TYPE_PLAMO2 = LLAMA_VOCAB_TYPE_PLAMO2,
	};

protected:
	static void _bind_methods();

public:
	Error init_from_model(LlamaModelInstance *p_model);
	void destroy();

	bool is_valid() const { return vocab != nullptr; }

	int32_t get_token_count() const;
	VocabType get_vocab_type() const;

	int32_t bos() const;
	int32_t eos() const;
	int32_t eot() const;
	int32_t sep() const;
	int32_t nl() const;
	int32_t pad() const;
	int32_t mask() const;
	bool add_bos() const;
	bool add_eos() const;
	bool add_sep() const;

	// Queries
	bool is_eog(int32_t p_token) const;
	bool is_control(int32_t p_token) const;
	String get_text(int32_t p_token) const;
	double get_score(int32_t p_token) const;
	String token_to_piece(int32_t p_token, int32_t p_lstrip = 0, bool p_special = true) const;

	// Tokenizer helpers
	PackedInt32Array tokenize(const String &p_text, bool p_add_special = true, bool p_parse_special = true) const;
	String detokenize(const PackedInt32Array &p_tokens, bool p_remove_special = false, bool p_unparse_special = true) const;

	const llama_vocab *get_ptr() const { return vocab; }

	LlamaVocab();
	~LlamaVocab();
};

VARIANT_ENUM_CAST(LlamaVocab::VocabType);