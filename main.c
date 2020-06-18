#include "Module/Module.h"
#include "API.h"
#include "Drivers.S32K1xx.h"


//__root PORT_Type *ABC;

__task void Task_Main(void *Args);

__task void Task_Main(void *Args)
{

	int h=Device_Open("GPIO",0);
	
	Device_Control(h,Drivers_S32K1xx_GPIO_Cmd_SET_Clock_Enable|Drivers_S32K1xx_GPIOx(Drivers_S32K1xx_GPIO_E),Enable);
	Device_Control(h,Drivers_S32K1xx_GPIO_Cmd_SET_Option|Drivers_S32K1xx_GPIOx(Drivers_S32K1xx_GPIO_E)|21,
					(Drivers_S32K1xx_GPIO_Cmd_Option_Type)
					{
						.Pull=Drivers_S32K1xx_GPIO_Option_Pull_None,//PS/PE
						.Passive_Filter_Enable=Disable,//PFE
						.Drive_Strength_Enable=Disable,//DSE
						.MUX_Control=Drivers_S32K1xx_GPIO_Option_MUX_Control_Alternative_1_GPIO,//MUX
						.Lock_Register_Enable=Disable,//LK
						.Interrupt_Config=Drivers_S32K1xx_GPIO_Option_Interrupt_Config_Disabled,//IRQC
					}
					);
	Device_Control(h,Drivers_S32K1xx_GPIO_Cmd_SET_DDR_Set|Drivers_S32K1xx_GPIOx(Drivers_S32K1xx_GPIO_E),1<<21);
	//PCC->PCCn[PCC_PORTE_INDEX]  |=  PCC_PCCn_CGC_MASK; //使能PTE端口时钟
	//PTE->PDDR  |=  (1<<21);      //配置PTE9为输出端口
	//PORTE->PCR[21] = 0x00000100; //配置PTE9为GPIO，不使用其他复用功能
	//PTE->PDOR &= ~(1<<21);       //PTE9输出低电平


	
	while(1)
	{

		//PTE->PDOR |= (1<<21);
		Device_Control(h,Drivers_S32K1xx_GPIO_Cmd_SET_OUT_DATA_Set|Drivers_S32K1xx_GPIOx(Drivers_S32K1xx_GPIO_E),1<<21);
		Scheduling_Sleep_Task(1000);
		
		//PTE->PDOR &= ~(1<<21);
		Device_Control(h,Drivers_S32K1xx_GPIO_Cmd_SET_OUT_DATA_Clear|Drivers_S32K1xx_GPIOx(Drivers_S32K1xx_GPIO_E),1<<21);
		Scheduling_Sleep_Task(1000);
	}


}


__Sys_Module_Init_Task("Task.Main",Task_Main,Null,150,Null,100,Scheduling_Task_Option_Null);