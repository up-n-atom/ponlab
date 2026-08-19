#include <gpy211.h>
#include <gpy211_common.h>
#include <gpy211_macsec.h>
#include <registers/phy/std.h>
#include <registers/p31g/pmu_pdi_registers.h>
#include <registers/p31g/smdio_pdi_smdio_registers.h>

struct gpy2xx_mdio {
	int fd;
	int xgmac_id;
};

int gpy211_mdio_read(void *busdata, u16 phyaddr, u32 regnum);
int gpy211_mdio_write(void *busdata, u16 phyaddr, u32 regnum, u16 data);
