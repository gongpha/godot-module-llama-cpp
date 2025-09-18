#include "llama_common_chat.h"
#include "llama_common.h"
#include "llama_common_defined.h"
#include "llama_common_utils.h"

#include "../../thirdparty/llama/common/chat.h"
#include "../../thirdparty/llama/common/common.h"

#include "core/object/class_db.h"

// LlamaCommonChat

void LlamaCommonChat::_bind_methods() {
	ClassDB::bind_static_method("LlamaCommonChat", D_METHOD("verify_template", "tmpl", "use_jinja"), &LlamaCommonChat::verify_template);
	ClassDB::bind_static_method("LlamaCommonChat", D_METHOD("get_format_name", "format"), &LlamaCommonChat::get_format_name);

	BIND_ENUM_CONSTANT(CHAT_TOOL_CHOICE_AUTO);
	BIND_ENUM_CONSTANT(CHAT_TOOL_CHOICE_REQUIRED);
	BIND_ENUM_CONSTANT(CHAT_TOOL_CHOICE_NONE);

	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_CONTENT_ONLY);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_GENERIC);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_MISTRAL_NEMO);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_LLAMA_3_X);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_LLAMA_3_X_WITH_BUILTIN_TOOLS);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_DEEPSEEK_R1);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_FIREFUNCTION_V2);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_FUNCTIONARY_V3_2);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_FUNCTIONARY_V3_1_LLAMA_3_1);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_DEEPSEEK_V3_1);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_HERMES_2_PRO);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_COMMAND_R7B);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_GRANITE);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_GPT_OSS);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_SEED_OSS);
	BIND_ENUM_CONSTANT(COMMON_CHAT_FORMAT_NEMOTRON_V2);
}

bool LlamaCommonChat::verify_template(const String &tmpl, bool use_jinja) {
	return common_chat_verify_template(tmpl.utf8().get_data(), use_jinja);
}

String LlamaCommonChat::get_format_name(LlamaCommonChat::ChatFormat format) {
	const char *name = common_chat_format_name((common_chat_format)format);
	return String(name);
}

// LlamaCommonChatSyntax

void LlamaCommonChatSyntax::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_format", "format"), &LlamaCommonChatSyntax::set_format);
	ClassDB::bind_method(D_METHOD("get_format"), &LlamaCommonChatSyntax::get_format);
	ClassDB::bind_method(D_METHOD("set_reasoning_format", "format"), &LlamaCommonChatSyntax::set_reasoning_format);
	ClassDB::bind_method(D_METHOD("get_reasoning_format"), &LlamaCommonChatSyntax::get_reasoning_format);
	ClassDB::bind_method(D_METHOD("set_reasoning_in_content", "in_content"), &LlamaCommonChatSyntax::set_reasoning_in_content);
	ClassDB::bind_method(D_METHOD("get_reasoning_in_content"), &LlamaCommonChatSyntax::get_reasoning_in_content);
	ClassDB::bind_method(D_METHOD("set_thinking_forced_open", "forced"), &LlamaCommonChatSyntax::set_thinking_forced_open);
	ClassDB::bind_method(D_METHOD("get_thinking_forced_open"), &LlamaCommonChatSyntax::get_thinking_forced_open);
	ClassDB::bind_method(D_METHOD("set_parse_tool_calls", "parse"), &LlamaCommonChatSyntax::set_parse_tool_calls);
	ClassDB::bind_method(D_METHOD("get_parse_tool_calls"), &LlamaCommonChatSyntax::get_parse_tool_calls);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "format", PROPERTY_HINT_ENUM, "CONTENT_ONLY,GENERIC,MISTRAL_NEMO,LLAMA_3_X,LLAMA_3_X_WITH_BUILTIN_TOOLS,DEEPSEEK_R1,FIREFUNCTION_V2,FUNCTIONARY_V3_2,FUNCTIONARY_V3_1_LLAMA_3_1,DEEPSEEK_V3_1,HERMES_2_PRO,COMMAND_R7B,GRANITE,GPT_OSS,SEED_OSS,NEMOTRON_V2"), "set_format", "get_format");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "reasoning_format", PROPERTY_HINT_ENUM, "NONE,AUTO,DEEPSEEK_LEGACY,DEEPSEEK"), "set_reasoning_format", "get_reasoning_format");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "reasoning_in_content"), "set_reasoning_in_content", "get_reasoning_in_content");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "thinking_forced_open"), "set_thinking_forced_open", "get_thinking_forced_open");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "parse_tool_calls"), "set_parse_tool_calls", "get_parse_tool_calls");
}

void LlamaCommonChatSyntax::set_syntax(const common_chat_syntax &p_syntax) {
	m_syntax = p_syntax;
}
common_chat_syntax LlamaCommonChatSyntax::get_syntax() const {
	return m_syntax;
}

void LlamaCommonChatSyntax::set_format(LlamaCommonChat::ChatFormat p_format) { m_syntax.format = (common_chat_format)p_format; }
LlamaCommonChat::ChatFormat LlamaCommonChatSyntax::get_format() const { return (LlamaCommonChat::ChatFormat)m_syntax.format; }

void LlamaCommonChatSyntax::set_reasoning_format(LlamaCommon::ReasoningFormat p_reasoning_format) { m_syntax.reasoning_format = (common_reasoning_format)p_reasoning_format; }
LlamaCommon::ReasoningFormat LlamaCommonChatSyntax::get_reasoning_format() const { return (LlamaCommon::ReasoningFormat)m_syntax.reasoning_format; }

void LlamaCommonChatSyntax::set_reasoning_in_content(bool p_in_content) { m_syntax.reasoning_in_content = p_in_content; }
bool LlamaCommonChatSyntax::get_reasoning_in_content() const { return m_syntax.reasoning_in_content; }

void LlamaCommonChatSyntax::set_thinking_forced_open(bool p_forced) { m_syntax.thinking_forced_open = p_forced; }
bool LlamaCommonChatSyntax::get_thinking_forced_open() const { return m_syntax.thinking_forced_open; }

void LlamaCommonChatSyntax::set_parse_tool_calls(bool p_parse) { m_syntax.parse_tool_calls = p_parse; }
bool LlamaCommonChatSyntax::get_parse_tool_calls() const { return m_syntax.parse_tool_calls; }

LlamaCommonChatSyntax::LlamaCommonChatSyntax() {}
LlamaCommonChatSyntax::~LlamaCommonChatSyntax() {}
