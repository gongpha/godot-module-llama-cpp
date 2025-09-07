#pragma once

#include "core/io/resource.h"
#include "core/io/resource_loader.h"

// A simple Resource that stores a path to a .gguf model.
class LlamaGGUF : public Resource {
	GDCLASS(LlamaGGUF, Resource);

	String path;
	Dictionary metadata;

	friend class ResourceFormatLoaderLlamaGGUF;

protected:
	static void _bind_methods();

public:
	void set_gguf_path(const String &p_path) { path = p_path; }
	String get_gguf_path() const { return path; }

	Dictionary get_metadata() const { return metadata; }

	String get_architecture() const { return metadata.get("general.architecture", ""); }
	String get_model_name() const { return metadata.get("general.name", ""); }
	int64_t get_context_length() const {
		String arch = get_architecture();
		return metadata.get(arch + ".context_length", -1);
	}
	String get_chat_template() const { return metadata.get("tokenizer.chat_template", ""); }

	String get_finetune() const { return metadata.get("general.finetune", ""); }
	String get_size_label() const { return metadata.get("general.size_label", ""); }
	String get_quantization_version() const { return metadata.get("general.quantization_version", ""); }

	int get_version() const { return metadata.get("general.version", -1); }
	int get_tensor_count() const { return metadata.get("tensor_count", -1); }
	int get_kv_count() const { return metadata.get("kv_count", -1); }
	String get_tokenizer_model() const { return metadata.get("tokenizer.ggml.model", ""); }
	String get_tokenizer_pre() const { return metadata.get("tokenizer.ggml.pre", ""); }
	int get_tokenizer_bos_id() const { return metadata.get("tokenizer.ggml.bos_token_id", -1); }
	int get_tokenizer_eos_id() const { return metadata.get("tokenizer.ggml.eos_token_id", -1); }
	int get_tokenizer_padding_id() const { return metadata.get("tokenizer.ggml.padding_token_id", -1); }
	bool get_tokenizer_add_bos() const { return metadata.get("tokenizer.ggml.add_bos_token", false); }
};

class ResourceFormatLoaderLlamaGGUF : public ResourceFormatLoader {
	GDCLASS(ResourceFormatLoaderLlamaGGUF, ResourceFormatLoader);

public:
	virtual void get_recognized_extensions(List<String> *p_extensions) const override;
	virtual bool handles_type(const String &p_type) const override;
	virtual String get_resource_type(const String &p_path) const override;
	virtual Ref<Resource> load(const String &p_path, const String &p_original_path = "", Error *r_error = nullptr, bool p_use_sub_threads = false, float *r_progress = nullptr, CacheMode p_cache_mode = CACHE_MODE_REUSE) override;
};
