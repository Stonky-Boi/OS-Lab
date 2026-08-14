#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_ENTRY_NAME "hello"
#define BUFFER_SIZE 100

static ssize_t process_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    static int completed = 0;
    char buffer[BUFFER_SIZE];
    int length = 0;
    if (completed)
    {
        completed = 0;
        return 0;
    }
    length = scnprintf(buffer, sizeof(buffer), "Hello World from Kernel Space! Current Jiffies: %lu\n", jiffies);
    if (copy_to_user(user_buffer, buffer, length) != 0)
        return -EFAULT;
    completed = 1;
    return length;
}

static const struct proc_ops process_operations = {
    .proc_read = process_read,
};

static int __init process_init(void)
{
    struct proc_dir_entry *entry;
    entry = proc_create(PROC_ENTRY_NAME, 0666, NULL, &process_operations);
    if (entry == NULL)
    {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_ENTRY_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROC_ENTRY_NAME);
    return 0;
}

static void __exit process_exit(void)
{
    remove_proc_entry(PROC_ENTRY_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_ENTRY_NAME);
}

module_init(process_init);
module_exit(process_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple /proc kernel module that reports current jiffies");
MODULE_AUTHOR("Arnav Kumar");