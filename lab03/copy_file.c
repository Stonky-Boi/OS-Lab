#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

const int BUFFER_SIZE = 4096;

static void write_string(int file_descriptor, const char *text)
{
    const char *current_position = text;
    while (*current_position != '\0')
    {
        ssize_t bytes_written = write(file_descriptor, current_position, 1);
        if (bytes_written < 0)
            _exit(1);
        current_position += bytes_written;
    }
}

static int read_filename(char *buffer, size_t buffer_size)
{
    ssize_t bytes_read = read(STDIN_FILENO, buffer, buffer_size - 1);
    if (bytes_read < 0)
        return -1;
    buffer[bytes_read] = '\0';
    if (bytes_read > 0 && buffer[bytes_read - 1] == '\n')
        buffer[bytes_read - 1] = '\0';
    return 0;
}

static int write_all(int file_descriptor, const char *buffer, size_t size)
{
    int total_written = 0;
    while (total_written < size)
    {
        ssize_t bytes_written = write(file_descriptor, buffer + total_written, size - total_written);
        if (bytes_written < 0)
        {
            if (errno == EINTR)
                continue;
            return -1;
        }
        total_written += (size_t)bytes_written;
    }
    return 0;
}

int main(void)
{
    char source_filename[1024];
    char destination_filename[1024];
    char buffer[BUFFER_SIZE];
    write_string(STDOUT_FILENO, "Enter source file name: ");
    if (read_filename(source_filename, sizeof(source_filename)) < 0)
    {
        write_string(STDERR_FILENO, "Error reading source filename\n");
        return 1;
    }
    write_string(STDOUT_FILENO, "Enter destination file name: ");
    if (read_filename(destination_filename, sizeof(destination_filename)) < 0)
    {
        write_string(STDERR_FILENO, "Error reading destination filename\n");
        return 1;
    }
    int source_file = open(source_filename, O_RDONLY);
    if (source_file < 0)
    {
        write_string(STDERR_FILENO, "Error opening source file\n");
        return 1;
    }
    int destination_file = open(destination_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_file < 0)
    {
        write_string(STDERR_FILENO, "Error opening destination file\n");
        close(source_file);
        return 1;
    }
    while (1)
    {
        ssize_t bytes_read = read(source_file, buffer, sizeof(buffer));
        if (bytes_read == 0)
            break;
        if (bytes_read < 0)
        {
            if (errno == EINTR)
                continue;
            write_string(STDERR_FILENO, "Error reading source file\n");
            close(source_file);
            close(destination_file);
            return 1;
        }
        if (write_all(destination_file, buffer, (size_t)bytes_read) < 0)
        {
            write_string(STDERR_FILENO, "Error writing destination file\n");
            close(source_file);
            close(destination_file);
            return 1;
        }
    }
    close(source_file);
    close(destination_file);
    write_string(STDOUT_FILENO, "File copied successfully\n");
    return 0;
}