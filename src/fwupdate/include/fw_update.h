#ifndef FW_UPDATE_H
#define FW_UPDATE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "proj_config.h"
#include "util_common.h"
#include "util_freeipmi.h"
#include "dev_fw_update.h"

#define log_num_to_name(x) #x

extern uint8_t force_update_flag;

extern const char *const prj_comp_name[];
extern const char *const prj_stage_name[];

typedef struct __attribute__((packed)) {
    uint8_t platform_name[16];
    uint8_t version[13];
    union {
		uint8_t value[3];
        struct __attribute__((packed)) {
            uint8_t board_id : 5;
            uint8_t board_stage : 3;
            uint8_t comp_id : 3;
            uint16_t inst : 12;
			uint8_t rsv : 1;
		} fields;
	} board_info;
} board_info_t;

typedef enum {
    IMG_COMP_CPLD = 1,
    IMG_COMP_BIC,
    IMG_COMP_BIOS,
    MAX_IMG_COMP,
} component_t;

typedef enum {
    STAGE_POC,
    STAGE_EVT,
    STAGE_DVT,
    STAGE_PVT,
    STAGE_MP,
    MAX_STAGE,
} proj_stage_t;

/* for command NetFN:0x38 Cmd:0x0A */
typedef enum {
	BIC_PLAT_NAME = 1,
	BIC_PLAT_BOARD_ID,
	BIC_PROJ_NAME,
	BIC_PROJ_STAGE,
} fw_info_t;

/* Firware update command relative config(using ipmi-raw) */
#define FW_UPDATE_NETFN CONFIG_OEM_38
#define OEM_CMD_FW_UPDATE 0x09
#define OEM_CMD_GET_BIC_FW_INFO 0xA
#define FW_UPDATE_LUN 0x00

#define IPMI_RAW_RETRY 2

typedef enum fw_type {
    FW_T_BIC = 0,
    FW_T_MAX_IDX
} fw_type_t;

char *IMG_TYPE_LST[FW_T_MAX_IDX];

int img_parsing_and_validate(ipmi_ctx_t ipmi_ctx, uint8_t *buff, uint32_t buff_len, fw_type_t dev_type);
int fw_update(ipmi_ctx_t ipmi_ctx, uint8_t *buff, uint32_t buff_len, fw_type_t dev_type, int max_retry);

#endif
