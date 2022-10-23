int read(int fd, char *buf, int n){
  buf[0] = 'p';
  buf[1] = 'w';
  buf[2] = 'n';
  buf[3] = 'e';
  buf[4] = 'd';
  buf[5] = '\n';
  return 6;
}
