 /*
 * @<code.c>::
 */

/*******************************************************
* Authors: Bruna F. Lamim
          Guilherme M. Caes
          Pedro H. Botecchi
********************************************************/

/*
 * Módulo para impressão de código Assembly
 * *************************************************
 */

/* Include Session */
#include<stdio.h>
#include<stdlib.h>
#include<code.h>
#include<reg.h>

/* This part of the code just create a method to return the correct register given an alias to the register */
char* code_register(int reg) {
	switch(reg) {	
		case EAX : return "%eax";
		case EBX : return "%ebx";
		case ECX : return "%ecx";
		case EDX : return "%edx";
		case EBP : return "%ebp";
		case ESP : return "%esp";
	}	
	return 0;
}

/* This function makes the end of program in assembly */
void code_end() {
	//printf("_start:\n");          Desocmentar quando conseguir fazer o rótulo de procedures funcionar
	//code_sentence("call\t.P0");   Descomentar isso quando o rótulo para procedures funcionar
	code_sentence("movl\t$1, %eax");
	code_sentence("movl\t$0, %ebx");
	code_sentence("int\t$0x80");

}

/* Function just to print a sentence */
void code_sentence(char* sentence) {
	printf("\t%s\n", sentence);
}

/* Function to print a LABEL: */
void code_label(char * labelt, int labeln)
{
    printf(".%s%d:\n",labelt,labeln);
}

/* This function prints a operation that uses a register and a immediate */
void code_op_imm_2(const char * op, int regdest, char * imm){
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t$%s, %s",op,imm, code_register(regdest));
    code_sentence(sentence);
}

/* This function puts a cmpl in the screen, using a register and a immediate */
void code_cmp_imm(int regdest, char * imm) {
    code_op_imm_2("cmpl",regdest, imm);
}

/* This function puts zero jump in the screen */
void code_false(int labeln){
    code_jump_label("jz",labeln);
}

/* This function puts a goto jump in the screen */
void code_goto(int labeln){
    code_jump_label("jmp",labeln);
}

/* This function puts jump greater in the screen */
void code_jg(int labeln){
    code_jump_label("jg",labeln);
}

/* This function puts a jump if less in the screen */
void code_jl(int labeln){
    code_jump_label("jl",labeln);
}

/* This function puts a jump if equal in the screen */
void code_je(int labeln){
    code_jump_label("je",labeln);
}

/* This function puts a jump if notequal in the screen */
void code_jne(int labeln){
    code_jump_label("jne",labeln);
}

/* This function puts a jump if less or equal in the screen */
void code_jle(int labeln){
    code_jump_label("jle",labeln);
}

/* This function puts a jump if greater or equal in the screen */
void code_jge(int labeln){
    code_jump_label("jge",labeln);
}

/* This function creates the jump operation to print in the screen */
void code_jump_label(char * op, int labeln) {
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t.L%d",op,labeln);
    code_sentence(sentence);
}

/* This function puts the label at the screen */
void code_loop_label(int labeln)
{
    code_label("L",labeln);
}

/* This function do a push */
void code_push(int reg)
{
    code_op_register_1("pushl",reg);
}

/* This function do a function with one register like imull*/
void code_op_register_1(const char * op,int reg)
{
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t%s",op,code_register(reg));
    code_sentence(sentence);
}

/* This function do a function with two register, like addl, subl, etc */
void code_op_register_2(const char * op,int destRegister,int sourceRegister)
{
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t$%s, %s",op,code_register(sourceRegister),code_register(destRegister));
    code_sentence(sentence);
}

/* This function do the prologue of a function */
void code_prologue_relop()
{
    code_pop(EBX);
    code_cmp(EBX,EAX);
}

/* This function do a pop */
void code_pop(int reg)
{
    code_op_register_1("popl",reg);
}

/* This function do a cmpl */
void code_cmp(int destRegister, int sourceRegister)
{
    code_op_register_2("cmpl",sourceRegister,destRegister);
}

/* This function do a epilogue */
void code_epilogue_relop(int labelT,int labelF) {
    code_mov_imm(EAX,"0");
    code_goto(labelF);
    code_loop_label(labelT);
    code_mov_imm(EAX,"1");
    code_loop_label(labelF);
}

/* This function do a movl */
void code_mov(int destRegister,int sourceRegister)
{
    code_op_register_2("movl", destRegister,sourceRegister);
}

/* This function do a movl with a immediate */
void code_mov_imm(int destRegister, char * imm)
{
    code_op_imm_2("movl",destRegister,imm);
}

/* This function do a addl */
void code_add(int destReg, int sourceReg)
{
    code_op_register_2("addl",destReg,sourceReg);
}

/* This function do a subl */
void code_sub(int destReg, int sourceReg)
{
    code_op_register_2("subl",destReg,sourceReg);
}

/* This function do a orl */
void code_or(int destReg, int sourceReg)
{
    code_op_register_2("orl",destReg,sourceReg);
}

/* This function do a andl */
void code_and(int destReg, int sourceReg)
{
    code_op_register_2("andl",destReg,sourceReg);
}

/* This function do a imull operation */
void code_imull(int reg) 
{
    code_op_register_1("imull",reg);
}

/* This function do a imull operation */
void code_idiv(int reg)
{
    code_op_register_1("idiv",reg);
}

/* This function do a mod operation */
void code_mod(int reg)
{
    code_op_register_1("idivl",reg);
    code_mov(EAX,EDX);
}