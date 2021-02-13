#ifndef CIMENU_H
#define CIMENU_H

#include <CEvent.h>

#include <vector>
#include <map>
#include <sys/types.h>

class CIMenuBase;
class CIMenuItem;

//---

// box containing items
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

  //---

 protected:
  BorderStyle borderStyle_ { BorderStyle::NONE }; // box border
  bool        checkable_   { false };             // are items checkable
  CIMenuBox*  parent_      { nullptr };           // parent box
  Items       items_;                             // child items
  mutable int numColumns_  { -1 };
};

//---

// menu item
class CIMenuItem {
 public:
  CIMenuItem() { }

  explicit CIMenuItem(const std::string &name) :
   name_(name) {
  }

  virtual ~CIMenuItem() { }

  CIMenuBase *base() const { return base_; }
  void setBase(CIMenuBase *p) { base_ = p; }

  // get/set name
  std::string getName() const { return name_; }
  void setName(const std::string &name) { name_ = name; }

  // get/set command
  std::string getCommand() const { return (command_.size() ? command_ : name_); }
  void setCommand(const std::string &command) { command_ = command; }

  // get/set is selectable
  bool isSelectable() const { return selectable_; }
  void setSelectable(bool b) { selectable_ = b; }

  // get/set is checled
  bool isChecked() const { return checked_; }
  void setChecked(bool b) { checked_ = b; }

  // get/set column
  void setColumn(int column) { column_ = column; }
  int getColumn() const { return column_; }

  // get/set row
  void setRow(int row) { row_ = row; }
  int getRow() const { return row_; }

  // get/set row span
  void setColumnSpan(int columnSpan) { columnSpan_ = columnSpan; }
  int getColumnSpan() const { return columnSpan_; }

  //---

  // draw
  virtual void draw();

  // handle press
  virtual void press();

 protected:
  CIMenuBase* base_       { nullptr };
  std::string name_;
  std::string command_;
  bool        selectable_ { true };
  bool        checked_    { false };
  int         column_     { 1 };
  int         row_        { -1 };
  int         columnSpan_ { 1 };
};

//---

class CIMenuText : public CIMenuItem {
 public:
  CIMenuText(const std::string &text) :
   CIMenuItem(text) {
    selectable_ = false;
  }

  // draw
  void draw() override;

  // handle press
  void press() override;
};

//---

class CIMenuApp;

// base class for menu
class CIMenuBase : public CIMenuBox {
 public:
  CIMenuBase();

  virtual ~CIMenuBase();

  //---

  // top level margin
  int xMargin() const { return xMargin_; }
  void setXMargin(int i) { xMargin_ = i; }

  int yMargin() const { return yMargin_; }
  void setYMargin(int i) { yMargin_ = i; }

  //---

  // column width
  int columnWidth() const { return columnWidth_; }
  void setColumnWidth(int i) { columnWidth_ = i; }

  //---

  // add item to top box
  CIMenuItem *addItem(const std::string &item) override;

  void addItem(CIMenuItem *item);

  //---

  virtual void initDrawItems();
  virtual void termDrawItems();

  void fixRow();

  //--

  virtual void enterItem() { }
  virtual void leaveItem() { }

  //---

  // get number of columns
  uint getNumColumns() const;

  //---

  // draw items
  virtual void drawItems();

  // draw cursor
  virtual void drawCursor() const;

  //---

  // get row/col character position
  virtual int getRowPos(int row) const;
  virtual int getColPos(int col) const;

  //---

  // get/set current row/col
  int  currentRow() const;
  void setCurrentRow(int row);

  int currentCol() const { return currentCol_; }
  void setCurrentCol(int col) { currentCol_ = col; }

  //---

  // get current item
  CIMenuItem *getCurrentItem() const;

  // get item at row in current column
  CIMenuItem *getItem(int row) const;

  // get item at row and column
  CIMenuItem *getItem(int row, int col) const;

  //---

  // get maximum number of rows in all columns
  uint getMaxRows() const;

  // get number of rows in current column
  uint getNumRows() const;

  // get number of rows in specified column
  uint getNumRows(int col) const;

  //---

  // handle key press
  void keyPress(const CKeyEvent &event);

 public:
  // display items and handle user inpu
  void mainLoop();

  // get user command
  std::string currentCommand() const;

  // get user checked command
  std::vector<std::string> checkedCommands() const;

  // run command
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

// menu app
class CIMenuApp : public CTermApp {
 public:
  CIMenuApp(CIMenuBase *menu) :
   menu_(menu) {
  }

  void redraw() override {
    menu_->drawItems();
  }

  void keyPress(const CKeyEvent &event) override {
    menu_->keyPress(event);
  }

 private:
  CIMenuBase *menu_ { nullptr };
};

#endif
