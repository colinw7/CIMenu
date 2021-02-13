#ifndef CTERM_APP_H
#define CTERM_APP_H

#include <CEvent.h>

class CTermApp {
 public:
  CTermApp();

  virtual ~CTermApp();

  bool isMouse() const { return mouse_; }
  void setMouse(bool mouse) { mouse_ = mouse; }

  bool isAutoExit() const { return autoExit_; }
  void setAutoExit(bool exit) { autoExit_ = exit; }

  void mainLoop();

  virtual void keyPress(const CKeyEvent &) { }

  virtual void mousePress  (const CMouseEvent &) { }
  virtual void mouseRelease(const CMouseEvent &) { }

  virtual void redraw() { }

  void setDone(bool done) { done_ = done; }

  void runCommand(const std::string &cmd);

 private:
  void processString(const std::string &str);
  bool processStringChar(unsigned char c);
  void processChar(unsigned char c);

  bool setRaw(int fd);
  bool resetRaw(int fd);

 private:
  bool            mouse_     { false };
  bool            autoExit_  { true };
  bool            done_      { false };
  bool            inEscape_  { false };
  std::string     escapeString_;
  struct termios *ios_       { nullptr };
};

#endif
