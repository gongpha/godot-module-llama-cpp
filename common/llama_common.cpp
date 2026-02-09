#include "llama_common.h"

#include "core/object/class_db.h"

void LlamaCommon::_bind_methods() {
	ClassDB::bind_static_method("LlamaCommon", D_METHOD("get_reasoning_format_name", "format"), &LlamaCommon::get_reasoning_format_name);
	ClassDB::bind_static_method("LlamaCommon", D_METHOD("get_reasoning_format_from_name", "name"), &LlamaCommon::get_reasoning_format_from_name);

	ClassDB::bind_static_method("LlamaCommon", D_METHOD("tokenize_from_context", "context", "text", "add_special", "parse_special"), &LlamaCommon::tokenize_from_context, DEFVAL(false), DEFVAL(0));
	ClassDB::bind_static_method("LlamaCommon", D_METHOD("tokenize", "vocab", "text", "add_special", "parse_special"), &LlamaCommon::tokenize, DEFVAL(false), DEFVAL(0));

	BIND_ENUM_CONSTANT(COMMON_REASONING_FORMAT_NONE);
	BIND_ENUM_CONSTANT(COMMON_REASONING_FORMAT_AUTO);
	BIND_ENUM_CONSTANT(COMMON_REASONING_FORMAT_DEEPSEEK_LEGACY);
	BIND_ENUM_CONSTANT(COMMON_REASONING_FORMAT_DEEPSEEK);
}

String LlamaCommon::get_reasoning_format_name(LlamaCommon::ReasoningFormat format) {
	switch (format) {
		case COMMON_REASONING_FORMAT_NONE: return "none";
		case COMMON_REASONING_FORMAT_AUTO: return "auto";
		case COMMON_REASONING_FORMAT_DEEPSEEK_LEGACY: return "deepseek-legacy";
		case COMMON_REASONING_FORMAT_DEEPSEEK: return "deepseek";
		default: return "unknown";
	}
}

LlamaCommon::ReasoningFormat LlamaCommon::get_reasoning_format_from_name(const String &name) {
	if (name == "none") return COMMON_REASONING_FORMAT_NONE;
	if (name == "auto") return COMMON_REASONING_FORMAT_AUTO;
	if (name == "deepseek-legacy") return COMMON_REASONING_FORMAT_DEEPSEEK_LEGACY;
	if (name == "deepseek") return COMMON_REASONING_FORMAT_DEEPSEEK;
	return COMMON_REASONING_FORMAT_NONE;
}

PackedInt32Array LlamaCommon::tokenize_from_context(const Ref<LlamaContext> &p_context, const String &p_text, bool p_add_special, bool p_parse_special) {
	ERR_FAIL_COND_V(p_context.is_null(), PackedInt32Array());
	PackedInt32Array result;
	std::vector<int32_t> tokens = common_tokenize(p_context->get_native(), p_text.utf8().get_data(), p_add_special, p_parse_special);
	result.resize(tokens.size());
	for (size_t i = 0; i < tokens.size(); ++i) {
		result.set(i, tokens[i]);
	}
	return result;
}

PackedInt32Array LlamaCommon::tokenize(const Ref<LlamaVocab> &p_vocab, const String &p_text, bool p_add_special, bool p_parse_special) {
	ERR_FAIL_COND_V(p_vocab.is_null(), PackedInt32Array());
	PackedInt32Array result;
	std::vector<int32_t> tokens = common_tokenize(p_vocab->get_ptr(), p_text.utf8().get_data(), p_add_special, p_parse_special);
	result.resize(tokens.size());
	for (size_t i = 0; i < tokens.size(); ++i) {
		result.set(i, tokens[i]);
	}
	return result;
}

void LlamaCommonGrammarTrigger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_type", "type"), &LlamaCommonGrammarTrigger::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &LlamaCommonGrammarTrigger::get_type);
	ClassDB::bind_method(D_METHOD("set_value", "value"), &LlamaCommonGrammarTrigger::set_value);
	ClassDB::bind_method(D_METHOD("get_value"), &LlamaCommonGrammarTrigger::get_value);
	ClassDB::bind_method(D_METHOD("set_token", "token"), &LlamaCommonGrammarTrigger::set_token);
	ClassDB::bind_method(D_METHOD("get_token"), &LlamaCommonGrammarTrigger::get_token);

	BIND_ENUM_CONSTANT(COMMON_GRAMMAR_TRIGGER_TYPE_TOKEN);
	BIND_ENUM_CONSTANT(COMMON_GRAMMAR_TRIGGER_TYPE_WORD);
	BIND_ENUM_CONSTANT(COMMON_GRAMMAR_TRIGGER_TYPE_PATTERN);
	BIND_ENUM_CONSTANT(COMMON_GRAMMAR_TRIGGER_TYPE_PATTERN_FULL);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "type", PROPERTY_HINT_ENUM, "TOKEN,WORD,PATTERN,PATTERN_FULL"), "set_type", "get_type");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "value"), "set_value", "get_value");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "token"), "set_token", "get_token");
}

void LlamaCommonGrammarTrigger::set_grammar_trigger(const common_grammar_trigger &p_trigger) {
	grammar_trigger = p_trigger;
}

common_grammar_trigger LlamaCommonGrammarTrigger::get_grammar_trigger() const {
	return grammar_trigger;
}

void LlamaCommonGrammarTrigger::set_type(TriggerType p_type) {
	grammar_trigger.type = (common_grammar_trigger_type)p_type;
}
LlamaCommonGrammarTrigger::TriggerType LlamaCommonGrammarTrigger::get_type() const {
	return (TriggerType)grammar_trigger.type;
}

void LlamaCommonGrammarTrigger::set_value(const String &p_value) {
	grammar_trigger.value = p_value.utf8().get_data();
}
String LlamaCommonGrammarTrigger::get_value() const {
	return String::utf8(grammar_trigger.value.c_str());
}

void LlamaCommonGrammarTrigger::set_token(int32_t p_token) {
	grammar_trigger.token = p_token;
}
int32_t LlamaCommonGrammarTrigger::get_token() const {
	return (int32_t)grammar_trigger.token;
}

LlamaCommonGrammarTrigger::LlamaCommonGrammarTrigger() {
}
LlamaCommonGrammarTrigger::~LlamaCommonGrammarTrigger() {
}
