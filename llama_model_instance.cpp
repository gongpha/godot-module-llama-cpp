#include "llama_model_instance.h"

#include "core/error/error_macros.h"
#include "core/io/file_access.h"

#include "thirdparty/llama/include/llama.h"
#include "core/config/project_settings.h"

#include "llama_cpp.h"

#define LLAMA_MI_USE_THREAD

void LlamaModelInstance::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_model", "n_gpu_layers"), &LlamaModelInstance::load_model, DEFVAL(99));
	ClassDB::bind_method(D_METHOD("unload"), &LlamaModelInstance::unload);
	ClassDB::bind_method(D_METHOD("is_model_loaded"), &LlamaModelInstance::is_model_loaded);
	ClassDB::bind_method(D_METHOD("is_loading"), &LlamaModelInstance::is_loading);

	ClassDB::bind_method(D_METHOD("get_model_file"), &LlamaModelInstance::get_model_file);
	ClassDB::bind_method(D_METHOD("set_model_file", "model_file"), &LlamaModelInstance::set_model_file);

	ClassDB::bind_method(D_METHOD("get_chat_template", "name"), &LlamaModelInstance::get_chat_template, DEFVAL(String()));

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "model_file", PROPERTY_HINT_RESOURCE_TYPE, "LlamaGGUF"), "set_model_file", "get_model_file");

	ADD_SIGNAL(MethodInfo("model_load_progress", PropertyInfo(Variant::FLOAT, "progress")));
	ADD_SIGNAL(MethodInfo("model_loaded", PropertyInfo(Variant::BOOL, "success")));
}

void LlamaModelInstance::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_EXIT_TREE:
		case NOTIFICATION_PREDELETE: {
			unload();
		} break;
	}
}

void LlamaModelInstance::_load_model_task(const String &p_path, int p_gpu_layers) {
	ERR_FAIL_COND_MSG(model != nullptr, "LlamaModelInstance: model is already loaded.");

	auto prog = callable_mp(this, &LlamaModelInstance::_load_model_prog);
	auto done = callable_mp(this, &LlamaModelInstance::_load_model_done);

#ifdef LLAMA_MI_USE_THREAD
#define LLAMA_MI_USE_THREAD_CALL call_deferred
#else
#define LLAMA_MI_USE_THREAD_CALL call
#endif

	String model_path = p_path;
	ERR_FAIL_COND_MSG(model_path.is_empty(), "LlamaModelInstance: model path is empty.");
	if (!FileAccess::exists(model_path)) {
		ERR_PRINT(vformat("LlamaModelInstance: model file not found: %s", model_path));
		done.LLAMA_MI_USE_THREAD_CALL(false);
		return;
	}
	
	loading = true;
	loaded = false;

	struct ProgressContext {
		LlamaModelInstance* instance;
		Callable prog;
	};

	ProgressContext pctx = {
		this, prog
	};

	llama_model_params params = llama_model_default_params();

	params.n_gpu_layers = p_gpu_layers;
	params.split_mode = LLAMA_SPLIT_MODE_NONE;

	params.progress_callback = [](float progress, void *user_data) -> bool {
		ProgressContext* pctx = static_cast<ProgressContext*>(user_data);
		if (pctx) {
			pctx->prog.LLAMA_MI_USE_THREAD_CALL(progress);
		}
		if (pctx->instance) {
			return !pctx->instance->request_cancel_loading.is_set();
		}
		return true;
	};
	params.progress_callback_user_data = &pctx;

	model = llama_model_load_from_file(model_path.utf8().get_data(), params);
	if (!model) {
		if (request_cancel_loading.is_set()) {

		} else {
			ERR_PRINT(vformat("LlamaModelInstance: failed to load model from path: %s", model_path));
		}
		done.LLAMA_MI_USE_THREAD_CALL(false);
		return;
	}

	done.LLAMA_MI_USE_THREAD_CALL(true);
}
#undef LLAMA_MI_USE_THREAD_CALL

void LlamaModelInstance::_load_model_done(bool p_success) {
	loaded = true;
	loading = false;

	request_cancel_loading.clear();

	emit_signal("model_loaded", p_success);
}

void LlamaModelInstance::_load_model_prog(float p_progress) {
	emit_signal("model_load_progress", p_progress);
}

Error LlamaModelInstance::load_model(int p_gpu_layers) {
	ERR_FAIL_COND_V(loading || is_model_loaded(), ERR_ALREADY_IN_USE);
	unload();

	ERR_FAIL_COND_V(model_file.is_null(), ERR_INVALID_PARAMETER);

	String path = model_file->get_gguf_path();
	ERR_FAIL_COND_V(path.is_empty(), ERR_INVALID_PARAMETER);
	if (!FileAccess::exists(path)) {
		ERR_PRINT(vformat("LlamaModelInstance: model file not found: %s", path));
		return ERR_FILE_NOT_FOUND;
	}

	LlamaCPP::get_singleton()->ensure_llama_backend_initialized();

#ifdef LLAMA_MI_USE_THREAD
	WorkerThreadPool::TaskID task = WorkerThreadPool::get_singleton()->add_task(callable_mp(this, &LlamaModelInstance::_load_model_task).bind(path, p_gpu_layers), true, "LLama.cpp - Load Model");

	if (task == WorkerThreadPool::INVALID_TASK_ID) {
		ERR_PRINT("LlamaModelInstance: failed to create loading task.");
		return ERR_CANT_CREATE;
	}

	loading = true;
	loaded = false;
#else
	_load_model_task(path);
#endif
	
	return OK;
}

bool LlamaModelInstance::cancel_loading() {
	if (loading) {
		request_cancel_loading.set();
		return true;
	}
	return false;
}

void LlamaModelInstance::unload() {
	if (loading) {
		cancel_loading();
		return;
	}

	if (model) {
		llama_model_free(model);
		model = nullptr;
		llama_backend_free();
	}
	loaded = false;
}


LlamaModelInstance::LlamaModelInstance() {
	request_cancel_loading.clear();
}

String LlamaModelInstance::get_chat_template(const String &name) {
	const char * c_name = name.is_empty() ? nullptr : name.utf8().get_data();
	const char * tmpl = llama_model_chat_template(model, c_name);
	if (!tmpl) return String();
	return String::utf8(tmpl);
}