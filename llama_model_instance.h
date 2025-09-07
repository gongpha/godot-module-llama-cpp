#pragma once

#include "scene/main/node.h"
#include "core/variant/dictionary.h"
#include "core/string/ustring.h"
#include "llama_gguf.h"

struct llama_model;

class LlamaModelInstance : public Node {
	GDCLASS(LlamaModelInstance, Node);

public:
	llama_model *model = nullptr;

	bool loading = false;
	bool loaded = false;

	SafeFlag request_cancel_loading;

	Ref<LlamaGGUF> model_file;

	WorkerThreadPool::TaskID loading_task_id = -1;

	void _load_model_task(const String &p_path, int p_gpu_layers);
	void _load_model_done(bool p_success);
	void _load_model_prog(float p_prog);

protected:
	static void _bind_methods();
	virtual void _notification(int p_what);

public:
	void set_model_file(const Ref<LlamaGGUF> &p_file) { model_file = p_file; }
	Ref<LlamaGGUF> get_model_file() const { return model_file; }

	Error load_model(int p_gpu_layers = 99);
	bool cancel_loading();

	void unload();
	bool is_model_loaded() const { return loaded && model != nullptr; }
	bool is_loading() const { return loading; }

	llama_model *get_model_ptr() const { return model; }
	
	//

	String get_chat_template(const String &name = String());

	LlamaModelInstance();
};
