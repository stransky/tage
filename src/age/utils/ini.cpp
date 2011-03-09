/*
 *        .þÛÛþ þ    þ þÛÛþ.     þ    þ þÛÛÛþ.  þÛÛÛþ .þÛÛþ. þ    þ
 *       .þ   Û Ûþ.  Û Û   þ.    Û    Û Û    þ  Û.    Û.   Û Ûþ.  Û
 *       Û    Û Û Û  Û Û    Û    Û   þ. Û.   Û  Û     Û    Û Û Û  Û
 *     .þþÛÛÛÛþ Û  Û Û þÛÛÛÛþþ.  þþÛÛ.  þþÛÛþ.  þÛ    Û    Û Û  Û Û
 *    .Û      Û Û  .þÛ Û      Û. Û   Û  Û    Û  Û.    þ.   Û Û  .þÛ
 *    þ.      þ þ    þ þ      .þ þ   .þ þ    .þ þÛÛÛþ .þÛÛþ. þ    þ
 *
 * Berusky (C) AnakreoN
 * Martin Stransky <stransky@anakreon.cz> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* ini file handling
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "ini.h"

char *ini_remove_end_of_line(char *p_line)
{
  char *p_start = p_line;

  while (*p_line && *p_line != '\n')
    p_line++;

  if (*p_line == '\n')
    *p_line = '\0';

  return (p_start);
}

bool ini_is_space(char c)
{
  return(c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool ini_is_separator(char c)
{
  return(c == SEPARATOR_INI);
}

char *ini_skip_spaces(char *p_line)
{
  while (*p_line && ini_is_space(*p_line))
    p_line++;
  return (p_line);
}

char *ini_skip_separator(char *p_line, char separator)
{
  char *p_tmp = strchr(p_line, separator);
  if (p_tmp) {
    return (ini_skip_spaces(p_tmp + 1));
  }
  else {
    return (NULL);
  }
}

char *ini_read_param(char *p_line, char *p_param, int max_len)
{
  char *p_start = p_param;
  int i = 0;

  p_line = ini_skip_spaces(p_line);
  
  while (*p_line && i < max_len - 1 && !ini_is_space(*p_line) && !ini_is_separator(*p_line)) {
    *p_param++ = *p_line++;
    i++;
  }
  *p_param = '\0';

  return (p_start);
}


char *ini_read_string(FFILE f, const char *p_template, char *p_out,
                      int max_len, const char *p_default)
{
  char line[MAX_TOKEN_LEN];

  fseek(f, SEEK_SET, 0);
  while (fgets(line, MAX_TOKEN_LEN, f)) {
    int len = is_token(line, p_template);
    char *p_rest;
    if (len && (p_rest = ini_skip_separator(line + len))) {
      return (ini_read_param(p_rest, p_out, max_len));
    }
  }

  return (strcpy(p_out, p_default));
}

char *ini_read_string(const char *p_file, const char *p_template, char *p_out,
                      int max_len, const char *p_default)
{
  char line[MAX_TOKEN_LEN];
  FFILE f(NULL, p_file, "r", FALSE);

  if (!f)
    return (strcpy(p_out, p_default));

  while (fgets(line, MAX_TOKEN_LEN, f)) {
    int len = is_token(line, p_template);
    char *p_rest;
    if (len && (p_rest = ini_skip_separator(line + len))) {
      fclose(f);
      return (ini_read_param(p_rest, p_out, max_len));
    }
  }

  fclose(f);
  return (strcpy(p_out, p_default));
}

int ini_read_int(FFILE f, const char *p_template, int dflt)
{
  char line[MAX_TOKEN_LEN];

  fseek(f, SEEK_SET, 0);
  while (fgets(line, MAX_TOKEN_LEN, f)) {
    int len = is_token(line, p_template);
    char *p_rest;
    if (len && (p_rest = ini_skip_separator(line + len))) {
      return (atoi(ini_remove_end_of_line(p_rest)));
    }
  }
  return (dflt);
}

int ini_read_int(const char *p_file, const char *p_template, int dflt)
{
  char line[MAX_TOKEN_LEN];
  FFILE f(NULL, p_file, "r", FALSE);

  if (!f)
    return (dflt);

  while (fgets(line, MAX_TOKEN_LEN, f)) {
    int len = is_token(line, p_template);
    char *p_rest;
    if (len && (p_rest = ini_skip_separator(line + len))) {
      fclose(f);
      return (atoi(ini_remove_end_of_line(p_rest)));
    }
  }
  fclose(f);
  return (dflt);
}

float ini_read_float(FFILE f, const char *p_template, float dflt)
{
  char line[MAX_TOKEN_LEN];

  fseek(f, SEEK_SET, 0);
  while (fgets(line, MAX_TOKEN_LEN, f)) {
    int len = is_token(line, p_template);
    char *p_rest;
    if (len && (p_rest = ini_skip_separator(line + len))) {
      return (atof(ini_remove_end_of_line(p_rest)));
    }
  }
  return (dflt);
}

float ini_read_float(const char *p_file, const char *p_template, int dflt)
{
  char line[MAX_TOKEN_LEN];
  FFILE f(NULL, p_file, "r", FALSE);

  if (!f)
    return (dflt);

  while (fgets(line, MAX_TOKEN_LEN, f)) {
    int len = is_token(line, p_template);
    char *p_rest;
    if (len && (p_rest = ini_skip_separator(line + len))) {
      fclose(f);
      return (atof(ini_remove_end_of_line(p_rest)));
    }
  }
  fclose(f);
  return (dflt);
}

int ini_read_bool(FFILE f, const char *p_template, int dflt)
{
  char line[MAX_TOKEN_LEN];

  ini_read_string(f, p_template, line, MAX_TOKEN_LEN, "");
  if (line[0] == '\0')
    return (dflt);
  else {
    if (is_token(line, TOKEN_FALSE1) || is_token(line, TOKEN_FALSE2))
      return (FALSE);
    else if (is_token(line, TOKEN_TRUE1) || is_token(line, TOKEN_TRUE2))
      return (TRUE);
    else
      return (dflt);
  }
}

int ini_read_bool(const char *p_file, const char *p_template, int dflt)
{
  char line[MAX_TOKEN_LEN];
  FFILE f(NULL, p_file, "r", FALSE);

  if (!f)
    return (dflt);

  ini_read_string(f, p_template, line, MAX_TOKEN_LEN, "");
  fclose(f);

  if (line[0] == '\0')
    return (dflt);
  else {
    if (is_token(line, TOKEN_FALSE1) || is_token(line, TOKEN_FALSE2))
      return (FALSE);
    else if (is_token(line, TOKEN_TRUE1) || is_token(line, TOKEN_TRUE2))
      return (TRUE);
    else
      return (dflt);
  }
}


/* Check if the given string is a token
*/
int is_token(const char *p_line, const char *p_token)
{
  const char *p_start = p_line;

  while (*p_line && *p_token && tolower(*p_line) == tolower(*p_token)) {
    p_line++;
    p_token++;
  }

  return (*p_token ? 0 : p_line - p_start);
}

/* Reading token (between %) from file
*/
int read_token(FFILE f_in, char *p_line, size_t max, char separator)
{
  size_t len;
  int c;

  for (len = 0;
       (c = fgetc(f_in)) != EOF && c != separator && len + 2 < max;
       len++, p_line++) {
    *p_line = c;
  }

  if (c == separator) {
    *p_line++ = c;
  }
  *p_line = 0;

  return (c == separator);
}

FFILE ini_open(const char *p_file, bool safe)
{
  FFILE f(NULL,p_file,"r",safe);
  return(f);
}

void ini_close(FFILE f)
{
  f.close();
}
