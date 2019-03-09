#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>


MODULE_LICENSE("GPL");


static char* file = NULL;
module_param( file, charp, 0 );

#define BUF_LEN 255
#define DEFNAME "/Desktop/project/requirements.txt"; // txt file
static char buff[ BUF_LEN + 1 ] = DEFNAME;




static int __init _init(void) {
	printk (KERN_INFO, "Hello\n");
	return 0;
}

static int __init kread_init(void) {
	
	struct file *f;
	size_t n;
	if( file != NULL ) strcpy( buff, file );
	printk( "*** openning file: %s\n", buff );
	f = filp_open( buff, O_RDONLY, 0 );

	if( IS_ERR( f ) ) {
		printk( "*** file open failed: %s\n", buff );
		return -ENOENT;
	}

	n = kernel_read( f, buff, 0, BUF_LEN);
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
}

static void __exit k_exit(void)
{
    printk( KERN_INFO "! Module is unload... \n");
    
}

module_init(_init);
module_exit(k_exit);
