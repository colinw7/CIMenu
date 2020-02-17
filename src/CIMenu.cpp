#include <CIMenu.h>

#include <COSRead.h>
#include <COSTerm.h>
#include <CFuncs.h>
#include <CEscape.h>

CIMenuBase::
CIMenuBase()
{
  app_ = new CIMenuApp(this);

  app_->setAutoExit(false);

  setCurrentRow(0);
}

CIMenuBase::
~CIMenuBase()
{
  clearItems();

  delete app_;
}

void
CIMenuBase::
updateState()
{
  COSTerm::getCharSize(&screenRows_, &screenCols_);
}

CIMenuItem *
CIMenuBase::
addItem(const std::string &name)
{
  CIMenuItem *item = CIMenuBox::addItem(name);

  item->setBase(this);

  return item;
}

void
CIMenuBase::
mainLoop()
{
  app_->mainLoop();
}

std::string
CIMenuBase::
currentCommand() const
{
  auto *item = getCurrentItem();

  if (! item)
    return "";

  return item->getCommand();
}

std::vector<std::string>
CIMenuBase::
checkedCommands() const
{
  std::vector<std::string> commands;

  for (const auto &item : items()) {
    if (item->isChecked())
      commands.push_back(item->getCommand());
  }

  return commands;
}

void
CIMenuBase::
runCommand(const std::string &cmd)
{
  app_->runCommand(cmd);
}

void
CIMenuBase::
keyPress(const CKeyEvent &event)
{
  const std::string &text = event.getText();

  char c = '\0';

  if (text.size() == 1)
    c = text[0];

  //---

  // search for matching menu item if alphabetic
  if (isalpha(c)) {
    int pos = 0;

    for (const auto &item : items()) {
      int col = item->getColumn() - 1;

      if (col != currentCol()) continue;

      std::string name = item->getName();

      if (name[0] == c) {
        setCurrentRow(pos);
        break;
      }

      ++pos;
    }

    return;
  }

  //---

  CKeyType type = event.getType();

  // accept
  if      (type == CKEY_TYPE_LineFeed || type == CKEY_TYPE_Return) {
    app_->setDone(true);
  }
  // next column
  else if (type == CKEY_TYPE_Greater) {
    if (currentCol() < int(getNumColumns()) - 1)
      setCurrentCol(currentCol() + 1);
  }
  // previous column
  else if (type == CKEY_TYPE_Less) {
    if (currentCol() >= 1)
      setCurrentCol(currentCol() - 1);
  }
  // previous row
  else if (type == CKEY_TYPE_Up) {
    int row = currentRow();

    if (row > 0)
      setCurrentRow(row - 1);
  }
  // next row
  else if (type == CKEY_TYPE_Down) {
    int row = currentRow();

    if (row < int(getNumRows()))
      setCurrentRow(row + 1);
  }
  // enter
  else if (type == CKEY_TYPE_Right) {
    enterItem();
  }
  // leave
  else if (type == CKEY_TYPE_Left) {
    leaveItem();
  }
  // press
  else if (type == CKEY_TYPE_Space) {
    auto *item = getCurrentItem();

    if (item)
      item->press();
  }
}

void
CIMenuBase::
drawItems()
{
  updateState();

  initItems();

  //---

  // clear screen
  COSRead::write(1, CEscape::ED(2));

  //---

  // update current row
  uint num_rows = getNumRows();

  int row = currentRow();

  if (row <  0            ) setCurrentRow(0);
  if (row >= int(num_rows)) setCurrentRow(num_rows - 1);

  //---

  // draw box
  if (borderStyle() != BorderStyle::NONE) {
    int c1 = getColPos(0) - 1;
    int c2 = getColPos(getNumColumns());
    int r1 = getRowPos(0) - 1;
    int r2 = getRowPos(getNumRows());

    drawBox(r1, c1, r2, c2);
  }

  // draw items
  currentColRow_.clear();

  for (const auto &item : items())
    drawItem(item);

  //---

  // draw cursor
  drawCursor();

  //---

  termItems();
}

void
CIMenuBase::
drawBox(int r1, int c1, int r2, int c2) const
{
  drawChar(r1, c1, '+');
  drawChar(r1, c2, '+');
  drawChar(r2, c1, '+');
  drawChar(r2, c2, '+');

  for (int r = r1 + 1; r <= r2 - 1; ++r) {
    drawChar(r, c1, '|');
    drawChar(r, c2, '|');
  }

  for (int c = c1 + 1; c <= c2 - 1; ++c) {
    drawChar(r1, c, '-');
    drawChar(r2, c, '-');
  }
}

void
CIMenuBase::
drawChar(int row, int col, char c) const
{
  COSRead::write(1, CEscape::CUP(row, col));
  COSRead::write(1, c);
}

void
CIMenuBase::
drawItem(CIMenuItem *item) const
{
  int col = item->getColumn() - 1;

  int row = 0;

  auto p = currentColRow_.find(col);

  if (p != currentColRow_.end())
    row = (*p).second;

  int rpos = getRowPos(row);
  int cpos = getColPos(col);

  COSRead::write(1, CEscape::CUP(rpos, cpos));

  item->draw();

  currentColRow_[col] = ++row;
}

void
CIMenuBase::
drawCursor() const
{
  int rpos = getRowPos(currentRow());
  int cpos = getColPos(currentCol()) - 2;

  COSRead::write(1, CEscape::CUP(rpos, cpos));

  COSRead::write(1, CEscape::SGR(32) + ">" + CEscape::SGR(0));
}

int
CIMenuBase::
getRowPos(int row) const
{
  int r = yMargin() + row + 1;

  if (borderStyle() != BorderStyle::NONE)
    ++r;

  return r;
}

int
CIMenuBase::
getColPos(int col) const
{
  int c = 1;

  // left margin
  c += xMargin();

  // left border
  if (borderStyle() != BorderStyle::NONE)
    ++c;

  if (col > 0) {
    // column contents + gap
    c += col*(columnWidth() + 2);

    // check (TODO: per column
    if (isCheckable())
      c += col*4;

    if (borderStyle() != BorderStyle::NONE)
      c += col;
  }

  return c;
}

CIMenuItem *
CIMenuBase::
getCurrentItem() const
{
  return getItem(currentRow());
}

CIMenuItem *
CIMenuBase::
getItem(int row) const
{
  return getItem(row, currentCol());
}

CIMenuItem *
CIMenuBase::
getItem(int row, int col) const
{
  int pos = 0;

  for (const auto &item : items()) {
    int col1 = item->getColumn() - 1;

    if (col != col1) continue;

    if (pos == row)
      return item;

    ++pos;
  }

  return NULL;
}

uint
CIMenuBase::
getNumRows() const
{
  return getNumRows(currentCol());
}

uint
CIMenuBase::
getNumRows(int col) const
{
  int num = 0;

  for (const auto &item : items()) {
    int col1 = item->getColumn() - 1;

    if (col != col1) continue;

    ++num;
  }

  return num;
}

int
CIMenuBase::
currentRow() const
{
  auto p = cursorColRow_.find(currentCol());

  if (p == cursorColRow_.end()) {
    CIMenuBase *th = const_cast<CIMenuBase *>(this);

    th->cursorColRow_[currentCol()] = 0;
  }

  return (*p).second;
}

void
CIMenuBase::
setCurrentRow(int row)
{
  cursorColRow_[currentCol()] = row;
}

//-------------

void
CIMenuBox::
clearItems()
{
  for (auto &item : items_)
    delete item;

  items_.clear();
}

CIMenuItem *
CIMenuBox::
addItem(const std::string &name)
{
  auto *item = new CIMenuItem(name);

  addItem(item);

  return item;
}

void
CIMenuBox::
addItem(CIMenuItem *item)
{
  items_.push_back(item);
}

//-------------

void
CIMenuItem::
draw()
{
  if (base_ && base_->isCheckable()) {
    COSRead::write(1, CEscape::SGR(31) + " [");

    if (isChecked())
      COSRead::write(1, "+");
    else
      COSRead::write(1, " ");

    COSRead::write(1, "]" + CEscape::SGR(0));
  }

  std::string name = getName();

  COSRead::write(1, " " + CEscape::SGR(31) + name + CEscape::SGR(0));
}

void
CIMenuItem::
press()
{
  if (base_ && base_->isCheckable()) {
    setChecked(! isChecked());

    if (base_)
      base_->drawItems();
  }
}
