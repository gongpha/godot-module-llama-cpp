#include "llama_common_chat_parameters.h"
#include "llama_common.h"
#include "llama_common_defined.h"
#include "llama_common_utils.h"

#include "common/chat.h"

#include "core/object/class_db.h"

void LlamaCommonChatParameters::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_chat_format", "format"), &LlamaCommonChatParameters::set_chat_format);
	ClassDB::bind_method(D_METHOD("get_chat_format"), &LlamaCommonChatParameters::get_chat_format);
	ClassDB::bind_method(D_METHOD("set_prompt", "prompt"), &LlamaCommonChatParameters::set_prompt);
	ClassDB::bind_method(D_METHOD("get_prompt"), &LlamaCommonChatParameters::get_prompt);
	ClassDB::bind_method(D_METHOD("set_grammar", "grammar"), &LlamaCommonChatParameters::set_grammar);
	ClassDB::bind_method(D_METHOD("get_grammar"), &LlamaCommonChatParameters::get_grammar);
	ClassDB::bind_method(D_METHOD("set_grammar_lazy", "lazy"), &LlamaCommonChatParameters::set_grammar_lazy);
	ClassDB::bind_method(D_METHOD("get_grammar_lazy"), &LlamaCommonChatParameters::get_grammar_lazy);
	ClassDB::bind_method(D_METHOD("set_thinking_forced_open", "forced"), &LlamaCommonChatParameters::set_thinking_forced_open);
	ClassDB::bind_method(D_METHOD("get_thinking_forced_open"), &LlamaCommonChatParameters::get_thinking_forced_open);
	ClassDB::bind_method(D_METHOD("set_grammar_triggers", "triggers"), &LlamaCommonChatParameters::set_grammar_triggers);
	ClassDB::bind_method(D_METHOD("get_grammar_triggers"), &LlamaCommonChatParameters::get_grammar_triggers);
	ClassDB::bind_method(D_METHOD("set_preserved_tokens", "tokens"), &LlamaCommonChatParameters::set_preserved_tokens);
	ClassDB::bind_method(D_METHOD("get_preserved_tokens"), &LlamaCommonChatParameters::get_preserved_tokens);
	ClassDB::bind_method(D_METHOD("set_additional_stops", "stops"), &LlamaCommonChatParameters::set_additional_stops);
	ClassDB::bind_method(D_METHOD("get_additional_stops"), &LlamaCommonChatParameters::get_additional_stops);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "chat_format"), "set_chat_format", "get_chat_format");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "prompt"), "set_prompt", "get_prompt");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "grammar"), "set_grammar", "get_grammar");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "grammar_lazy"), "set_grammar_lazy", "get_grammar_lazy");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "thinking_forced_open"), "set_thinking_forced_open", "get_thinking_forced_open");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "grammar_triggers"), "set_grammar_triggers", "get_grammar_triggers");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "preserved_tokens"), "set_preserved_tokens", "get_preserved_tokens");
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "additional_stops"), "set_additional_stops", "get_additional_stops");
}

void LlamaCommonChatParameters::set_params(const common_chat_params &p_params) {
	params = p_params;
}
const common_chat_params &LlamaCommonChatParameters::get_params() const {
	return params;
}

void LlamaCommonChatParameters::set_chat_format(LlamaCommonChat::ChatFormat p_format) { params.format = (common_chat_format)p_format; }
LlamaCommonChat::ChatFormat LlamaCommonChatParameters::get_chat_format() const { return (LlamaCommonChat::ChatFormat)params.format; }

void LlamaCommonChatParameters::set_prompt(const String &p_prompt) { params.prompt = p_prompt.utf8().get_data(); }
String LlamaCommonChatParameters::get_prompt() const { return String::utf8(params.prompt.c_str()); }

void LlamaCommonChatParameters::set_grammar(const String &p_grammar) { params.grammar = p_grammar.utf8().get_data(); }
String LlamaCommonChatParameters::get_grammar() const { return String::utf8(params.grammar.c_str()); }

void LlamaCommonChatParameters::set_grammar_lazy(bool p_lazy) { params.grammar_lazy = p_lazy; }
bool LlamaCommonChatParameters::get_grammar_lazy() const { return params.grammar_lazy; }

void LlamaCommonChatParameters::set_thinking_forced_open(bool p_forced) { params.thinking_forced_open = p_forced; }
bool LlamaCommonChatParameters::get_thinking_forced_open() const { return params.thinking_forced_open; }

void LlamaCommonChatParameters::set_grammar_triggers(const TypedArray<LlamaCommonGrammarTrigger> &p_triggers) {
	params.grammar_triggers.clear();
	for (int i = 0; i < p_triggers.size(); ++i) {
		Ref<LlamaCommonGrammarTrigger> t = p_triggers[i];
		if (t.is_valid())
			params.grammar_triggers.push_back(t->get_grammar_trigger());
	}
}

TypedArray<LlamaCommonGrammarTrigger> LlamaCommonChatParameters::get_grammar_triggers() const {
	TypedArray<LlamaCommonGrammarTrigger> arr;
	for (const auto &t : params.grammar_triggers) {
		Ref<LlamaCommonGrammarTrigger> rt;
		rt.instantiate();
		rt->set_grammar_trigger(t);
		arr.push_back(rt);
	}
	return arr;
}

void LlamaCommonChatParameters::set_preserved_tokens(const PackedStringArray &p_tokens) {
	params.preserved_tokens.clear();
	for (int i = 0; i < p_tokens.size(); ++i)
		params.preserved_tokens.push_back(p_tokens[i].utf8().get_data());
}

PackedStringArray LlamaCommonChatParameters::get_preserved_tokens() const {
	PackedStringArray arr;
	for (const auto &s : params.preserved_tokens)
		arr.push_back(String::utf8(s.c_str()));
	return arr;
}

void LlamaCommonChatParameters::set_additional_stops(const PackedStringArray &p_stops) {
	params.additional_stops.clear();
	for (int i = 0; i < p_stops.size(); ++i)
		params.additional_stops.push_back(p_stops[i].utf8().get_data());
}

PackedStringArray LlamaCommonChatParameters::get_additional_stops() const {
	PackedStringArray arr;
	for (const auto &s : params.additional_stops)
		arr.push_back(String::utf8(s.c_str()));
	return arr;
}

LlamaCommonChatParameters::LlamaCommonChatParameters() {
}
LlamaCommonChatParameters::~LlamaCommonChatParameters() {
}
