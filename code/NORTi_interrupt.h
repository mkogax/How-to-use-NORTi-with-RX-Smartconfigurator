/*
 * NORTi_interrupt.h
 *
 *  Created on: Jun 24, 2022
 *      Author: NoMaY
 *     License: MIT
 */

#ifndef NORTI_INTERRUPT_H_
#define NORTI_INTERRUPT_H_

// 0..15	unconditional trap (INT,BRK)

#define INTERRUPT___0 nonOS
#define INTERRUPT___1 nonOS
#define INTERRUPT___2 nonOS
#define INTERRUPT___3 nonOS
#define INTERRUPT___4 nonOS
#define INTERRUPT___5 nonOS
#define INTERRUPT___6 nonOS
#define INTERRUPT___7 nonOS
#define INTERRUPT___8 nonOS
#define INTERRUPT___9 nonOS
#define INTERRUPT__10 nonOS
#define INTERRUPT__11 nonOS
#define INTERRUPT__12 nonOS
#define INTERRUPT__13 nonOS
#define INTERRUPT__14 nonOS
#define INTERRUPT__15 nonOS
#define INTERRUPT__16 nonOS     // BUSERR   defaultでレベル15なので NORTiの V3_ent_intをコールできない(重複割込みとなり暴走する) 2022.12.23 M.Kogan
#define INTERRUPT__17 NORTi
#define INTERRUPT__18 NORTi
#define INTERRUPT__19 NORTi
#define INTERRUPT__20 NORTi
#define INTERRUPT__21 NORTi
#define INTERRUPT__22 NORTi
#define INTERRUPT__23 NORTi
#define INTERRUPT__24 NORTi
#define INTERRUPT__25 NORTi
#define INTERRUPT__26 NORTi
#define INTERRUPT__27 NORTi
#define INTERRUPT__28 NORTi
#define INTERRUPT__29 NORTi
#define INTERRUPT__30 NORTi
#define INTERRUPT__31 NORTi
#define INTERRUPT__32 NORTi
#define INTERRUPT__33 NORTi
#define INTERRUPT__34 NORTi
#define INTERRUPT__35 NORTi
#define INTERRUPT__36 NORTi
#define INTERRUPT__37 NORTi
#define INTERRUPT__38 NORTi
#define INTERRUPT__39 NORTi
#define INTERRUPT__40 NORTi
#define INTERRUPT__41 NORTi
#define INTERRUPT__42 NORTi
#define INTERRUPT__43 NORTi
#define INTERRUPT__44 NORTi
#define INTERRUPT__45 NORTi
#define INTERRUPT__46 NORTi
#define INTERRUPT__47 NORTi
#define INTERRUPT__48 NORTi
#define INTERRUPT__49 NORTi
#define INTERRUPT__50 NORTi
#define INTERRUPT__51 NORTi
#define INTERRUPT__52 NORTi
#define INTERRUPT__53 NORTi
#define INTERRUPT__54 NORTi
#define INTERRUPT__55 NORTi
#define INTERRUPT__56 NORTi
#define INTERRUPT__57 NORTi
#define INTERRUPT__58 NORTi
#define INTERRUPT__59 NORTi
#define INTERRUPT__60 NORTi
#define INTERRUPT__61 NORTi
#define INTERRUPT__62 NORTi
#define INTERRUPT__63 NORTi
#define INTERRUPT__64 NORTi
#define INTERRUPT__65 NORTi
#define INTERRUPT__66 NORTi
#define INTERRUPT__67 NORTi
#define INTERRUPT__68 NORTi
#define INTERRUPT__69 NORTi
#define INTERRUPT__70 NORTi
#define INTERRUPT__71 NORTi
#define INTERRUPT__72 NORTi
#define INTERRUPT__73 NORTi
#define INTERRUPT__74 NORTi
#define INTERRUPT__75 NORTi
#define INTERRUPT__76 NORTi
#define INTERRUPT__77 NORTi
#define INTERRUPT__78 NORTi
#define INTERRUPT__79 NORTi
#define INTERRUPT__80 NORTi
#define INTERRUPT__81 NORTi
#define INTERRUPT__82 NORTi
#define INTERRUPT__83 NORTi
#define INTERRUPT__84 NORTi
#define INTERRUPT__85 NORTi
#define INTERRUPT__86 NORTi
#define INTERRUPT__87 NORTi
#define INTERRUPT__88 NORTi
#define INTERRUPT__89 NORTi
#define INTERRUPT__90 NORTi
#define INTERRUPT__91 NORTi
#define INTERRUPT__92 NORTi
#define INTERRUPT__93 NORTi
#define INTERRUPT__94 NORTi
#define INTERRUPT__95 NORTi
#define INTERRUPT__96 NORTi
#define INTERRUPT__97 NORTi
#define INTERRUPT__98 NORTi
#define INTERRUPT__99 NORTi
#define INTERRUPT_100 NORTi
#define INTERRUPT_101 NORTi
#define INTERRUPT_102 NORTi
#define INTERRUPT_103 NORTi
#define INTERRUPT_104 NORTi
#define INTERRUPT_105 NORTi
#define INTERRUPT_106 NORTi
#define INTERRUPT_107 NORTi
#define INTERRUPT_108 NORTi
#define INTERRUPT_109 NORTi
#define INTERRUPT_110 NORTi
#define INTERRUPT_111 NORTi
#define INTERRUPT_112 NORTi
#define INTERRUPT_113 NORTi
#define INTERRUPT_114 NORTi
#define INTERRUPT_115 NORTi
#define INTERRUPT_116 NORTi
#define INTERRUPT_117 NORTi
#define INTERRUPT_118 NORTi
#define INTERRUPT_119 NORTi
#define INTERRUPT_120 NORTi
#define INTERRUPT_121 NORTi
#define INTERRUPT_122 NORTi
#define INTERRUPT_123 NORTi
#define INTERRUPT_124 NORTi
#define INTERRUPT_125 NORTi
#define INTERRUPT_126 NORTi
#define INTERRUPT_127 NORTi
#define INTERRUPT_128 NORTi
#define INTERRUPT_129 NORTi
#define INTERRUPT_130 NORTi
#define INTERRUPT_131 NORTi
#define INTERRUPT_132 NORTi
#define INTERRUPT_133 NORTi
#define INTERRUPT_134 NORTi
#define INTERRUPT_135 NORTi
#define INTERRUPT_136 NORTi
#define INTERRUPT_137 NORTi
#define INTERRUPT_138 NORTi
#define INTERRUPT_139 NORTi
#define INTERRUPT_140 NORTi
#define INTERRUPT_141 NORTi
#define INTERRUPT_142 NORTi
#define INTERRUPT_143 NORTi
#define INTERRUPT_144 NORTi
#define INTERRUPT_145 NORTi
#define INTERRUPT_146 NORTi
#define INTERRUPT_147 NORTi
#define INTERRUPT_148 NORTi
#define INTERRUPT_149 NORTi
#define INTERRUPT_150 NORTi
#define INTERRUPT_151 NORTi
#define INTERRUPT_152 NORTi
#define INTERRUPT_153 NORTi
#define INTERRUPT_154 NORTi
#define INTERRUPT_155 NORTi
#define INTERRUPT_156 NORTi
#define INTERRUPT_157 NORTi
#define INTERRUPT_158 NORTi
#define INTERRUPT_159 NORTi
#define INTERRUPT_160 NORTi
#define INTERRUPT_161 NORTi
#define INTERRUPT_162 NORTi
#define INTERRUPT_163 NORTi
#define INTERRUPT_164 NORTi
#define INTERRUPT_165 NORTi
#define INTERRUPT_166 NORTi
#define INTERRUPT_167 NORTi
#define INTERRUPT_168 NORTi
#define INTERRUPT_169 NORTi
#define INTERRUPT_170 NORTi
#define INTERRUPT_171 NORTi
#define INTERRUPT_172 NORTi
#define INTERRUPT_173 NORTi
#define INTERRUPT_174 NORTi
#define INTERRUPT_175 NORTi
#define INTERRUPT_176 NORTi
#define INTERRUPT_177 NORTi
#define INTERRUPT_178 NORTi
#define INTERRUPT_179 NORTi
#define INTERRUPT_180 NORTi
#define INTERRUPT_181 NORTi
#define INTERRUPT_182 NORTi
#define INTERRUPT_183 NORTi
#define INTERRUPT_184 NORTi
#define INTERRUPT_185 NORTi
#define INTERRUPT_186 NORTi
#define INTERRUPT_187 NORTi
#define INTERRUPT_188 NORTi
#define INTERRUPT_189 NORTi
#define INTERRUPT_190 NORTi
#define INTERRUPT_191 NORTi
#define INTERRUPT_192 NORTi
#define INTERRUPT_193 NORTi
#define INTERRUPT_194 NORTi
#define INTERRUPT_195 NORTi
#define INTERRUPT_196 NORTi
#define INTERRUPT_197 NORTi
#define INTERRUPT_198 NORTi
#define INTERRUPT_199 NORTi
#define INTERRUPT_200 NORTi
#define INTERRUPT_201 NORTi
#define INTERRUPT_202 NORTi
#define INTERRUPT_203 NORTi
#define INTERRUPT_204 NORTi
#define INTERRUPT_205 NORTi
#define INTERRUPT_206 NORTi
#define INTERRUPT_207 NORTi
#define INTERRUPT_208 NORTi
#define INTERRUPT_209 NORTi
#define INTERRUPT_210 NORTi
#define INTERRUPT_211 NORTi
#define INTERRUPT_212 NORTi
#define INTERRUPT_213 NORTi
#define INTERRUPT_214 NORTi
#define INTERRUPT_215 NORTi
#define INTERRUPT_216 NORTi
#define INTERRUPT_217 NORTi
#define INTERRUPT_218 NORTi
#define INTERRUPT_219 NORTi
#define INTERRUPT_220 NORTi
#define INTERRUPT_221 NORTi
#define INTERRUPT_222 NORTi
#define INTERRUPT_223 NORTi
#define INTERRUPT_224 NORTi
#define INTERRUPT_225 NORTi
#define INTERRUPT_226 NORTi
#define INTERRUPT_227 NORTi
#define INTERRUPT_228 NORTi
#define INTERRUPT_229 NORTi
#define INTERRUPT_230 NORTi
#define INTERRUPT_231 NORTi
#define INTERRUPT_232 NORTi
#define INTERRUPT_233 NORTi
#define INTERRUPT_234 NORTi
#define INTERRUPT_235 NORTi
#define INTERRUPT_236 NORTi
#define INTERRUPT_237 NORTi
#define INTERRUPT_238 NORTi
#define INTERRUPT_239 NORTi
#define INTERRUPT_240 NORTi
#define INTERRUPT_241 NORTi
#define INTERRUPT_242 NORTi
#define INTERRUPT_243 NORTi
#define INTERRUPT_244 NORTi
#define INTERRUPT_245 NORTi
#define INTERRUPT_246 NORTi
#define INTERRUPT_247 NORTi
#define INTERRUPT_248 NORTi
#define INTERRUPT_249 NORTi
#define INTERRUPT_250 NORTi
#define INTERRUPT_251 NORTi
#define INTERRUPT_252 NORTi
#define INTERRUPT_253 NORTi
#define INTERRUPT_254 NORTi
#define INTERRUPT_255 NORTi

#endif /* NORTI_INTERRUPT_H_ */
