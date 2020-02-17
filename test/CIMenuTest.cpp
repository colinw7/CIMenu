#include <CIMenu.h>
#include <iostream>
#include <cstdio>

int
main(int argc, char **argv)
{
  if (argc < 2) exit(1);

  using Items = std::vector<std::string>;

  Items items;
  bool  checkable = false;
  bool  border    = false;
  int   width     = 0;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      std::string arg = &argv[i][1];

      if      (arg == "checkable")
        checkable = true;
      else if (arg == "border")
        border = true;
      else {
        std::cerr << "Invalid arg '" << arg << "'\n";
        exit(1);
      }
    }
    else {
      std::string item = argv[i];

      width = std::max(width, int(items.size()));

      items.push_back(item);
    }
  }

  //---

  auto *menu = new CIMenuBase;

  if (border)
    menu->setBorderStyle(CIMenuBase::BorderStyle::LINE);

  if (checkable)
    menu->setCheckable(true);

  menu->setColumnWidth(width + 2);

  for (const auto &item : items)
    menu->addItem(item);

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
