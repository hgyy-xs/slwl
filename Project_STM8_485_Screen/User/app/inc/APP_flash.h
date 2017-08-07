#ifndef _APP_FLASH_H_
#define _APP_FLASH_H_


#define SNSTARTADD	0x13E0
#define SNLENG		5
#define SNFLAGADD	0x13E7
#define SNFLAGDATA	0x79
#define SNHAVEADD	0X13E5


void APP_SN_Write(void);
u8  APP_SN_Init(void);



#endif