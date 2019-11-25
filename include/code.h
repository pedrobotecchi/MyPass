#define MAXSENTENCE 100

char* code_register(int reg);
void code_start();
void code_end();
void code_sentence(char* sentence);
void code_label(char * labelt, int labeln);
void code_op_imm_2(const char * op, int regdest, char * imm);
void code_cmp_imm(int regdest, char * imm);
void code_false(int labelnum);
void code_jump_label(char * op, int label);
void code_goto(int labelnum);
void code_jg(int labelnum);
void code_jl(int labelnum);
void code_je(int labelnum);
void code_jne(int labeln);
void code_jle(int labeln);
void code_jge(int labeln);
void code_loop_label(int labeln);

void code_push(int register);
void code_op_register_1(const char * op,int register);
void code_op_register_2(const char * op,int destRegister,int sourceRegister);
void code_prologue_relop();
void code_epilogue_relop(int labelT,int labelF);
void code_mov_imm(int destRegister, char * imm);
void code_pop(int reg);
void code_cmp(int destRegister, int sourceRegister);
void code_add(int destReg, int sourceReg);
void code_sub(int destReg, int sourceReg);
void code_or(int destReg, int sourceReg);
void code_and(int destReg, int sourceReg);
void code_mov(int destRegister,int sourceRegister);
void code_imull(int reg);
void code_idiv(int reg);
void code_mod(int reg);
