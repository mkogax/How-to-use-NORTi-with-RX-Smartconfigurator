/*
 * NORTi_interrupt.c
 *
 *       Created on: Jun 24, 2022
 * Last Modified on: Jun 29, 2022 9:00
 *           Author: NoMaY
 *          License: MIT
 */

#include "NORTi_interrupt.h"
#include <machine.h>

static void (* const Int_Hook_Vectors[])(void);

void NORTi_INT_HOOK_init(void)
{
    set_intb((void *)Int_Hook_Vectors);
}

static void dummy_NORTi_INT_HOOK(void)
{
    // To prevent developers from being misled regarding the following inline assemble error.
    // E0552111:Symbol is undefined
    for(;;);
}

#pragma inline_asm INT_COMMON_nonOS
static void INT_COMMON_nonOS(void)
{
    XCHG [SP], R15 // the address of hook_entry_XXX --> R15
    ADD #(- (__$hook_entry___0 + 7)), R15 // 7 = 4 * size of NOP + size of BSR.W
    SHLR #1, R15
    ADD #TOPOF C$VECT, R15 // i.e. C$VECT + ((hook_entry_XXX - hook_entry___0) / 8) * 4 --> R15
    MOV.L [R15], R15
    XCHG [SP], R15
    RTS // i.e. JMP [SP+]
}

#pragma inline_asm INT_COMMON_NORTi
static void INT_COMMON_NORTi(void)
{
    POP R15 // the address of hook_entry_XXX --> R15
    ADD #(- (__$hook_entry___0 + 7)), R15 // 7 = size of BSR.A + size of BSR.W
    SHLR #1, R15
    ADD #TOPOF C$VECT, R15 // i.e. C$VECT + ((hook_entry_XXX - hook_entry___0) / 8) * 4 --> R15
    MOV.L [R15], R15
    PUSHC PSW
    MOV.L #?+, R14
    PUSH.L R14
    JMP R15 // Program will return to the next label from the interrupt service routine by RTE.
?:

    BRA.A _v3_ret_int // Call NORTi API (in this case, jump to NORTi).
}

#pragma inline_asm INT_ENTRY_nonOS
static void INT_ENTRY_nonOS(void)
{
    NOP
    NOP
    NOP
    NOP
    BSR.W __$INT_COMMON_nonOS
}

#pragma inline_asm INT_ENTRY_NORTi
static void INT_ENTRY_NORTi(void)
{
    BSR.A _v3_ent_int // Call NORTi API.
    BSR.W __$INT_COMMON_NORTi
}

// Documents
// https://www.mispo.co.jp/document/no4guid.pdf
// https://www.renesas.com/jp/ja/document/mas/rx-family-rxv3-instruction-set-architecture-users-manual-software-rev100

// Labels and functions
extern void v3_ent_int(void);
extern void v3_ret_int (void);
static void INT_COMMON_nonOS(void);
static void INT_COMMON_NORTi(void);

// To prevent the symbol from being optimized out.
void * const dummy_NORTi_INT_enter  = (void *)v3_ent_int;
void * const dummy_NORTi_INT_return = (void *)v3_ret_int;
void * const dummy_INT_COMMON_nonOS = (void *)INT_COMMON_nonOS;
void * const dummy_INT_COMMON_NORTi = (void *)INT_COMMON_NORTi;

// To prevent the section name symbol `C$VECT` from being optimized out.
void * const dummy_VECTTBL = (void *)__sectop("C$VECT");

// To prevent a compile warning.
void * const dummy2_NORTi_INT_HOOK  = (void *)dummy_NORTi_INT_HOOK;

// The code size of the following functions is just 8 bytes.
// For nonOS
// 00000000 03                      NOP
// 00000001 03                      NOP
// 00000002 03                      NOP
// 00000003 03                      NOP
// 00000004 39rrrr               W  BSR.W __$INT_COMMON_nonOS
// 00000007 02                      RTS // Not used.
// For NORTi
// 00000000 05rrrrrr             A  BSR.A _v3_ret_int
// 00000004 39rrrr               W  BSR.W __$INT_COMMON_NORTi
// 00000007 02                      RTS // Not used.

#define _INT_ENTRY(sel) INT_ENTRY_##sel()
#define INT_ENTRY(sel) _INT_ENTRY(sel)

static void hook_entry___0(void) { INT_ENTRY(INTERRUPT___0); }
static void hook_entry___1(void) { INT_ENTRY(INTERRUPT___1); }
static void hook_entry___2(void) { INT_ENTRY(INTERRUPT___2); }
static void hook_entry___3(void) { INT_ENTRY(INTERRUPT___3); }
static void hook_entry___4(void) { INT_ENTRY(INTERRUPT___4); }
static void hook_entry___5(void) { INT_ENTRY(INTERRUPT___5); }
static void hook_entry___6(void) { INT_ENTRY(INTERRUPT___6); }
static void hook_entry___7(void) { INT_ENTRY(INTERRUPT___7); }
static void hook_entry___8(void) { INT_ENTRY(INTERRUPT___8); }
static void hook_entry___9(void) { INT_ENTRY(INTERRUPT___9); }
static void hook_entry__10(void) { INT_ENTRY(INTERRUPT__10); }
static void hook_entry__11(void) { INT_ENTRY(INTERRUPT__11); }
static void hook_entry__12(void) { INT_ENTRY(INTERRUPT__12); }
static void hook_entry__13(void) { INT_ENTRY(INTERRUPT__13); }
static void hook_entry__14(void) { INT_ENTRY(INTERRUPT__14); }
static void hook_entry__15(void) { INT_ENTRY(INTERRUPT__15); }
static void hook_entry__16(void) { INT_ENTRY(INTERRUPT__16); }
static void hook_entry__17(void) { INT_ENTRY(INTERRUPT__17); }
static void hook_entry__18(void) { INT_ENTRY(INTERRUPT__18); }
static void hook_entry__19(void) { INT_ENTRY(INTERRUPT__19); }
static void hook_entry__20(void) { INT_ENTRY(INTERRUPT__20); }
static void hook_entry__21(void) { INT_ENTRY(INTERRUPT__21); }
static void hook_entry__22(void) { INT_ENTRY(INTERRUPT__22); }
static void hook_entry__23(void) { INT_ENTRY(INTERRUPT__23); }
static void hook_entry__24(void) { INT_ENTRY(INTERRUPT__24); }
static void hook_entry__25(void) { INT_ENTRY(INTERRUPT__25); }
static void hook_entry__26(void) { INT_ENTRY(INTERRUPT__26); }
static void hook_entry__27(void) { INT_ENTRY(INTERRUPT__27); }
static void hook_entry__28(void) { INT_ENTRY(INTERRUPT__28); }
static void hook_entry__29(void) { INT_ENTRY(INTERRUPT__29); }
static void hook_entry__30(void) { INT_ENTRY(INTERRUPT__30); }
static void hook_entry__31(void) { INT_ENTRY(INTERRUPT__31); }
static void hook_entry__32(void) { INT_ENTRY(INTERRUPT__32); }
static void hook_entry__33(void) { INT_ENTRY(INTERRUPT__33); }
static void hook_entry__34(void) { INT_ENTRY(INTERRUPT__34); }
static void hook_entry__35(void) { INT_ENTRY(INTERRUPT__35); }
static void hook_entry__36(void) { INT_ENTRY(INTERRUPT__36); }
static void hook_entry__37(void) { INT_ENTRY(INTERRUPT__37); }
static void hook_entry__38(void) { INT_ENTRY(INTERRUPT__38); }
static void hook_entry__39(void) { INT_ENTRY(INTERRUPT__39); }
static void hook_entry__40(void) { INT_ENTRY(INTERRUPT__40); }
static void hook_entry__41(void) { INT_ENTRY(INTERRUPT__41); }
static void hook_entry__42(void) { INT_ENTRY(INTERRUPT__42); }
static void hook_entry__43(void) { INT_ENTRY(INTERRUPT__43); }
static void hook_entry__44(void) { INT_ENTRY(INTERRUPT__44); }
static void hook_entry__45(void) { INT_ENTRY(INTERRUPT__45); }
static void hook_entry__46(void) { INT_ENTRY(INTERRUPT__46); }
static void hook_entry__47(void) { INT_ENTRY(INTERRUPT__47); }
static void hook_entry__48(void) { INT_ENTRY(INTERRUPT__48); }
static void hook_entry__49(void) { INT_ENTRY(INTERRUPT__49); }
static void hook_entry__50(void) { INT_ENTRY(INTERRUPT__50); }
static void hook_entry__51(void) { INT_ENTRY(INTERRUPT__51); }
static void hook_entry__52(void) { INT_ENTRY(INTERRUPT__52); }
static void hook_entry__53(void) { INT_ENTRY(INTERRUPT__53); }
static void hook_entry__54(void) { INT_ENTRY(INTERRUPT__54); }
static void hook_entry__55(void) { INT_ENTRY(INTERRUPT__55); }
static void hook_entry__56(void) { INT_ENTRY(INTERRUPT__56); }
static void hook_entry__57(void) { INT_ENTRY(INTERRUPT__57); }
static void hook_entry__58(void) { INT_ENTRY(INTERRUPT__58); }
static void hook_entry__59(void) { INT_ENTRY(INTERRUPT__59); }
static void hook_entry__60(void) { INT_ENTRY(INTERRUPT__60); }
static void hook_entry__61(void) { INT_ENTRY(INTERRUPT__61); }
static void hook_entry__62(void) { INT_ENTRY(INTERRUPT__62); }
static void hook_entry__63(void) { INT_ENTRY(INTERRUPT__63); }
static void hook_entry__64(void) { INT_ENTRY(INTERRUPT__64); }
static void hook_entry__65(void) { INT_ENTRY(INTERRUPT__65); }
static void hook_entry__66(void) { INT_ENTRY(INTERRUPT__66); }
static void hook_entry__67(void) { INT_ENTRY(INTERRUPT__67); }
static void hook_entry__68(void) { INT_ENTRY(INTERRUPT__68); }
static void hook_entry__69(void) { INT_ENTRY(INTERRUPT__69); }
static void hook_entry__70(void) { INT_ENTRY(INTERRUPT__70); }
static void hook_entry__71(void) { INT_ENTRY(INTERRUPT__71); }
static void hook_entry__72(void) { INT_ENTRY(INTERRUPT__72); }
static void hook_entry__73(void) { INT_ENTRY(INTERRUPT__73); }
static void hook_entry__74(void) { INT_ENTRY(INTERRUPT__74); }
static void hook_entry__75(void) { INT_ENTRY(INTERRUPT__75); }
static void hook_entry__76(void) { INT_ENTRY(INTERRUPT__76); }
static void hook_entry__77(void) { INT_ENTRY(INTERRUPT__77); }
static void hook_entry__78(void) { INT_ENTRY(INTERRUPT__78); }
static void hook_entry__79(void) { INT_ENTRY(INTERRUPT__79); }
static void hook_entry__80(void) { INT_ENTRY(INTERRUPT__80); }
static void hook_entry__81(void) { INT_ENTRY(INTERRUPT__81); }
static void hook_entry__82(void) { INT_ENTRY(INTERRUPT__82); }
static void hook_entry__83(void) { INT_ENTRY(INTERRUPT__83); }
static void hook_entry__84(void) { INT_ENTRY(INTERRUPT__84); }
static void hook_entry__85(void) { INT_ENTRY(INTERRUPT__85); }
static void hook_entry__86(void) { INT_ENTRY(INTERRUPT__86); }
static void hook_entry__87(void) { INT_ENTRY(INTERRUPT__87); }
static void hook_entry__88(void) { INT_ENTRY(INTERRUPT__88); }
static void hook_entry__89(void) { INT_ENTRY(INTERRUPT__89); }
static void hook_entry__90(void) { INT_ENTRY(INTERRUPT__90); }
static void hook_entry__91(void) { INT_ENTRY(INTERRUPT__91); }
static void hook_entry__92(void) { INT_ENTRY(INTERRUPT__92); }
static void hook_entry__93(void) { INT_ENTRY(INTERRUPT__93); }
static void hook_entry__94(void) { INT_ENTRY(INTERRUPT__94); }
static void hook_entry__95(void) { INT_ENTRY(INTERRUPT__95); }
static void hook_entry__96(void) { INT_ENTRY(INTERRUPT__96); }
static void hook_entry__97(void) { INT_ENTRY(INTERRUPT__97); }
static void hook_entry__98(void) { INT_ENTRY(INTERRUPT__98); }
static void hook_entry__99(void) { INT_ENTRY(INTERRUPT__99); }
static void hook_entry_100(void) { INT_ENTRY(INTERRUPT_100); }
static void hook_entry_101(void) { INT_ENTRY(INTERRUPT_101); }
static void hook_entry_102(void) { INT_ENTRY(INTERRUPT_102); }
static void hook_entry_103(void) { INT_ENTRY(INTERRUPT_103); }
static void hook_entry_104(void) { INT_ENTRY(INTERRUPT_104); }
static void hook_entry_105(void) { INT_ENTRY(INTERRUPT_105); }
static void hook_entry_106(void) { INT_ENTRY(INTERRUPT_106); }
static void hook_entry_107(void) { INT_ENTRY(INTERRUPT_107); }
static void hook_entry_108(void) { INT_ENTRY(INTERRUPT_108); }
static void hook_entry_109(void) { INT_ENTRY(INTERRUPT_109); }
static void hook_entry_110(void) { INT_ENTRY(INTERRUPT_110); }
static void hook_entry_111(void) { INT_ENTRY(INTERRUPT_111); }
static void hook_entry_112(void) { INT_ENTRY(INTERRUPT_112); }
static void hook_entry_113(void) { INT_ENTRY(INTERRUPT_113); }
static void hook_entry_114(void) { INT_ENTRY(INTERRUPT_114); }
static void hook_entry_115(void) { INT_ENTRY(INTERRUPT_115); }
static void hook_entry_116(void) { INT_ENTRY(INTERRUPT_116); }
static void hook_entry_117(void) { INT_ENTRY(INTERRUPT_117); }
static void hook_entry_118(void) { INT_ENTRY(INTERRUPT_118); }
static void hook_entry_119(void) { INT_ENTRY(INTERRUPT_119); }
static void hook_entry_120(void) { INT_ENTRY(INTERRUPT_120); }
static void hook_entry_121(void) { INT_ENTRY(INTERRUPT_121); }
static void hook_entry_122(void) { INT_ENTRY(INTERRUPT_122); }
static void hook_entry_123(void) { INT_ENTRY(INTERRUPT_123); }
static void hook_entry_124(void) { INT_ENTRY(INTERRUPT_124); }
static void hook_entry_125(void) { INT_ENTRY(INTERRUPT_125); }
static void hook_entry_126(void) { INT_ENTRY(INTERRUPT_126); }
static void hook_entry_127(void) { INT_ENTRY(INTERRUPT_127); }
static void hook_entry_128(void) { INT_ENTRY(INTERRUPT_128); }
static void hook_entry_129(void) { INT_ENTRY(INTERRUPT_129); }
static void hook_entry_130(void) { INT_ENTRY(INTERRUPT_130); }
static void hook_entry_131(void) { INT_ENTRY(INTERRUPT_131); }
static void hook_entry_132(void) { INT_ENTRY(INTERRUPT_132); }
static void hook_entry_133(void) { INT_ENTRY(INTERRUPT_133); }
static void hook_entry_134(void) { INT_ENTRY(INTERRUPT_134); }
static void hook_entry_135(void) { INT_ENTRY(INTERRUPT_135); }
static void hook_entry_136(void) { INT_ENTRY(INTERRUPT_136); }
static void hook_entry_137(void) { INT_ENTRY(INTERRUPT_137); }
static void hook_entry_138(void) { INT_ENTRY(INTERRUPT_138); }
static void hook_entry_139(void) { INT_ENTRY(INTERRUPT_139); }
static void hook_entry_140(void) { INT_ENTRY(INTERRUPT_140); }
static void hook_entry_141(void) { INT_ENTRY(INTERRUPT_141); }
static void hook_entry_142(void) { INT_ENTRY(INTERRUPT_142); }
static void hook_entry_143(void) { INT_ENTRY(INTERRUPT_143); }
static void hook_entry_144(void) { INT_ENTRY(INTERRUPT_144); }
static void hook_entry_145(void) { INT_ENTRY(INTERRUPT_145); }
static void hook_entry_146(void) { INT_ENTRY(INTERRUPT_146); }
static void hook_entry_147(void) { INT_ENTRY(INTERRUPT_147); }
static void hook_entry_148(void) { INT_ENTRY(INTERRUPT_148); }
static void hook_entry_149(void) { INT_ENTRY(INTERRUPT_149); }
static void hook_entry_150(void) { INT_ENTRY(INTERRUPT_150); }
static void hook_entry_151(void) { INT_ENTRY(INTERRUPT_151); }
static void hook_entry_152(void) { INT_ENTRY(INTERRUPT_152); }
static void hook_entry_153(void) { INT_ENTRY(INTERRUPT_153); }
static void hook_entry_154(void) { INT_ENTRY(INTERRUPT_154); }
static void hook_entry_155(void) { INT_ENTRY(INTERRUPT_155); }
static void hook_entry_156(void) { INT_ENTRY(INTERRUPT_156); }
static void hook_entry_157(void) { INT_ENTRY(INTERRUPT_157); }
static void hook_entry_158(void) { INT_ENTRY(INTERRUPT_158); }
static void hook_entry_159(void) { INT_ENTRY(INTERRUPT_159); }
static void hook_entry_160(void) { INT_ENTRY(INTERRUPT_160); }
static void hook_entry_161(void) { INT_ENTRY(INTERRUPT_161); }
static void hook_entry_162(void) { INT_ENTRY(INTERRUPT_162); }
static void hook_entry_163(void) { INT_ENTRY(INTERRUPT_163); }
static void hook_entry_164(void) { INT_ENTRY(INTERRUPT_164); }
static void hook_entry_165(void) { INT_ENTRY(INTERRUPT_165); }
static void hook_entry_166(void) { INT_ENTRY(INTERRUPT_166); }
static void hook_entry_167(void) { INT_ENTRY(INTERRUPT_167); }
static void hook_entry_168(void) { INT_ENTRY(INTERRUPT_168); }
static void hook_entry_169(void) { INT_ENTRY(INTERRUPT_169); }
static void hook_entry_170(void) { INT_ENTRY(INTERRUPT_170); }
static void hook_entry_171(void) { INT_ENTRY(INTERRUPT_171); }
static void hook_entry_172(void) { INT_ENTRY(INTERRUPT_172); }
static void hook_entry_173(void) { INT_ENTRY(INTERRUPT_173); }
static void hook_entry_174(void) { INT_ENTRY(INTERRUPT_174); }
static void hook_entry_175(void) { INT_ENTRY(INTERRUPT_175); }
static void hook_entry_176(void) { INT_ENTRY(INTERRUPT_176); }
static void hook_entry_177(void) { INT_ENTRY(INTERRUPT_177); }
static void hook_entry_178(void) { INT_ENTRY(INTERRUPT_178); }
static void hook_entry_179(void) { INT_ENTRY(INTERRUPT_179); }
static void hook_entry_180(void) { INT_ENTRY(INTERRUPT_180); }
static void hook_entry_181(void) { INT_ENTRY(INTERRUPT_181); }
static void hook_entry_182(void) { INT_ENTRY(INTERRUPT_182); }
static void hook_entry_183(void) { INT_ENTRY(INTERRUPT_183); }
static void hook_entry_184(void) { INT_ENTRY(INTERRUPT_184); }
static void hook_entry_185(void) { INT_ENTRY(INTERRUPT_185); }
static void hook_entry_186(void) { INT_ENTRY(INTERRUPT_186); }
static void hook_entry_187(void) { INT_ENTRY(INTERRUPT_187); }
static void hook_entry_188(void) { INT_ENTRY(INTERRUPT_188); }
static void hook_entry_189(void) { INT_ENTRY(INTERRUPT_189); }
static void hook_entry_190(void) { INT_ENTRY(INTERRUPT_190); }
static void hook_entry_191(void) { INT_ENTRY(INTERRUPT_191); }
static void hook_entry_192(void) { INT_ENTRY(INTERRUPT_192); }
static void hook_entry_193(void) { INT_ENTRY(INTERRUPT_193); }
static void hook_entry_194(void) { INT_ENTRY(INTERRUPT_194); }
static void hook_entry_195(void) { INT_ENTRY(INTERRUPT_195); }
static void hook_entry_196(void) { INT_ENTRY(INTERRUPT_196); }
static void hook_entry_197(void) { INT_ENTRY(INTERRUPT_197); }
static void hook_entry_198(void) { INT_ENTRY(INTERRUPT_198); }
static void hook_entry_199(void) { INT_ENTRY(INTERRUPT_199); }
static void hook_entry_200(void) { INT_ENTRY(INTERRUPT_200); }
static void hook_entry_201(void) { INT_ENTRY(INTERRUPT_201); }
static void hook_entry_202(void) { INT_ENTRY(INTERRUPT_202); }
static void hook_entry_203(void) { INT_ENTRY(INTERRUPT_203); }
static void hook_entry_204(void) { INT_ENTRY(INTERRUPT_204); }
static void hook_entry_205(void) { INT_ENTRY(INTERRUPT_205); }
static void hook_entry_206(void) { INT_ENTRY(INTERRUPT_206); }
static void hook_entry_207(void) { INT_ENTRY(INTERRUPT_207); }
static void hook_entry_208(void) { INT_ENTRY(INTERRUPT_208); }
static void hook_entry_209(void) { INT_ENTRY(INTERRUPT_209); }
static void hook_entry_210(void) { INT_ENTRY(INTERRUPT_210); }
static void hook_entry_211(void) { INT_ENTRY(INTERRUPT_211); }
static void hook_entry_212(void) { INT_ENTRY(INTERRUPT_212); }
static void hook_entry_213(void) { INT_ENTRY(INTERRUPT_213); }
static void hook_entry_214(void) { INT_ENTRY(INTERRUPT_214); }
static void hook_entry_215(void) { INT_ENTRY(INTERRUPT_215); }
static void hook_entry_216(void) { INT_ENTRY(INTERRUPT_216); }
static void hook_entry_217(void) { INT_ENTRY(INTERRUPT_217); }
static void hook_entry_218(void) { INT_ENTRY(INTERRUPT_218); }
static void hook_entry_219(void) { INT_ENTRY(INTERRUPT_219); }
static void hook_entry_220(void) { INT_ENTRY(INTERRUPT_220); }
static void hook_entry_221(void) { INT_ENTRY(INTERRUPT_221); }
static void hook_entry_222(void) { INT_ENTRY(INTERRUPT_222); }
static void hook_entry_223(void) { INT_ENTRY(INTERRUPT_223); }
static void hook_entry_224(void) { INT_ENTRY(INTERRUPT_224); }
static void hook_entry_225(void) { INT_ENTRY(INTERRUPT_225); }
static void hook_entry_226(void) { INT_ENTRY(INTERRUPT_226); }
static void hook_entry_227(void) { INT_ENTRY(INTERRUPT_227); }
static void hook_entry_228(void) { INT_ENTRY(INTERRUPT_228); }
static void hook_entry_229(void) { INT_ENTRY(INTERRUPT_229); }
static void hook_entry_230(void) { INT_ENTRY(INTERRUPT_230); }
static void hook_entry_231(void) { INT_ENTRY(INTERRUPT_231); }
static void hook_entry_232(void) { INT_ENTRY(INTERRUPT_232); }
static void hook_entry_233(void) { INT_ENTRY(INTERRUPT_233); }
static void hook_entry_234(void) { INT_ENTRY(INTERRUPT_234); }
static void hook_entry_235(void) { INT_ENTRY(INTERRUPT_235); }
static void hook_entry_236(void) { INT_ENTRY(INTERRUPT_236); }
static void hook_entry_237(void) { INT_ENTRY(INTERRUPT_237); }
static void hook_entry_238(void) { INT_ENTRY(INTERRUPT_238); }
static void hook_entry_239(void) { INT_ENTRY(INTERRUPT_239); }
static void hook_entry_240(void) { INT_ENTRY(INTERRUPT_240); }
static void hook_entry_241(void) { INT_ENTRY(INTERRUPT_241); }
static void hook_entry_242(void) { INT_ENTRY(INTERRUPT_242); }
static void hook_entry_243(void) { INT_ENTRY(INTERRUPT_243); }
static void hook_entry_244(void) { INT_ENTRY(INTERRUPT_244); }
static void hook_entry_245(void) { INT_ENTRY(INTERRUPT_245); }
static void hook_entry_246(void) { INT_ENTRY(INTERRUPT_246); }
static void hook_entry_247(void) { INT_ENTRY(INTERRUPT_247); }
static void hook_entry_248(void) { INT_ENTRY(INTERRUPT_248); }
static void hook_entry_249(void) { INT_ENTRY(INTERRUPT_249); }
static void hook_entry_250(void) { INT_ENTRY(INTERRUPT_250); }
static void hook_entry_251(void) { INT_ENTRY(INTERRUPT_251); }
static void hook_entry_252(void) { INT_ENTRY(INTERRUPT_252); }
static void hook_entry_253(void) { INT_ENTRY(INTERRUPT_253); }
static void hook_entry_254(void) { INT_ENTRY(INTERRUPT_254); }
static void hook_entry_255(void) { INT_ENTRY(INTERRUPT_255); }

static void (* const Int_Hook_Vectors[])(void) =
{
    (void (*)(void))hook_entry___0,
    (void (*)(void))hook_entry___1,
    (void (*)(void))hook_entry___2,
    (void (*)(void))hook_entry___3,
    (void (*)(void))hook_entry___4,
    (void (*)(void))hook_entry___5,
    (void (*)(void))hook_entry___6,
    (void (*)(void))hook_entry___7,
    (void (*)(void))hook_entry___8,
    (void (*)(void))hook_entry___9,
    (void (*)(void))hook_entry__10,
    (void (*)(void))hook_entry__11,
    (void (*)(void))hook_entry__12,
    (void (*)(void))hook_entry__13,
    (void (*)(void))hook_entry__14,
    (void (*)(void))hook_entry__15,
    (void (*)(void))hook_entry__16,
    (void (*)(void))hook_entry__17,
    (void (*)(void))hook_entry__18,
    (void (*)(void))hook_entry__19,
    (void (*)(void))hook_entry__20,
    (void (*)(void))hook_entry__21,
    (void (*)(void))hook_entry__22,
    (void (*)(void))hook_entry__23,
    (void (*)(void))hook_entry__24,
    (void (*)(void))hook_entry__25,
    (void (*)(void))hook_entry__26,
    (void (*)(void))hook_entry__27,
    (void (*)(void))hook_entry__28,
    (void (*)(void))hook_entry__29,
    (void (*)(void))hook_entry__30,
    (void (*)(void))hook_entry__31,
    (void (*)(void))hook_entry__32,
    (void (*)(void))hook_entry__33,
    (void (*)(void))hook_entry__34,
    (void (*)(void))hook_entry__35,
    (void (*)(void))hook_entry__36,
    (void (*)(void))hook_entry__37,
    (void (*)(void))hook_entry__38,
    (void (*)(void))hook_entry__39,
    (void (*)(void))hook_entry__40,
    (void (*)(void))hook_entry__41,
    (void (*)(void))hook_entry__42,
    (void (*)(void))hook_entry__43,
    (void (*)(void))hook_entry__44,
    (void (*)(void))hook_entry__45,
    (void (*)(void))hook_entry__46,
    (void (*)(void))hook_entry__47,
    (void (*)(void))hook_entry__48,
    (void (*)(void))hook_entry__49,
    (void (*)(void))hook_entry__50,
    (void (*)(void))hook_entry__51,
    (void (*)(void))hook_entry__52,
    (void (*)(void))hook_entry__53,
    (void (*)(void))hook_entry__54,
    (void (*)(void))hook_entry__55,
    (void (*)(void))hook_entry__56,
    (void (*)(void))hook_entry__57,
    (void (*)(void))hook_entry__58,
    (void (*)(void))hook_entry__59,
    (void (*)(void))hook_entry__60,
    (void (*)(void))hook_entry__61,
    (void (*)(void))hook_entry__62,
    (void (*)(void))hook_entry__63,
    (void (*)(void))hook_entry__64,
    (void (*)(void))hook_entry__65,
    (void (*)(void))hook_entry__66,
    (void (*)(void))hook_entry__67,
    (void (*)(void))hook_entry__68,
    (void (*)(void))hook_entry__69,
    (void (*)(void))hook_entry__70,
    (void (*)(void))hook_entry__71,
    (void (*)(void))hook_entry__72,
    (void (*)(void))hook_entry__73,
    (void (*)(void))hook_entry__74,
    (void (*)(void))hook_entry__75,
    (void (*)(void))hook_entry__76,
    (void (*)(void))hook_entry__77,
    (void (*)(void))hook_entry__78,
    (void (*)(void))hook_entry__79,
    (void (*)(void))hook_entry__80,
    (void (*)(void))hook_entry__81,
    (void (*)(void))hook_entry__82,
    (void (*)(void))hook_entry__83,
    (void (*)(void))hook_entry__84,
    (void (*)(void))hook_entry__85,
    (void (*)(void))hook_entry__86,
    (void (*)(void))hook_entry__87,
    (void (*)(void))hook_entry__88,
    (void (*)(void))hook_entry__89,
    (void (*)(void))hook_entry__90,
    (void (*)(void))hook_entry__91,
    (void (*)(void))hook_entry__92,
    (void (*)(void))hook_entry__93,
    (void (*)(void))hook_entry__94,
    (void (*)(void))hook_entry__95,
    (void (*)(void))hook_entry__96,
    (void (*)(void))hook_entry__97,
    (void (*)(void))hook_entry__98,
    (void (*)(void))hook_entry__99,
    (void (*)(void))hook_entry_100,
    (void (*)(void))hook_entry_101,
    (void (*)(void))hook_entry_102,
    (void (*)(void))hook_entry_103,
    (void (*)(void))hook_entry_104,
    (void (*)(void))hook_entry_105,
    (void (*)(void))hook_entry_106,
    (void (*)(void))hook_entry_107,
    (void (*)(void))hook_entry_108,
    (void (*)(void))hook_entry_109,
    (void (*)(void))hook_entry_110,
    (void (*)(void))hook_entry_111,
    (void (*)(void))hook_entry_112,
    (void (*)(void))hook_entry_113,
    (void (*)(void))hook_entry_114,
    (void (*)(void))hook_entry_115,
    (void (*)(void))hook_entry_116,
    (void (*)(void))hook_entry_117,
    (void (*)(void))hook_entry_118,
    (void (*)(void))hook_entry_119,
    (void (*)(void))hook_entry_120,
    (void (*)(void))hook_entry_121,
    (void (*)(void))hook_entry_122,
    (void (*)(void))hook_entry_123,
    (void (*)(void))hook_entry_124,
    (void (*)(void))hook_entry_125,
    (void (*)(void))hook_entry_126,
    (void (*)(void))hook_entry_127,
    (void (*)(void))hook_entry_128,
    (void (*)(void))hook_entry_129,
    (void (*)(void))hook_entry_130,
    (void (*)(void))hook_entry_131,
    (void (*)(void))hook_entry_132,
    (void (*)(void))hook_entry_133,
    (void (*)(void))hook_entry_134,
    (void (*)(void))hook_entry_135,
    (void (*)(void))hook_entry_136,
    (void (*)(void))hook_entry_137,
    (void (*)(void))hook_entry_138,
    (void (*)(void))hook_entry_139,
    (void (*)(void))hook_entry_140,
    (void (*)(void))hook_entry_141,
    (void (*)(void))hook_entry_142,
    (void (*)(void))hook_entry_143,
    (void (*)(void))hook_entry_144,
    (void (*)(void))hook_entry_145,
    (void (*)(void))hook_entry_146,
    (void (*)(void))hook_entry_147,
    (void (*)(void))hook_entry_148,
    (void (*)(void))hook_entry_149,
    (void (*)(void))hook_entry_150,
    (void (*)(void))hook_entry_151,
    (void (*)(void))hook_entry_152,
    (void (*)(void))hook_entry_153,
    (void (*)(void))hook_entry_154,
    (void (*)(void))hook_entry_155,
    (void (*)(void))hook_entry_156,
    (void (*)(void))hook_entry_157,
    (void (*)(void))hook_entry_158,
    (void (*)(void))hook_entry_159,
    (void (*)(void))hook_entry_160,
    (void (*)(void))hook_entry_161,
    (void (*)(void))hook_entry_162,
    (void (*)(void))hook_entry_163,
    (void (*)(void))hook_entry_164,
    (void (*)(void))hook_entry_165,
    (void (*)(void))hook_entry_166,
    (void (*)(void))hook_entry_167,
    (void (*)(void))hook_entry_168,
    (void (*)(void))hook_entry_169,
    (void (*)(void))hook_entry_170,
    (void (*)(void))hook_entry_171,
    (void (*)(void))hook_entry_172,
    (void (*)(void))hook_entry_173,
    (void (*)(void))hook_entry_174,
    (void (*)(void))hook_entry_175,
    (void (*)(void))hook_entry_176,
    (void (*)(void))hook_entry_177,
    (void (*)(void))hook_entry_178,
    (void (*)(void))hook_entry_179,
    (void (*)(void))hook_entry_180,
    (void (*)(void))hook_entry_181,
    (void (*)(void))hook_entry_182,
    (void (*)(void))hook_entry_183,
    (void (*)(void))hook_entry_184,
    (void (*)(void))hook_entry_185,
    (void (*)(void))hook_entry_186,
    (void (*)(void))hook_entry_187,
    (void (*)(void))hook_entry_188,
    (void (*)(void))hook_entry_189,
    (void (*)(void))hook_entry_190,
    (void (*)(void))hook_entry_191,
    (void (*)(void))hook_entry_192,
    (void (*)(void))hook_entry_193,
    (void (*)(void))hook_entry_194,
    (void (*)(void))hook_entry_195,
    (void (*)(void))hook_entry_196,
    (void (*)(void))hook_entry_197,
    (void (*)(void))hook_entry_198,
    (void (*)(void))hook_entry_199,
    (void (*)(void))hook_entry_200,
    (void (*)(void))hook_entry_201,
    (void (*)(void))hook_entry_202,
    (void (*)(void))hook_entry_203,
    (void (*)(void))hook_entry_204,
    (void (*)(void))hook_entry_205,
    (void (*)(void))hook_entry_206,
    (void (*)(void))hook_entry_207,
    (void (*)(void))hook_entry_208,
    (void (*)(void))hook_entry_209,
    (void (*)(void))hook_entry_210,
    (void (*)(void))hook_entry_211,
    (void (*)(void))hook_entry_212,
    (void (*)(void))hook_entry_213,
    (void (*)(void))hook_entry_214,
    (void (*)(void))hook_entry_215,
    (void (*)(void))hook_entry_216,
    (void (*)(void))hook_entry_217,
    (void (*)(void))hook_entry_218,
    (void (*)(void))hook_entry_219,
    (void (*)(void))hook_entry_220,
    (void (*)(void))hook_entry_221,
    (void (*)(void))hook_entry_222,
    (void (*)(void))hook_entry_223,
    (void (*)(void))hook_entry_224,
    (void (*)(void))hook_entry_225,
    (void (*)(void))hook_entry_226,
    (void (*)(void))hook_entry_227,
    (void (*)(void))hook_entry_228,
    (void (*)(void))hook_entry_229,
    (void (*)(void))hook_entry_230,
    (void (*)(void))hook_entry_231,
    (void (*)(void))hook_entry_232,
    (void (*)(void))hook_entry_233,
    (void (*)(void))hook_entry_234,
    (void (*)(void))hook_entry_235,
    (void (*)(void))hook_entry_236,
    (void (*)(void))hook_entry_237,
    (void (*)(void))hook_entry_238,
    (void (*)(void))hook_entry_239,
    (void (*)(void))hook_entry_240,
    (void (*)(void))hook_entry_241,
    (void (*)(void))hook_entry_242,
    (void (*)(void))hook_entry_243,
    (void (*)(void))hook_entry_244,
    (void (*)(void))hook_entry_245,
    (void (*)(void))hook_entry_246,
    (void (*)(void))hook_entry_247,
    (void (*)(void))hook_entry_248,
    (void (*)(void))hook_entry_249,
    (void (*)(void))hook_entry_250,
    (void (*)(void))hook_entry_251,
    (void (*)(void))hook_entry_252,
    (void (*)(void))hook_entry_253,
    (void (*)(void))hook_entry_254,
    (void (*)(void))hook_entry_255
};
