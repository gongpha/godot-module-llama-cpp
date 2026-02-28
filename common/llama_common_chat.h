#pragma once

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/variant.h"
#include "core/variant/typed_array.h"
#include "llama_common_defined.h"
#include "llama_common.h"

class LlamaCommonChatSyntax;

class LlamaCommonChat : public Object {
	GDCLASS(LlamaCommonChat, Object);

public:
	enum ChatToolChoice {
		CHAT_TOOL_CHOICE_AUTO,
		CHAT_TOOL_CHOICE_REQUIRED,
		CHAT_TOOL_CHOICE_NONE,
	};

	enum ChatFormat {
		COMMON_CHAT_FORMAT_CONTENT_ONLY,
		COMMON_CHAT_FORMAT_GENERIC,
		COMMON_CHAT_FORMAT_MISTRAL_NEMO,
		COMMON_CHAT_FORMAT_LLAMA_3_X,
		COMMON_CHAT_FORMAT_LLAMA_3_X_WITH_BUILTIN_TOOLS,
		COMMON_CHAT_FORMAT_DEEPSEEK_R1,
		COMMON_CHAT_FORMAT_FIREFUNCTION_V2,
		COMMON_CHAT_FORMAT_FUNCTIONARY_V3_2,
		COMMON_CHAT_FORMAT_FUNCTIONARY_V3_1_LLAMA_3_1,
		COMMON_CHAT_FORMAT_DEEPSEEK_V3_1,
		COMMON_CHAT_FORMAT_HERMES_2_PRO,
		COMMON_CHAT_FORMAT_COMMAND_R7B,
		COMMON_CHAT_FORMAT_GRANITE,
		COMMON_CHAT_FORMAT_GPT_OSS,
		COMMON_CHAT_FORMAT_SEED_OSS,
		COMMON_CHAT_FORMAT_NEMOTRON_V2,

		COMMON_CHAT_FORMAT_COUNT,
	};

protected:
	static void _bind_methods();
public:

	static bool verify_template(const String &tmpl, bool use_jinja);

	static String get_format_name(ChatFormat format);

	LlamaCommonChat() {}
	~LlamaCommonChat() {}
};

VARIANT_ENUM_CAST(LlamaCommonChat::ChatToolChoice);
VARIANT_ENUM_CAST(LlamaCommonChat::ChatFormat);

class LlamaCommonChatSyntax : public RefCounted {
	GDCLASS(LlamaCommonChatSyntax, RefCounted);

	common_chat_parser_params m_syntax;

protected:
	static void _bind_methods();

public:

	void set_syntax(const common_chat_parser_params &p_syntax);
	common_chat_parser_params get_syntax() const;

	void set_format(LlamaCommonChat::ChatFormat p_format);
	LlamaCommonChat::ChatFormat get_format() const;

	void set_reasoning_format(LlamaCommon::ReasoningFormat p_reasoning_format);
	LlamaCommon::ReasoningFormat get_reasoning_format() const;

	void set_reasoning_in_content(bool p_in_content);
	bool get_reasoning_in_content() const;

	void set_thinking_forced_open(bool p_forced);
	bool get_thinking_forced_open() const;

	void set_parse_tool_calls(bool p_parse);
	bool get_parse_tool_calls() const;

	LlamaCommonChatSyntax();
	~LlamaCommonChatSyntax();
};