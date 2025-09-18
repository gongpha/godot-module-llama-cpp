#include "llama_common_sampler.h"
#include "llama_common_sampler_params.h"

#include "core/object/class_db.h"

void LlamaCommonSampler::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_valid"), &LlamaCommonSampler::is_valid);
	ClassDB::bind_method(D_METHOD("init", "model", "params"), &LlamaCommonSampler::init);
	ClassDB::bind_method(D_METHOD("free"), &LlamaCommonSampler::free);
	ClassDB::bind_method(D_METHOD("reset"), &LlamaCommonSampler::reset);
	ClassDB::bind_method(D_METHOD("accept", "token", "accept_grammar"), &LlamaCommonSampler::accept, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("sample", "ctx", "idx", "grammar_first"), &LlamaCommonSampler::sample, DEFVAL(-1), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("clone"), &LlamaCommonSampler::clone);
	ClassDB::bind_method(D_METHOD("get_seed"), &LlamaCommonSampler::get_seed);
	ClassDB::bind_method(D_METHOD("get_candidates", "do_sort"), &LlamaCommonSampler::get_candidates, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("last"), &LlamaCommonSampler::last);
	ClassDB::bind_method(D_METHOD("print"), &LlamaCommonSampler::print);
	ClassDB::bind_method(D_METHOD("prev_str", "ctx", "n"), &LlamaCommonSampler::prev_str);
}

bool LlamaCommonSampler::is_valid() const {
	if (!gsmpl) {
		return false;
	}

	Object* o = ObjectDB::get_instance(model_instance_id);
	LlamaModelInstance *mi = o ? Object::cast_to<LlamaModelInstance>(o) : nullptr;
	if (!mi) {
		return false;
	}

	if (!mi->is_model_loaded()) {
		return false;
	}
	return true;
}

void LlamaCommonSampler::init(const LlamaModelInstance *p_model, const Ref<LlamaCommonSamplerParams> &p_params) {
	free();

	ERR_FAIL_COND(p_params.is_null());
	ERR_FAIL_COND_MSG(!p_model || !p_model->is_model_loaded(), "Invalid or unloaded model instance provided.");
	
	model_instance_id = p_model->get_instance_id();

	const llama_model *model =  p_model ? p_model->get_model_ptr() : nullptr;
	ERR_FAIL_COND_MSG(!model, "Invalid model instance provided.");
	gsmpl = common_sampler_init(model, p_params->get_params());
}

void LlamaCommonSampler::free() {
	if (gsmpl) {
		common_sampler_free(gsmpl);
		gsmpl = nullptr;
	}
	model_instance_id = ObjectID();
}

void LlamaCommonSampler::reset() {
	ERR_FAIL_COND(!is_valid());
	common_sampler_reset(gsmpl);
}

void LlamaCommonSampler::accept(int32_t token, bool accept_grammar) {
	ERR_FAIL_COND(!is_valid());
	common_sampler_accept(gsmpl, (llama_token)token, accept_grammar);
}

int32_t LlamaCommonSampler::sample(const Ref<LlamaContext> &ctx, int32_t idx, bool grammar_first) {
	ERR_FAIL_COND_V(!is_valid() || ctx.is_null() || !ctx->is_valid(), -1);
	llama_context *c = ctx->get_native();
	llama_token t = common_sampler_sample(gsmpl, c, idx, grammar_first);
	return (int32_t)t;
}

Ref<LlamaCommonSampler> LlamaCommonSampler::clone() {
	Ref<LlamaCommonSampler> r;
	r.instantiate();
	if (gsmpl) {
		r->gsmpl = common_sampler_clone(gsmpl);
	}
	return r;
}

uint32_t LlamaCommonSampler::get_seed() const {
	ERR_FAIL_COND_V(!is_valid(), 0);
	return common_sampler_get_seed(gsmpl);
}

Dictionary LlamaCommonSampler::get_candidates(bool do_sort) const {
	Dictionary out;
	ERR_FAIL_COND_V(!is_valid(), out);
	llama_token_data_array *arr = common_sampler_get_candidates(gsmpl, do_sort);
	Array data;
	if (arr && arr->data && arr->size > 0) {
		for (size_t i = 0; i < arr->size; ++i) {
			const auto &e = arr->data[i];
			Dictionary d; d["id"] = (int32_t)e.id; d["logit"] = e.logit; d["p"] = e.p; data.push_back(d);
		}
	}
	out["data"] = data;
	out["size"] = (int32_t)(arr ? arr->size : 0);
	out["selected"] = (int32_t)(arr ? arr->selected : -1);
	out["sorted"] = (bool)(arr ? arr->sorted : false);
	return out;
}

int32_t LlamaCommonSampler::last() const {
	ERR_FAIL_COND_V(!is_valid(), -1);
	return (int32_t)common_sampler_last(gsmpl);
}

String LlamaCommonSampler::print() const {
	ERR_FAIL_COND_V(!is_valid(), String());
	return String::utf8(common_sampler_print(gsmpl).c_str());
}

String LlamaCommonSampler::prev_str(const Ref<LlamaContext> &ctx, int32_t n) {
	ERR_FAIL_COND_V(!is_valid() || ctx.is_null() || !ctx->is_valid(), String());
	return String::utf8(common_sampler_prev_str(gsmpl, ctx->get_native(), n).c_str());
}

LlamaCommonSampler::LlamaCommonSampler() {
	gsmpl = nullptr;
}
LlamaCommonSampler::~LlamaCommonSampler() {
	free();
}