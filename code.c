#include<stdio.h>
#include<stdlib.h>
#include<code.h>
#include<reg.h>

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

void code_end() {
	printf("_start:\n");
	//code_sentence("call\t.P0");   Descomentar isso quando o rótulo para procedures funcionar
	code_sentence("movl\t$1, %eax");
	code_sentence("movl\t$0, %ebx");
	code_sentence("int\t$0x80");

}

void code_sentence(char* sentence) {
	printf("\t%s\n", sentence);
}

void code_label(char * labelt, int labeln)
{
    printf(".%s%d:\n",labelt,labeln);
}

void code_op_imm_2(const char * op, int regdest, char * imm){
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t$%s, %s",op,imm, code_register(regdest));
    code_sentence(sentence);
}

void code_cmp_imm(int regdest, char * imm) {
    code_op_imm_2("cmpl",regdest, imm);
}

void code_false(int labeln){
    code_jump_label("jz",labeln);
}

void code_goto(int labeln){
    code_jump_label("jmp",labeln);
}

void code_jg(int labeln){
    code_jump_label("jg",labeln);
}

void code_jl(int labeln){
    code_jump_label("jl",labeln);
}

void code_je(int labeln){
    code_jump_label("je",labeln);
}

void code_jne(int labeln){
    code_jump_label("jne",labeln);
}

void code_jle(int labeln){
    code_jump_label("jle",labeln);
}

void code_jge(int labeln){
    code_jump_label("jge",labeln);
}

void code_jump_label(char * op, int labeln) {
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t.L%d",op,labeln);
    code_sentence(sentence);
}

void code_loop_label(int labeln)
{
    code_label("L",labeln);
}

void code_push(int reg)
{
    code_op_register_1("pushl",reg);
}

void code_op_register_1(const char * op,int reg)
{
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t%s",op,code_register(reg));
    code_sentence(sentence);
}

void code_op_register_2(const char * op,int destRegister,int sourceRegister)
{
    char sentence[MAXSENTENCE];
    sprintf(sentence,"%s\t$%s, %s",op,code_register(sourceRegister),code_register(destRegister));
    code_sentence(sentence);
}

void code_prologue_relop()
{
    code_pop(EBX);
    code_cmp(EBX,EAX);
}

void code_pop(int reg)
{
    code_op_register_1("popl",reg);
}

void code_cmp(int destRegister, int sourceRegister)
{
    code_op_register_2("cmpl",sourceRegister,destRegister);
}

void code_epilogue_relop(int labelT,int labelF) {
    code_mov_imm(EAX,"0");
    code_goto(labelF);
    code_loop_label(labelT);
    code_mov_imm(EAX,"1");
    code_loop_label(labelF);
}

void code_mov(int destRegister,int sourceRegister)
{
    code_op_register_2("movl", destRegister,sourceRegister);
}

void code_mov_imm(int destRegister, char * imm)
{
    code_op_imm_2("movl",destRegister,imm);
}

void code_add(int destReg, int sourceReg)
{
    code_op_register_2("addl",destReg,sourceReg);
}

void code_sub(int destReg, int sourceReg)
{
    code_op_register_2("subl",destReg,sourceReg);
}

void code_or(int destReg, int sourceReg)
{
    code_op_register_2("orl",destReg,sourceReg);
}

void code_and(int destReg, int sourceReg)
{
    code_op_register_2("andl",destReg,sourceReg);
}

void code_imull(int reg) 
{
    code_op_register_1("imull",reg);
}

void code_idiv(int reg)
{
    code_op_register_1("idiv",reg);
}

void code_mod(int reg)
{
    code_op_register_1("idivl",reg);
    code_mov(EAX,EDX);
}