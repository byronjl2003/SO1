/**
*	mem_201222626.c
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
//#include <linux/quicklist.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <linux/utsname.h>
#include <linux/vmalloc.h>
#include <linux/percpu.h>
#ifdef CONFIG_CMA
#include <linux/cma.h>
#endif



#define PROCFS_NAME "mem_201222626"

static int memoria_show(struct seq_file *m, void *v){
  struct sysinfo i;

  unsigned long pages[NR_LRU_LISTS];

  int lru;

  #define K(x) ((x)<<(PAGE_SHIFT - 10))
  si_meminfo(&i);

  for(lru = LRU_BASE; lru < NR_LRU_LISTS; lru++){
    pages[lru] = global_node_page_state(NR_LRU_BASE + lru);
  }

  unsigned long total;
  unsigned long libre;
  total = K(i.totalram);
  libre = K(i.freeram);
  unsigned long prc ;
  unsigned long usado = total - libre;
  prc = usado * 10000 /total;
  unsigned long prc2 = usado * 100 / total;
  unsigned long dec = (prc - prc2 * 100);
  /**
   * total - 100
   * usado - x
   * */

  seq_printf(m, "Carné: 201222626\n");
  seq_printf(m, "Nombre: Byron Jose Lopez Herrera\n");
  seq_printf(m, "Sistema Operativo:%s %s %s\n",utsname()->sysname,utsname()->release, utsname()->version);
  seq_printf(m, "MemoriaTotal: %8lu kB\n", total);
  seq_printf(m, "MemoriaLibre: %8lu kB\n", libre);
  seq_printf(m, "MemoriaUsada: %lu.%lu %%\n", prc2, dec);

  return 0;
}

static int memoria_open(struct inode *inode, struct file *file){
  return single_open(file, memoria_show, NULL);
}

static const struct file_operations memoria_fops = {
  .owner = THIS_MODULE,
  .open = memoria_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init memoria_init(void){
	printk(KERN_INFO "201222626\n");
  proc_create(PROCFS_NAME, 0, NULL, &memoria_fops);
  printk(KERN_INFO "Completado. Proceso: /proc/%s.\n", PROCFS_NAME);
	return 0;
}

static void __exit memoria_exit(void){
  remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_INFO "Sistemas Operativos 1\n");
  printk(KERN_INFO "Modulo deshabilidato\n");
}

module_init(memoria_init);
module_exit(memoria_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("dragonsor");
MODULE_DESCRIPTION("Modulo realizado como practica dos de sistemas operativos1");

