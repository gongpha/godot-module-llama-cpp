#pragma once

#include "core/io/resource.h"
#include "llama_common_defined.h"
#include "../llama_context.h"
#include "../llama_vocab.h"

class LlamaCommon : public Object {
	GDCLASS(LlamaCommon, Object);

public:
	enum ReasoningFormat {
		COMMON_REASONING_FORMAT_NONE,
		COMMON_REASONING_FORMAT_AUTO,
		COMMON_REASONING_FORMAT_DEEPSEEK_LEGACY,
		COMMON_REASONING_FORMAT_DEEPSEEK
	};

protected:
	static void _bind_methods();
public:

	static String get_reasoning_format_name(ReasoningFormat format);
	static ReasoningFormat get_reasoning_format_from_name(const String &name);

	static PackedInt32Array tokenize_from_context(const Ref<LlamaContext> &p_context, const String &p_text, bool p_add_special, bool p_parse_special = false);
	static PackedInt32Array tokenize(const Ref<LlamaVocab> &p_vocab, const String &p_text, bool p_add_special, bool p_parse_special = false);

	LlamaCommon() {}
	~LlamaCommon() {}
};

VARIANT_ENUM_CAST(LlamaCommon::ReasoningFormat);

struct common_grammar_trigger;

class LlamaCommonGrammarTrigger : public Resource {
	GDCLASS(LlamaCommonGrammarTrigger, Resource);

	common_grammar_trigger grammar_trigger;

public:
	enum TriggerType {
		COMMON_GRAMMAR_TRIGGER_TYPE_TOKEN,
		COMMON_GRAMMAR_TRIGGER_TYPE_WORD,
		COMMON_GRAMMAR_TRIGGER_TYPE_PATTERN,
		COMMON_GRAMMAR_TRIGGER_TYPE_PATTERN_FULL
	};

protected:
	static void _bind_methods();

public:

	void set_grammar_trigger(const common_grammar_trigger &p_trigger);
	common_grammar_trigger get_grammar_trigger() const;

	void set_type(TriggerType p_type);
	TriggerType get_type() const;

	void set_value(const String &p_value);
	String get_value() const;

	void set_token(int32_t p_token);
	int32_t get_token() const;

	LlamaCommonGrammarTrigger();
	~LlamaCommonGrammarTrigger();
};

VARIANT_ENUM_CAST(LlamaCommonGrammarTrigger::TriggerType);