
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            start.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"

PUBLIC	void*	memcpy(void* pDst, void* pSrc, int iSize);
PUBLIC void	disp_str(char * pszInfo);

PUBLIC	t_8			gdt_ptr[6];	// 0~15:Limit  16~47:Base
PUBLIC	DESCRIPTOR		gdt[GDT_SIZE];


/*======================================================================*
                            cstart
 *======================================================================*/
PUBLIC void cstart()
{
	disp_str("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n-----\"cstart\" begins-----\n");

	// �� LOADER �е� GDT ���Ƶ��µ� GDT ��
	memcpy(	&gdt,				    // New GDT
		(void*)(*((t_32*)(&gdt_ptr[2]))),   // Base  of Old GDT
		*((t_16*)(&gdt_ptr[0])) + 1	    // Limit of Old GDT
		);
	// gdt_ptr[6] �� 6 ���ֽڣ�0~15:Limit  16~47:Base������ sgdt �Լ� lgdt �Ĳ�����
	t_16* p_gdt_limit = (t_16*)(&gdt_ptr[0]);
	t_32* p_gdt_base  = (t_32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base  = (t_32)&gdt;

	disp_str("-----\"cstart\" finished-----\n");
}
