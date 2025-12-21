#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <fcntl.h>

#define TOKEN_BUFSIZE 64

bool check_if_in_background(char* line);
char *lsh_read_line(void);
void lsh_loop(void);
char **lsh_split_line(char* line);
int lsh_launch(char **args, bool inBackground);
int execute_piped_commands(char **commands, int num_pipes, bool inBackground);
int handle_redirections(char **args);
int lsh_execute(char **args, bool b);
int lsh_cd(char **args);
int lsh_exit(char **args);
char **split_pipes(char *line, int *num_pipes);

char *builtin_str[] = {
  "cd",
  "exit"
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_exit
};

int main(int argc, char **argv)
{
  lsh_loop();
  return EXIT_SUCCESS;
}

void lsh_loop(void) 
{
    char *line;
    char **commands;
    int status;
    int num_pipes = 0;

    do 
    {
        printf("> ");
        line = lsh_read_line();
        bool inBackground = check_if_in_background(line);

        // Rozdziel komendy z potokami
        commands = split_pipes(line, &num_pipes);

        if (num_pipes > 0) 
        {
            status = execute_piped_commands(commands, num_pipes, inBackground);
        } 
        else 
        {
            // Brak potoków, standardowe wykonanie
            char **args = lsh_split_line(commands[0]);
            status = lsh_execute(args, inBackground);
            free(args);
        }

        free(line);
        for (int i = 0; commands[i] != NULL; i++) 
        {
            free(commands[i]);
        }
        free(commands);
    } while (status);
}


char *lsh_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0;

  if (getline(&line, &bufsize, stdin) == -1)
  {
    if (feof(stdin)) 
    {
      exit(EXIT_SUCCESS);
    } 
    else  
    {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

char **lsh_split_line(char *line) 
{
    int bufsize = TOKEN_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    char *delimiters = " \t\n";

    if (!tokens) 
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Rozdzielanie linii na tokeny z uwzględnieniem specjalnych operatorów
    while (*line) 
    {
        // Pomiń białe znaki
        while (*line && strchr(delimiters, *line)) 
        {
            line++;
        }

        if (*line == '\0') break;

        // Sprawdź specjalne operatory
        if (*line == '|' || *line == '<' || *line == '>') 
        {
            size_t len = 1;
            if (*(line + 1) == '>') 
            { // Obsługa "2>"
                len = 2;
            }
            tokens[position++] = strndup(line, len);
            line += len;
        } 
        else 
        {
            // Normalne tokeny
            char *start = line;
            while (*line && !strchr(" \t\n|<>", *line)) 
            {
                line++;
            }
            tokens[position++] = strndup(start, line - start);
        }

        if (position >= bufsize) 
        {
            bufsize += TOKEN_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) 
            {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    tokens[position] = NULL;
    return tokens;
}


char **split_pipes(char *line, int *num_pipes) 
{
    char **commands = malloc(64 * sizeof(char *));
    char *token;
    int count = 0;

    if (!commands) 
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, "|");
    while (token != NULL) 
    {
        commands[count++] = strdup(token);
        token = strtok(NULL, "|");
    }
    commands[count] = NULL;
    *num_pipes = count - 1; // Liczba potoków to liczba komend - 1
    return commands;
}

int execute_piped_commands(char **commands, int num_pipes, bool inBackground) 
{
    int pipefds[2 * num_pipes];
    pid_t pid;
    int i;

    // Create pipes
    for (i = 0; i < num_pipes; i++) 
    {
        if (pipe(pipefds + i * 2) < 0) 
        {
            perror("lsh: pipe");
            return 1;
        }
    }

    for (i = 0; i <= num_pipes; i++) 
    {
        pid = fork();
        if (pid == 0) 
        {
            // Child process
            if (i > 0) 
            {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO); // Input from previous pipe
            }
            if (i < num_pipes) 
            {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO); // Output to next pipe
            }

            // Close all pipes in the child
            for (int k = 0; k < 2 * num_pipes; k++) 
            {
                close(pipefds[k]);
            }

            // Handle redirections
            char **args = lsh_split_line(commands[i]);
            if (handle_redirections(args) < 0) 
            {
                exit(EXIT_FAILURE);
            }

            if (execvp(args[0], args) < 0) 
            {
                perror("lsh");
                exit(EXIT_FAILURE);
            }
        } 
        else if (pid < 0) 
        {
            perror("lsh: fork");
            return 1;
        }
    }

    // Close all pipes in the parent
    for (i = 0; i < 2 * num_pipes; i++) 
    {
        close(pipefds[i]);
    }

    if (!inBackground) 
    {
        // Wait for all child processes
        for (i = 0; i <= num_pipes; i++) 
        {
            wait(NULL);
        }
    }
    else 
    {
        printf("Processes running in background\n");
        signal(SIGCHLD, SIG_IGN);
    }

    return 1;
}



int handle_redirections(char **args) 
{
    for (int i = 0; args[i] != NULL; i++) 
    {
        if (strcmp(args[i], ">") == 0) 
        {
            int fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) 
            {
                perror("lsh");
                return -1;
            }
            dup2(fd, STDOUT_FILENO); // Redirect stdout
            close(fd);
            args[i] = NULL; // Remove redirection from arguments
            args[i + 1] = NULL;
        } 
        else if (strcmp(args[i], "2>") == 0) 
        {
            int fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) 
            {
                perror("lsh");
                return -1;
            }
            dup2(fd, STDERR_FILENO); // Redirect stderr
            close(fd);
            args[i] = NULL; 
            args[i + 1] = NULL;
        } 
        else if (strcmp(args[i], "<") == 0) 
        {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) 
            {
                perror("lsh");
                return -1;
            }
            dup2(fd, STDIN_FILENO); // Redirect stdin
            close(fd);
            args[i] = NULL; 
            args[i + 1] = NULL;
        }
    }

    return 0;
}



int lsh_launch(char **args, bool inBackground)
{
    pid_t pid = fork();

    if (pid < 0) 
    {
        perror("lsh");
        return 1;
    } 
    else if (pid == 0) 
    {
        if (execvp(args[0], args) == -1) 
        {
            perror("lsh");
            exit(EXIT_FAILURE); 
        }
    } 
    else 
    {
        if (inBackground) 
        {
            printf("Process running in background with PID: %d\n", pid);
            signal(SIGCHLD, SIG_IGN);
        } 
        else 
        {
            int status;
            do 
            {
                pid_t child_pid = waitpid(pid, &status, WUNTRACED);
                if (child_pid == -1) 
                {
                    perror("waitpid");
                    return 1;
                }
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

    return 1;
}

void restore_file_descriptors(int saved_stdin, int saved_stdout, int saved_stderr) 
{
    if (dup2(saved_stdin, STDIN_FILENO) < 0) 
    {
        perror("dup2 restore stdin");
    }
    if (dup2(saved_stdout, STDOUT_FILENO) < 0) 
    {
        perror("dup2 restore stdout");
    }
    if (dup2(saved_stderr, STDERR_FILENO) < 0) 
    {
        perror("dup2 restore stderr");
    }

    // Close saved descriptors
    close(saved_stdin);
    close(saved_stdout);
    close(saved_stderr);
}

int lsh_execute(char **args, bool inBackground) 
{
    int saved_stdin = dup(STDIN_FILENO);  // Save original stdin
    int saved_stdout = dup(STDOUT_FILENO); // Save original stdout
    int saved_stderr = dup(STDERR_FILENO); // Save original stderr

    if (args[0] == NULL) 
    {
        return 1; // Pusta komenda
    }

    // Obsługa przekierowań
    if (handle_redirections(args) < 0) {
        return 1;
    }

    // Obsługa wbudowanych poleceń
    for (int i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    int status = lsh_launch(args, inBackground);
    restore_file_descriptors(saved_stdin, saved_stdout, saved_stderr);

    return status;
}


bool check_if_in_background(char *line) 
{
    size_t len = strlen(line);

    // Trim trailing whitespace
    while (len > 0 && isspace(line[len - 1])) {
        len--;
    }

    // Check if the line ends with '&'
    if (len > 0 && line[len - 1] == '&') 
    {
        line[len - 1] = '\0'; // Remove '&'
        len--;

        // Trim trailing whitespace after removing '&'
        while (len > 0 && isspace(line[len - 1])) {
            len--;
        }

        line[len] = '\0'; // Ensure the string is null-terminated
        return true; // Background process detected
    }

    return false; // Foreground process
}





int lsh_cd(char **args)
{
  if (args[1] == NULL) 
  {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } 
  else 
  {
    if (chdir(args[1]) != 0) 
    {
      perror("lsh");
    }
  }
  return 1;
}

int lsh_exit(char **args)
{
  return 0;
}
