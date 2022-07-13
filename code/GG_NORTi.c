/*

MIT License

Copyright (c) 2022 Masao Koga

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

//--------------------------------
// GG
//--------------------------------
#include "GG.h"             // GGツールヘッダ

//--------------------------------
//  NORTi関連の定義(この場所だけでinclude)
//--------------------------------
// TSKIDD_MAXなど指定するときはこの直下で
#include "kernel.h"
#include "nocfg4.h"


//---------------------------------
//  NORTi用10msec割込み処理(FITモジュール利用)
//---------------------------------
#include "r_cmt_rx_if.h"
static uint32_t cmt_10msec_ch = 0xffffffff;
static uint32_t cmt_event_channel_number = 0xffffffff;
static void cmt_10msec_callback(void *pdata)
{
    GG_TP_ON(900);
    cmt_event_channel_number = *((uint32_t *)pdata); //cast pointer to uint32_t
    isig_tim();
    GG_TP_OFF(900);
}


//---------------------------------------------------------------------
//  CRETSK
//  タスク生成の一般化
//---------------------------------------------------------------------
typedef struct tagTSK_EXINF {       // タスクごとのワークエリア(仮)
    int dummy;
} T_TSK_EXINF;
#define CRETSK(id, func, level, stk, help) {            /* タスク生成の一般化   2022.07.10 M.Kogan */\
        static uint32_t stack[stk/sizeof(uint32_t)];    /* スタックは固定で取らないとosコマンドでtopアドレスが分からなくなる */\
        static T_TSK_EXINF exinf;                       /* タスクごとのワークエリア。ref_tsk()で参照可能 */\
        static const T_CTSK ctsk = {TA_HLNG, &exinf, func, level, stk,  stack, help};\
        gg_memset(&exinf, 0, sizeof(exinf));\
        cre_tsk(id, &ctsk);\
    }



//---------------------------------
//  GGコマンド
//---------------------------------
int C_test1(int argc, char **argv)  // test1コマンド実行部
{
    int i;
    gg_printf("argc=%d\n", argc);
    for (i=0; i<argc; i++) {
        long x = 0;
        if (gg_isdigit(argv[i][0])) x = gg_asc2int(argv[i]);
        gg_printf("argv[%2d] %-16s (0x%08lX,%ld)\n", i, argv[i], x, x);
    }
    return 0;
}

extern int C_task(int argc, char **argv);  // タスク状態の表示  GG_NORTi_task.c

int C_cmt(int argc, char **argv)  // CMT(FITモジュール) NORTi用タイマ設定状態表示
{
    gg_printf("cmt_10msec_ch = %d\n", cmt_10msec_ch);
    gg_printf("cmt_event_channel_number = %d\n", cmt_event_channel_number);
    return 0;
} 



//---------------------------------
//  タスク
//---------------------------------
int dummy(int n)            // ダミーソフト負荷
{
    static int c = 0;
    while(n>0) {
        c += n--;
    }
    return c;
}
TASK taskg(void)       // GGコンソール(taskg)
{

    gg_start("** GG for CCRX **\n");   // Start processing GG for CCRX
    gg_con_RegistMemCMD();              // メモリ系コマンド登録

    // Register with console command (you can add more and more in this way)
    GG_CON_CMDMRK("-- Example of original command registration --");     // Delimited line (displayed in the command list)
    GG_CON_CMDADD(C_test1,  "test1",    "[...]",    "Display command line arguments when executing this command" );  // [1]command(function),[2]command(name),[3]help(parameters),[4]help(description)
    GG_CON_CMDADD(C_task,   "task",     "[...]",    "Display and control task status" );        // タスク状態の表示と制御
    GG_CON_CMDADD(C_cmt,    "cmt",      "",         "CMT(FITモジュール) NORTi用タイマ設定状態表示" ); 

#ifdef GG_TP_ENABLE
    gg_tp_start();                      // TP機能の開始(TPコマンド登録)
#endif

    {
        PRI p;
        get_pri(0, &p);
        gg_printf("tskpri=%d\n", p);
    }

    gg_con_MonPrompt();

    while(1) {

        // Turn number 10 twice on and off at the beginning of loop
        GG_TP_ON(10);   // number 10 ON     440nsec(実測)
        GG_TP_OFF(10);  // number 10 OFF    440nsec(実測)
        GG_TP_ON(10);   // number 10 ON
        GG_TP_OFF(10);  // number 10 OFF

        // number 20 : Observe the processing time of gg_con_Check()
        GG_TP_ON(20);   // number 20 ON     2.00usec(実測)
        gg_con_Check(); // console processing (returns immediately if nothing is done)
        GG_TP_OFF(20);  // number 20 OFF

        // number 30 : Observe the processing time of dummy(50)-->delay(10msec)
        GG_TP_ON(30);   // number 30 ON
        dly_tsk(10/MSEC);
        GG_TP_OFF(30);  // number 30 OFF

    }

}

extern int str2tid(char *name);      // 文字列(タスク名またはID数値)からタスクID(1..)を取得(0=該当なし)

#include "Config_TMR0.h"
void task1_TMR0_callback(void)
{
    GG_TP_ON(101);
    wup_tsk(str2tid("task1"));
    GG_TP_OFF(101);
}
TASK task1(int stacd)       // タスク１(task1)
{
    PRI pri;
    get_pri(0, &pri);
    gg_printf("\n** task1 ** stacd=%d, tskpri=%d\n", stacd, pri);
    gg_printf("#pragma interruptによるFIT/CG割り込みルーチンの実験--\n");
    gg_printf("R_Config_TMR0_Start()\n");    R_Config_TMR0_Start();
    for (;;) {
        slp_tsk();
        GG_TP_ON(100);
        dummy(50);
        GG_TP_OFF(100);
    }
}

#include "Config_TMR1.h"
void task2_TMR1_callback(void)
{
    GG_TP_ON(201);
    wup_tsk(str2tid("task2"));
    GG_TP_OFF(201);
}
TASK task2(int stacd)       // タスク２(task2)
{
    PRI pri;
    get_pri(0, &pri);
    gg_printf("\n** task2 ** stacd=%d, tskpri=%d\n", stacd, pri);
    gg_printf("def_inh()によるNORTi標準割り込みルーチンの実験--\n");
    gg_printf("norti_TMR1_def_inh()\n");    norti_TMR1_def_inh();   // TMR1の割込み処理の登録(def_inh())
    gg_printf("R_Config_TMR1_Start()\n");   R_Config_TMR1_Start();
    for (;;) {
        slp_tsk();
        GG_TP_ON(200);
        dummy(50);
        GG_TP_OFF(200);
    }
}


#include "Config_TMR2.h"
void task3_TMR2_callback(void)
{
    GG_TP_ON(301);
    wup_tsk(str2tid("task3"));
    GG_TP_OFF(301);
}
void task3_TMR2_callback2(void)
{
    GG_TP_ON(302);
    wup_tsk(str2tid("task2"));
    GG_TP_OFF(302);
}
TASK task3(int stacd)       // タスク３(task3)
{
    PRI pri;
    get_pri(0, &pri);
    gg_printf("\n** task3 ** stacd=%d, tskpri=%d\n", stacd, pri);
    gg_printf("cre_isr()によるNORTi標準割り込みサービスルーチンの実験--\n");
    gg_printf("norti_TMR2_cre_isr()\n");    norti_TMR2_cre_isr();   // TMR2の割込みサービスルーチンの登録(cre_isr())
    gg_printf("R_Config_TMR2_Start()\n");   R_Config_TMR2_Start();
    for (;;) {
        slp_tsk();
        GG_TP_ON(300);
        dummy(50);
        GG_TP_OFF(300);
    }
}


#include "Config_TMR3.h"
static int task4_counter = 0;
void task4_TMR3_callback(void)  // NORTi非対応
{
    GG_TP_ON(401);
    task4_counter++;
    GG_TP_OFF(401);
}
TASK task4(int stacd)       // タスク４(task4)
{
    PRI pri;
    get_pri(0, &pri);
    gg_printf("\n** task4 ** stacd=%d, tskpri=%d\n", stacd, pri);
    gg_printf("非NORTi割り込みによるコールバックの実験--\n");
    gg_printf("nonOS_TMR3_def_inh()\n");    nonOS_TMR3_def_inh();   // TMR2の非NORTi割込みの登録(def_inh())
    gg_printf("R_Config_TMR3_Start()\n");   R_Config_TMR3_Start();
    for (;;) {
        if (task4_counter) {
            task4_counter--;
            GG_TP_ON(400);
            dummy(50);
            GG_TP_OFF(400);
        }
        GG_TP_ON(410);
        dly_tsk(10/MSEC);
        GG_TP_OFF(410);
        dummy(50);
    }
}

void main(void)
{
    GG_DI_ON();     // 割込み禁止

    sysini();       // NORTi初期化

    // CMT(FITモジュール) タイマ設定(10msec周期割込み)
    R_CMT_CreatePeriodic(100, cmt_10msec_callback, &cmt_10msec_ch);

    // タスク生成
    CRETSK(1, taskg, TMIN_TPRI, 1024, "taskg" );
    CRETSK(2, task1, TPRI_MAX ,  256, "task1" );
    CRETSK(3, task2, TPRI_MAX ,  256, "task2" );
    CRETSK(4, task3, TPRI_MAX ,  256, "task3" );
    CRETSK(5, task4, TPRI_MAX ,  256, "task4" );

    // GGコンソールタスク起動
    sta_tsk(1, 0);  // id:1=taskg

    syssta();       // NORTi開始
}
