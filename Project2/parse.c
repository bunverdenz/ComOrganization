/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   CS311 KAIST                                               */
/*   parse.c                                                   */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <string.h>
#include "util.h"
#include "parse.h"

int text_size;
int data_size;

instruction parsing_instr(const char *buffer, const int index)
{
    instruction instr;
	/** Implement this function */
	//printf("%s\n",buffer);
	int i,len;
	char *type = NULL;
	short op,func,imm;
	unsigned char rs,rt,rd,shamt;
	uint32_t target;
	len = strlen(buffer);
	type = malloc(sizeof(char)*(len+1));
	strcpy(type,buffer);


	char opstring[7];
	strncpy(opstring, type, 6);
	opstring[6] = '\0';

	char rsstring[6];
	strncpy(rsstring, type+6, 5);
	rsstring[5] = '\0';

	char rtstring[6];
	strncpy(rtstring, type+11, 5);
	rtstring[5] = '\0';

	char rdstring[6];
	strncpy(rdstring, type+16, 5);
	rdstring[5] = '\0';

	char shamt_string[6];
	strncpy(shamt_string, type+21, 5);
	shamt_string[5] = '\0';

	char funcstring[7];
	strncpy(funcstring, type+26, 6);
	funcstring[6] = '\0';

	op = fromBinary(opstring);
	rs = fromBinary(rsstring);
	rt = fromBinary(rtstring);
	rd = fromBinary(rdstring);
	shamt = fromBinary(shamt_string);
	func = fromBinary(funcstring);

	char target_string[27];
	strncpy(target_string, type+6, 26);
	target_string[26] = '\0';

	target = fromBinary(target_string);

	char immstring[17];
	strncpy(immstring, type+16, 16);
	immstring[16] = '\0';

	imm = fromBinary(immstring);

	instr.opcode = op;
	instr.value = fromBinary(type);
	//mem_write_32(MEM_TEXT_START+index,instr.value);
	free(type);

	if(op==0){
		instr.func_code = func;
		instr.r_t.r_i.rs = rs;
		instr.r_t.r_i.rt = rt;
		instr.r_t.r_i.r_i.r.rd = rd;
		instr.r_t.r_i.r_i.r.shamt = shamt;

	}
	else if(op==2||op==3){
		instr.r_t.target = target;
	}
	else{
		instr.r_t.r_i.rs = rs;
		instr.r_t.r_i.rt = rt;
		instr.r_t.r_i.r_i.imm = imm;
		//printf("Type I \n");
	}
    return instr;
}

void parsing_data(const char *buffer, const int index)
{
	/** Implement this function */
	int value,len,ind;
	char *type=NULL;
	len = strlen(buffer);
	ind = index;
	type = malloc(sizeof(char)*(len+1));
	strcpy(type,buffer);
	value = fromBinary(type);
	free(type);
	mem_write_32(MEM_DATA_START+ind,value);
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
	printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
	printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	switch(INST_INFO[i].opcode)
	{
	    //Type I
	    case 0x9:		//(0x001001)ADDIU
	    case 0xc:		//(0x001100)ANDI
	    case 0xf:		//(0x001111)LUI
	    case 0xd:		//(0x001101)ORI
	    case 0xb:		//(0x001011)SLTIU
	    case 0x23:		//(0x100011)LW
	    case 0x2b:		//(0x101011)SW
	    case 0x4:		//(0x000100)BEQ
	    case 0x5:		//(0x000101)BNE
		printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
		printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
		printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
		break;

    	    //TYPE R
	    case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
		printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
		printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
		printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
		printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
		printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
		break;

    	    //TYPE J
	    case 0x2:		//(0x000010)J
	    case 0x3:		//(0x000011)JAL
		printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
		break;

	    default:
		printf("Not available instruction\n");
		assert(0);
	}
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
	printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
	printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
