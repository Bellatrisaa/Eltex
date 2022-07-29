#include <time.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ncurses.h>

#define MAX_PATH 2600

bool is_first_win = true;

void sort_dirent_arr(struct dirent **arr, int n)
{
  for (int i = 0; i < n; i++){
    for (int j = 1; j < n - i; j++) {
      if(strcmp(arr[j]->d_name, arr[j - 1]->d_name) < 0){
        struct dirent* temp = arr[j];
        arr[j] = arr[j - 1];
        arr[j - 1] = temp;
      }
    }
  }
}

void show_dirent_arr(struct dirent **arr1, struct dirent **arr2, int n1, int n2, int choice)
{
  int row, col;
  getmaxyx(stdscr, row, col);

  for (int i = 0; i < n1; i++) {
    if(i == choice && is_first_win) mvwprintw(stdscr, 1 + i, 0, ">");
    else mvwprintw(stdscr, 1 + i, 0, " ");
    mvwprintw(stdscr, 1 + i, 1, " %s \n", arr1[i]->d_name);
    if(arr1[i]->d_type == 4) mvwprintw(stdscr, 1 + i, 1 + col / 4, " folder \n");
  }

  for (int i = 0; i < n2; i++) {
    if(i == choice && !is_first_win) mvwprintw(stdscr, 1 + i, col / 2 + 1, ">");
    else mvwprintw(stdscr, 1 + i, col / 2 + 1, " ");
    mvwprintw(stdscr, 1 + i, col / 2 + 2, " %s \n", arr2[i]->d_name);
    if(arr2[i]->d_type == 4) mvwprintw(stdscr, 1 + i, col / 2 + 2 + col / 4, " folder \n");
  }

  for (int i = 0; i < row; i++)
  mvwprintw(stdscr, 0 + i, col / 2, "|");
}

int open_dir(char* dirname_1, char* dirname_2)
{
  DIR* dir1 = opendir(dirname_1);
  DIR* dir2 = opendir(dirname_2);

  if(!dir1) return 1;
  if(!dir2) return 1;

  struct dirent* catalog_1[MAX_PATH];
  struct dirent* catalog_2[MAX_PATH];

  int content_1_quantity = 0;
  int content_2_quantity = 0;

  for (; content_1_quantity < MAX_PATH; content_1_quantity++) {
    if(!(catalog_1[content_1_quantity] = readdir(dir1))) break;
  }

  for (; content_2_quantity < MAX_PATH; content_2_quantity++) {
    if(!(catalog_2[content_2_quantity] = readdir(dir2))) break;
  }

  sort_dirent_arr(catalog_1, content_1_quantity);
  sort_dirent_arr(catalog_2, content_2_quantity);

  int choice = 1;

  while(1){
    show_dirent_arr(catalog_1, catalog_2, content_1_quantity, content_2_quantity, choice);
    int key = getch();
    clear();
    refresh();
    if(is_first_win)
    switch (key) {
      case 10: if(catalog_1[choice]->d_type == 4) strcat(dirname_1, catalog_1[choice]->d_name); return 0;
      case 66: if(choice < content_1_quantity - 1) choice++; break;
      case 65: if(choice > 0) choice--; break;
      case 67: is_first_win = false; break;
      case 68: is_first_win = true; break;
    }
    else
    switch (key) {
      case 10: if(catalog_2[choice]->d_type == 4) strcat(dirname_2, catalog_2[choice]->d_name); return 0;
      case 66: if(choice < content_2_quantity - 1) choice++; break;
      case 65: if(choice > 0) choice--; break;
      case 67: is_first_win = false; break;
      case 68: is_first_win = true; break;
    }
  }

  return 0;
}

int main()
{
  initscr();

  char *dirname_1 = (char*) malloc(MAX_PATH);
  char *dirname_2 = (char*) malloc(MAX_PATH);

  strcpy(dirname_1, "/");
  strcpy(dirname_2, "/");

  while(true){
    if(open_dir(dirname_1, dirname_2)){
      printw("ERROR: Unable to open the folder \n");
      return 1;
    }
    if(is_first_win) strcat(dirname_1,"/");
    else strcat(dirname_2,"/");
  }

  endwin();
  free(dirname_1);
  free(dirname_2);

  return 0;
}
