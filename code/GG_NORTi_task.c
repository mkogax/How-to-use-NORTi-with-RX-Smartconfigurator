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


#include "GG.h"             // GGツールヘッダ
#include "kernel.h"
#include "nosys4.h"


//---------------------------------
//  taskコマンド
//---------------------------------

static const char * str_tskstat(int tts)
{
	switch(tts) {
	case TTS_RUN:	return "RUN ";
	case TTS_RDY:	return "RDY ";
	case TTS_WAI:	return "WAIT";
	case TTS_SUS:	return "SUS ";
	case TTS_WAS:	return "WSUS";
	case TTS_DMT:	return "DMT ";
	}
	return "????";
}
int str2tid(char *name)        // 文字列(タスク名またはID数値)からタスクID(1..)を取得(0=該当なし)
{
    int tid;
    for (tid=1; tid<=CFG.tskid_max; tid++) {
        T_RTSK rtsk;
        if (E_OK==ref_tsk(tid, &rtsk)) {      // タスク状態参照
            T_TCB *tcb = _pTCB[tid-1];
            if (!gg_strcmpx(name, tcb->ctsk->name)) return tid;
        }
    }
    tid = gg_asc2int(name);
    if (tid<1) return 0;
    if (tid>CFG.tskid_max) return 0;
    return tid;
}
static void C_task_help(void)
{
    gg_printf(  "(usage)\n"
                "task                       タスク一覧(状態表示)\n"
                "task act <tid>             act_tsk(tid)\n"
                "task cana <tid>            can_act(tid)\n"
                "task sta <tid> [<stacd>]   sta_tsk(tid,stacd)\n"
                "task ter <tid>             ter_tsk(tid)\n"
                "task chg <tid> <tskpri>    chg_pri(tid,tskpri)\n"
                "task ref <tid>             ref_tsk(tid,&rtsk)\n"
                "task sus <tid>             sus_tsk(tid)\n"
                "task rsm <tid>             rsm_tsk(tid)\n"
                "task frsm <tid>            frsm_tsk(tid)\n"
                "task wup <tid>             wup_tsk(tid)\n"
                "task canw <tid>            can_wup(tid)\n"
                "task rel <tid>             rel_wai(tid)\n"
                "(tidは数値でもタスク名でも可)\n"
    );
}
int C_task(int argc, char **argv)  // タスク状態の表示
{
    int tid;
    ER er;
    if (argc==1) {
        for (tid=1; tid<=CFG.tskid_max; tid++) {
            T_RTSK rtsk;
            if (E_OK==ref_tsk(tid, &rtsk)) {      // タスク状態参照
                T_TCB *tcb = _pTCB[tid-1];
                gg_printf("tid=%2d %-10s %s pri=%2d stack=%08lX(size=%5d)\n", tid, tcb->ctsk->name, str_tskstat(rtsk.tskstat), tcb->pri, tcb->ctsk->stk, tcb->ctsk->stksz);
            }
        }
    } else if (!gg_strcmpx("act", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = act_tsk(tid);
            gg_printf("act_tsk(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("cana", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = can_act(tid);
            gg_printf("can_act(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("sta", argv[1])) {
        if ((argc==3)||(argc==4)) {
            int stacd = 0;
            tid = str2tid(argv[2]);
            if (argc==4) stacd = gg_asc2int(argv[3]);
            er = sta_tsk(tid, (VP_INT)stacd);
            gg_printf("sta_tsk(%d,%d) = %d\n", tid, stacd, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("ter", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = ter_tsk(tid);
            gg_printf("ter_tsk(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("chg", argv[1])) {
        if (argc==4) {
            int tskpri = 0;
            tid = str2tid(argv[2]);
            tskpri = gg_asc2int(argv[3]);
            er = chg_pri(tid, tskpri);
            gg_printf("chg_pri(%d,%d) = %d\n", tid, tskpri, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("ref", argv[1])) {
        if (argc==3) {
            T_RTSK rtsk;
            tid = str2tid(argv[2]);
            gg_memset(&rtsk, 0, sizeof(rtsk));
            er = ref_tsk(tid, &rtsk);
            gg_printf("ref_tsk(%d,&rtsk) = %d\n", tid, er);
            gg_printf("  rtsk.tskstat = %d\n", rtsk.tskstat);      /* task state */
            gg_printf("  rtsk.tskpri = %d\n", rtsk.tskpri);        /* current priority */
            gg_printf("  rtsk.tskbpri = %d\n", rtsk.tskbpri);      /* base task priority */
            gg_printf("  rtsk.tskwait = %d\n", rtsk.tskwait);      /* cause of wait */
            gg_printf("  rtsk.wid = %d\n", rtsk.wid);              /* ID of object being waited for */
            gg_printf("  rtsk.lefttmo = %d\n", rtsk.lefttmo);      /* left time until timeout */
            gg_printf("  rtsk.actcnt = %d\n", rtsk.actcnt);        /* activate request count */
            gg_printf("  rtsk.wupcnt = %d\n", rtsk.wupcnt);        /* wakeup request count */
            gg_printf("  rtsk.suscnt = %d\n", rtsk.suscnt);        /* SUSPEND request count */
            gg_printf("  rtsk.exinf = %d\n", rtsk.exinf);          /* extended information */
            gg_printf("  rtsk.tskatr = %d\n", rtsk.tskatr);        /* task attributes */
            gg_printf("  rtsk.task = 0x%08lX\n", rtsk.task);       /* task start address */
            gg_printf("  rtsk.itskpri = %d\n", rtsk.itskpri);      /* initial task priority */
            gg_printf("  rtsk.stksz = %d\n", rtsk.stksz);          /* stack size */
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("sus", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = sus_tsk(tid);
            gg_printf("sus_tsk(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("rsm", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = rsm_tsk(tid);
            gg_printf("rsm_tsk(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("frsm", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = frsm_tsk(tid);
            gg_printf("frsm_tsk(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("wup", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = wup_tsk(tid);
            gg_printf("wup_tsk(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("canw", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = can_wup(tid);
            gg_printf("can_wup(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else if (!gg_strcmpx("rel", argv[1])) {
        if (argc==3) {
            tid = str2tid(argv[2]);
            er = rel_wai(tid);
            gg_printf("rel_wai(%d) = %d\n", tid, er);
        } else {
            C_task_help();
        }
    } else {
        C_task_help();
    }
    return 0;
}



