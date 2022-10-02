#include <CTermApp.h>
#include <COSRead.h>
#include <COSPty.h>
#include <COSTerm.h>
#include <CStrParse.h>
#include <CEscape.h>

#include <termios.h>
#include <unistd.h>

CTermApp::
CTermApp()
{
  setRaw(STDIN_FILENO);
}

CTermApp::
~CTermApp()
{
  resetRaw(STDIN_FILENO);
}

void
CTermApp::
mainLoop()
{
  if (mouse_)
    COSRead::write(STDOUT_FILENO, CEscape::DECSET(1002));

  redraw();

  if (autoExit_) return;

  for (;;) {
    if (! COSRead::wait_read(STDIN_FILENO, 0, 100)) continue;

    std::string buffer;

    if (! COSRead::read(STDIN_FILENO, buffer)) continue;

    uint len = uint(buffer.size());

    if (len == 0) continue;

    processString(buffer);

    if (done_) break;

    redraw();
  }

  if (mouse_)
    COSRead::write(STDOUT_FILENO, CEscape::DECRST(1002));
}

void
CTermApp::
processString(const std::string &str)
{
  inEscape_ = false;

  uint i   = 0;
  uint len = uint(str.size());

  //---

  // process mouse (only if mouse enabled ?)
  static int pressButton = 0;

  int  button, col, row;
  bool release;

  if (len == 6 && CEscape::parseMouse(str, &button, &col, &row, &release)) {
    int rows, cols, width, height;

    if (! CEscape::getWindowCharSize(&rows, &cols))
      return;

    if (! CEscape::getWindowPixelSize(&width, &height))
      return;

    if (rows <= 0) rows = 1;
    if (cols <= 0) cols = 1;

    int cw = width /cols;
    int ch = height/rows;

    int x1 = (col - 1)*cw + cw/2;
    int y1 = (row - 1)*ch + ch/2;

    CIPoint2D pos(x1, y1);

    if (! release) {
      CMouseEvent event(pos, CMouseButton(button + 1));

      mousePress(event);

      pressButton = button;
    }
    else {
      CMouseEvent event(pos, CMouseButton(pressButton + 1));

      mouseRelease(event);
    }

    return;
  }

  //---

  // skip status report
  if (len > 8 && str[0] == '' && str[1] == '[' && str[len - 1] == 't')
    return;

  //---

  for (uint pos = 0; pos < len; ++pos) {
    if (processStringChar(str[pos]))
      i = pos + 1;

    if (done_) return;
  }

  if (i < len) {
    for (uint pos = i; pos < len; ++pos) {
      processChar(str[pos]);

      if (done_) return;
    }
  }
}

bool
CTermApp::
processStringChar(unsigned char c)
{
  if (c == '') { // control backslash
    resetRaw(STDIN_FILENO);
    exit(1);
  }

  CKeyEvent event;

  if (! inEscape_) {
    if (c == '\033') {
      inEscape_     = true;
      escapeString_ = std::string(reinterpret_cast<const char *>(&c), 1);
      return false;
    }

    processChar(c);

    return true;
  }
  else {
    escapeString_ += c;

    CStrParse parse(escapeString_);

    if (! parse.isChar(''))
      return false;

    parse.skipChar();

    if (! parse.isChar('['))
      return false;

    parse.skipChar();

    while (parse.isDigit()) {
      int i;

      parse.readInteger(&i);

      if (! parse.isChar(';'))
        break;

      parse.skipChar();
    }

    if (! parse.isAlpha())
      return false;

    char c1 = parse.getCharAt();

    if      (c1 == 'A') { // up
      event.setType(CKEY_TYPE_Up);
    }
    else if (c1 == 'B') { // down
      event.setType(CKEY_TYPE_Down);
    }
    else if (c1 == 'C') { // right
      event.setType(CKEY_TYPE_Right);
    }
    else if (c1 == 'D') { // left
      event.setType(CKEY_TYPE_Left);
    }

    inEscape_ = false;

    keyPress(event);

    return true;
  }
}

void
CTermApp::
processChar(unsigned char c)
{
  CKeyEvent event;

  switch (c) {
    case 0     : event.setType(CKEY_TYPE_NUL); break;
    case ''  : event.setType(CKEY_TYPE_SOH); break;
    case ''  : event.setType(CKEY_TYPE_STX); break;
    case ''  : event.setType(CKEY_TYPE_ETX); break;
    case ''  : event.setType(CKEY_TYPE_EOT); break;
    case ''  : event.setType(CKEY_TYPE_ENQ); break;
    case ''  : event.setType(CKEY_TYPE_ACK); break;
    case ''  : event.setType(CKEY_TYPE_BEL); break;
    case ''  : event.setType(CKEY_TYPE_BackSpace); break;
    case '\011': event.setType(CKEY_TYPE_TAB); break;
    case '\012': event.setType(CKEY_TYPE_LineFeed); break;
    case ''  : event.setType(CKEY_TYPE_Clear); break;
    case ''  : event.setType(CKEY_TYPE_FF); break;
    case '\015': event.setType(CKEY_TYPE_Return); break;
    case ''  : event.setType(CKEY_TYPE_SO); break;
    case ''  : event.setType(CKEY_TYPE_SI); break;
    case ''  : event.setType(CKEY_TYPE_DLE); break;
    case '\021': event.setType(CKEY_TYPE_DC1); break;
    case ''  : event.setType(CKEY_TYPE_DC2); break;
    case '\023': event.setType(CKEY_TYPE_Pause); break;
    case ''  : event.setType(CKEY_TYPE_Scroll_Lock); break;
    case ''  : event.setType(CKEY_TYPE_Sys_Req); break;
    case ''  : event.setType(CKEY_TYPE_SYN); break;
    case ''  : event.setType(CKEY_TYPE_ETB); break;
    case ''  : event.setType(CKEY_TYPE_CAN); break;
    case ''  : event.setType(CKEY_TYPE_EM); break;
    case ''  : event.setType(CKEY_TYPE_SUB); break;
    case '\033': event.setType(CKEY_TYPE_Escape); break;
    case '\034': event.setType(CKEY_TYPE_FS); break;
    case '\035': event.setType(CKEY_TYPE_GS); break;
    case '\036': event.setType(CKEY_TYPE_RS); break;
    case '\037': event.setType(CKEY_TYPE_US); break;
    case ' '   : event.setType(CKEY_TYPE_Space); break;
    case '!'   : event.setType(CKEY_TYPE_Space); break;
    case '\"'  : event.setType(CKEY_TYPE_QuoteDbl); break;
    case '#'   : event.setType(CKEY_TYPE_NumberSign); break;
    case '$'   : event.setType(CKEY_TYPE_Dollar); break;
    case '%'   : event.setType(CKEY_TYPE_Percent); break;
    case '&'   : event.setType(CKEY_TYPE_Ampersand); break;
    case '\''  : event.setType(CKEY_TYPE_Apostrophe); break;
    case '('   : event.setType(CKEY_TYPE_ParenLeft); break;
    case ')'   : event.setType(CKEY_TYPE_ParenRight); break;
    case '*'   : event.setType(CKEY_TYPE_Asterisk); break;
    case '+'   : event.setType(CKEY_TYPE_Plus); break;
    case ','   : event.setType(CKEY_TYPE_Comma); break;
    case '-'   : event.setType(CKEY_TYPE_Minus); break;
    case '.'   : event.setType(CKEY_TYPE_Period); break;
    case '/'   : event.setType(CKEY_TYPE_Slash); break;
    case '0'   : event.setType(CKEY_TYPE_0); break;
    case '1'   : event.setType(CKEY_TYPE_1); break;
    case '2'   : event.setType(CKEY_TYPE_2); break;
    case '3'   : event.setType(CKEY_TYPE_3); break;
    case '4'   : event.setType(CKEY_TYPE_4); break;
    case '5'   : event.setType(CKEY_TYPE_5); break;
    case '6'   : event.setType(CKEY_TYPE_6); break;
    case '7'   : event.setType(CKEY_TYPE_7); break;
    case '8'   : event.setType(CKEY_TYPE_8); break;
    case '9'   : event.setType(CKEY_TYPE_9); break;
    case ':'   : event.setType(CKEY_TYPE_Colon); break;
    case ';'   : event.setType(CKEY_TYPE_Semicolon); break;
    case '<'   : event.setType(CKEY_TYPE_Less); break;
    case '='   : event.setType(CKEY_TYPE_Equal); break;
    case '>'   : event.setType(CKEY_TYPE_Greater); break;
    case '?'   : event.setType(CKEY_TYPE_Question); break;
    case '@'   : event.setType(CKEY_TYPE_At); break;
    case 'A'   : event.setType(CKEY_TYPE_A); break;
    case 'B'   : event.setType(CKEY_TYPE_B); break;
    case 'C'   : event.setType(CKEY_TYPE_C); break;
    case 'D'   : event.setType(CKEY_TYPE_D); break;
    case 'E'   : event.setType(CKEY_TYPE_E); break;
    case 'F'   : event.setType(CKEY_TYPE_F); break;
    case 'G'   : event.setType(CKEY_TYPE_G); break;
    case 'H'   : event.setType(CKEY_TYPE_H); break;
    case 'I'   : event.setType(CKEY_TYPE_I); break;
    case 'J'   : event.setType(CKEY_TYPE_J); break;
    case 'K'   : event.setType(CKEY_TYPE_K); break;
    case 'L'   : event.setType(CKEY_TYPE_L); break;
    case 'M'   : event.setType(CKEY_TYPE_M); break;
    case 'N'   : event.setType(CKEY_TYPE_N); break;
    case 'O'   : event.setType(CKEY_TYPE_O); break;
    case 'P'   : event.setType(CKEY_TYPE_P); break;
    case 'Q'   : event.setType(CKEY_TYPE_Q); break;
    case 'R'   : event.setType(CKEY_TYPE_R); break;
    case 'S'   : event.setType(CKEY_TYPE_S); break;
    case 'T'   : event.setType(CKEY_TYPE_T); break;
    case 'U'   : event.setType(CKEY_TYPE_U); break;
    case 'V'   : event.setType(CKEY_TYPE_V); break;
    case 'W'   : event.setType(CKEY_TYPE_W); break;
    case 'X'   : event.setType(CKEY_TYPE_X); break;
    case 'Y'   : event.setType(CKEY_TYPE_Y); break;
    case 'Z'   : event.setType(CKEY_TYPE_Z); break;
    case '['   : event.setType(CKEY_TYPE_BracketLeft); break;
    case '\\'  : event.setType(CKEY_TYPE_Backslash); break;
    case ']'   : event.setType(CKEY_TYPE_BracketRight); break;
    case '~'   : event.setType(CKEY_TYPE_AsciiCircum); break;
    case '_'   : event.setType(CKEY_TYPE_Underscore); break;
    case '`'   : event.setType(CKEY_TYPE_QuoteLeft); break;
    case 'a'   : event.setType(CKEY_TYPE_a); break;
    case 'b'   : event.setType(CKEY_TYPE_b); break;
    case 'c'   : event.setType(CKEY_TYPE_c); break;
    case 'd'   : event.setType(CKEY_TYPE_d); break;
    case 'e'   : event.setType(CKEY_TYPE_e); break;
    case 'f'   : event.setType(CKEY_TYPE_f); break;
    case 'g'   : event.setType(CKEY_TYPE_g); break;
    case 'h'   : event.setType(CKEY_TYPE_h); break;
    case 'i'   : event.setType(CKEY_TYPE_i); break;
    case 'j'   : event.setType(CKEY_TYPE_j); break;
    case 'k'   : event.setType(CKEY_TYPE_k); break;
    case 'l'   : event.setType(CKEY_TYPE_l); break;
    case 'm'   : event.setType(CKEY_TYPE_m); break;
    case 'n'   : event.setType(CKEY_TYPE_n); break;
    case 'o'   : event.setType(CKEY_TYPE_o); break;
    case 'p'   : event.setType(CKEY_TYPE_p); break;
    case 'q'   : event.setType(CKEY_TYPE_q); break;
    case 'r'   : event.setType(CKEY_TYPE_r); break;
    case 's'   : event.setType(CKEY_TYPE_s); break;
    case 't'   : event.setType(CKEY_TYPE_t); break;
    case 'u'   : event.setType(CKEY_TYPE_u); break;
    case 'v'   : event.setType(CKEY_TYPE_v); break;
    case 'w'   : event.setType(CKEY_TYPE_w); break;
    case 'x'   : event.setType(CKEY_TYPE_x); break;
    case 'y'   : event.setType(CKEY_TYPE_y); break;
    case 'z'   : event.setType(CKEY_TYPE_z); break;
    case '{'   : event.setType(CKEY_TYPE_BraceLeft); break;
    case '|'   : event.setType(CKEY_TYPE_Bar); break;
    case '}'   : event.setType(CKEY_TYPE_BraceRight); break;
    case 0x7f  : event.setType(CKEY_TYPE_DEL); break;
    default    : break;
  }

  if (c >= ' ' && c <= '}')
    event.setText(std::string(reinterpret_cast<char *>(&c), 1));

  keyPress(event);
}

bool
CTermApp::
setRaw(int fd)
{
  delete ios_;

  ios_ = new struct termios;

  COSPty::set_raw(fd, ios_);

  COSRead::write(STDOUT_FILENO, CEscape::DECSET(1049) + CEscape::DECRST(12,25));

  return true;
}

bool
CTermApp::
resetRaw(int fd)
{
  if (tcsetattr(fd, TCSAFLUSH, ios_) < 0)
    return false;

  COSRead::write(STDOUT_FILENO, CEscape::DECRST(1049) + CEscape::DECSET(12,25));

  COSRead::write(STDOUT_FILENO, CEscape::SGR(0));

  delete ios_;

  ios_ = NULL;

  return true;
}

void
CTermApp::
runCommand(const std::string &cmd)
{
  int fd = COSTerm::getTerminalId(STDOUT_FILENO);

  std::string cmd1 = cmd + "\n";

  resetRaw(STDIN_FILENO);

  COSRead::write(fd, cmd1.c_str());

  COSRead::write(2, cmd1.c_str());

  setRaw(STDIN_FILENO);
}
