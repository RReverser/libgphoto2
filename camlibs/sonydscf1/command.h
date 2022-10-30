/* command.h
 *
 * Copyright (C) M. Adam Kendall <joker@penguinpub.com>
 * Copyright (C) 2002 Bart van Leeuwen <bart@netage.nl>
 *
 * Based on the chotplay CLI interface from Ken-ichi Hayashi 1996,1997
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef CAMLIBS_SONYDSCF1_COMMAND_H
#define CAMLIBS_SONYDSCF1_COMMAND_H

#define MAX_PICTURE_NUM 200

typedef struct {
  unsigned char index;
  unsigned short thumbnail_index;
  unsigned char protect;
  unsigned char rotate;
} picture_t;

struct _CameraPrivateLibrary {
  picture_t pictures[MAX_PICTURE_NUM];
  unsigned char address;
  int sw_mode;
  int pic_num;
  int pic_num2;
  int year, month, date;
  int hour, minutes;
};

int	F1ok (Camera *);
long	F1getdata (Camera *,char *, unsigned char *);
int	F1status (Camera *);
char	F1newstatus (Camera *port, int, char *);
int	F1howmany (Camera *);
int	F1fopen (Camera *,char *);
long	F1fread(Camera *,unsigned char *data, long len);
long	F1fwrite(Camera *,unsigned char *data, long len, unsigned char b);
long	F1fseek (Camera *,long, int);
unsigned long	F1finfo (Camera *,char *);
int	F1fclose (Camera *);
int	F1deletepicture (Camera *,int);

#endif /* !defined(CAMLIBS_SONYDSCF1_COMMAND_H) */
