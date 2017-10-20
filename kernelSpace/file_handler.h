//
// Created by Emil Vannebäck on 2017-10-20.
//

#ifndef INC_5DV171_PROJECT_FILE_HANDLER_H
#define INC_5DV171_PROJECT_FILE_HANDLER_H

/**
 * Opens a file.
 * @param path: Path to the file.
 * @param flags: Flags set when opening file. Same flags as for fopen()
 * @param rights: If flag = O_CREAT this sets the permission to the file.
 * @return file: returns the opened file as a file struct
 */
struct file *file_open(const char *path, int flags, int rights);

/**
 * Closes the file.
 * @param file
 */
void file_close(struct file *file);

/**
 * Read data from a file.
 * @param file: The file to read from.
 * @param offset: The offset to start reading on.
 * @param data: The buffer to store the data read.
 * @param size: The size to read.
 * @return
 */
int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);

/**
 * Writes data to a file.
 * @param file: The file to write to.
 * @param offset: The offset to start writing on.
 * @param data: The data to be written.
 * @param size: Total size to write.
 * @return
 */
int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);

/**
 * Synchronizes the file to the file system.
 * @param file
 * @return
 */
int file_sync(struct file *file);

/**
 * read all the data in the file filp and stores the entire file to the buffer data:
 * @param filp: The file to read from.
 * @param data: The buffer to store the data to.
 * @return
 */
int read_file_to_buffer(struct file *filp, char **data);

#endif //INC_5DV171_PROJECT_FILE_HANDLER_H
