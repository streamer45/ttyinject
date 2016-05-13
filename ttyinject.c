#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[]) {
  char c;
  char pc;
  int fd;
  int ret;
  int rc;
  struct termios old_term;
  struct termios term;
  struct pollfd fds[1];
  rc = 0;
  pc = 0;
  if (argc != 2) {
    printf("Usage:\n  %s /dev/tty1\n  %s /dev/pts/3\n", argv[0], argv[0]);
    exit(1);
  }
  fd = open(argv[1], O_WRONLY);
  if(fd < 0) {
    perror("open() failed");
    exit(1);
  }
  tcgetattr(0, &term);
  tcgetattr(0, &old_term);
  cfmakeraw(&term);
  tcsetattr(0, TCSANOW, &term);
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;
  for (;;) {
    ret = poll(fds, 1, 200);
    if (ret < 0) {
      perror("poll() failed");
      rc = 1;
      break;
    }
    if(fds[0].revents == POLLIN) {
      ret = read(0, &c, sizeof(c));
      if (ret != 1) {
        perror("read() failed");
        rc = 1;
        break;
      }
      if (c == 27 && pc == 27) break;
      ret = ioctl(fd, TIOCSTI, &c);
      if (ret == -1) {
        if (errno == 1) {
          fprintf(stderr, "%s: try running as root\n", strerror(errno));
          rc = 1;
          break;
        }
        if (fcntl(fd, F_GETFD) == 0) close(fd);
        fd = open(argv[1], O_RDWR);
        if(fd < 0) {
          perror("open() failed");
          rc = 1;
          break;
        }
        ioctl(fd, TIOCSTI, &c);
      }
      pc = c;
    }
  }
  tcsetattr(0, TCSANOW, &old_term);
  close(fd);
  return rc;
}
