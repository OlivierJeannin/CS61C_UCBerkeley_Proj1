#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_t *game, unsigned int snum);
static char next_square(game_t *game, unsigned int snum);
static void update_tail(game_t *game, unsigned int snum);
static void update_head(game_t *game, unsigned int snum);

/* Task 1 */
game_t *create_default_game() {
  // primary data of the default game
  unsigned int board_num_rows = 18;
  unsigned int board_num_cols = 20;
  char *board_edge_row   = "####################\n";
  char *board_middle_row = "#                  #\n";
  char *board_snake_row  = "# d>D    *         #\n";

  // game_t struct
  game_t *default_game = (game_t *) malloc(sizeof(game_t));
  if (default_game == NULL) exit(1);

  // board
  default_game->num_rows = board_num_rows;
  default_game->board = (char **) malloc(board_num_rows * sizeof(char *));
  if (default_game->board == NULL) exit(1);

  // allocate memory for board
  for (unsigned int i = 0; i < board_num_rows; i++) {
    default_game->board[i] = (char *) malloc((board_num_cols + 2) * sizeof(char));
    if (default_game->board[i] == NULL) exit(1);
  }

  // fill in board strings
  strcpy(default_game->board[0], board_edge_row);
  strcpy(default_game->board[1], board_middle_row);
  strcpy(default_game->board[2], board_snake_row);
  for (unsigned int i = 3; i <= board_num_rows - 2; i++) {
    strcpy(default_game->board[i], board_middle_row);
  }
  strcpy(default_game->board[board_num_rows-1], board_edge_row);

  default_game->num_snakes = 1;

  // snakes
  default_game->snakes = (snake_t *) malloc(sizeof(snake_t));
  if (default_game->snakes == NULL) exit(1);
  default_game->snakes[0] = (snake_t) {
    .tail_row = 2, .tail_col = 2,
    .head_row = 2, .head_col = 4,
    .live = true
  };

  return default_game;
}

/* Task 2 */
void free_game(game_t *game) {
  for (unsigned int i = 0; i < game->num_rows; i++) {
    free(game->board[i]);
  }
  free(game->board);
  free(game->snakes);
  free(game);
  return;
}

/* Task 3 */
void print_board(game_t *game, FILE *fp) {
  for (unsigned int i = 0; i < game->num_rows; i++) {
    fprintf(fp, "%s", game->board[i]);
  }
  return;
}

/*
  Saves the current game into filename. Does not modify the game object.
  (already implemented for you).
*/
void save_board(game_t *game, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(game, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_t *game, unsigned int row, unsigned int col) { return game->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch) {
  game->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  return (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  char *str = "wasd^<v>WASDx";
  while (*str != 0) {
    if (c == *str) {
      return true;
    }
    str++;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  switch (c) {
    case '^':
      return 'w';
    case '<':
      return 'a';
    case 'v':
      return 's';
    case '>':
      return 'd';
    default:
      return '?';
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  switch (c) {
    case 'W':
      return '^';
    case 'A':
      return '<';
    case 'S':
      return 'v';
    case 'D':
      return '>';
    default:
      return '?';
  }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  switch (c) {
    case 'v':
    case 's':
    case 'S':
      return cur_row + 1;
    case '^':
    case 'w':
    case 'W':
      return cur_row - 1;
    default:
      return cur_row;
  }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  switch (c) {
    case '>':
    case 'd':
    case 'D':
      return cur_col + 1;
    case '<':
    case 'a':
    case 'A':
      return cur_col - 1;
    default:
      return cur_col;
  }
}

/*
  Task 4.2

  Helper function for update_game. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_t *game, unsigned int snum) {
  unsigned int old_head_row = game->snakes[snum].head_row;
  unsigned int old_head_col = game->snakes[snum].head_col;

  char old_head = get_board_at(game, old_head_row, old_head_col);

  unsigned int new_head_row = get_next_row(old_head_row, old_head);
  unsigned int new_head_col = get_next_col(old_head_col, old_head);

  return get_board_at(game, new_head_row, new_head_col);
}

/*
  Task 4.3

  Helper function for update_game. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_t *game, unsigned int snum) {
  // compute the location of new head
  unsigned int old_head_row = game->snakes[snum].head_row;
  unsigned int old_head_col = game->snakes[snum].head_col;

  char old_head = get_board_at(game, old_head_row, old_head_col);

  unsigned int new_head_row = get_next_row(old_head_row, old_head);
  unsigned int new_head_col = get_next_col(old_head_col, old_head);

  // on the board
  set_board_at(game, new_head_row, new_head_col, old_head);
  set_board_at(game, old_head_row, old_head_col, head_to_body(old_head));

  // in the snake struct
  game->snakes[snum].head_row = new_head_row;
  game->snakes[snum].head_col = new_head_col;

  return;
}

/*
  Task 4.4

  Helper function for update_game. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_t *game, unsigned int snum) {
  // compute the location of new tail
  unsigned int old_tail_row = game->snakes[snum].tail_row;
  unsigned int old_tail_col = game->snakes[snum].tail_col;

  char old_tail = get_board_at(game, old_tail_row, old_tail_col);

  unsigned int new_tail_row = get_next_row(old_tail_row, old_tail);
  unsigned int new_tail_col = get_next_col(old_tail_col, old_tail);

  // on the board
  set_board_at(game, old_tail_row, old_tail_col, ' ');
  char body = get_board_at(game, new_tail_row, new_tail_col);
  set_board_at(game, new_tail_row, new_tail_col, body_to_tail(body));

  // in the snake struct
  game->snakes[snum].tail_row = new_tail_row;
  game->snakes[snum].tail_col = new_tail_col;

  return;
}

/* Task 4.5 */
void update_game(game_t *game, int (*add_food)(game_t *game)) {
  // update for each snake
  for (unsigned int i = 0; i < game->num_snakes; i++) {
    char ns = next_square(game, i);
    if (ns == ' ') {
      // take one step forward
      update_head(game, i);
      update_tail(game, i);
    }
    else if (is_snake(ns) || ns == '#') {
      // die
      snake_t *snake = game->snakes + i;
      snake->live = false;
      set_board_at(game, snake->head_row, snake->head_col, 'x');
    }
    else if (ns == '*') {
      // eat fruit
      update_head(game, i);
      add_food(game);
    }
    else {
      printf("Invalid character on the board. Can't happen.\n");
      exit(1);
    }
  }
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  size_t buf_size = 10;
  char *buf = (char *) malloc(buf_size * sizeof(char));
  if (buf == NULL) exit(1);

  // base case 1: error or empty line
  if (fgets(buf, (int) buf_size, fp) == NULL) {
    free(buf);
    return NULL;
  }

  // base case 2: line is short
  if (strchr(buf, '\n') != NULL) {
    buf = (char *) realloc(buf, strlen(buf) + 1);
    if (buf == NULL) exit(1);
    return buf;
  }

  // recursive case: line is very long
  char *remainder = read_line(fp);
  if (remainder == NULL) {
    // error
    free(buf);
    return NULL;
  }
  buf = (char *) realloc(buf, strlen(buf) + strlen(remainder) + 1);
  if (buf == NULL) exit(1);
  buf = strcat(buf, remainder);
  free(remainder);
  return buf;
}

/* Task 5.2 */
game_t *load_board(FILE *fp) {
  unsigned int num_rows_read = 0;
  char **board = (char **) malloc(sizeof(char *));
  if (board == NULL) exit(1);
  unsigned int board_capacity = 1;

  // read line from file
  char *line;
  while ((line = read_line(fp)) != NULL) {
    if (num_rows_read == board_capacity) {
      // resize board
      board_capacity *= 2;
      board = (char **) realloc(board, board_capacity * sizeof(char *));
      if (board == NULL) exit(1);
    }
    // add the line to board
    board[num_rows_read] = line;
    num_rows_read ++;
  }

  if (num_rows_read == 0) {  // empty file
    free(board);
    return NULL;
  }

  if (board_capacity > num_rows_read) {  // allocated too much memory
    board = (char **) realloc(board, num_rows_read * sizeof(char *));
    if (board == NULL) exit(1);
  }

  // fill in game struct
  game_t *game = (game_t *) malloc(sizeof(game_t));
  if (game == NULL) exit(1);
  game->num_rows = num_rows_read;
  game->board = board;
  game->num_snakes = 0;
  game->snakes = NULL;

  return game;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_t *initialize_snakes(game_t *game) {
  // TODO: Implement this function.
  return NULL;
}
