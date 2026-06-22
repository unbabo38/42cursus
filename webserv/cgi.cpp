#include "cgi.hpp"
#include "unistd.h"
#include <sys/wait.h>
#include "Client.hpp"
#include <cstdlib>
std::string CGI::do_cgi(std::string cgi_path, std::string filepath, std::string query, Client* client) {
  std::cout << "do_cgi" << std::endl;

  int pipe_fd[2];
  if (pipe(pipe_fd) < 0) {
        return ""; // pipeのエラーハンドリングがあるとさらに強固です
  }
  int pid = fork();
  if (pid < 0) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return "";
  }

  if (pid == 0) {
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	char *argv[] = {
		(char *)cgi_path.c_str(),
		(char *)filepath.c_str(),
		NULL
	};
    std::string method = "REQUEST_METHOD=GET";
	std::string qs = "QUERY_STRING=" + query;

    char *envp[] = {
        (char *)method.c_str(),
        (char *)qs.c_str(),
        NULL
    };
	execve(argv[0], argv, envp);
	exit(1);
  }
  close(pipe_fd[1]);
  int flags = fcntl(pipe_fd[0], F_GETFL, 0);
  fcntl(pipe_fd[0], F_SETFL, flags | O_NONBLOCK);
  client->setCgiPid(pid);
  client->setCgiOutFd(pipe_fd[0]);
  client->setIsCgiRunning(true);

//   char buffer[4096];
//   std::string body;

//   ssize_t readsize;
//   while((readsize = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
// 	body.append(buffer, readsize);
//   close(pipe_fd[0]);
//   waitpid(pid, NULL, WNOHANG);
  return "";
}


std::string CGI::do_cgi_post(std::string cgi_path, std::string filepath, std::string body) {
  std::cout << "do_cgi_post" << std::endl;
  int in_pipe[2];
  int out_pipe[2];

  pipe(in_pipe);
  pipe(out_pipe);

  int pid = fork();

  if (pid == 0) {
	dup2(in_pipe[0], STDIN_FILENO);
	dup2(out_pipe[1], STDOUT_FILENO);
	close(in_pipe[0]);
	close(out_pipe[1]);
	close(in_pipe[1]);
	close(out_pipe[0]);
	char *argv[] = {
		(char *)cgi_path.c_str(),
		(char *)filepath.c_str(),
		NULL
	};
    std::string method = "REQUEST_METHOD=POST";
	std::stringstream ss;
	ss << body;
    std::string len = "CONTENT_LENGTH=" + ss.str().size();

    char *envp[] = {
        (char *)method.c_str(),
        (char *)len.c_str(),
        NULL
    };
	execve(argv[0], argv, envp);
	exit(1);
  }
  close(in_pipe[0]);
  close(out_pipe[1]);

  write(in_pipe[1], body.c_str(), body.size());
  close(in_pipe[1]);

  char buffer[4096];
  std::string result;

  ssize_t readsize;
  while((readsize = read(out_pipe[0], buffer, sizeof(buffer))) > 0)
	result.append(buffer, readsize);
  close(out_pipe[0]);
  waitpid(pid, NULL, 0);
  return result;
}
