#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in game.c
#include "game.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) { return x > 42; }

bool is_vowel(char c) {
  char *vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
  char msg[11];
  for (int i = 0; i < 128; i++) {
    sprintf(msg, "output_%d", i);
    char testcase = (char) i;
    bool output = is_tail(testcase);
    if (testcase == 'w' || testcase == 'a'
        || testcase == 's' || testcase == 'd') {
      if (!assert_true(msg, output)) {
        return false;
      }
    }
    else {
      if (!assert_false(msg, output)) {
        return false;
      }
    }
  }
  return true;
}

bool test_is_head() {
  char msg[11];
  for (int i = 0; i < 128; i++) {
    sprintf(msg, "output_%d", i);
    char testcase = (char) i;
    bool output = is_head(testcase);
    if (testcase == 'W' || testcase == 'A'
        || testcase == 'S' || testcase == 'D'
        || testcase == 'x') {
      if (!assert_true(msg, output)) {
        return false;
      }
    }
    else {
      if (!assert_false(msg, output)) {
        return false;
      }
    }
  }
  return true;
}

bool test_is_snake() {
  char msg[11];
  for (int i = 0; i < 128; i++) {
    sprintf(msg, "output_%d", i);
    char testcase = (char) i;
    bool output = is_snake(testcase);
    if (testcase == 'w' || testcase == 'a' || testcase == 's' || testcase == 'd'
        || testcase == '^' || testcase == '<' || testcase == 'v' || testcase == '>'
        || testcase == 'W' || testcase == 'A' || testcase == 'S' || testcase == 'D' || testcase == 'x') {
      if (!assert_true(msg, output)) {
        return false;
      }
    }
    else {
      if (!assert_false(msg, output)) {
        return false;
      }
    }
  }
  return true;
}

bool test_body_to_tail() {
  char testcase_1 = '^';
  char output_1 = body_to_tail(testcase_1);
  if (!assert_equals_char("testcase_1", 'w', output_1)) {
    return false;
  }

  char testcase_2 = '<';
  char output_2 = body_to_tail(testcase_2);
  if (!assert_equals_char("testcase_2", 'a', output_2)) {
    return false;
  }

  char testcase_3 = 'v';
  char output_3 = body_to_tail(testcase_3);
  if (!assert_equals_char("testcase_3", 's', output_3)) {
    return false;
  }

  char testcase_4 = '>';
  char output_4 = body_to_tail(testcase_4);
  if (!assert_equals_char("testcase_4", 'd', output_4)) {
    return false;
  }

  return true;
}

bool test_head_to_body() {
  char testcase_1 = 'W';
  char output_1 = head_to_body(testcase_1);
  if (!assert_equals_char("testcase_1", '^', output_1)) {
    return false;
  }

  char testcase_2 = 'A';
  char output_2 = head_to_body(testcase_2);
  if (!assert_equals_char("testcase_2", '<', output_2)) {
    return false;
  }

  char testcase_3 = 'S';
  char output_3 = head_to_body(testcase_3);
  if (!assert_equals_char("testcase_3", 'v', output_3)) {
    return false;
  }

  char testcase_4 = 'D';
  char output_4 = head_to_body(testcase_4);
  if (!assert_equals_char("testcase_4", '>', output_4)) {
    return false;
  }

  return true;
}

bool test_get_next_row() {
  char msg[19];

  for (unsigned int testcase_cur_row = 0; testcase_cur_row < 100; testcase_cur_row++) {
    for (int i = 0; i < 128; i++) {
      char testcase_c = (char) i;
      sprintf(msg, "(%u, %d)", testcase_cur_row, testcase_c);
      unsigned int output = get_next_row(testcase_cur_row, testcase_c);
      if (testcase_c == 'v' || testcase_c == 's' || testcase_c == 'S') {
        if (!assert_equals_unsigned_int(msg, testcase_cur_row + 1, output)) {
          return false;
        }
      }
      else if (testcase_c == '^' || testcase_c == 'w' || testcase_c == 'W') {
        if (!assert_equals_unsigned_int(msg, testcase_cur_row - 1, output)) {
          return false;
        }
      }
      else {
        if (!assert_equals_unsigned_int(msg, testcase_cur_row, output)) {
          return false;
        }
      }
    }
  }

  return true;
}

bool test_get_next_col() {
  char msg[19];

  for (unsigned int testcase_cur_col = 0; testcase_cur_col < 100; testcase_cur_col++) {
    for (int i = 0; i < 128; i++) {
      char testcase_c = (char) i;
      sprintf(msg, "(%u, %d)", testcase_cur_col, testcase_c);
      unsigned int output = get_next_col(testcase_cur_col, testcase_c);
      if (testcase_c == '>' || testcase_c == 'd' || testcase_c == 'D') {
        if (!assert_equals_unsigned_int(msg, testcase_cur_col + 1, output)) {
          return false;
        }
      }
      else if (testcase_c == '<' || testcase_c == 'a' || testcase_c == 'A') {
        if (!assert_equals_unsigned_int(msg, testcase_cur_col - 1, output)) {
          return false;
        }
      }
      else {
        if (!assert_equals_unsigned_int(msg, testcase_cur_col, output)) {
          return false;
        }
      }
    }
  }

  return true;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
