#include <linux/init.h>
#include <linux/module.h>  
#include <linux/version.h>  
#include <linux/proc_fs.h>  

#include <linux/seq_file.h>
 
MODULE_LICENSE("Dual BSD/GPL");  
MODULE_AUTHOR("<freshpassport@gmail.com>");  
 
#define USER_ROOT_DIR "world_proc"  
#define USER_ENTRY1   "pt_entry1"  

static int hello_proc_show(struct seq_file *m, void *v) 
{
 	seq_printf(m, "Hello proc!\n");
 	return 0;
}

static int hello_proc_open(struct inode *inode, struct file *file) 
{
 	return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations proc_fops = {
 	.owner = THIS_MODULE,
 	.open = hello_proc_open,
 	.read = seq_read,
 	.llseek = seq_lseek,
 	.release = single_release,
};


// user defined directory  
static struct proc_dir_entry *pt_root;  
static struct proc_dir_entry *pt_entry1;  
 
static int proc_test_init(void)  
{  
 	// Create user root dir under /proc  
 	pt_root = proc_mkdir(USER_ROOT_DIR, NULL);  
  	if (NULL==pt_root)  
  	{  
    		printk(KERN_ALERT "Create dir /proc/%s error!\n",  
        		USER_ROOT_DIR);  
    		return -1;  
  	}  
  	printk(KERN_INFO "Create dir /proc/%s\n", USER_ROOT_DIR);  
 
  	// Create a test entry under USER_ROOT_DIR  
  	//pt_entry1 = create_proc_entry(USER_ENTRY1, 0666, pt_root);  //"create_proc_entry" is replaced by "create_proc" after 3.10 version kernel.
  	pt_entry1 = proc_create(USER_ENTRY1, 0666, pt_root, &proc_fops);  
  	if (NULL == pt_entry1)  
  	{  
    		printk(KERN_ALERT "Create entry %s under /proc/%s error!\n",  
        		USER_ENTRY1, USER_ROOT_DIR);  
    		goto err_out;  
  	}  
  	printk(KERN_INFO "Create /proc/%s/%s\n",  
      		USER_ROOT_DIR, USER_ENTRY1);  
 
 	// pt_entry1->read_proc = NULL;  
 	// pt_entry1->write_proc = NULL;  
 
  	return 0;  
 
	err_out:  
  		remove_proc_entry(USER_ROOT_DIR, pt_root);  
  		return -1;  
}  
 
static void proc_test_exit(void)  
{  
  	// Remove all entries  
  	remove_proc_entry(USER_ENTRY1, pt_root);  
  	remove_proc_entry(USER_ROOT_DIR, NULL);  
  	printk(KERN_INFO "All Proc Entry Removed!\n");  
}  
 
module_init(proc_test_init);  
module_exit(proc_test_exit);

