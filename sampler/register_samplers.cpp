#include "core/object/class_db.h"

#include "llama_sampler_greedy.h"
#include "llama_sampler_dist.h"
#include "llama_sampler_top_k.h"
#include "llama_sampler_top_p.h"
#include "llama_sampler_min_p.h"
#include "llama_sampler_typical.h"
#include "llama_sampler_temp.h"
#include "llama_sampler_temp_ext.h"
#include "llama_sampler_xtc.h"
#include "llama_sampler_mirostat.h"
#include "llama_sampler_mirostat_v2.h"
#include "llama_sampler_penalties.h"
#include "llama_sampler_top_n_sigma.h"
#include "llama_sampler_dry.h"
#include "llama_sampler_logit_bias.h"
#include "llama_sampler_grammar.h"
#include "llama_sampler_infill.h"
#include "llama_sampler_grammar_lazy.h"

void register_llama_samplers() {
	ClassDB::register_class<LlamaSamplerGreedy>();
	ClassDB::register_class<LlamaSamplerDist>();
	ClassDB::register_class<LlamaSamplerTopK>();
	ClassDB::register_class<LlamaSamplerTopP>();
	ClassDB::register_class<LlamaSamplerMinP>();
	ClassDB::register_class<LlamaSamplerTypical>();
	ClassDB::register_class<LlamaSamplerTemp>();
	ClassDB::register_class<LlamaSamplerTempExt>();
	ClassDB::register_class<LlamaSamplerXTC>();
	ClassDB::register_class<LlamaSamplerMirostat>();
	ClassDB::register_class<LlamaSamplerMirostatV2>();
	ClassDB::register_class<LlamaSamplerPenalties>();
	ClassDB::register_class<LlamaSamplerTopNSigma>();
	ClassDB::register_class<LlamaSamplerDRY>();
	ClassDB::register_class<LlamaSamplerLogitBias>();
	ClassDB::register_class<LlamaSamplerGrammar>();
	ClassDB::register_class<LlamaSamplerInfill>();
	ClassDB::register_class<LlamaSamplerGrammarLazyPattern>();
}
