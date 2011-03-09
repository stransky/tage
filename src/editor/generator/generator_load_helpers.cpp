/*
 *        .þÛÛþ þ    þ þÛÛþ.     þ    þ þÛÛÛþ.  þÛÛÛþ .þÛÛþ. þ    þ
 *       .þ   Û Ûþ.  Û Û   þ.    Û    Û Û    þ  Û.    Û.   Û Ûþ.  Û
 *       Û    Û Û Û  Û Û    Û    Û   þ. Û.   Û  Û     Û    Û Û Û  Û
 *     .þþÛÛÛÛþ Û  Û Û þÛÛÛÛþþ.  þþÛÛ.  þþÛÛþ.  þÛ    Û    Û Û  Û Û
 *    .Û      Û Û  .þÛ Û      Û. Û   Û  Û    Û  Û.    þ.   Û Û  .þÛ
 *    þ.      þ þ    þ þ      .þ þ   .þ þ    .þ þÛÛÛþ .þÛÛþ. þ    þ
 *
 * AGE (C) AnakreoN
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

#include "editor.h"

//
// Load helpers
//

// (v1,v2,v3,v4)
bool load_line_vect(char *line, float &v1, float &v2, float &v3, float &v4)
{
  char *p_value = strdup(line);
  char *p_tmp = strtok(p_value,",");
  
  if(!p_tmp || *p_tmp != '(')
    return(FALSE);
  else
    p_tmp++;  
  v1 = atof(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  v2 = atof(p_tmp);

  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  v3 = atof(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  char *p_end = strchr(p_tmp,')');
  if(p_end)
    *p_end = '\0';  
  v4 = atof(p_tmp);
  
  ffree(p_value);
  return(TRUE);
}

// (v1,v2,v3)
bool load_line_vect(char *line, float &v1, float &v2, float &v3)
{
  char *p_value = strdup(line);
  char *p_tmp = strtok(p_value,",");
  
  if(!p_tmp || *p_tmp != '(')
    return(FALSE);
  else
    p_tmp++;  
  v1 = atof(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  v2 = atof(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  char *p_end = strchr(p_tmp,')');
  if(p_end)
    *p_end = '\0';  
  v3 = atof(p_tmp);
  
  ffree(p_value);
  return(TRUE);
}

// (v1,v2)
bool load_line_vect(char *line, float &v1, float &v2)
{
  char *p_value = strdup(line);
  char *p_tmp = strtok(p_value,",");
  
  if(!p_tmp || *p_tmp != '(')
    return(FALSE);
  else
    p_tmp++;  
  v1 = atof(p_tmp);
    
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  char *p_end = strchr(p_tmp,')');
  if(p_end)
    *p_end = '\0';  
  v2 = atof(p_tmp);
  
  ffree(p_value);
  return(TRUE);
}

// (v1,v2,v3,v4)
bool load_line_vect(char *line, int &v1, int &v2, int &v3, int &v4)
{
  char *p_value = strdup(line);
  char *p_tmp = strtok(p_value,",");
  
  if(!p_tmp || *p_tmp != '(')
    return(FALSE);
  else
    p_tmp++;  
  v1 = atoi(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  v2 = atoi(p_tmp);

  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  v3 = atoi(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  char *p_end = strchr(p_tmp,')');
  if(p_end)
    *p_end = '\0';  
  v4 = atoi(p_tmp);
  
  ffree(p_value);
  return(TRUE);
}

// (v1,v2,v3)
bool load_line_vect(char *line, int &v1, int &v2, int &v3)
{
  char *p_value = strdup(line);
  char *p_tmp = strtok(p_value,",");
  
  if(!p_tmp || *p_tmp != '(')
    return(FALSE);
  else
    p_tmp++;  
  v1 = atoi(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  v2 = atoi(p_tmp);
  
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  char *p_end = strchr(p_tmp,')');
  if(p_end)
    *p_end = '\0';  
  v3 = atoi(p_tmp);
  
  ffree(p_value);
  return(TRUE);
}

// (v1,v2)
bool load_line_vect(char *line, int &v1, int &v2)
{
  char *p_value = strdup(line);
  char *p_tmp = strtok(p_value,",");
  
  if(!p_tmp || *p_tmp != '(')
    return(FALSE);
  else
    p_tmp++;  
  v1 = atoi(p_tmp);
    
  p_tmp = strtok(NULL,",");
  if(!p_tmp)
    return(FALSE);
  char *p_end = strchr(p_tmp,')');
  if(p_end)
    *p_end = '\0';  
  v2 = atoi(p_tmp);
  
  ffree(p_value);
  return(TRUE);
}
