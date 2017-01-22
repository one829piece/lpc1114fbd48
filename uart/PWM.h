#ifndef __NXPLPC11xx_PWM_H__
#define __NXPLPC11xx_PWM_H__

#define MATCH0	(1<<0)
#define MATCH1	(1<<1)
#define MATCH2	(1<<2)
#define MATCH3	(1<<3)

extern uint16_t cycle;
extern uint8_t duty;
extern void T16B0_init(void);
extern void T16B0_delay_ms(uint16_t ms);
extern void T16B0_delay_us(uint16_t us);
extern void T16B0_cnt_init(void);
extern void T16B0_CAP_Init(void);
extern void T16B0M0_PWM_Init(void);
extern void T16B0M0_PWM_SET(void);
void TIM32B1_PWM(uint16_t cycle,uint8_t duty);
#endif