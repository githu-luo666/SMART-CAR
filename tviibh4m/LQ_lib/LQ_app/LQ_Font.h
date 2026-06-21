/*******************************************************************************
LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技CYT4BB7核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V0.1版权所有，单位使用请先联系授权
【最后更新】2025年2月24日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】IAR Systems 9.40.1版本
【Target 】 CYT4BB7(Dual Core M7, cFlash:4MB wFlash:256KB RAM:758KB  144Pins)
【Crystal】 16.000Mhz
【SYS PLL】 100MHz + 250MHz + 250MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
*******************************************************************************/
#ifndef __LQ_Font_H__
#define __LQ_Font_H__
/** 字库
字库码的引索
--------------------------------------------------------------
(0)  !(1)  "(2)  #(3)  $(4)  %(5)  &(6)  '(7)
((8)  )(9)  *(10) +(11) ,(12) -(13) .(14) /(15)
0(16) 1(17) 2(18) 3(19) 4(20) 5(21) 6(22) 7(23)
8(24) 9(25) :(26) ;(27) <(28) =(29) >(30) ?(31)
@(32) A(33) B(34) C(35) D(36) E(37) F(38) G(39)
H(40) I(41) J(42) K(43) L(44) M(45) N(46) O(47)
P(48) Q(49) R(50) S(51) T(52) U(53) V(54) W(55)
X(56) Y(57) Z(58) [(59) \(60) ](61) ^(62) _(63)
`(64) a(65) b(66) c(67) d(68) e(69) f(70) g(71)
h(72) i(73) j(74) k(75) l(76) m(77) n(78) o(79)
p(80) q(81) r(82) s(83) t(84) u(85) v(86) w(87)
x(88) y(89) z(90) {(91) |(92) }(93) ~(94)
--------------------------------------------------------------
字体:   8*6字体
取码规则:   低位在前, 列行扫描, 阴码(1-亮, 0-灭)
查表方法: 要显示的ASCII码-32就可以得到字库码的指针 */
extern const unsigned char Font_code8[][6];

/** 字库
字库码的引索
--------------------------------------------------------------
(0)  !(1)  "(2)  #(3)  $(4)  %(5)  &(6)  '(7)
((8)  )(9)  *(10) +(11) ,(12) -(13) .(14) /(15)
0(16) 1(17) 2(18) 3(19) 4(20) 5(21) 6(22) 7(23)
8(24) 9(25) :(26) ;(27) <(28) =(29) >(30) ?(31)
@(32) A(33) B(34) C(35) D(36) E(37) F(38) G(39)
H(40) I(41) J(42) K(43) L(44) M(45) N(46) O(47)
P(48) Q(49) R(50) S(51) T(52) U(53) V(54) W(55)
X(56) Y(57) Z(58) [(59) \(60) ](61) ^(62) _(63)
`(64) a(65) b(66) c(67) d(68) e(69) f(70) g(71)
h(72) i(73) j(74) k(75) l(76) m(77) n(78) o(79)
p(80) q(81) r(82) s(83) t(84) u(85) v(86) w(87)
x(88) y(89) z(90) {(91) |(92) }(93) ~(94)
--------------------------------------------------------------
字体:   GulimChe12 8*16字体
取码规则:   低位在前, 列行扫描, 阴码(1-亮, 0-灭)
查表方法: 要显示的ASCII码-32就可以得到字库码的指针 */
extern const unsigned char Font_code16[][16];



/** 16*16 简单的汉字字库*/
extern const unsigned char hanzi16x16[];
extern const unsigned char gImage_1[3200];
extern const unsigned char gImage_title2[5152];
extern const unsigned char gImage_Battery_S[400];
extern const unsigned char gImage_lo[17280];
extern const unsigned char Font_code16[][16];
extern const unsigned char Font_code8[][6];
extern const unsigned char hanzi_Idx[];
extern const unsigned char hanzi16x16[];
extern const unsigned char ascii_1206[][12];
extern const unsigned char ascii_1608[][16];
extern const unsigned char ascii_2412[][48];
extern const unsigned char ascii_3216[][64];
extern const unsigned char tfont_Idx[];
extern const unsigned char tfont_12x12[];
extern const unsigned char tfont_16x16[];
extern const unsigned char tfont_24x24[];
extern const unsigned char tfont_32x32[];
#endif
