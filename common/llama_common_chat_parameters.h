#pragma once

#include "core/io/resource.h"

#include "llama_common_chat.h"
#include "llama_common_defined.h"

class LlamaCommonGrammarTrigger;

class LlamaCommonChatParameters : public Resource {
	GDCLASS(LlamaCommonChatParameters, Resource);

	common_chat_params params;

protected:
	static void _bind_methods();

public:

	void set_params(const common_chat_params &p_params);
	const common_chat_params &get_params() const;

	void set_chat_format(LlamaCommonChat::ChatFormat p_format);
	LlamaCommonChat::ChatFormat get_chat_format() const;

	void set_prompt(const String &p_prompt);
	String get_prompt() const;

	void set_grammar(const String &p_grammar);
	String get_grammar() const;

	void set_grammar_lazy(bool p_lazy);
	bool get_grammar_lazy() const;

	void set_thinking_forced_open(bool p_forced);
	bool get_thinking_forced_open() const;

	void set_grammar_triggers(const TypedArray<LlamaCommonGrammarTrigger> &p_triggers);
	TypedArray<LlamaCommonGrammarTrigger> get_grammar_triggers() const;

	void set_preserved_tokens(const PackedStringArray &p_tokens);
	PackedStringArray get_preserved_tokens() const;

	void set_additional_stops(const PackedStringArray &p_stops);
	PackedStringArray get_additional_stops() const;

	LlamaCommonChatParameters();
	~LlamaCommonChatParameters();
};
