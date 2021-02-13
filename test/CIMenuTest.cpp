#include <CIMenu.h>
#include <iostream>
#include <cstdio>

int
main(int argc, char **argv)
{
  if (argc < 2) exit(1);

  using Items = std::vector<std::string>;

  std::string title;
  Items       items;
  bool        checkable = false;
  bool        border    = false;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      std::string arg = &argv[i][1];

      if      (arg == "checkable")
        checkable = true;
      else if (arg == "border")
        border = true;
      else if (arg == "title") {
        ++i;

        if (i < argc)
          title = argv[i];
        else {
          std::cerr << "Missing value for '-" << arg << "'\n";
          exit(1);
        }
      }
      else {
        std::cerr << "Invalid arg '" << arg << "'\n";
        exit(1);
      }
    }
    else {
      std::string item = argv[i];

      items.push_back(item);
    }
  }

  //---

  auto *menu = new CIMenuBase;

  if (border)
    menu->setBorderStyle(CIMenuBase::BorderStyle::LINE);

  if (checkable)
    menu->setCheckable(true);

  //---

  // set width
  int width     = 0;
  int maxColumn = 1;

  for (const auto &item : items) {
    auto p = item.find(':');

    if (p != std::string::npos) {
      try {
        int column = std::stoi(item.substr(p + 1));

        maxColumn = std::max(maxColumn, column);

        width = std::max(width, int(p));
      }
      catch (...) {
        width = std::max(width, int(item.size()));
      }
    }
    else {
      width = std::max(width, int(item.size()));
    }
  }

  menu->setColumnWidth(width + 2);

  //---

  if (! title.empty()) {
    CIMenuText *menuText = new CIMenuText(title);

    menuText->setColumnSpan(maxColumn);

    menu->addItem(menuText);
  }

  for (const auto &item : items) {
    auto p = item.find(':');

    if (p != std::string::npos) {
      try {
        int column = std::stoi(item.substr(p + 1));

        auto *menuItem = menu->addItem(item.substr(0, p));

        if (menuItem)
          menuItem->setColumn(column);
      }
      catch (...) {
        menu->addItem(item);
      }
    }
    else {
      menu->addItem(item);
    }
  }

  menu->mainLoop();

  using Commands = std::vector<std::string>;

  Commands commands;

  if (! checkable) {
    auto command = menu->currentCommand();

    commands.push_back(command);
  }
  else {
    commands = menu->checkedCommands();
  }

  //---

  // write result to file
  std::string homeDir  = getenv("HOME");
  std::string fileName = homeDir + "/.cimenu";

  FILE *fp = fopen(fileName.c_str(), "w");
  if (! fp) return 1;

  for (const auto &command : commands)
    fprintf(fp, "%s\n", command.c_str());

  fclose(fp);

  //---

  delete menu;

  return 0;
}
