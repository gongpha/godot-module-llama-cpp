#pragma once

#include "core/object/ref_counted.h"
#include "llama_common_defined.h"

class LlamaModelInstance;
class LlamaCommonChatParameters;
class LlamaCommonChatTemplatesInput;
class LlamaCommonChatMessage;
class LlamaCommonChatMessages;

class LlamaCommonChatTemplates : public RefCounted {
	GDCLASS(LlamaCommonChatTemplates, RefCounted);

	common_chat_templates_ptr ptr;

protected:
	static void _bind_methods();

public:
	static Ref<LlamaCommonChatTemplates> init(
		const LlamaModelInstance *model,
		const String &chat_template_override = "",
		const String &bos_token_override = "",
		const String &eos_token_override = ""
	);

	bool is_valid() const { return ptr != nullptr; }

	bool was_explicit() const;
	String get_source(const String &variant = "") const;

	Ref<LlamaCommonChatParameters> apply(const Ref<LlamaCommonChatTemplatesInput> &inputs) const;

	bool support_enable_thinking() const;

	//

	String format_single(
		const Ref<LlamaCommonChatMessages> &past_msg,
		const Ref<LlamaCommonChatMessage> &new_msg,
		bool add_assistant,
		bool use_jinja
	);
	String format_example(
		bool use_jinja,
		const Dictionary &chat_template_kwargs
	) const;

	LlamaCommonChatTemplates();
	~LlamaCommonChatTemplates();
};