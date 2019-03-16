#include <linux/module.h>       
#include <linux/kernel.h>
#include <linux/cpumask.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/stat.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/irqnr.h>
#include <linux/sched/cputime.h>
#include <linux/tick.h>
#include <linux/uaccess.h>
#include <asm/types.h>

//#define PROCFS_MAX_SIZE	30
#define BUFFER_LEN 512
#define FILE_NAME_LEN 128

MODULE_LICENSE("GPL");

/*static char* file = NULL;
module_param( file, charp, 0 );

#define BUF_LEN 255
#define DEFNAME "/Desktop/project/requirements.txt"; // txt file
static char buff[ BUF_LEN + 1 ];

*/



/*
static int __init kread_init(void) {
	
	struct file *f;
	size_t n;
	if( file != NULL ) strcpy( buff, file );
	printk( "*** openning file: %s\n", buff );
	f = filp_open( DEFNAME, O_RDONLY, 0 );

	if( IS_ERR( f ) ) {
		printk( "*** file open failed: %s\n", buff );
		return -ENOENT;
	}

	n = kernel_read( f, 0, buff, 1);
	if( n ) {
		printk( "*** read first %ld bytes:\n", n );
		buff[ n ] = '\0';
		printk( "%s\n", buff );
	} else {
		printk( "*** kernel_read failed\n" );
		return -EIO;
	}

	printk( "*** close file\n" );
	filp_close( f, NULL );
	return -EPERM;
}*/

/*int get_requirements(void){
	char file_name[FILE_NAME_LEN];
	char buffer[BUFFER_LEN];
	char* match;
	int raminfo;
	sprintf(file_name, "~/Desktop/project/reqiurements.txt");
	struct file *f = filp_open(file_name, O_RDONLY, 0 );
	if(IS_ERR(f)){
		printk(KERN_INFO "Error reading file");
		return -1;
	}
	mm_segment_t fs = get_fs();
	set_fs(get_ds());
	f->f_op->read(f, buffer, BUFFER_LEN, &f->f_pos);
	set_fs(fs);

	match = strstr (buffer, "CPU"); 
	if (match == NULL)  return 0;
	sscanf (match, "CPU : %d", &raminfo);

	
	filp_close(f, NULL);
	return raminfo;
}*/

int myram;
int get_memory_usage(void){
	char file_name[FILE_NAME_LEN];
	char buffer[BUFFER_LEN];
	char* match;
	int raminfo;
	sprintf(file_name, "/proc/meminfo");
	struct file *f = filp_open(file_name, O_RDONLY, 0 );
	if(IS_ERR(f)){
		printk(KERN_INFO "Error reading file");
		return -1;
	}
	/* save addr_limit of the current process */
	mm_segment_t fs = get_fs();
	/* set addr_limit of the current process to that of kernel */
	set_fs(get_ds());
	f->f_op->read(f, buffer, BUFFER_LEN, &f->f_pos);
	/* set addr_limit of the current process to that of kernel */
	set_fs(fs);

	match = strstr (buffer, "MemTotal"); 
	if (match == NULL)  return 0;
	sscanf (match, "MemTotal: %d", &raminfo);

	
	filp_close(f, NULL);
	return raminfo;
}

int mycpuhz;
char* mymodel;

int get_mycpuspeed(void)
{
	char file_name[FILE_NAME_LEN];
	char buffer[BUFFER_LEN];
	char* match;
	int clock_speed;
	sprintf(file_name, "/proc/cpuinfo");
	struct file *f = filp_open(file_name, O_RDONLY, 0 );
	if(IS_ERR(f)){
		printk(KERN_INFO "Error reading file");
		return -1;
	}
	mm_segment_t fs = get_fs();
	set_fs(get_ds());
	f->f_op->read(f, buffer, BUFFER_LEN, &f->f_pos);
	set_fs(fs);

	match = strstr (buffer, "cpu MHz"); 
	if (match == NULL)  return 0; 
	sscanf (match, "cpu MHz : %d", &clock_speed);
	
	filp_close(f, NULL);
	return clock_speed;
}

char* get_mymodel(void)
{
	char file_name[FILE_NAME_LEN];
	char buffer[BUFFER_LEN];
	char* match;
	char* model;
	sprintf(file_name, "/proc/cpuinfo");
	struct file *f = filp_open(file_name, O_RDONLY, 0 );
	if(IS_ERR(f)){
		printk(KERN_INFO "Error reading file");
		return "-1";
	}
	mm_segment_t fs = get_fs();
	set_fs(get_ds());
	f->f_op->read(f, buffer, BUFFER_LEN, &f->f_pos);
	set_fs(fs);

	match = strstr (buffer, "model name"); 
	if (match == NULL)  return 0; 
	sscanf (match, "model name : %s", model);
	
	//printk(KERN_INFO "%s", buffer);
	filp_close(f, NULL);
	return model;
}
/*
int get_mydisk(void)
{
	char file_name[FILE_NAME_LEN];
	char buffer[BUFFER_LEN];
	char* match;
	int clock_speed;
	sprintf(file_name, "/dev/sda1");
	struct file *f = filp_open(file_name, O_RDONLY, 0 );
	if(IS_ERR(f)){
		printk(KERN_INFO "Error reading file");
		return -1;
	}
	mm_segment_t fs = get_fs();
	set_fs(get_ds());
	f->f_op->read(f, buffer, BUFFER_LEN, &f->f_pos);
	set_fs(fs);

	

	printk(KERN_INFO "test: %s", buffer);
	//printk(KERN_INFO "%s", buffer);
	filp_close(f, NULL);
	return 0;
}*/



static int __init k_init(void) {
	myram = get_memory_usage()/1024;
	mycpuhz = get_mycpuspeed();
	printk (KERN_INFO "My CPU: %dMhz\n", mycpuhz);
	printk (KERN_INFO "My RAM: %dMb\n", myram);
	mymodel = get_mymodel();
	printk (KERN_INFO "My process model: %s\n", mymodel);
	int cpu = 2000;
	int ram = 4096;
	//int space = 20;

	char* n_model = "Intel(R)";
	printk (KERN_INFO "Needed CPU: %dMhz\n", cpu);
	printk (KERN_INFO "Needed RAM: %dMb\n", ram);
	//printk (KERN_INFO "Needed Disc space: %dGb\n", space);
	printk (KERN_INFO "Needed process model: %s\n", n_model);
	//int require = get_requirements();
	
	//printk (KERN_INFO "test: %d\n", require);
	if(mycpuhz>=cpu)
	{
		printk(KERN_INFO "Clock speed of cpu is enough\n");
	}else{
		printk(KERN_INFO "Clock speed of cpu is not enough\n");
	}

	if(myram>=ram)
	{
		printk(KERN_INFO "RAM is enough\n");
	}else{
		printk(KERN_INFO "RAM is not enough\n");
	}

	if(strcmp(n_model, mymodel)){
		printk(KERN_INFO "Process model is the same\n");
	}else{
		printk(KERN_INFO "Process model is different\n");
		
	}
	
	return 0;
}

static void __exit k_exit(void)
{
    printk( KERN_INFO "! Module is unload... \n");
    
}

module_init(k_init);
module_exit(k_exit);
