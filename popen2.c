#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <util.h>


pid_t popen2 (int *tty, char *cmd[]) {
  int master_fd;
  char *slave_dev = NULL;
  pid_t pid;
  
  if ((master_fd = posix_openpt(O_RDWR | O_NOCTTY)) < 0 ||
      grantpt(master_fd) != 0 ||
      unlockpt(master_fd) != 0 ||
      (slave_dev = ptsname(master_fd)) == NULL) return -1;

  int slave_fd = open(slave_dev, O_RDWR | O_NOCTTY);

  if (slave_fd == -1) return -1;

  pid = fork();
  if (pid < 0) return pid;

  if (pid == 0) {
    // child
    close(master_fd);

    struct termios slave_orig_term_settings; // Saved terminal settings
    struct termios new_term_settings; // Current terminal settings
    
    // Save the default parameters of the slave side of the PTY
    tcgetattr(slave_fd, &slave_orig_term_settings);

    // Set raw mode on the slave side of the PTY
    new_term_settings = slave_orig_term_settings;
    cfmakeraw (&new_term_settings);
    tcsetattr (slave_fd, TCSANOW, &new_term_settings);

    // The slave side of the PTY becomes the standard input and outputs of the child process
    dup2(slave_fd, 0);
    dup2(slave_fd, 1);
    dup2(slave_fd, 2);

    execvp(*cmd, cmd);
    perror("execvp");
    exit(1);
  }

  if (tty == NULL){
    close(master_fd);
  } else {
    *tty = master_fd;
  }

  return pid;
}
