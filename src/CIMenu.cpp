#include <CIMenu.h>

#include <COSRead.h>
#include <COSTerm.h>
#include <CFuncs.h>
#include <CEscape.h>

#include <cassert>

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
addItem(CIMenuItem *item)
{
  CIMenuBox::addItem(item);

  item->setBase(this);
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

      if (item->isSelectable()) {
        std::string name = item->getName();

        if (name[0] == c) {
          setCurrentRow(pos);
          break;
        }
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
    if (currentCol() < int(getNumColumns()) - 1) {
      setCurrentCol(currentCol() + 1);

      fixRow();
    }
  }
  // previous column
  else if (type == CKEY_TYPE_Less) {
    if (currentCol() >= 1) {
      setCurrentCol(currentCol() - 1);

      fixRow();
    }
  }
  // previous row
  else if (type == CKEY_TYPE_Up) {
    int row = currentRow();

    if (row > 0) {
      int row1 = row - 1;

      while (row1 >= 0) {
        CIMenuItem *item1 = getItem(row1);

        if (item1 && item1->isSelectable())
          break;

        --row1;
      }

      if (row1 >= 0)
        setCurrentRow(row1);
    }
  }
  // next row
  else if (type == CKEY_TYPE_Down) {
    int row = currentRow();

    int nr = getNumRows();

    if (row < nr - 1) {
      int row1 = row + 1;

      while (row1 < nr) {
        CIMenuItem *item1 = getItem(row1);

        if (item1 && item1->isSelectable())
          break;

        ++row1;
      }

      if (row1 < nr)
        setCurrentRow(row1);
    }
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

uint
CIMenuBase::
getNumColumns() const
{
  if (numColumns_ > 0)
    return numColumns_;

  if (items().empty())
    return 0;

  numColumns_ = 1;

  for (const auto &item : items()) {
    numColumns_ = std::max(numColumns_, item->getColumn());
  }

  return numColumns_;
}

void
CIMenuBase::
drawItems()
{
  updateState();

  initDrawItems();

  //---

  // clear screen
  COSRead::write(1, CEscape::ED(2));

  //---

  // draw box
  if (borderStyle() != BorderStyle::NONE) {
    int c1 = getColPos(0) - 3;
    int c2 = getColPos(getNumColumns());
    int r1 = getRowPos(0) - 1;
    int r2 = getRowPos(getMaxRows());

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

  termDrawItems();
}

void
CIMenuBase::
initDrawItems()
{
  using ColumnRowCount = std::map<int,int>;

  ColumnRowCount columnRowCount;

  for (const auto &item : items()) {
    int col = item->getColumn();
    int row = 1;

    for (int ic = 0; ic < item->getColumnSpan(); ++ic) {
      auto p = columnRowCount.find(col + ic);

      if (p == columnRowCount.end())
        p = columnRowCount.insert(p, ColumnRow::value_type(col + ic, 0));

      ++(*p).second;

      if (ic == 0)
        row = (*p).second;
    }

    item->setRow(row);
  }

  //---

  // update current row
  int nr = getNumRows();

  int row = currentRow();

  if      (row < 0)
    setCurrentRow(0);
  else if (row >= nr)
    setCurrentRow(nr - 1);

  fixRow();
}

void
CIMenuBase::
termDrawItems()
{
}

void
CIMenuBase::
fixRow()
{
  int nr = getNumRows();

  int row = currentRow();

  while (row < nr) {
    CIMenuItem *item1 = getItem(row);

    if (item1 && item1->isSelectable())
      break;

    ++row;
  }

  while (row >= 0) {
    CIMenuItem *item1 = getItem(row);

    if (item1 && item1->isSelectable())
      break;

    --row;
  }

  if (row >= 0)
    setCurrentRow(row);
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
  int row = item->getRow   () - 1;

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
  int cpos = getColPos(currentCol()) - 1;

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
    // column contents + 2*gap (1)
    c += col*(columnWidth() + 3);

    // check (TODO: per column)
    if (isCheckable())
      c += col*4;

    if (borderStyle() != BorderStyle::NONE)
      c += col;
  }

  c += 2; // cursor

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
  for (const auto &item : items()) {
    int row1 = item->getRow   () - 1;
    int col1 = item->getColumn() - 1;

    if (row == row1 && col == col1)
      return item;
  }

  return nullptr;
}

uint
CIMenuBase::
getMaxRows() const
{
  int row = 0;

  for (const auto &item : items())
    row = std::max(row, item->getRow());

  return row;
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

  numColumns_ = -1;
}

//-------------

void
CIMenuItem::
draw()
{
  assert(base_);

  int col = getColumn() - 1;
  int row = getRow   () - 1;

  int rpos = base_->getRowPos(row);
  int cpos = base_->getColPos(col);

  COSRead::write(1, CEscape::CUP(rpos, cpos));

  if (base_->isCheckable()) {
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

//-------------

void
CIMenuText::
draw()
{
  assert(base_);

  int col = getColumn() - 2;
  int row = getRow   () - 1;

  int rpos = base_->getRowPos(row);
  int cpos = base_->getColPos(col);

  COSRead::write(1, CEscape::CUP(rpos, cpos));

  std::string name = getName();

  COSRead::write(1, CEscape::SGR(31) + name + CEscape::SGR(0));
}

void
CIMenuText::
press()
{
}
