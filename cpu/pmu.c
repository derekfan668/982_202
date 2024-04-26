#include "includes.h"

u8 pmu_sfr_rd(u8 addr) 
{
	u8 reg;
	local_irq_disable();
	PMU_ADR = addr;
	_nop_();
	_nop_();
	reg = PMU_DAT;
	local_irq_enable();
	return reg;
}

void pmu_sfr_wr(u8 addr, u8 dat) 
{
	local_irq_disable();
	PMU_ADR = addr;
	PMU_DAT = dat;
	local_irq_enable();
}

void pmu_sfr_and(u8 addr, u8 dat) 
{
	local_irq_disable();
	PMU_ADR = addr;
	_nop_();
	_nop_();
	PMU_DAT &= dat;	
	local_irq_enable();
}

void pmu_sfr_or(u8 addr, u8 dat)
{
	local_irq_disable();
	PMU_ADR = addr;
	_nop_();
	_nop_();
	PMU_DAT |= dat;	
	local_irq_enable();
}

void pmu_sfr_xor(u8 addr, u8 dat) 
{
	local_irq_disable();
	PMU_ADR = addr;
	_nop_();
	_nop_();
	PMU_DAT ^= dat;
	local_irq_enable();
}

void PMU_CON_SET(u8 addr, u8 mark, u8 dat)
{
    u8 reg;
	local_irq_disable();
	PMU_ADR = addr;
	_nop_();
	_nop_();
	reg = PMU_DAT;
	reg &= ~mark;
	reg |= dat;
	PMU_ADR = addr;
	PMU_DAT = reg;
	local_irq_enable();
}
