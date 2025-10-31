#ifndef __MAIN_H__
#define __MAIN_H__

/*STACK DEFINITION*/
#define T_STACK_SIZE         1024U
#define SCHEDULER_STACK_SIZE 1024U

#define SRAM_START           0x20000000U
#define SRAM_SIZE            ((1024) * (128))
#define SRAM_END             ( (SRAM_START) + (SRAM_SIZE) )

#define T0_STACK_START  		(( SRAM_END ) - ( 0*T_STACK_SIZE ))
#define T1_STACK_START  		(( SRAM_END ) - ( 1*T_STACK_SIZE ))
#define T2_STACK_START  		(( SRAM_END ) - ( 2*T_STACK_SIZE ))
#define T3_STACK_START  		(( SRAM_END ) - ( 3*T_STACK_SIZE ))
#define IDLE_STACK_START 		(( SRAM_END ) - ( 4*T_STACK_SIZE ))
#define SCHEDULER_STACK_START 	(( SRAM_END ) - ( 4*T_STACK_SIZE )- ( SCHEDULER_STACK_SIZE ))

/*SYSTICK TIMER*/
#define SYST_SAFETY_BIT_MASK	0x00FFFFFFU			/* 24bit register = clear upper bit , ex: SYST_SAFETY_BIT_MASK & SYST_RELOAD_VALUE;*/
#define SYST_RELOAD_VALUE		16000U				/* Timer reload value = 1 ms/1KHz */
#define SYST_RVR				0xE000E014U			/* SYST_RVR SysTick (Reload Value Register) value to 16 000 */
#define SYST_CVR				0xE000E018U			/* SYST_CVR SysTick (Current Value Register) */
#define SYST_CSR				0xE000E010U			/* SYST_CSR (SysTick Control and Status Register)*/

/*TASKS*/
#define MAXTASKS 4

/*TASK SAVE STATE STRUCTURE*/
typedef struct {
	uint32_t 					PSP;
	//uint8_t 					state;
	void (*taks_handler)(void);
} task_store_t;





#endif
