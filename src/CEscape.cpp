#include <CEscape.h>
#include <CFile.h>
#include <CDir.h>
#include <COSRead.h>
#include <CStrUtil.h>
#include <cassert>

namespace {
  int s_readResultSecs  = 1;
  int s_readResultMSecs = 0;

  bool  s_logResult = false;
  FILE* s_logFile   = nullptr;
}

// Escape Codes as String
#define CTRL_A_s ""
#define CTRL_B_s ""
#define CTRL_C_s ""
#define CTRL_D_s ""
#define CTRL_E_s ""
#define CTRL_F_s ""
#define CTRL_G_s ""
#define CTRL_H_s ""
#define CTRL_I_s "\011" // ^I
#define CTRL_J_s "\012" // ^J
#define CTRL_K_s ""
#define CTRL_L_s ""
#define CTRL_M_s "\015" // ^M
#define CTRL_N_s ""
#define CTRL_O_s ""
#define CTRL_P_s ""
#define CTRL_Q_s "\021" // ^Q
#define CTRL_R_s ""
#define CTRL_S_s "\023" // ^S
#define CTRL_T_s ""
#define CTRL_U_s ""
#define CTRL_V_s ""
#define CTRL_W_s ""
#define CTRL_X_s ""
#define CTRL_Y_s ""
#define CTRL_Z_s ""

#define ESC_s ""
#define OSC_s "]"
#define ST_s  "\\"

std::string
CEscape::
NUL()
{
  return "\000";
}

std::string
CEscape::
SOH()
{
  return CTRL_A_s;
}

std::string
CEscape::
STX()
{
  return CTRL_B_s;
}

std::string
CEscape::
ETX()
{
  return CTRL_C_s;
}

std::string
CEscape::
EOT()
{
  return CTRL_D_s;
}

std::string
CEscape::
ENQ()
{
  return CTRL_E_s;
}

std::string
CEscape::
ACK()
{
  return CTRL_F_s;
}

std::string
CEscape::
BEL()
{
  return CTRL_G_s;
}

std::string
CEscape::
BS()
{
  return CTRL_H_s;
}

std::string
CEscape::
HT()
{
  return CTRL_I_s;
}

std::string
CEscape::
LF()
{
  return CTRL_J_s;
}

std::string
CEscape::
VT()
{
  return CTRL_K_s;
}

std::string
CEscape::
FF()
{
  return CTRL_L_s;
}

std::string
CEscape::
CR()
{
  return CTRL_M_s;
}

std::string
CEscape::
SO()
{
  return CTRL_N_s;
}

std::string
CEscape::
SI()
{
  return CTRL_O_s;
}

std::string
CEscape::
DLE()
{
  return CTRL_P_s;
}

std::string
CEscape::
DC1()
{
  return CTRL_Q_s;
}

std::string
CEscape::
DC2()
{
  return CTRL_R_s;
}

std::string
CEscape::
DC3()
{
  return CTRL_S_s;
}

std::string
CEscape::
DC4()
{
  return CTRL_T_s;
}

std::string
CEscape::
NAK()
{
  return CTRL_U_s;
}

std::string
CEscape::
SYN()
{
  return CTRL_V_s;
}

std::string
CEscape::
ETB()
{
  return CTRL_W_s;
}

std::string
CEscape::
CAN()
{
  return CTRL_X_s;
}

std::string
CEscape::
EM()
{
  return CTRL_Y_s;
}

std::string
CEscape::
SUB()
{
  return CTRL_Z_s;
}

std::string
CEscape::
ESC()
{
  return "";
}

std::string
CEscape::
FS()
{
  return "";
}

std::string
CEscape::
GS()
{
  return "";
}

std::string
CEscape::
RS()
{
  return "";
}

std::string
CEscape::
US()
{
  return "";
}

std::string
CEscape::
DEL()
{
  static char c[2] = { 0x7f, 0x00 };

  return &c[0];
}

std::string
CEscape::
SP()
{
  return " ";
}

std::string
CEscape::
IND()
{
  return ESC_s "D";
}

std::string
CEscape::
NEL()
{
  return ESC_s "E";
}

std::string
CEscape::
HTS()
{
  return ESC_s "H";
}

std::string
CEscape::
RI()
{
  return ESC_s "M";
}

std::string
CEscape::
SS2()
{
  return ESC_s "N";
}

std::string
CEscape::
SS3()
{
  return ESC_s "O";
}

std::string
CEscape::
DCS()
{
  return ESC_s "P";
}

std::string
CEscape::
SPA()
{
  return ESC_s "V";
}

std::string
CEscape::
EPA()
{
  return ESC_s "W";
}

std::string
CEscape::
SOS()
{
  return ESC_s "X";
}

std::string
CEscape::
DECID()
{
  return ESC_s "Z";
}

std::string
CEscape::
G0(const std::string &s)
{
  return ESC_s "(" + s;
}

std::string
CEscape::
G1(const std::string &s)
{
  return ESC_s ")" + s;
}

std::string
CEscape::
G2(const std::string &s)
{
  return ESC_s "*" + s;
}

std::string
CEscape::
G3(const std::string &s)
{
  return ESC_s "+" + s;
}

//----------------

// CSI 1t : de-iconify
std::string
CEscape::
windowOpDeiconify()
{
  return windowOp(WindowOp::DEICONIFY);
}

// CSI 2t : iconify
std::string
CEscape::
windowOpIconify()
{
  return windowOp(WindowOp::ICONIFY);
}

// CSI 3;<x>;<y>t : move to x,y
std::string
CEscape::
windowOpMove(int x, int y)
{
  return windowOp(WindowOp::MOVE, CStrUtil::toString(x), CStrUtil::toString(y));
}

// CSI 4;<h>;<w>t : resize to h,w pixels
std::string
CEscape::
windowOpPixelSize(int h, int w)
{
  return windowOp(WindowOp::PIXEL_SIZE, CStrUtil::toString(h), CStrUtil::toString(w));
}

// CSI 5t : raise
std::string
CEscape::
windowOpRaise()
{
  return windowOp(WindowOp::RAISE);
}

// CSI 6t : lower
std::string
CEscape::
windowOpLower()
{
  return windowOp(WindowOp::LOWER);
}

// CSI 7t : refresh
std::string
CEscape::
windowOpRefresh()
{
  return windowOp(WindowOp::REFRESH);
}

// CSI 8;<h>;<w>t : resize to h,w chars
std::string
CEscape::
windowOpCharSize(int h, int w)
{
  return windowOp(WindowOp::CHAR_SIZE, CStrUtil::toString(h), CStrUtil::toString(w));
}

// CSI 9;0t : restore maximized window
std::string
CEscape::
windowOpRestoreMaximized()
{
  return windowOp(WindowOp::MAXIMIZE, "0");
}

// CSI 9;1t : maximize window
std::string
CEscape::
windowOpMaximize()
{
  return windowOp(WindowOp::MAXIMIZE, "1");
}

// CSI 11t : report state as CSI 1 or CSI 2
std::string
CEscape::
windowOpReportState()
{
  return windowOp(WindowOp::REPORT_STATE);
}

// CSI 13t : report pos as CSI 3;<x>;<y>t
std::string
CEscape::
windowOpReportPos()
{
  return windowOp(WindowOp::REPORT_POS);
}

// CSI 14t : report size as CSI 4;<h>;<w>t
std::string
CEscape::
windowOpReportPixelSize()
{
  return windowOp(WindowOp::REPORT_PIXEL_SIZE);
}

// CSI 18t : report size as CSI 8;<h>;<w>t
std::string
CEscape::
windowOpReportCharSize()
{
  return windowOp(WindowOp::REPORT_CHAR_SIZE);
}

// CSI 19t : report screen size as CSI 9;<h>;<w>t
std::string
CEscape::
windowOpReportScreenSize()
{
  return windowOp(WindowOp::REPORT_SCREEN_SIZE);
}

// CSI 20t : report icon label as OSC L label ST
std::string
CEscape::
windowOpReportIconLabel()
{
  return windowOp(WindowOp::REPORT_ICON_STR);
}

// CSI 21t : report window title as OSC l label ST
std::string
CEscape::
windowOpReportWindowTitle()
{
  return windowOp(WindowOp::REPORT_TITLE_STR);
}

// CSI 24 ... : resize to 24 or greater lines
std::string
CEscape::
windowOpResizeNLines(int n)
{
  return windowOp((WindowOp) n);
}

// CSI <op>;<arg1>;<arg2>t : window_op <op>
std::string
CEscape::
windowOp(WindowOp op, const std::string &arg1, const std::string &arg2)
{
  std::string str = CStrUtil::toString(int(op));

  if (! arg1.empty()) {
    str += ";" + arg1;

    if (! arg2.empty())
      str += ";" + arg2;
  }

  str += "t";

  return CSI(str);
}

bool
CEscape::
decodeWindowOp(const std::string &str, WindowOp &op, std::string &arg1, std::string &arg2)
{
  int len = str.size();

  if (len < 3) return false;

  if (str[0] != '\033' || str[1] != '[' || str[len - 1] != 't')
    return false;

  int i = 2;

  std::vector<std::string> args;

  while (i < len && str[i] != 't') {
    std::string arg;

    while (i < len && str[i] != ';' && str[i] != 't')
      arg += str[i++];

    if (i < len && str[i] == ';')
      ++i;

    args.push_back(arg);
  }

  uint num_args = args.size();

  if (num_args < 1)
    return false;

  int op_num;

  if (! CStrUtil::toInteger(args[0], &op_num))
    return false;

  op = (WindowOp) op_num;

  if (i != len - 1)
    return false;

  arg1 = (num_args >= 2 ? args[1] : "");
  arg2 = (num_args >= 3 ? args[2] : "");

  return true;
}

std::string
CEscape::
osc()
{
  return OSC_s;
}

std::string
CEscape::
oscIconWindowTitle(const std::string &str)
{
  return OSC_s "0;" + str + ST_s;
}

std::string
CEscape::
oscIconTitle(const std::string &str)
{
  return OSC_s "1;" + str + ST_s;
}

std::string
CEscape::
oscWindowTitle(const std::string &str)
{
  return OSC_s "2;" + str + ST_s;
}

std::string
CEscape::
oscWindowProp(const std::string &str)
{
  return OSC_s "3;" + str + ST_s;
}

std::string
CEscape::
oscColor(int num, const std::string &str)
{
  return OSC_s "4;" + CStrUtil::toString(num) + ";" + str + ST_s;
}

std::string
CEscape::
oscFg(const std::string &str)
{
  return OSC_s "10;" + str + ST_s;
}

std::string
CEscape::
oscBg(const std::string &str)
{
  return OSC_s "11;" + str + ST_s;
}

std::string
CEscape::
oscCursorColor(const std::string &str)
{
  return OSC_s "12;" + str + ST_s;
}

std::string
CEscape::
oscFont(const std::string &str)
{
  return OSC_s "50;" + str + ST_s;
}

std::string
CEscape::
st()
{
  return ST_s;
}

//-----------

std::string
CEscape::
DECSC()
{
  return ESC_s "7";
}

std::string
CEscape::
DECRC()
{
  return ESC_s "8";
}

std::string
CEscape::
DECPAM()
{
  return ESC_s "=";
}

std::string
CEscape::
DECPNM()
{
  return ESC_s ">";
}

std::string
CEscape::
RIS()
{
  return ESC_s "c";
}

std::string
CEscape::
DECALN()
{
  return ESC_s "#8";
}

std::string
CEscape::
ICH(int n)
{
  if (n < 0)
    return CSI("@");
  else
    return CSI(CStrUtil::toString(n) + "@");
}

std::string
CEscape::
CUU(int n)
{
  if (n < 0)
    return CSI("A");
  else
    return CSI(CStrUtil::toString(n) + "A");
}

std::string
CEscape::
CUD(int n)
{
  if (n < 0)
    return CSI("B");
  else
    return CSI(CStrUtil::toString(n) + "B");
}

std::string
CEscape::
CUF(int n)
{
  if (n < 0)
    return CSI("C");
  else
    return CSI(CStrUtil::toString(n) + "C");
}

std::string
CEscape::
CUB(int n)
{
  if (n < 0)
    return CSI("D");
  else
    return CSI(CStrUtil::toString(n) + "D");
}

std::string
CEscape::
CNL(int n)
{
  if (n < 0)
    return CSI("E");
  else
    return CSI(CStrUtil::toString(n) + "E");
}

std::string
CEscape::
CPL(int n)
{
  if (n < 0)
    return CSI("F");
  else
    return CSI(CStrUtil::toString(n) + "F");
}

std::string
CEscape::
CHA(int n)
{
  if (n < 0)
    return CSI("G");
  else
    return CSI(CStrUtil::toString(n) + "G");
}

std::string
CEscape::
CUP(int row, int col)
{
  if (row < 0 || col < 0)
    return CSI("H");
  else
    return CSI(CStrUtil::toString(row) + ";" + CStrUtil::toString(col) + "H");
}

std::string
CEscape::
CHT(int n)
{
  if (n < 0)
    return CSI("I");
  else
    return CSI(CStrUtil::toString(n) + "I");
}

std::string
CEscape::
ED(int n)
{
  if (n < 0)
    return CSI("J");
  else
    return CSI(CStrUtil::toString(n) + "J");
}

std::string
CEscape::
DECSED(int n)
{
  if (n < 0)
    return CSI("?J");
  else
    return CSI("?" + CStrUtil::toString(n) + "J");
}

std::string
CEscape::
EL(int n)
{
  if (n < 0)
    return CSI("K");
  else
    return CSI(CStrUtil::toString(n) + "K");
}

std::string
CEscape::
DECSEL(int n)
{
  if (n < 0)
    return CSI("?K");
  else
    return CSI("?" + CStrUtil::toString(n) + "K");
}

std::string
CEscape::
IL(int n)
{
  if (n < 0)
    return CSI("L");
  else
    return CSI(CStrUtil::toString(n) + "L");
}

std::string
CEscape::
DL(int n)
{
  if (n < 0)
    return CSI("M");
  else
    return CSI(CStrUtil::toString(n) + "M");
}

std::string
CEscape::
DCH(int n)
{
  if (n < 0)
    return CSI("P");
  else
    return CSI(CStrUtil::toString(n) + "P");
}

std::string
CEscape::
SU(int n)
{
  if (n < 0)
    return CSI("S");
  else
    return CSI(CStrUtil::toString(n) + "S");
}

std::string
CEscape::
SD(int n)
{
  if (n < 0)
    return CSI("T");
  else
    return CSI(CStrUtil::toString(n) + "T");
}

std::string
CEscape::
ECH(int n)
{
  if (n < 0)
    return CSI("X");
  else
    return CSI(CStrUtil::toString(n) + "X");
}

std::string
CEscape::
CBT(int n)
{
  if (n < 0)
    return CSI("Z");
  else
    return CSI(CStrUtil::toString(n) + "Z");
}

std::string
CEscape::
HPA(int n)
{
  if (n < 0)
    return CSI("`");
  else
    return CSI(CStrUtil::toString(n) + "`");
}

std::string
CEscape::
REP(int n)
{
  if (n < 0)
    return CSI("b");
  else
    return CSI(CStrUtil::toString(n) + "b");
}

std::string
CEscape::
DA1(int n)
{
  if (n < 0)
    return CSI("c");
  else
    return CSI(CStrUtil::toString(n) + "c");
}

std::string
CEscape::
DA2(int n)
{
  if (n < 0)
    return CSI(">c");
  else
    return CSI(std::string(">") + CStrUtil::toString(n) + "c");
}

std::string
CEscape::
VPA(int n)
{
  if (n < 0)
    return CSI("d");
  else
    return CSI(CStrUtil::toString(n) + "d");
}

std::string
CEscape::
HVP(int row, int col)
{
  if (row == 1 && col == 1)
    return CSI("f");
  else
    return CSI(CStrUtil::toString(row) + ";" + CStrUtil::toString(col) + "f");
}

std::string
CEscape::
TBC(int n)
{
  if (n < 0)
    return CSI("g");
  else
    return CSI(CStrUtil::toString(n) + "g");
}

std::string
CEscape::
SM(int n)
{
  if (n < 0)
    return CSI("h");
  else
    return CSI(CStrUtil::toString(n) + "h");
}

std::string
CEscape::
DECSET(int n)
{
  if (n < 0)
    return CSI("?h");
  else
    return CSI("?" + CStrUtil::toString(n) + "h");
}

std::string
CEscape::
DECSET(int n1, int n2)
{
  return CSI("?" + CStrUtil::toString(n1) + ";" + CStrUtil::toString(n2) + "h");
}

std::string
CEscape::
MC(int n)
{
  if (n < 0)
    return CSI("i");
  else
    return CSI(CStrUtil::toString(n) + "i");
}

std::string
CEscape::
DECMC(int n)
{
  if (n < 0)
    return CSI("?i");
  else
    return CSI("?" + CStrUtil::toString(n) + "i");
}

std::string
CEscape::
RM(int n)
{
  if (n < 0)
    return CSI("l");
  else
    return CSI(CStrUtil::toString(n) + "l");
}

std::string
CEscape::
DECRST(int n)
{
  if (n < 0)
    return CSI("?l");
  else
    return CSI("?" + CStrUtil::toString(n) + "l");
}

std::string
CEscape::
DECRST(int n1, int n2)
{
  return CSI("?" + CStrUtil::toString(n1) + ";" + CStrUtil::toString(n2) + "l");
}

std::string
CEscape::
SGR(int n)
{
  if (n < 0)
    return CSI("m");
  else
    return CSI(CStrUtil::toString(n) + "m");
}

#if 0
std::string
CEscape::
SGR(int n, int r, int g, int b)
{
  return CSI(CStrUtil::toString(n) + ";" + CStrUtil::toString(r) + ";" +
             CStrUtil::toString(g) + ";" + CStrUtil::toString(b) + "m");
}
#endif

std::string
CEscape::
SGR_fg(int n)
{
  return CSI("38;5;" + CStrUtil::toString(n) + "m");
}

std::string
CEscape::
SGR_fg(int r, int g, int b)
{
  return CSI("38;2;" + CStrUtil::toString(r) + ";" + CStrUtil::toString(g) + ";" +
             CStrUtil::toString(b) + "m");
}

std::string
CEscape::
SGR_bg(int n)
{
  return CSI("48;5;" + CStrUtil::toString(n) + "m");
}

std::string
CEscape::
SGR_bg(int r, int g, int b)
{
  return CSI("48;2;" + CStrUtil::toString(r) + ";" + CStrUtil::toString(g) + ";" +
             CStrUtil::toString(b) + "m");
}

std::string
CEscape::
DSR(int n)
{
  if (n < 0)
    n = 0;

  return CSI(CStrUtil::toString(n) + "n");
}

std::string
CEscape::
DECDSR(int n)
{
  if (n < 0)
    n = 0;

  return CSI("?" + CStrUtil::toString(n) + "n");
}

std::string
CEscape::
DECSTR()
{
  return CSI("!p");
}

std::string
CEscape::
DECSCL(int n, int m)
{
  if (n < 0)
    n = 0;

  return CSI(CStrUtil::toString(n) + ";" + CStrUtil::toString(m) + "\"p");
}

std::string
CEscape::
DECSTBM(int top, int bottom)
{
  if (top < 0 || bottom < 0)
    return CSI("r");
  else
    return CSI(CStrUtil::toString(top) + ";" + CStrUtil::toString(bottom) + "r");
}

std::string
CEscape::
DECCARA(int top, int left, int bottom, int right, int attr)
{
  if (top    < 0) top    = 0;
  if (left   < 0) left   = 0;
  if (bottom < 0) bottom = 0;
  if (right  < 0) right  = 0;
  if (attr   < 0) attr   = 0;

  return CSI(CStrUtil::toString(top   ) + ";" + CStrUtil::toString(left  ) + ";" +
             CStrUtil::toString(bottom) + ";" + CStrUtil::toString(right ) + ";" +
             CStrUtil::toString(attr  ) + "$r");
}

std::string
CEscape::
SC()
{
  return CSI("s");
}

std::string
CEscape::
DECRARA(int top, int left, int bottom, int right, int attr)
{
  if (top    < 0) top    = 0;
  if (left   < 0) left   = 0;
  if (bottom < 0) bottom = 0;
  if (right  < 0) right  = 0;
  if (attr   < 0) attr   = 0;

  return CSI(CStrUtil::toString(top   ) + ";" + CStrUtil::toString(left  ) + ";" +
             CStrUtil::toString(bottom) + ";" + CStrUtil::toString(right ) + ";" +
             CStrUtil::toString(attr  ) + "$t");
}

std::string
CEscape::
SC1()
{
  return CSI("u");
}

std::string
CEscape::
DECCRA(int src_top, int src_left, int src_bottom, int src_right, int src,
       int dst_top, int dst_left, int dst)
{
  if (src_top    < 0) src_top    = 0;
  if (src_left   < 0) src_left   = 0;
  if (src_bottom < 0) src_bottom = 0;
  if (src_right  < 0) src_right  = 0;
  if (src        < 0) src        = 0;
  if (dst_top    < 0) src_top    = 0;
  if (dst_left   < 0) src_left   = 0;
  if (dst        < 0) dst        = 0;

  return CSI(CStrUtil::toString(src_top   ) + ";" + CStrUtil::toString(src_left  ) + ";" +
             CStrUtil::toString(src_bottom) + ";" + CStrUtil::toString(src_right ) + ";" +
             CStrUtil::toString(src       ) + ";" + CStrUtil::toString(dst_top   ) + ";" +
             CStrUtil::toString(dst_left  ) + ";" + CStrUtil::toString(dst       ) + "$v");
}

std::string
CEscape::
DECEFR(int top, int left, int bottom, int right)
{
  if (top    < 0) top    = 0;
  if (left   < 0) left   = 0;
  if (bottom < 0) bottom = 0;
  if (right  < 0) right  = 0;

  return CSI(CStrUtil::toString(top   ) + ";" + CStrUtil::toString(left  ) + ";" +
             CStrUtil::toString(bottom) + ";" + CStrUtil::toString(right ) + "'w");
}

std::string
CEscape::
DECREQTPARM(int n)
{
  if (n < 0) n = 0;

  return CSI(CStrUtil::toString(n) + "x");
}

std::string
CEscape::
DECSACE(int n)
{
  if (n < 0) n = 0;

  return CSI(CStrUtil::toString(n) + "*x");
}

std::string
CEscape::
DECFRA(int c, int top, int left, int bottom, int right)
{
  if (c      < 0) c      = 32;
  if (top    < 0) top    = 0;
  if (left   < 0) left   = 0;
  if (bottom < 0) bottom = 0;
  if (right  < 0) right  = 0;

  return CSI(CStrUtil::toString(c     ) + ";" + CStrUtil::toString(top   ) + ";" +
             CStrUtil::toString(left  ) + ";" + CStrUtil::toString(bottom) + ";" +
             CStrUtil::toString(right ) + "$x");
}

std::string
CEscape::
DECELR(int n1, int n2)
{
  if (n1 < 0) n1 = 0;
  if (n2 < 0) n2 = 0;

  return CSI(CStrUtil::toString(n1 ) + ";" + CStrUtil::toString(n2) + "'z");
}

std::string
CEscape::
DECERA(int top, int left, int bottom, int right)
{
  if (top    < 0) top    = 0;
  if (left   < 0) left   = 0;
  if (bottom < 0) bottom = 0;
  if (right  < 0) right  = 0;

  return CSI(CStrUtil::toString(top   ) + ";" + CStrUtil::toString(left  ) + ";" +
             CStrUtil::toString(bottom) + ";" + CStrUtil::toString(right ) + "$z");
}

std::string
CEscape::
DECSLE(int n)
{
  if (n < 0) n = 0;

  return CSI(CStrUtil::toString(n) + "'{");
}

std::string
CEscape::
DECSERA(int top, int left, int bottom, int right)
{
  if (top    < 0) top    = 0;
  if (left   < 0) left   = 0;
  if (bottom < 0) bottom = 0;
  if (right  < 0) right  = 0;

  return CSI(CStrUtil::toString(top   ) + ";" + CStrUtil::toString(left  ) + ";" +
             CStrUtil::toString(bottom) + ";" + CStrUtil::toString(right ) + "${");
}

std::string
CEscape::
DECRQLP(int n)
{
  if (n < 0) n = 0;

  return CSI(CStrUtil::toString(n) + "'|");
}

// set/reset inverse video
std::string
CEscape::
DECSCNM(bool b)
{
  if (b)
    return DECSET(5);
  else
    return DECRST(5);
}

std::string
CEscape::
DECTEK(bool b)
{
  if (b)
    return DECSET(38);
  else
    return DECRST(38);
}

void
CEscape::
APC(std::ostream &os, const std::string &str)
{
  os << APC(str);
}

std::string
CEscape::
APC(const std::string &str)
{
  return ESC_s "_" + str + ESC_s "\\";
}

// ESC[ !?>= <n> "`$*& @ABCDEFGHIKMLMPRSTXZ`bcdfghilmnpqrstuvwxyz{|
//
void
CEscape::
CSI(std::ostream &os, const std::string &str, bool bit8)
{
  os << CSI(str, bit8);
}

std::string
CEscape::
CSI(const std::string &str, bool bit8)
{
  if (! bit8)
    return ESC_s "[" + str;
  else
    return "\x9b" + str;
}

//-------------

static bool parseInteger
             (const std::vector<std::string> &words, int pos, int *i, bool opt=false);
static bool checkNumArgs
             (const std::vector<std::string> &args, int num, bool opt=false);

std::string
CEscape::
stringToEscape(const std::string &str)
{
  OptString ostr = stringToOptEscape(str);

  if (! ostr.valid) {
    std::cerr << "Invalid command: " <<  str << std::endl;
    return "";
  }

  return ostr.str;
}

bool
CEscape::
stringToEscape(const std::string &str, std::string &escapeStr)
{
  OptString ostr = stringToOptEscape(str);

  if (! ostr.valid)
    return false;

  escapeStr = ostr.str;

  return true;
}

CEscape::OptString
CEscape::
stringToOptEscape(const std::string &str)
{
  int i1, i2, i3, i4, i5;

  std::vector<std::string> words;

  CStrUtil::addFields(str, words, ";");

  uint num_words = words.size();

  if      (words[0] == "NUL") return NUL();
  else if (words[0] == "SOH") return SOH();  // Ctrl A
  else if (words[0] == "STX") return STX();  // Ctrl B
  else if (words[0] == "ETX") return ETX();  // Ctrl C
  else if (words[0] == "EOT") return EOT();  // Ctrl D
  else if (words[0] == "ENQ") return ENQ();  // Ctrl E
  else if (words[0] == "ACK") return ACK();  // Ctrl F
  else if (words[0] == "BEL") return BEL();  // Ctrl G
  else if (words[0] == "BS" ) return BS ();  // Ctrl H
  else if (words[0] == "HT" ) return HT ();  // Ctrl I
  else if (words[0] == "TAB") return HT ();  // Ctrl I
  else if (words[0] == "LF" ) return LF ();  // Ctrl J
  else if (words[0] == "NL" ) return LF ();  // Ctrl J
  else if (words[0] == "VT" ) return VT ();  // Ctrl K
  else if (words[0] == "FF" ) return FF ();  // Ctrl L
  else if (words[0] == "NP" ) return FF ();  // Ctrl L
  else if (words[0] == "CR" ) return CR ();  // Ctrl M
  else if (words[0] == "SO" ) return SO ();  // Ctrl N
  else if (words[0] == "SI" ) return SI ();  // Ctrl O
  else if (words[0] == "DLE") return DLE();
  else if (words[0] == "DC1") return DC1();
  else if (words[0] == "DC2") return DC2();
  else if (words[0] == "DC3") return DC3();
  else if (words[0] == "DC4") return DC4();
  else if (words[0] == "NAK") return NAK();
  else if (words[0] == "SYN") return SYN();
  else if (words[0] == "ETB") return ETB();
  else if (words[0] == "CAN") return CAN();
  else if (words[0] == "EM" ) return EM();
  else if (words[0] == "SUB") return SUB();
  else if (words[0] == "FS" ) return FS();
  else if (words[0] == "GS" ) return GS();
  else if (words[0] == "RS" ) return RS();
  else if (words[0] == "US" ) return US();
  else if (words[0] == "DEL") return DEL();

  else if (words[0] == "SP" ) return SP();

  else if (words[0] == "ESC") return ESC();

  else if (words[0] == "IND") return IND();
  else if (words[0] == "NEL") return NEL();
  else if (words[0] == "HTS") return HTS();
  else if (words[0] == "RI" ) return RI();
  else if (words[0] == "SS2") return SS2();
  else if (words[0] == "SS3") return SS3();
  else if (words[0] == "DCS") return DCS();
  else if (words[0] == "SPA") return SPA();
  else if (words[0] == "EPA") return EPA();
  else if (words[0] == "SOS") return SOS();
  else if (words[0] == "DECID") return DECID();

  else if (words[0] == "G0") { return (num_words > 1 ? G0(words[1]) : G0()); }
  else if (words[0] == "G1") { return (num_words > 1 ? G1(words[1]) : G1()); }
  else if (words[0] == "G2") { return (num_words > 1 ? G2(words[1]) : G1()); }
  else if (words[0] == "G3") { return (num_words > 1 ? G3(words[1]) : G1()); }

  else if (words[0] == "WIN") return stringWinOpToEscape(words);
  else if (words[0] == "CSI") return stringCSIOpToEscape(words);
  else if (words[0] == "OSC") return stringOSCOpToEscape(words);

  else if (words[0] == "DECSC" ) return DECSC();
  else if (words[0] == "DECRC" ) return DECRC();
  else if (words[0] == "DECPAM") return DECPAM();
  else if (words[0] == "DECPNM") return DECPNM();

  else if (words[0] == "SCS") return stringSCSOpToEscape(words);

  else if (words[0] == "RIS") return RIS();

  else if (words[0] == "DECALN") return DECALN();

  else if (words[0] == "ESC_STX") return ESC() + STX();
  else if (words[0] == "ESC_ENQ") return ESC() + ENQ();
  else if (words[0] == "ESC_FF" ) return ESC() + FF ();
  else if (words[0] == "ESC_SI" ) return ESC() + SI ();
  else if (words[0] == "ESC_ETB") return ESC() + ETB();
  else if (words[0] == "ESC_CAN") return ESC() + CAN();
  else if (words[0] == "ESC_SUB") return ESC() + SUB();
  else if (words[0] == "ESC_FS" ) return ESC() + FS ();
  else if (words[0] == "ESC_8"  ) return ESC() + "8";
  else if (words[0] == "ESC_9"  ) return ESC() + "9";
  else if (words[0] == "ESC_:"  ) return ESC() + ":";
  else if (words[0] == "ESC_;"  ) return ESC() + ";";
  else if (words[0] == "ESC_`"  ) return ESC() + "`";
  else if (words[0] == "ESC_a"  ) return ESC() + "a";
  else if (words[0] == "ESC_b"  ) return ESC() + "b";
  else if (words[0] == "ESC_c"  ) return ESC() + "c";
  else if (words[0] == "ESC_d"  ) return ESC() + "d";
  else if (words[0] == "ESC_h"  ) return ESC() + "h";
  else if (words[0] == "ESC_i"  ) return ESC() + "i";
  else if (words[0] == "ESC_j"  ) return ESC() + "j";
  else if (words[0] == "ESC_k"  ) return ESC() + "k";
  else if (words[0] == "ESC_l"  ) return ESC() + "l";
  else if (words[0] == "ESC_p"  ) return ESC() + "p";
  else if (words[0] == "ESC_q"  ) return ESC() + "q";
  else if (words[0] == "ESC_r"  ) return ESC() + "r";
  else if (words[0] == "ESC_s"  ) return ESC() + "s";
  else if (words[0] == "ESC_t"  ) return ESC() + "t";

  else if (words[0] == "ICH") {
    if (parseInteger(words, 1, &i1, true))
      return ICH(i1);
    else
      return ICH();
  }
  else if (words[0] == "CUU") {
    if (parseInteger(words, 1, &i1, true))
      return CUU(i1);
    else
      return CUU();
  }
  else if (words[0] == "CUD") {
    if (parseInteger(words, 1, &i1, true))
      return CUD(i1);
    else
      return CUD();
  }
  else if (words[0] == "CUF") {
    if (parseInteger(words, 1, &i1, true))
      return CUF(i1);
    else
      return CUF();
  }
  else if (words[0] == "CUB") {
    if (parseInteger(words, 1, &i1, true))
      return CUB(i1);
    else
      return CUB();
  }
  else if (words[0] == "CNL") {
    if (parseInteger(words, 1, &i1, true))
      return CNL(i1);
    else
      return CNL();
  }
  else if (words[0] == "CPL") {
    if (parseInteger(words, 1, &i1, true))
      return CPL(i1);
    else
      return CPL();
  }
  else if (words[0] == "CHA") {
    if (parseInteger(words, 1, &i1, true))
      return CHA(i1);
    else
      return CHA();
  }
  else if (words[0] == "CUP") {
    if (parseInteger(words, 1, &i1, true) &&
        parseInteger(words, 2, &i2, true))
      return CUP(i1, i2);
    else
      return CUP();
  }
  else if (words[0] == "CHT") {
    if (parseInteger(words, 1, &i1, true))
      return CHT(i1);
    else
      return CHT();
  }
  else if (words[0] == "ED") {
    if (parseInteger(words, 1, &i1, true))
      return ED(i1);
    else
      return ED();
  }
  else if (words[0] == "DECSED") {
    if (parseInteger(words, 1, &i1, true))
      return DECSED(i1);
    else
      return DECSED();
  }
  else if (words[0] == "EL") {
    if (parseInteger(words, 1, &i1, true))
      return EL(i1);
    else
      return EL();
  }
  else if (words[0] == "DECSEL") {
    if (parseInteger(words, 1, &i1, true))
      return DECSEL(i1);
    else
      return DECSEL();
  }
  else if (words[0] == "IL") {
    if (parseInteger(words, 1, &i1, true))
      return IL(i1);
    else
      return IL();
  }
  else if (words[0] == "DL") {
    if (parseInteger(words, 1, &i1, true))
      return DL(i1);
    else
      return DL();
  }
  else if (words[0] == "DCH") {
    if (parseInteger(words, 1, &i1, true))
      return DCH(i1);
    else
      return DCH();
  }
  else if (words[0] == "SU") {
    if (parseInteger(words, 1, &i1, true))
      return SU(i1);
    else
      return SU();
  }
  else if (words[0] == "SD") {
    if (parseInteger(words, 1, &i1, true))
      return SD(i1);
    else
      return SD();
  }
  else if (words[0] == "ECH") {
    if (parseInteger(words, 1, &i1, true))
      return ECH(i1);
    else
      return ECH();
  }
  else if (words[0] == "CBT") {
    if (parseInteger(words, 1, &i1, true))
      return CBT(i1);
    else
      return CBT();
  }
  else if (words[0] == "HPA") {
    if (parseInteger(words, 1, &i1, true))
      return HPA(i1);
    else
      return HPA();
  }
  else if (words[0] == "REP") {
    if (parseInteger(words, 1, &i1, true))
      return REP(i1);
    else
      return REP();
  }
  else if (words[0] == "DA1") {
    if (parseInteger(words, 1, &i1, true))
      return DA1(i1);
    else
      return DA1();
  }
  else if (words[0] == "DA2") {
    if (parseInteger(words, 1, &i1, true))
      return DA2(i1);
    else
      return DA2();
  }
  else if (words[0] == "VPA") {
    if (parseInteger(words, 1, &i1, true))
      return VPA(i1);
    else
      return VPA();
  }
  else if (words[0] == "HVP") {
    if (parseInteger(words, 1, &i1, true) &&
        parseInteger(words, 2, &i2, true))
      return HVP(i1, i2);
    else
      return HVP();
  }
  else if (words[0] == "TBC") {
    if (parseInteger(words, 1, &i1, true))
      return TBC(i1);
    else
      return TBC();
  }
  else if (words[0] == "SM") {
    if (parseInteger(words, 1, &i1, true))
      return SM(i1);
    else
      return SM();
  }
  else if (words[0] == "DECSET") {
    if (parseInteger(words, 1, &i1, true))
      return DECSET(i1);
    else
      return DECSET();
  }
  else if (words[0] == "MC") {
    if (parseInteger(words, 1, &i1, true))
      return MC(i1);
    else
      return MC();
  }
  else if (words[0] == "DECMC") {
    if (parseInteger(words, 1, &i1, true))
      return DECMC(i1);
    else
      return DECMC();
  }
  else if (words[0] == "RM") {
    if (parseInteger(words, 1, &i1, true))
      return RM(i1);
    else
      return RM();
  }
  else if (words[0] == "DECRST") {
    if (parseInteger(words, 1, &i1, true))
      return DECRST(i1);
    else
      return DECRST();
  }
  else if (words[0] == "SGR") {
    if (parseInteger(words, 1, &i1, true)) {
      if (i1 == 38 || i1 == 48) {
        int r, g, b;

        if (parseInteger(words, 2, &r, true) &&
            parseInteger(words, 3, &g, true) &&
            parseInteger(words, 4, &b, true)) {
          if (i1 == 38)
            return SGR_fg(r, g, b);
          else
            return SGR_bg(r, g, b);
          //return SGR(i1, r, g, b);
        }
        else
          return SGR(i1);
      }
      else
        return SGR(i1);
    }
    else
      return SGR();
  }
  else if (words[0] == "DSR") {
    if (! parseInteger(words, 1, &i1, true))
      i1 = 0;

    return DSR(i1);
  }
  else if (words[0] == "DECDSR") {
    if (! parseInteger(words, 1, &i1, true))
      i1 = 0;

    return DECDSR(i1);
  }
  else if (words[0] == "DECSTR") {
    return DECSTR();
  }
  else if (words[0] == "DECSCL") {
    if (parseInteger(words, 1, &i1, true) &&
        parseInteger(words, 2, &i2, true))
      return DECSCL(i1, i2);
    else
      return DECSCL();
  }
  else if (words[0] == "DECSTBM") {
    if (parseInteger(words, 1, &i1, true) &&
        parseInteger(words, 2, &i2, true))
      return DECSTBM(i1, i2);
    else
      return DECSTBM();
  }
  else if (words[0] == "DECCARA") {
    if (parseInteger(words, 1, &i1, true) &&
        parseInteger(words, 2, &i2, true) &&
        parseInteger(words, 3, &i3, true) &&
        parseInteger(words, 4, &i4, true) &&
        parseInteger(words, 5, &i5, true))
      return DECCARA(i1, i2, i3, i4, i5);
    else
      return DECCARA();
  }
  else if (words[0] == "SC") {
    return SC();
  }
  else if (words[0] == "DECRARA") {
    if (parseInteger(words, 1, &i1, true) &&
        parseInteger(words, 2, &i2, true) &&
        parseInteger(words, 3, &i3, true) &&
        parseInteger(words, 4, &i4, true) &&
        parseInteger(words, 5, &i5, true))
      return DECRARA(i1, i2, i3, i4, i5);
    else
      return DECRARA();
  }
  else if (words[0] == "DECREQTPARM") {
    if (parseInteger(words, 1, &i1, true))
      return DECREQTPARM(i1);
    else
      return DECREQTPARM();
  }
  else if (words[0] == "DECELR") {
    if (parseInteger(words, 1, &i1, true) &&
        parseInteger(words, 2, &i2, true))
      return DECELR(i1, i2);
    else
      return DECELR();
  }
  else if (words[0] == "DECSLE") {
    if (parseInteger(words, 1, &i1, true))
      return DECSLE(i1);
    else
      return DECSLE();
  }
  else if (words[0] == "DECRQLP") {
    if (parseInteger(words, 1, &i1, true))
      return DECRQLP(i1);
    else
      return DECRQLP();
  }
  else if (words[0] == "DECSCNM") {
    if (! parseInteger(words, 1, &i1, true))
      i1 = 1;

    return DECSCNM(i1);
  }
  else if (words[0] == "DECTEK") {
    if (! parseInteger(words, 1, &i1, true))
      i1 = 1;

    return DECTEK(i1);
  }

  else if (words[0] == "s") {
    return words[1];
  }
  else if (words[0] == "srep") {
    int n;

    if (! parseInteger(words, 1, &n, false))
      n = 1;

    std::string s;

    for (int i = 0; i < n; ++i)
      s +=  words[2];

    return s;
  }
  else if (words[0] == "file") {
    CFile file(words[1]);

    std::string text;

    std::string line;

    while (file.readLine(line))
      text += line + "\n";

    return text;
  }
  else if (words[0] == "paste") {
    if (num_words > 1)
      return APC("<paste text=\"" + words[1] + "\"/>");
    else
      return APC("<paste/>");
  }
  else if (words[0] == "dir") {
    if (num_words > 1)
      return APC("<state dir=\"" + words[1] + "\"/>");
    else
      return APC("<state dir=\"" + CDir::getCurrent() + "\"/>");
  }
  else if (words[0] == "pixel") {
    if (num_words > 3)
      return APC("<pixel x=\"" + words[1] + "\" y=\"" + words[2] + "\" color=\"" +
                 words[3] + "\"/>");
    else
      return OptString();
  }
  else if (words[0] == "line") {
    if (num_words > 5)
      return APC("<line x1=\"" + words[1] + "\" y1=\"" + words[2] + "\" "
                 "x2=\"" + words[3] + "\" y2=\"" + words[4] + "\" color=\"" + words[5] + "\"/>");
    else
      return OptString();
  }
  else {
    return OptString();
  }
}

std::string
CEscape::
stringWinOpToEscape(const std::vector<std::string> &words)
{
  if (words.size() < 2) {
    std::cerr << "Wrong number of arguments: got " <<
                 words.size() << " need at least " << 2 << std::endl;
    return "";
  }

  if      (words[1] == "deiconify") { // 1
    return windowOpDeiconify();
  }
  else if (words[1] == "iconify") { // 2
    return windowOpIconify();
  }
  else if (words[1] == "move") { // 3
    int x, y;

    if (! parseInteger(words, 2, &x) || ! parseInteger(words, 3, &y)) {
      return "";
    }

    return windowOpMove(x, y);
  }
  else if (words[1] == "pixel_resize") { // 4
    int h, w;

    if (! parseInteger(words, 2, &h) || ! parseInteger(words, 3, &w))
      return "";

    return windowOpPixelSize(h, w);
  }
  else if (words[1] == "raise") { // 5
    return windowOpRaise();
  }
  else if (words[1] == "lower") { // 6
    return windowOpLower();
  }
  else if (words[1] == "refresh") { // 7
    return windowOpRefresh();
  }
  else if (words[1] == "resize" || words[1] == "char_resize") { // 8
    int h, w;

    if (! parseInteger(words, 2, &h) || ! parseInteger(words, 3, &w))
      return "";

    return windowOpCharSize(h, w);
  }
  else if (words[1] == "restore_maximized") { // 9;0
    return windowOpRestoreMaximized();
  }
  else if (words[1] == "maximize") { // 9;1
    return windowOpMaximize();
  }
  else if (words[1] == "report_state") { // 11
    return windowOpReportState();
  }
  else if (words[1] == "report_pos") { // 13
    return windowOpReportPos();
  }
  else if (words[1] == "report_pixel_size") { // 14
    return windowOpReportPixelSize();
  }
  else if (words[1] == "report_size" || words[1] == "report_char_size") { // 18
    return windowOpReportCharSize();
  }
  else if (words[1] == "report_screen_size") { // 19
    return windowOpReportScreenSize();
  }
  else if (words[1] == "report_icon_label") { // 20
    return windowOpReportIconLabel();
  }
  else if (words[1] == "report_window_title") { // 21
    return windowOpReportWindowTitle();
  }
  else if (words[1] == "resize_lines") { // 24
    int n;

    if (! parseInteger(words, 2, &n))
      return "";

    if (n < 24) {
      std::cerr << "Invalid number of lines: " <<  n << std::endl;
      return "";
    }

    return windowOpResizeNLines(n);
  }
  else {
    std::cerr << "Invalid WIN op: " <<  words[1] << std::endl;
    return "";
  }
}

std::string
CEscape::
stringCSIOpToEscape(const std::vector<std::string> &words)
{
  uint num_words = words.size();

  if (num_words == 1)
    return CSI();

  std::string str = CSI();

  for (uint i = 1; i < num_words; ++i)
    str += words[i];

  return str;
}

std::string
CEscape::
stringOSCOpToEscape(const std::vector<std::string> &words)
{
  if (! checkNumArgs(words, 2)) return "";

  if      (words[1] == "icon_window_title") { // 0
    if (! checkNumArgs(words, 3)) return "";

    return oscIconWindowTitle(words[2]);
  }
  else if (words[1] == "icon_title") { // 1
    if (! checkNumArgs(words, 3)) return "";

    return oscIconTitle(words[2]);
  }
  else if (words[1] == "window_title") { // 2
    if (! checkNumArgs(words, 3)) return "";

    return oscWindowTitle(words[2]);
  }
  else if (words[1] == "window_prop") { // 3
    if (! checkNumArgs(words, 3)) return "";

    return oscWindowProp(words[2]);
  }
  else if (words[1] == "color") { // 4
    int n;

    if (! parseInteger(words, 2, &n)) return "";

    if (! checkNumArgs(words, 4)) return "";

    return oscColor(n, words[3]);
  }
  else if (words[1] == "fg") { // 10
    if (! checkNumArgs(words, 3)) return "";

    return oscFg(words[2]);
  }
  else if (words[1] == "bg") { // 11
    if (! checkNumArgs(words, 3)) return "";

    return oscBg(words[2]);
  }
  else if (words[1] == "cursor_color") {  // 12
    if (! checkNumArgs(words, 3)) return "";

    return oscCursorColor(words[2]);
  }
  else if (words[1] == "font") { // 50
    if (! checkNumArgs(words, 3)) return "";

    return oscFont(words[2]);
  }
  else {
    std::cerr << "Invalid OSC op: " <<  words[1] << std::endl;
    return "";
  }
}

std::string
CEscape::
stringSCSOpToEscape(const std::vector<std::string> &words)
{
  if (! checkNumArgs(words, 3, 3)) return "";

  std::string str;

  if      (words[1] == "0") {
    str += "(";
    str += words[2][0];
    str += ")B";
    str += SI();
  }
  else if (words[1] == "1") {
    str += ")";
    str += words[2][0];
    str += "(B";
    str += SO();
  }

  return str;
}

std::string
CEscape::
imageToEscape(const std::string &filename, int size, int x1, int y1, int x2, int y2)
{
  std::string str;

  str  = "<image ";
  str += "filename=\"" + filename + "\" ";
  str += "size=\"" + CStrUtil::toString(size) + "\" ";
  str += "x1=\"" + CStrUtil::toString(x1) + "\" ";
  str += "y1=\"" + CStrUtil::toString(y1) + "\" ";
  str += "x2=\"" + CStrUtil::toString(x2) + "\" ";
  str += "y2=\"" + CStrUtil::toString(y2) + "\"";
  str += "/>";

  return CEscape::APC(str);
}

std::string
CEscape::
imageToEscape(const std::string &filename, int size)
{
  std::string str;

  str  = "<image ";
  str += "filename=\"" + filename + "\" ";
  str += "size=\"" + CStrUtil::toString(size) + "\"";
  str += "/>";

  return CEscape::APC(str);
}

std::string
CEscape::
pixelPointToEscape(int x, int y, const std::string &color)
{
  std::string str;

  str  = "<pixel ";
  str += "x=\"" + CStrUtil::toString(x) + "\" ";
  str += "y=\"" + CStrUtil::toString(y) + "\" ";
  str += "color=\"" + color + "\"";
  str += "/>";

  return CEscape::APC(str);
}

std::string
CEscape::
pixelLineToEscape(int x1, int y1, int x2, int y2, const std::string &color)
{
  std::string str;

  str  = "<line ";
  str += "x1=\"" + CStrUtil::toString(x1) + "\" ";
  str += "y1=\"" + CStrUtil::toString(y1) + "\" ";
  str += "x2=\"" + CStrUtil::toString(x2) + "\" ";
  str += "y2=\"" + CStrUtil::toString(y2) + "\" ";
  str += "color=\"" + color + "\"";
  str += "/>";

  return CEscape::APC(str);
}

std::string
CEscape::
linkToEscape(const std::string &name, const std::string &path, const std::string &type)
{
  std::string str;

  str  = "<link ";
  str += "name=\"" + name + "\" ";
  str += "dir=\""  + path + "\" ";
  str += "type=\"" + type + "\" ";
  str += "/>";

  return CEscape::APC(str);
}

std::string
CEscape::
commandToEscape(const std::string &name, const std::string &dir, bool start)
{
  std::string str;

  str  = "<command ";
  str += "name=\"" + name + "\" ";
  str += "dir=\""  + dir + "\" ";
  str += "start=\""  + CStrUtil::toString(start) + "\" ";
  str += "/>";

  return CEscape::APC(str);
}

bool
CEscape::
getWindowCharSize(int *rows, int *cols)
{
  // get window char size
  COSRead::write(0, CEscape::windowOpReportCharSize().c_str());

  std::string result = readResult();

  std::vector<std::string> args;

  if (parseEscape(result, args)) {
    uint num_args = args.size();

    // CSI 8 ; <r> ; <c> t
    if (num_args == 3 && CStrUtil::isInteger(args[0]) &&
        CStrUtil::isInteger(args[1]) && CStrUtil::isInteger(args[2])) {
      int t = CStrUtil::toInteger(args[0]); if (t != 8) return false;

      *rows = CStrUtil::toInteger(args[1]);
      *cols = CStrUtil::toInteger(args[2]);

      return true;
    }
  }

  return false;
}

bool
CEscape::
getWindowPixelSize(int *width, int *height)
{
  // get window pixel size
  COSRead::write(0, CEscape::windowOpReportPixelSize().c_str());

  std::string result = readResult();

  std::vector<std::string> args;

  if (parseEscape(result, args)) {
    uint num_args = args.size();

    // CSI 4 ; <h> ; <w> t
    if (num_args == 3 && CStrUtil::isInteger(args[0]) &&
        CStrUtil::isInteger(args[1]) && CStrUtil::isInteger(args[2])) {
      int t = CStrUtil::toInteger(args[0]); if (t != 4) return false;

      *width  = CStrUtil::toInteger(args[2]);
      *height = CStrUtil::toInteger(args[1]);

      return true;
    }
  }

  return false;
}

bool
CEscape::
getWindowPos(int *row, int *col)
{
  // get window pixel size
  COSRead::write(0, CEscape::DSR(6).c_str());

  std::string result = readResult();

  std::vector<std::string> args;

  if (parseEscape(result, args)) {
    uint num_args = args.size();

    // CSI <row> ; <col> R
    if (num_args == 2 && CStrUtil::isInteger(args[0]) && CStrUtil::isInteger(args[1])) {
      *row = CStrUtil::toInteger(args[0]);
      *col = CStrUtil::toInteger(args[1]);

      return true;
    }
  }

  return false;
}

std::string
CEscape::
readResult()
{
  std::string result;

  if (COSRead::wait_read(0, s_readResultSecs, s_readResultMSecs))
    COSRead::read(0, result);

  if (s_logResult) {
    if (! s_logFile)
      s_logFile = fopen(".logfile", "w");

    fprintf(s_logFile, "%s\n", result.c_str());
  }

  return result;
}

void
CEscape::
setLogResult(bool b)
{
  s_logResult = b;

  if (! s_logResult) {
    if (s_logFile) {
      fclose(s_logFile);

      s_logFile = nullptr;
    }
  }
}

void
CEscape::
setReadResultTime(int secs, int msecs)
{
  s_readResultSecs  = secs;
  s_readResultMSecs = msecs;
}

bool
CEscape::
parseEscape(const std::string &str, std::vector<std::string> &args)
{
  int len = str.size();

  if (len < 3) return false;

  if (str[0] != '' || str[1] != '[' || ! isalpha(str[len - 1]))
    return false;

  int i = 2;

  while (i < len && ! isalpha(str[i])) {
    std::string arg;

    while (i < len && str[i] != ';' && ! isalpha(str[i]))
      arg += str[i++];

    if (i < len && str[i] == ';')
      ++i;

    args.push_back(arg);
  }

  return true;
}

bool
CEscape::
parseMouse(const std::string &str, int *button, int *x, int *y, bool *release)
{
  if (str.size() != 6) return false;

  if (str[0] != '' || str[1] != '[' || str[2] != 'M')
    return false;

  *button  = str[3] - 32;
  *x       = str[4] - 32;
  *y       = str[5] - 32;
  *release = (*button == 3);

  if (*release)
    *button = 1;

  return true;
}

//------------

std::string
CEscape::
tek4014Coord(uint x, uint y)
{
  static unsigned char lloy = -1, lhix = -1, lhiy = -1, leb = -1;

  if (x > 4095)
    x = 4095;

  if (y > 3071)
    y = 3071;

  uchar hiy = (y >> 7) & 0x1f;
  uchar loy = (y >> 2) & 0x1f;
  uchar hix = (x >> 7) & 0x1f;
  uchar lox = (x >> 2) & 0x1f;

  uchar eb = (x & 3) | ((y & 3) << 2);

  std::string str;

  if (hiy != lhiy)
    str += char(hiy | 0x20);

  if (eb != leb)
    str += char(eb | 0x60);

  if (eb != leb || loy != lloy || hix != lhix)
    str += char(loy | 0x60);

  if (hix != lhix)
    str += char(hix | 0x20);

  str += char(lox | 0x40);

  lhiy = hiy;
  lhix = hix;
  lloy = loy;
  leb  = eb;

  return str;
}

//------------

static bool
parseInteger(const std::vector<std::string> &words, int pos, int *i, bool opt)
{
  if (! checkNumArgs(words, pos + 1, opt)) return false;

  if (! CStrUtil::toInteger(words[pos], i)) {
    if (! opt)
      std::cerr << "Invalid integer: " <<  words[pos] << std::endl;
    return false;
  }

  return true;
}

static bool
checkNumArgs(const std::vector<std::string> &args, int num, bool opt)
{
  if ((int) args.size() < num) {
    if (! opt)
      std::cerr << "Wrong number of arguments: got " <<
                   args.size() << " need at least " << num + 1 << std::endl;

    return false;
  }

  return true;
}
