#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_AUTHOR("Humphrey Winnebago");
MODULE_LICENSE("GPL");

#define PROC_NAME "blah"
#define TASKSIZE 2048

static unsigned m_numTasks = 5;

static void* seq_start(struct seq_file* unused_s, loff_t* pos) {
    static unsigned task = 0;

    printk(KERN_INFO "Start. Task: %u. Pos: %lld\n", task, *pos);

	if (task < m_numTasks) {
		return &task;
	}
    
    *pos = 0;
    task = 0;
    return NULL;
}

static void* seq_next(struct seq_file* unused_s, void* v, loff_t* pos) {
	unsigned* pTask = (unsigned*)v;

    printk(KERN_INFO "Next. Pos: %lld\n", *pos);

    *pos += TASKSIZE;
    *pTask += 1;

    if (*pTask < m_numTasks) {
        return pTask;
    }

	return NULL;
}

static void seq_stop(struct seq_file* unused_s, void* unused_v) {
    printk(KERN_INFO "Stop.\n");
}

static int seq_show(struct seq_file* s, void* v) {
    unsigned long* pTask = (unsigned long*)v;
    int i = 0;
    for (i = 0; i < TASKSIZE; ++i) {
        seq_printf(s, "%c", 'a' + (char)*pTask);
    }
    return 0;
}

static struct seq_operations seq_ops = {
    .start = seq_start,
    .next  = seq_next,
    .stop  = seq_stop,
    .show  = seq_show,
};

static int proc_open(struct inode *inode, struct  file *file) {
    return seq_open(file, &seq_ops);
}

static const struct file_operations proc_fops = {
    .owner   = THIS_MODULE,
    .open    = proc_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = seq_release,
};

static int __init mod_init(void) {
    printk(KERN_INFO "Blah loaded.\n");
    proc_create(PROC_NAME, 0, NULL, &proc_fops);
    return 0;
}

static void __exit mod_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "Blah unloaded.\n");
}

module_init(mod_init);
module_exit(mod_exit);
