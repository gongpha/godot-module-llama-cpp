#include "llama_gguf.h"

#include "core/io/file_access.h"
#include "core/config/project_settings.h"
#include "core/object/class_db.h"

#include "thirdparty/llama/ggml/include/gguf.h"

void LlamaGGUF::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_gguf_path", "path"), &LlamaGGUF::set_gguf_path);
	ClassDB::bind_method(D_METHOD("get_gguf_path"), &LlamaGGUF::get_gguf_path);

	ClassDB::bind_method(D_METHOD("get_metadata"), &LlamaGGUF::get_metadata);

	ClassDB::bind_method(D_METHOD("get_architecture"), &LlamaGGUF::get_architecture);
	ClassDB::bind_method(D_METHOD("get_model_name"), &LlamaGGUF::get_model_name);
	ClassDB::bind_method(D_METHOD("get_context_length"), &LlamaGGUF::get_context_length);
	ClassDB::bind_method(D_METHOD("get_chat_template"), &LlamaGGUF::get_chat_template);

	ClassDB::bind_method(D_METHOD("get_finetune"), &LlamaGGUF::get_finetune);
	ClassDB::bind_method(D_METHOD("get_size_label"), &LlamaGGUF::get_size_label);
	ClassDB::bind_method(D_METHOD("get_quantization_version"), &LlamaGGUF::get_quantization_version);

	ClassDB::bind_method(D_METHOD("get_version"), &LlamaGGUF::get_version);
	ClassDB::bind_method(D_METHOD("get_tensor_count"), &LlamaGGUF::get_tensor_count);
	ClassDB::bind_method(D_METHOD("get_kv_count"), &LlamaGGUF::get_kv_count);
	ClassDB::bind_method(D_METHOD("get_tokenizer_model"), &LlamaGGUF::get_tokenizer_model);
	ClassDB::bind_method(D_METHOD("get_tokenizer_pre"), &LlamaGGUF::get_tokenizer_pre);
	ClassDB::bind_method(D_METHOD("get_tokenizer_bos_id"), &LlamaGGUF::get_tokenizer_bos_id);
	ClassDB::bind_method(D_METHOD("get_tokenizer_eos_id"), &LlamaGGUF::get_tokenizer_eos_id);
	ClassDB::bind_method(D_METHOD("get_tokenizer_padding_id"), &LlamaGGUF::get_tokenizer_padding_id);
	ClassDB::bind_method(D_METHOD("get_tokenizer_add_bos"), &LlamaGGUF::get_tokenizer_add_bos);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "gguf_path", PROPERTY_HINT_FILE, "*.gguf", PROPERTY_USAGE_NO_EDITOR), "set_gguf_path", "get_gguf_path");
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "metadata"), "", "get_metadata");
}

void ResourceFormatLoaderLlamaGGUF::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("gguf");
}

bool ResourceFormatLoaderLlamaGGUF::handles_type(const String &p_type) const {
	return p_type == "LlamaGGUF" || p_type == String();
}

String ResourceFormatLoaderLlamaGGUF::get_resource_type(const String &p_path) const {
	if (p_path.get_extension().to_lower() == "gguf") {
		return "LlamaGGUF";
	}
	return String();
}

Ref<Resource> ResourceFormatLoaderLlamaGGUF::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	if (r_error) *r_error = OK;
	if (!FileAccess::exists(p_path)) {
		if (r_error) *r_error = ERR_FILE_NOT_FOUND;
		return Ref<Resource>();
	}
	Ref<LlamaGGUF> res;
	res.instantiate();
	res->set_gguf_path(ProjectSettings::get_singleton()->globalize_path(p_path));

	// retrieve metadata now

	gguf_init_params params{};
	params.no_alloc = true;
	gguf_context *ctx = gguf_init_from_file(res->get_gguf_path().utf8().get_data(), params);

	if (!ctx) {
		if (r_error) *r_error = ERR_FILE_CORRUPT;
		return Ref<Resource>();
	}

	// read all keys
	const int64_t n = gguf_get_n_kv(ctx);
	for (int64_t i = 0; i < n; i++) {
		const char *key = gguf_get_key(ctx, i);
		String keystr = String(key);

		// data too large
		if (keystr.begins_with("tokenizer.ggml.tokens") ||
			keystr.begins_with("tokenizer.ggml.token_type") ||
			keystr.begins_with("tokenizer.ggml.merges")) {
			continue;
		}

		const gguf_type type = gguf_get_kv_type(ctx, i);

		Variant value;
		switch (type) {
			case GGUF_TYPE_UINT8:
				value = gguf_get_val_u8(ctx, i);
				break;
			case GGUF_TYPE_INT8:
				value = gguf_get_val_i8(ctx, i);
				break;
			case GGUF_TYPE_UINT16:
				value = gguf_get_val_u16(ctx, i);
				break;
			case GGUF_TYPE_INT16:
				value = gguf_get_val_i16(ctx, i);
				break;
			case GGUF_TYPE_UINT32:
				value = gguf_get_val_u32(ctx, i);
				break;
			case GGUF_TYPE_INT32:
				value = gguf_get_val_i32(ctx, i);
				break;
			case GGUF_TYPE_FLOAT32:
				value = gguf_get_val_f32(ctx, i);
				break;
			case GGUF_TYPE_BOOL:
				value = gguf_get_val_bool(ctx, i);
				break;
			case GGUF_TYPE_STRING:
				value = String::utf8(gguf_get_val_str(ctx, i));
				break;
			case GGUF_TYPE_UINT64:
				value = gguf_get_val_u64(ctx, i);
				break;
			case GGUF_TYPE_INT64:
				value = gguf_get_val_i64(ctx, i);
				break;
			case GGUF_TYPE_FLOAT64:
				value = gguf_get_val_f64(ctx, i);
				break;

			case GGUF_TYPE_ARRAY:
				{
					const int64_t an = gguf_get_arr_n(ctx, i);

					const gguf_type at = gguf_get_arr_type(ctx, i);
					switch (at) {
						case GGUF_TYPE_UINT8: {
							PackedByteArray arr;
							arr.resize(an);
							const uint8_t *data = (const uint8_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_INT8: {
							PackedInt32Array arr;
							arr.resize(an);
							const int8_t *data = (const int8_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_UINT16: {
							PackedInt32Array arr;
							arr.resize(an);
							const uint16_t *data = (const uint16_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_INT16: {
							PackedInt32Array arr;
							arr.resize(an);
							const int16_t *data = (const int16_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_UINT32: {
							PackedInt64Array arr;
							arr.resize(an);
							const uint32_t *data = (const uint32_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_INT32: {
							PackedInt32Array arr;
							arr.resize(an);
							const int32_t *data = (const int32_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_UINT64: {
							PackedInt64Array arr;
							arr.resize(an);
							const uint64_t *data = (const uint64_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_INT64: {
							PackedInt64Array arr;
							arr.resize(an);
							const int64_t *data = (const int64_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_FLOAT32: {
							PackedFloat32Array arr;
							arr.resize(an);
							const float *data = (const float *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_FLOAT64: {
							PackedFloat32Array arr;
							arr.resize(an);
							const double *data = (const double *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_BOOL: {
							PackedByteArray arr;
							arr.resize(an);
							const uint8_t *data = (const uint8_t *)gguf_get_arr_data(ctx, i);
							for (int64_t j = 0; j < an; ++j) arr.set(j, data[j]);
							value = arr;
						} break;
						case GGUF_TYPE_STRING: {
							PackedStringArray arr;
							arr.resize(an);
							for (int64_t j = 0; j < an; ++j) {
								const char *s = gguf_get_arr_str(ctx, i, j);
								if (s) {
									arr.set(j, String::utf8(s));
								} else {
									arr.set(j, String());
								}
							}
							value = arr;
						} break;
						default: {
							// unsupported array type
							value = Variant();
						}
					}
					break;
				}
			default:
				// unsupported type
				value = Variant();
				break;
		}
		res->metadata[key] = value;
	}
	gguf_free(ctx);
	return res;
}
