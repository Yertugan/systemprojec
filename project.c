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

int cpureq, ramreq;
char uploadReq[300], downloadReq[300];
char myUpload[1000], myDownload[1000];
char modelreq[300];

int mycpuhz;
char* mymodel;
int myram;
void get_requirements_speed(void)
{
	struct file *f;
	mm_segment_t fs;
	loff_t offset;
	offset = 0;
	char secret[300];

	f = filp_open("/home/yertugan/Desktop/project/requirements.txt",O_RDWR | O_CREAT,0644);
	fs = get_fs();
	set_fs(get_ds());
	vfs_read(f, secret, 300, &offset);
	
	set_fs(fs);
	//filp_close(f,NULL);
	//printk(KERN_ALERT "read: %s\n",secret);
	char* match;
	
	
	match = strstr (secret, "speed"); 
	if (match == NULL)  return 0;
	sscanf (match, "speed : %d", &cpureq);
	
	filp_close(f, NULL);
	
}

void get_requirements_ram(void)
{
	struct file *f;
	mm_segment_t fs;
	loff_t offset;
	offset = 0;
	char secret[300];

	f = filp_open("/home/yertugan/Desktop/project/requirements.txt",O_RDWR | O_CREAT,0644);
	fs = get_fs();
	set_fs(get_ds());
	vfs_read(f, secret, 300, &offset);
	
	set_fs(fs);
	//filp_close(f,NULL);
	//printk(KERN_ALERT "read: %s\n",secret);
	char* match;
	match = strstr (secret, "RAM"); 
	if (match == NULL)  return 0;
	sscanf (match, "RAM : %d", &ramreq);
	
	filp_close(f, NULL);
	
}
void get_req_upload_speed(void)
{
    
    struct file *f;
    mm_segment_t fs;
    loff_t offset;
    offset = 0;
    char secret[300];
    
    f = filp_open("/home/yertugan/Desktop/project/requirements.txt",O_RDWR | O_CREAT,0644);
    fs = get_fs();
    set_fs(get_ds());
    vfs_read(f, secret, 300, &offset);
    
    set_fs(fs);
   // filp_close(f,NULL);
    //printk(KERN_ALERT "read: %s\n",secret);
    char* match;
    match = strstr (secret, "Upload");
    if (match == NULL)  return 0;
    sscanf (match, "Upload: %s", &uploadReq);
    filp_close(f, NULL);
}
void get_req_download_speed(void)
{
    struct file *f;
    mm_segment_t fs;
    loff_t offset;
    offset = 0;
    char secret[300];
    
    f = filp_open("/home/yertugan/Desktop/project/requirements.txt",O_RDWR | O_CREAT,0644);
    fs = get_fs();
    set_fs(get_ds());
    vfs_read(f, secret, 300, &offset);
    
    set_fs(fs);
    //filp_close(f,NULL);
    //printk(KERN_ALERT "read: %s\n",secret);
    char* match;
    match = strstr (secret, "Download");
    if (match == NULL)  return 0;
    sscanf (match, "Download: %s", &downloadReq);
    filp_close(f, NULL);
}


void get_my_upload_speed(void)
{
    
    struct file *f;
    mm_segment_t fs;
    loff_t offset;
    offset = 0;
    char secret[1000];
    
    f = filp_open("/home/yertugan/Desktop/project/upload.txt",O_RDWR | O_CREAT,0644);
    fs = get_fs();
    set_fs(get_ds());
    vfs_read(f, secret, 1000, &offset);
    
    set_fs(fs);
    //filp_close(f,NULL);
    //printk(KERN_ALERT "read: %s\n",secret);
    char* match;
    match = strstr (secret, "Upload");
    if (match == NULL)  return 0;
    sscanf (match, "Upload: %s", &myUpload);
    filp_close(f, NULL);
}
void get_my_download_speed(void)
{
    struct file *f;
    mm_segment_t fs;
    loff_t offset;
    offset = 0;
    char secret[1000];
    
    f = filp_open("/home/yertugan/Desktop/project/download.txt",O_RDWR | O_CREAT,0644);
    fs = get_fs();
    set_fs(get_ds());
    vfs_read(f, secret, 1000, &offset);
    
    set_fs(fs);
    //filp_close(f,NULL);
    //printk(KERN_ALERT "read: %s\n",secret);
    char* match;
    match = strstr (secret, "Download");
    if (match == NULL)  return 0;
    sscanf (match, "Download: %s", &myDownload);
   	filp_close(f, NULL);
}


void get_requirements_model(void)
{
	struct file *f;
	mm_segment_t fs;
	loff_t offset;
	offset = 0;
	char secret[300];
	
	f = filp_open("/home/yertugan/Desktop/project/requirements.txt",O_RDWR | O_CREAT,0644);
	fs = get_fs();
	set_fs(get_ds());


	vfs_read(f, secret, 300, &offset);
	
	set_fs(fs);
	filp_close(f,NULL);
	
	char* match;
	match = strstr (secret, "model"); 
	if (match == NULL)  return 0;
	sscanf (match, "model : %s", modelreq);
	
	//filp_close(f, NULL);
	
	
}


int get_memory_usage(void)
{
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


static int __init k_init(void) 
{
	myram = get_memory_usage()/1024;
	mycpuhz = get_mycpuspeed();
    
	
	mymodel = get_mymodel();
	
	printk (KERN_INFO "My process model: %s\n", mymodel);
   
	get_my_download_speed();
	
	get_my_upload_speed();	
	get_requirements_speed();
	
	get_requirements_ram();
	
	get_requirements_model();

	printk (KERN_INFO "Needed process model: %s\n", modelreq);
	get_req_download_speed();
	
	get_req_upload_speed();
	
    
    printk("            |CPU speed  |       | RAM     |     download speed     |     upload speed  |\n");
    printk("Needed:     | %dMhz   |       | %dMB  |         %sMbit      |       %sMbit    |\n",cpureq, ramreq, downloadReq, uploadReq);
    printk("------------|-----------|-------|---------|------------------------|-------------------|\n");
    printk("Actual:     | %dMhz   |       | %dMB  |         %sMbit      |       %sMbit    |\n",mycpuhz, myram, myDownload, myUpload);

    

    
	//get_mydisk();
	//printk(KERN_INFO "read: %s\n",secret);
	//int space = 20;
////
	///char* n_model = "Intel(R)";
	//printk (KERN_INFO "Needed CPU: %dMhz\n", cpureq);
	
	//printk (KERN_INFO "test: %d\n", require);
	if(mycpuhz>=cpureq)
	{
		printk(KERN_INFO "Clock speed of cpu is enough\n");
	}else{
		printk(KERN_INFO "Clock speed of cpu is not enough\n");
	}

	if(myram>=ramreq)
	{
		printk(KERN_INFO "RAM is enough\n");
	}else{
		printk(KERN_INFO "RAM is not enough\n");
	}

	if(strcmp(modelreq, mymodel)){
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
