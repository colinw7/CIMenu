#ifndef CIMENU_H
#define CIMENU_H

#include <CEvent.h>

#include <vector>
#include <map>
#include <sys/types.h>

class CIMenuBase;
class CIMenuItem;

//---

class CIMenuBox {
 public:
  typedef std::vector<CIMenuItem *> Items;

  enum class BorderStyle {
    NONE,
    LINE
  };

 public:
  CIMenuBox(CIMenuBox *parent=nullptr) :
   parent_(parent) {
  }

  virtual ~CIMenuBox() { }

  //---

  const BorderStyle &borderStyle() const { return borderStyle_; }
  void setBorderStyle(const BorderStyle &v) { borderStyle_ = v; }

  bool isCheckable() const { return checkable_; }
  void setCheckable(bool b) { checkable_ = b; }

  //---

  const CIMenuBox *parent() const { return parent_; }
  void setParent(CIMenuBox *p) { parent_ = p; }

  //---

  const Items &items() const { return items_; }

  CIMenuItem *item(int i) const { return items_[i]; }

  void clearItems();

  virtual CIMenuItem *addItem(const std::string &item);

  void addItem(CIMenuItem *item);

 private:
  BorderStyle borderStyle_ { BorderStyle::NONE }; // box border
  bool        checkable_   { false };             // are items checkable
  CIMenuBox*  parent_      { nullptr };           // parent box
  Items       items_;                             // child items
};

//---

class CIMenuItem {
 public:
  CIMenuItem() { }

  explicit CIMenuItem(const std::string &name) :
   name_(name) {
  }

  virtual ~CIMenuItem() { }

  CIMenuBase *base() const { return base_; }
  void setBase(CIMenuBase *p) { base_ = p; }

  std::string getName() const { return name_; }
  void setName(const std::string &name) { name_ = name; }

  bool isChecked() const { return checked_; }
  void setChecked(bool b) { checked_ = b; }

  virtual std::string getCommand() const { return name_; }

  virtual int getColumn() const { return 1; }

  virtual void draw();

  virtual void press();

 protected:
  CIMenuBase* base_      { nullptr };
  std::string name_;
  bool        checked_   { false };
};

//---

class CIMenuApp;

class CIMenuBase : public CIMenuBox {
 public:
  CIMenuBase();

  virtual ~CIMenuBase();

  //---

  int xMargin() const { return xMargin_; }
  void setXMargin(int i) { xMargin_ = i; }

  int yMargin() const { return yMargin_; }
  void setYMargin(int i) { yMargin_ = i; }

  int columnWidth() const { return columnWidth_; }
  void setColumnWidth(int i) { columnWidth_ = i; }

  //---

  CIMenuItem *addItem(const std::string &item) override;

  //---

  virtual void initItems() { }
  virtual void termItems() { }

  //--

  virtual void enterItem() { }
  virtual void leaveItem() { }

  //---

  virtual uint getNumColumns() const { return 1; }

  //---

  virtual void drawItems();

  virtual void drawCursor() const;

  virtual int getRowPos(int row) const;
  virtual int getColPos(int col) const;

  //---

  int  currentRow() const;
  void setCurrentRow(int row);

  int currentCol() const { return currentCol_; }
  void setCurrentCol(int col) { currentCol_ = col; }

  //---

  CIMenuItem *getCurrentItem() const;

  CIMenuItem *getItem(int row) const;
  CIMenuItem *getItem(int row, int col) const;

  uint getNumRows() const;
  uint getNumRows(int col) const;

  void keyPress(const CKeyEvent &event);

 public:
  void mainLoop();

  std::string currentCommand() const;

  std::vector<std::string> checkedCommands() const;

  void runCommand(const std::string &cmd);

 private:
  void updateState();

  void processChar(unsigned char c);

  void drawItem(CIMenuItem *item) const;

  void drawBox(int r1, int c1, int r2, int c2) const;
  void drawChar(int row, int col, char c) const;

 private:
  friend class CIMenuApp;

  typedef std::map<int,int> ColumnRow;

  CIMenuApp*        app_           { nullptr };
  int               currentCol_    { 0 };
  ColumnRow         cursorColRow_;
  mutable ColumnRow currentColRow_;
  int               screenRows_    { 80 };
  int               screenCols_    { 60 };
  int               xMargin_       { 1 };
  int               yMargin_       { 1 };
  mutable bool      checkable_     { false };
  int               columnWidth_   { 30 };
};

//---

#include <CTermApp.h>

class CIMenuApp : public CTermApp {
 public:
  CIMenuApp(CIMenuBase *menu) :
   menu_(menu) {
  }

  void redraw() {
    menu_->drawItems();
  }

  void keyPress(const CKeyEvent &event) {
    menu_->keyPress(event);
  }

 private:
  CIMenuBase *menu_ { nullptr };
};

#endif
