# NORTi を RX Smartconfigurator と一緒に使う方法

<br />

かふぇルネから飛んでこられた方。こんにちは、ふぐりんです。  
このドキュメントは「NORTi を RX Smartconfigurator と一緒に使う方法」の解説です。  

**NoMaYさん考案のプログラムによって、NORTiとスマートコンフィグレータを一緒に使うことができるようになりました。スマートコンフィグレータ(FIT/CG)の自動生成コードは何も改変せずに、ユーザコードエリアやコールバックからNORTiの機能を使えます。またNORTi方式の割り込みもそのまま併用できます。**

経緯については以下のかふぇルネのスレッドをご覧ください。  

* [スマートコンフィグレータの自動生成をカスタマイズしたい](https://community-ja.renesas.com/cafe_rene/f/forum5/8248/thread)
* [MISPOさんのNORTiをRX SmartConfiguratorと一緒に使いたい場合の課題と対策を考察してみるスレッド](https://community-ja.renesas.com/cafe_rene/f/forum21/8266/mispo-norti-rx-smartconfigurator)

<sub>【謝辞】  
NoMaYさんが考案したプログラムを中心に解説しています。NoMaYさんにあらためてお礼申し上げます。  
株式会社ミスポ様には関連コードの公開を許諾いただきました。ありがとうございました。お礼申し上げます。  
ka.makiharaさんにはNoMaYさんのプログラム作成に協力いただき、また実証実験もしていただきました。ありがとうございました。お礼申し上げます。  
【お願い】  
ミスポ様にご協力いただいた情報を参考にしていますが、NoMaYさんのプログラムおよび今回の解説にミスポ様は直接関係されていません。NoMaYさんのプログラムおよび今回の解説について直接ミスポ様へ問い合わせすることはおやめください。  
【ご利用について】  
ここで解説されている情報は商用/非商用にかかわらずご利用いただけますが、何も保証しません。
</sub>

<br /><br /><br />

## 動作の解説
[**動作の解説**](README_exp.md) <sub><--詳細はクリック</sub>

・これまで（NoMaYさん考案前）  
・これから（NoMaYさん考案のフックルーチンがある）  
・割り込みレベル自動判定【2023.05.05 追記】  

<br /><br /><br />

## セットアップ

1. RXシリーズの開発環境(CS+,CC-RX)でプロジェクトを作ります
2. プロジェクトにNoMaYさんの下記プログラムを追加します  
<sub>動作の解説の「割り込みレベル自動判定 有/無 のどちらを使うべきか」参照</sub>  
* [NORTi_interrupt.c](code/NORTi_interrupt.c) または [NORTi_interrupt_safe.c](code/NORTi_interrupt_safe.c)
* [NORTi_interrupt.h](code/NORTi_interrupt.h)

3. NORTiを組み込みます
4. NORTiのintini()の "initialize interrupt vector table" 部分を以下と入れ替えます
```c
    /* initialize interrupt vector table (on RAM) */
#if (INTVSZ != 0)                               // NORTi管理の割り込みベクタテーブルがRAMであれば普通は1024
    extern void NORTi_INT_HOOK_init(void);      // Int_Hook_Vectors[]をINTBレジスタにセット(NoMaYさん作成)
    NORTi_INT_HOOK_init();                      // 
    {
        int i;
        UW *vect = (UW *)__sectop("C$VECT");    // C$VECTセクション(CC-RXオリジナルの割り込みベクタテーブル)
        UW *hook = (UW *)get_intb();            // NORTi対応割り込みベクタテーブル(NoMaYさん作成,Int_Hook_Vectors[])
        for (i = 0; i < INTVSZ / 4; i++) {      // 割り込みベクタテーブル(NORTi管理,RAM)全体にコピー
            intvec[i] = (i<16)?vect[i]:hook[i]; // 0～15は無条件トラップ専用なのでC$VECT、それ以外はInt_Hook_Vectors[]
        }
    }
#endif
```
5. NORTi_interrupt_safe.cを使う場合は、プログラムのどこかに以下を定義しておきます。
```c
uint8_t NORTi_INT_HOOK_KNL_LEVEL = KNL_LEVEL;
```


上記でセットアップは完了です。  

<br /><br /><br />

## 効果

### 1. FIT/CGは改変なしでそのままNORTiを利用できます。

ユーザコードエリアやコールバック内でNORTiを利用できます。

### 2. NORTiの割り込みハンドラ(def_inh())をそのまま利用できます。

### 3. NORTiの割り込みサービスルーチン(cre_isr())をそのまま利用できます。（最初におまじない必要）

割り込みサービスルーチン(cre_isr())を使う前に、1回だけ def_inh(割り込み番号, NULL) をしてください。（NULLを指定することで、割り込みベクタにNORTiの管理用ルーチンが上書きされます）

### 4. FIT/CGの割り込み処理はNORTi対応で少しオーバヘッドが増えますが、NORTiを使わないで高速に処理したい場合(nonOS)はdef_inh()で対応できます。(割り込みレベル注意)

<br /><br /><br />

## 【 注意 】

### 【注意】INT/BRK(ベクタ番号0～15)と例外ベクタテーブルの割り込みはNORTi非対応(nonOS)にしています。(NORTi_interrupt.h)

### 【注意２】BUSERR(ベクタ番号16)の割り込みはNORTi非対応(nonOS)しています。(NORTi_interrupt.h)

### 【注意３】NORTiのAPIを呼び出す予定の割込みは、割り込みレベルがNORTiのKNL_LEVELをこえないようにしてください。
<sub>動作の解説の「割り込みレベル自動判定【2023.05.05 追記】」参照</sub>

<br /><br /><br />

## デモ(Target Board for RX130,CS+)
[**デモ**](README_demo.md) <sub><--詳細はクリック</sub>

・使用ツールと実験プログラムについて  
・実験１．r_cmt_rx(FIT)の10msec周期コールバックからNORTiのチックタイム通知(isig_tim())  
・実験２．TMR0(CG)で#pragma interrupt の割り込みルーチンからタスク起床(wup_tsk())  
・実験３．TMR1(CG)の改造でdef_inh() のNORTi割り込みハンドラからタスク起床(wup_tsk())  
・実験４．TMR2(CG)の改造でcre_isr() のNORTi割り込みサービスルーチンからタスク起床(wup_tsk())  
・実験５．TMR3(CG)で#pragma interrupt の割り込みルーチンを非NORTiとして再登録(def_inh())してカウンタ変数更新を確認  
・おまけ１（スタックがぎりぎりやった）  
・おまけ２（エミュレータ経由のデバッグコンソール出力はけっこう遅い）  

<br /><br /><br />

## 応答時間(Target Board for RX65N,CS+)

ソフトウェア割り込み発生から実際の割り込み処理に到達するまでの時間を観測しました。  

|処理ケース|実測値|API|
|---|---|---|
|① CCRX 割り込みルーチン(#pragma interrupt)  | 0.66usec|×|
|② NORTi CCRX割り込み対応(ient_int())  | 1.25usec|〇|
|③ NORTi 割り込みハンドラ(def_inh())  | 1.12usec|〇|
|④ NORTi 割り込みサービスルーチン(cre_isr())  | 1.22usec|〇|
|⑤ NoMaYさんフックルーチン 割り込みレベル自動判定なし (NORTi_interrupt.c)  | 1.40usec|〇|
|⑥ NoMaYさんフックルーチン 割り込みレベル自動判定あり (NORTi_interrupt_safe.c)  | 1.50usec(≦KNL_LEVEL)|〇|
||1.04usec(> KNL_LEVEL)|×|

<sub>API：NORTiのAPI呼出の可否</sub>  

【条件】  
・Target Board for RX65N,CS+  
・システムクロック(ICLK)=120MHz  

<br />

【処理ケースの概要】 

#### ① CCRX 割り込みルーチン(#pragma interrupt)

<sub>#pragma interrupt を使ったCCRXの割込みルーチン。FITでも使用</sub>  
割り込み発生＋「CCRX 入口処理」--> 割り込み処理

#### ② NORTi CCRX割り込み対応(ient_int())

<sub>MiSPOさん提供のient_int()/iret_int()を使用して、#pragma interrupt を使ったCCRXの割込みルーチンをNORTi対応にした場合</sub>  
割り込み発生＋「CCRX 入口処理」＋（左記スタック破棄）＋「NORTi 入口処理」--> 割り込み処理  

#### ③ NORTi 割り込みハンドラ(def_inh())

<sub>NORTiの割込みハンドラの場合。NoMaYさんフックルーチン使用時も利用可能</sub>  
割り込み発生＋「NORTi 入口処理」--> 割り込み処理

#### ④ NORTi 割り込みサービスルーチン(cre_isr())

<sub>NORTiの割込みサービスルーチンの場合。NoMaYさんフックルーチン使用時も利用可能(おまじない必要)</sub>  
割り込み発生＋「NORTi 入口処理」＋isrサービス管理 --> 割り込み処理

#### ⑤ NoMaYさんフックルーチン 割り込みレベル自動判定なし (NORTi_interrupt.c)

<sub>NoMaYさんフックルーチン(NORTi_interrupt.c)を経由した#pragma interrupt を使ったCCRXの割込みルーチンの場合</sub>  
割り込み発生＋「NORTi 入口処理」＋「CCRX 入口処理」 --> 割り込み処理

#### ⑥ NoMaYさんフックルーチン 割り込みレベル自動判定あり (NORTi_interrupt_safe.c)

<sub>NoMaYさんフックルーチン(NORTi_interrupt_safe.c)を経由した#pragma interrupt を使ったCCRXの割込みルーチンの場合</sub>  
<sub>(≦KNL_LEVEL)であれば</sub>  
割り込み発生＋「割り込みレベル判定」＋「NORTi 入口処理」＋「CCRX 入口処理」 --> 割り込み処理

<sub>(> KNL_LEVEL)であれば</sub>  
割り込み発生＋「割り込みレベル判定」＋「CCRX 入口処理」 --> 割り込み処理


<br /><br /><br />


