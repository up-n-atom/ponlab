#include <local_types.h>
#include <mxl_specific.h>

int mxl_nvdat_open(void);
void mxl_nvdat_close(void);
CK_RV mxl_init_token_data(STDLL_TokData_t *tokdata, CK_SLOT_ID slot_id);
CK_RV mxl_save_token_data(STDLL_TokData_t *tokdata, CK_SLOT_ID slot_id);
CK_RV mxl_load_token_data(STDLL_TokData_t *tokdata, CK_SLOT_ID slot_id);
CK_RV mxl_save_token_object_internal(STDLL_TokData_t *tokdata, OBJECT *obj, CK_BBOOL is_private);
CK_RV mxl_save_token_object(STDLL_TokData_t *tokdata, OBJECT *obj);
CK_RV mxl_save_private_token_object(STDLL_TokData_t *tokdata, OBJECT *obj);
CK_RV mxl_save_public_token_object(STDLL_TokData_t *tokdata, OBJECT *obj);
CK_RV mxl_load_token_objects(STDLL_TokData_t *tokdata, CK_BBOOL is_private);
CK_RV mxl_load_public_token_objects(STDLL_TokData_t *tokdata);
CK_RV mxl_load_private_token_objects(STDLL_TokData_t *tokdata);
CK_RV mxl_reload_token_object(STDLL_TokData_t *tokdata, OBJECT *obj);
CK_RV mxl_delete_token_object(STDLL_TokData_t *tokdata, OBJECT *obj);
CK_RV mxl_destroy_token_objects(STDLL_TokData_t *tokdata);
