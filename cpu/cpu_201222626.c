/**
 * cpu_201222626.c
 **/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/utsname.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

#define PROCFS_NAME "cpu_201222626"
#define EJECUCION "\tEjecutandose\n"
#define DURMIENDO "\tDurmiendo\n"
#define PARADO "\tParado\n"
#define MUERTO "\tMuerto\n"
#define OTRO "\tOtro\n"
#define ZOMBI "\tZombie\n"
#define IDLE "\tIdle\n"
static void estado(struct seq_file *m, u32 est);

static int cpumod_show(struct seq_file *m, void *v){
  seq_printf(m, "Carné: 201222626\n");
  seq_printf(m, "Nombre: Byron Jose Lopez Herrera\n");
  seq_printf(m, "Sistema Operativo: %s %s %s\n",
             utsname()->sysname, utsname()->release, utsname()->version);
  seq_printf(m, "PID\tNombre\tEstado\n");

  struct task_struct *task;
  rcu_read_lock();

  for_each_process(task){
    task_lock(task);
    seq_printf(m, "%i\t%s", task->pid, task->comm);
    estado (m, task->state);
    task_unlock(task);
  }
  rcu_read_unlock();
  return 0;
}

static void estado(struct seq_file *m, u32 est){
  switch(est){
  case TASK_RUNNING:
    seq_printf(m, EJECUCION);
    break;
  case TASK_INTERRUPTIBLE:
  case TASK_UNINTERRUPTIBLE:
    seq_printf(m, DURMIENDO);
    break;
  case __TASK_STOPPED:
  case __TASK_TRACED:
  case TASK_STOPPED:
    seq_printf(m, PARADO);
    break;
  case EXIT_ZOMBIE:
    seq_printf(m, ZOMBI);
    break;
  case TASK_DEAD:
    seq_printf(m, MUERTO);
    break;
  case TASK_IDLE:
    seq_printf(m, IDLE);
    break;
  default:
    seq_printf(m, OTRO);
    break;
  }
}

static int cpumod_open(struct inode *inode, struct file *file){
  return single_open(file, cpumod_show, NULL);
}

static const struct file_operations cpu_fops= {
  .owner = THIS_MODULE,
  .open = cpumod_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};


static int __init cpumod_init(void){
  printk(KERN_INFO "Byron Jose Lopez Herrera\n");
  proc_create(PROCFS_NAME, 0, NULL, &cpu_fops);
  printk(KERN_INFO "Completado. Proceso: /proc/%s.\n", PROCFS_NAME);

  return 0;
}

static void __exit cpumod_exit(void){
  remove_proc_entry(PROCFS_NAME, NULL);
  printk(KERN_INFO "Sistemas Operativos 1\n");
  printk(KERN_INFO "Modulo deshabilidato\n");
}

module_init(cpumod_init);
module_exit(cpumod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dragonsor");
MODULE_DESCRIPTION("Modulo realizado como practica de sistemas operativos1");

