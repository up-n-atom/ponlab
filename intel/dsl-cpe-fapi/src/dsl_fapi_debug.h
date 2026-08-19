#define FAPI_DSL_INIT_DEBUG \
      		"DSL FAPI init arguments:: struct fapi_dsl_ctx --> fapi_dsl_ctx->entity: %d\tstruct fapi_dsl_init_cfg --> fapi_dsl_init_cfg->fapi_dsl_init_parameter_box.array_size: %d\t fapi_dsl_init_cfg->fapi_dsl_init_parameter_box.fapi_dsl_init_parameter->name:%s\t fapi_dsl_init_cfg->fapi_dsl_init_parameter_box.fapi_dsl_init_parameter->value:%s \n", ctx->entity, cfg->params.array_size, cfg->params.array->name, cfg->params.array->value

