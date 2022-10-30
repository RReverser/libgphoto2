/* command.c
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

#include "config.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <gphoto2/gphoto2.h>

#include "command.h"

static const unsigned char sendaddr[8] = { 0x00, 0x22, 0x44, 0x66, 0x88, 0xaa, 0xcc, 0xee };
static const unsigned char recvaddr[8] = { 0x0e, 0x20, 0x42, 0x64, 0x86, 0xa8, 0xca, 0xec };

static const unsigned char BOFRAME = 0xC0;
static const unsigned char EOFRAME = 0xC1;
#define CESCAPE 0x7D

static int F1reset(Camera *camera);

static int
wbyte(GPPort *port,unsigned char c)
{
  unsigned char temp = c;
  return gp_port_write(port, (char*)&temp, 1);
}

static unsigned char
checksum(unsigned char addr, unsigned char *cp, int len)
{
  int ret = addr;
  while(len --)
    ret = ret + (*cp++);
  return(0x100 -(ret & 0xff) );
}

static void
sendcommand(Camera *camera,unsigned char *p, int len)
{
  GPPort *port = camera->port;
  unsigned char address = camera->pl->address;
  wbyte(port,BOFRAME);
  wbyte(port,sendaddr[address]);
  gp_port_write (port, (char*)p, len);
  wbyte(port,checksum(sendaddr[address], p, len));
  wbyte(port,EOFRAME);
  address ++;
  if(address >7 ) address = 0;
}

static void
Abort(Camera *camera)
{
  unsigned char buf[4];
  buf[0] = BOFRAME;
  buf[1] = 0x85;
  buf[2] = 0x7B;
  buf[3] = EOFRAME;
  gp_port_write (camera->port, (char*)buf, 4);
}

static int recvdata(Camera *camera, unsigned char *p, int len)
{
  unsigned char s, t;
  int sum;
  int i;

  GPPort *port = camera->port;
  unsigned char address = camera->pl->address;

  gp_log (GP_LOG_DEBUG, "recvdata", "reading %d bytes", len);
  gp_port_read(port, (char *)&s, 1);  /* BOFL */
  gp_port_read(port, (char *)&t, 1);  /* recvaddr */

  if(t != recvaddr[address]){
    gp_log (GP_LOG_ERROR, "recvdata", "address %02x does not match %02x, draining 3 bytes", t, recvaddr[address]);
    gp_port_read(port, (char *)&s, 1);  /* drain */
    gp_port_read(port, (char *)&s, 1);  /* drain */
    gp_port_read(port, (char *)&s, 1);  /* drain */
    Abort(camera);
    return(-1);
  }
  i = len;
  sum = (int) t;
  while ((GP_OK <= gp_port_read(port, (char *)&s, 1)) && (s != EOFRAME)) {
    sum = sum + s;
    if(i > 0) {
      if(s == CESCAPE){
        gp_port_read(port, (char *)&s, 1);
        if(0x20 & s)
          s = 0xDF & s;
        else
          s = 0x20 | s;
      }
      *p = s;
      p++;
      i--;
    }
    t = s;
  }
  gp_log (GP_LOG_DEBUG, "recvdata", "checksum expected %02x (have %02x)", t, sum );
  gp_log (GP_LOG_DEBUG, "recvdata", "EOFL %02x (%d)", s, len - i);
  if(sum & 0xff){
    gp_log (GP_LOG_ERROR, "recvdata" ,"Checksum error.(%02x)\n", sum);
    return(-1);
  }
  return(len - i);
}

/*------------------------------------------------------------*/


char F1newstatus(Camera *camera, int verbose, char *return_buf)
{
  unsigned char buf[34];
  char status_buf[1000]="";
  char tmp_buf[150]="";
  int i;

  buf[0] = 0x03;
  buf[1] = 0x02;
  sendcommand(camera,buf, 2);
  i = recvdata(camera, buf, 33);
  gp_log (GP_LOG_DEBUG, "F1newstatus", "Status: %02x%02x:%02x(len = %d)", buf[0], buf[1], buf[2], i);
  if((buf[0] != 0x03) || (buf[1] != 0x02) ||(buf[2] != 0)){
    Abort(camera);
    return(-1);
  }
  camera->pl->sw_mode = buf[3];
  camera->pl->pic_num = buf[4] * 0x100 + buf[5];
  camera->pl->pic_num2 = buf[6] * 0x100 + buf[7];
  camera->pl->year = (buf[10] >> 4 ) * 10 + (buf[10] & 0x0f);
  camera->pl->month = (buf[11] >> 4 ) * 10 + (buf[11] & 0x0f);
  camera->pl->date = (buf[12] >> 4 ) * 10 + (buf[12] & 0x0f);
  camera->pl->hour = (buf[13] >> 4 ) * 10 + (buf[13] & 0x0f);
  camera->pl->minutes = (buf[14] >> 4 ) * 10 + (buf[14] & 0x0f);

  if(verbose){
    strcat(status_buf, "Current camera statistics\n\n");
    strcat(status_buf, "Mode: ");
    switch (camera->pl->sw_mode){
    case 1:
      strcat(status_buf, "Playback\n");
      break;
    case 2:
      strcat(status_buf, "Record[Auto]\n");
      break;
    case 3:
      strcat(status_buf, "Record[Manual]\n");
      break;
    default:
      strcat(status_buf, "Huh?\n");
      break;
    }
    sprintf(tmp_buf, "Total Pictures: %02d\n", camera->pl->pic_num);
    strcat(status_buf, tmp_buf);
    sprintf(tmp_buf, "Date: %02d/%02d/%02d\n", camera->pl->month, camera->pl->date, camera->pl->year);
    strcat(status_buf, tmp_buf);
    sprintf(tmp_buf, "Time: %02d:%02d\n",camera->pl->hour, camera->pl->minutes);
    strcat(status_buf, tmp_buf);
  }

    strcpy(return_buf, status_buf);
    return (buf[2]);   /*ok*/
}


int F1status(Camera *camera)
{
  unsigned char buf[34];
  int i;

  buf[0] = 0x03;
  buf[1] = 0x02;
  sendcommand(camera,buf, 2);
  i = recvdata(camera, buf, 33);
  gp_log (GP_LOG_DEBUG, "F1status", "Status: %02x%02x:%02x(len = %d)\n", buf[0], buf[1], buf[2], i);
  if((buf[0] != 0x03) || (buf[1] != 0x02) ||(buf[2] != 0)){
    Abort(camera);
    return(-1);
  }
  camera->pl->sw_mode = buf[3];
  camera->pl->pic_num = buf[4] * 0x100 + buf[5];
  camera->pl->pic_num2 = buf[6] * 0x100 + buf[7];
  camera->pl->year = (buf[10] >> 4 ) * 10 + (buf[10] & 0x0f);
  camera->pl->month = (buf[11] >> 4 ) * 10 + (buf[11] & 0x0f);
  camera->pl->date = (buf[12] >> 4 ) * 10 + (buf[12] & 0x0f);
  camera->pl->hour = (buf[13] >> 4 ) * 10 + (buf[13] & 0x0f);
  camera->pl->minutes = (buf[14] >> 4 ) * 10 + (buf[14] & 0x0f);

  if(0){
    fprintf(stdout, "FnDial: ");
    switch (camera->pl->sw_mode){
    case 1:
      fprintf(stdout, "play\n");
      break;
    case 2:
      fprintf(stdout, "rec[A]\n");
      break;
    case 3:
      fprintf(stdout, "rec[M]\n");
      break;
    default:
      fprintf(stdout, "unknown?\n");
      break;
    }
    fprintf(stdout, "Picture: %3d\n", camera->pl->pic_num);
    fprintf(stdout,"Date: %02d/%02d/%02d\nTime: %02d:%02d\n",
            camera->pl->year, camera->pl->month, camera->pl->date,
            camera->pl->hour, camera->pl->minutes);
  }
  return (buf[2]);              /*ok*/
}

int F1howmany(Camera *camera)
{
  F1status(camera);
  return(camera->pl->pic_num);
}

int F1fopen(Camera *camera, char *name)
{
  unsigned char buf[64];
  int len;
  buf[0] = 0x02;
  buf[1] = 0x0A;
  buf[2] = 0x00;
  buf[3] = 0x00;
  snprintf((char*)&buf[4], sizeof(buf)-4, "%s", name);
  len = strlen(name) + 5;
  sendcommand(camera,buf, len);
  recvdata(camera, buf, 6);
  if((buf[0] != 0x02) || (buf[1] != 0x0A) || (buf[2] != 0x00)){
    Abort(camera);
    fprintf(stderr,"F1fopen fail\n");
    return(-1);
  }

  return(buf[3]);
}

int F1fclose(Camera *camera)
{
  unsigned char buf[4];
  int i;

  buf[0] = 0x02;
  buf[1] = 0x0B;
  buf[2] = 0x00;
  buf[3] = 0x00;
  sendcommand(camera,buf, 4);
  i = recvdata(camera, buf, 3);
  gp_log (GP_LOG_DEBUG, "F1fclose", "Fclose: %02x%02x:%02x(len = %d)\n", buf[0], buf[1], buf[2], i);
  if((buf[0] != 0x02) || (buf[1] != 0x0B) || (buf[2] != 0x00)){
    fprintf(stderr,"F1fclose fail\n");
    Abort(camera);
    return(-1);
  }
  return (buf[2]);              /* ok == 0 */
}

long F1fread(Camera *camera, unsigned char *data, long len)
{

  long len2;
  long i = 0;
  unsigned char s;

  unsigned char buf[10];

  buf[0] = 0x02;
  buf[1] = 0x0C;
  buf[2] = 0x00;
  buf[3] = 0x00;

  buf[4] = 0; /* data block size */
  buf[5] = 0;

  buf[6] = (len >> 8) & 0xff;
  buf[7] = 0xff & len;
  sendcommand(camera,buf, 8);

  GPPort *port = camera->port;
  gp_port_read(port, (char *)buf, 9);
  if((buf[2] != 0x02) || (buf[3] != 0x0C) || (buf[4] != 0x00)){
    Abort(camera);
    fprintf(stderr,"F1fread fail\n");
    return(-1);
  }

  len2 = buf[7] * 0x100 + buf[8]; /* data size */
  if(len2 == 0) {
    gp_port_read(port, (char *)&s, 1); /* last block checksum */
    gp_port_read(port, (char *)&s, 1); /* last block EOFL */
    return(0);
  }
  while((GP_OK <= gp_port_read(port, (char *)&s, 1)) && (s != EOFRAME)){
    if(s == CESCAPE){
      gp_port_read(port, (char *)&s, 1);
      if(0x20 & s)
        s = 0xDF & s;
      else
        s = 0x20 | s;
    }
    if(i < len)
      data[i] = s;
    i++;

  }
  i--; /* checksum */
  return(i);
}

long F1fseek(Camera *camera,long offset, int base)
{
  unsigned char buf[10];

  buf[0] = 0x02;
  buf[1] = 0x0E;
  buf[2] = 0x00;
  buf[3] = 0x00;

  buf[4] = (offset >> 24) & 0xff;
  buf[5] = (offset >> 16) & 0xff;
  buf[6] = (offset >> 8) & 0xff;
  buf[7] = 0xff & offset;

  buf[8] = (base >> 8) & 0xff;
  buf[9] = 0xff & base;

  sendcommand(camera,buf, 10);
  recvdata(camera, buf, 3);
  if((buf[0] != 0x02) || (buf[1] != 0x0E) || (buf[2] != 0x00)){
    Abort(camera);
    return(-1);
  }

  return(buf[2]);
}

long F1fwrite(Camera *camera,unsigned char *data, long len, unsigned char b) /* this function not work well */
{

  long i = 0;
  unsigned char *p;
  unsigned char s;
  unsigned char buf[10];

  int checksum;

  GPPort *port = camera->port;
  unsigned char address = camera->pl->address;

  p = data;
  wbyte(port,BOFRAME);
  wbyte(port,sendaddr[address]);
  wbyte(port,0x02);
  wbyte(port,0x14);
  wbyte(port,b);
  wbyte(port,0x00);
  wbyte(port,0x00);

  wbyte(port,(len >> 8) & 0xff);
  wbyte(port,0xff & len);

  checksum = sendaddr[address] +
    0x02 + 0x14 + b + ((len >> 8) & 0xff) + (0xff & len);

  while(i < len){
    s = *p;
    if((s == 0x7D) || (s == 0xC1) || (s == 0xC0)){
      wbyte(port,CESCAPE);
      if(0x20 & s)
        s = 0xDF & s;
      else
        s = 0x20 | s;
      checksum = checksum + CESCAPE;
      i++;
    }
    wbyte(port,s);
    checksum = checksum + s;
    i++;
    p++;
  }
  wbyte(port,0x100 -(checksum & 0xff) );
  wbyte(port,EOFRAME);
  address ++;
  if(address >7 ) address = 0;

  gp_port_read(port, (char *)buf, 7);
  if((buf[2] != 0x02) || (buf[3] != 0x14) || (buf[4] != 0x00)){
    Abort(camera);
    fprintf(stderr,"F1fwrite fail\n");
    return(-1);
  }

  return(i);
}

unsigned long F1finfo(Camera *camera,char *name)
{
  unsigned char buf[64];
  int len;
  unsigned long flen;

  buf[0] = 0x02;
  buf[1] = 0x0F;
  snprintf((char*)&buf[2], sizeof(buf)-2, "%s", name);
  len = strlen(name) + 3;

  sendcommand(camera,buf, len);
  len = recvdata(camera, buf, 37);
  if((buf[0] != 0x02) || (buf[1] != 0x0F) || (buf[2] != 00)){
    Abort(camera);
    return(0);
  }

  flen = buf[33] * 0x1000000 + buf[34] * 0x10000 +
    buf[35] * 0x100 + buf[36];
  gp_log (GP_LOG_DEBUG , "F1finfo", "inf len = %ld %02x %02x %02x %02x\n", flen,
          buf[33], buf[34], buf[35], buf[36]);

  if(buf[2] != 0) return(0);
  return(flen);
}

long F1getdata(Camera *camera,char *name, unsigned char *data)
{
  long filelen;
  long total = 0;
  long len;
  unsigned char *p;

  F1status(camera);
  p = data;
  filelen = F1finfo(camera,name);
  if(filelen < 0)
    return(0);

  if(F1fopen(camera,name) != 0)
    return(0);

  while((len = F1fread(camera, p, 0x0400)) != 0){
    if(len < 0){
      F1fclose(camera);
      return(0);
    }
    p = p + len;
    total = total + len;
  }
  F1fclose(camera);
  return(total);
}

int F1deletepicture(Camera *camera,int n)
{
  unsigned char buf[4];

  gp_log (GP_LOG_DEBUG, "F1deletepicture", "Deleting picture %d...", n);
  buf[0] = 0x02;
  buf[1] = 0x15;
  buf[2] = 0x00;
  buf[3] = 0xff & n;
  sendcommand(camera,buf, 4);
  recvdata(camera, buf, 3);
  if((buf[0] != 0x02) || (buf[1] != 0x15) || (buf[2] != 0)){
    Abort(camera);
    return GP_ERROR;
  }
  return GP_OK;
}

int F1ok(Camera *camera)
{
  int retrycount = 100;
  unsigned char buf[64];

  gp_log (GP_LOG_DEBUG, "F1ok", "Asking for OK...");

  buf[0] = 0x01;
  buf[1] = 0x01;
  sprintf((char*)&buf[2],"SONY     MKY-1001         1.00");

  while(retrycount--){
    sendcommand(camera,buf, 32);
    recvdata(camera, buf, 32);
    gp_log (GP_LOG_DEBUG, "F1ok", "OK:%02x%02x:%c%c%c%c\n", buf[0], buf[1],
            buf[3],buf[4],buf[5],buf[6]);
    if((buf[0] != 0x01) || (buf[1] != 0x01) || (buf[2] != 0x00) ){
      Abort(camera);
      F1reset(camera);
   } else
      return 1;
  }
  return 0;                     /*ng*/
}

static int
F1reset(Camera *camera)
{
  unsigned char buf[3];
  gp_log (GP_LOG_DEBUG, "F1reset", "Resetting camera...");
 retryreset:
  buf[0] = 0x01;
  buf[1] = 0x02;
  sendcommand(camera,buf, 2);
  recvdata(camera, buf, 3);
  gp_log (GP_LOG_DEBUG, "F1reset", "Reset: %02x%02x:%02x\n", buf[0], buf[1], buf[2]);
  if(!((buf[0] == 0x01 ) && (buf[1] == 0x02) && (buf[2] == 0x00)))
    goto retryreset;
  return (int) buf[2];          /*ok*/
}
