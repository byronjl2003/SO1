# Practica 2 Sistemas Operativos 1
### Byron Jose Lopez Herrera
### 201222626


## Instalacion del hipervisor KVM en Fedora 29
1. Para instalar el hipervisor kvm es necesario verificar si nuestro cpu tiene la virtualizacion habilitada con el siguiente comando.
    ```bash
    $ cat /proc/cpuinfo | egrep "vmx|svm"
    ```
2. Instalar paquetes para virtualizar.
    ```bash
    $ sudo dnf -y install bridge-utils libvirt virt-install qemu-kvm
    ```
3. Habilitar e iniciar el proceso de KVM.
    ```bash
    $ sudo sudo systemctl start libvirtd
    $ sudo systemctl enable libvirtd
    ```
4. Instalar el gestor grafico para KVM
    ```bash
    $ sudo dnf -y install virt-manager
    ```
## Instalacion de Debian 10

La instalacion de Debian 10 es fastante facil  e intuitiva.Acontinuacion se muestran imagenes de algunos de los pasos que se siguen.
Algunas cosas a tomar en cuenta para la instancia para poder compilar el kernel sin ningun problema.
- Darle por lo menos a la maquina unos 40 GB de espacio.
- Si es posible darle la mayoria de hilos de ejecucion en mi caso son 3 nucleos.

### Imagenes

![debian0][debian0]

[debian0]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/creacion%20de%20vm1.png

![debian1][debian1]

[debian1]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/instalacionvm2.png


![debian2][debian2]

[debian2]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/instalacionvm4.png


![debian4][debian4]

[debian4]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/instalacion%20vm4.png


![debian5][debian5]

[debian5]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/instalacion%20vm4.png


## Compilacion del kernel
1. Instalar las librerias de GCC y otras herramientas para la compilacion del kernel.
    ```bash
    $ sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev
    ```

2. Descargar desde la pagina oficial del kernel linux los binarios.

    ```bash
    $ tar -Jxvf linux-5.2.13
    ```

    ![kernel0][kernel0]

    [kernel0]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/descarga-kernel.png


3. Extraer los archivos en cualquier directorio de preferencia.
    
    ![kernel1][kernel1]

    [kernel1]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/estraccion-archivosKernel.png



4. Ingreso a la carpeta que se descomprimiio con los archivos del kernel.
    
    ![kernel2][kernel2]

    [kernel2]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/ingreso%20carpeta-kernel.png

5. Utilizar el siguiente comando para limpiar el cache de make.
    

    ```bash
    $ make clean
    ```

    ![kernel3][kernel3]

    [kernel3]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/make-clean.png

5. Utilizar el siguiente comando copiar las configuraciones del kernel.
    

    ```bash
    $ cp -v /boot/config-$(uname -r) .config
    ```

    ![kernel4][kernel4]

    [kernel4]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/cp%20config.png

6. Utilizar el siguiente comando para seleccionar los modulos para instalar del kernel. Para nuestro caso instalaremos todo, lo que no se configura nada.
    ```bash
    $ make menuconfig
    ```

    ![kernel5][kernel5]

    [kernel5]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/menuconfig.png

    ![kernel6][kernel6]

    [kernel6]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/okmenuconfig.png

    ![kernel7][kernel7]

    [kernel7]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/pantallamenuconfig.png

    7. Si tu sistema utiliza Uefi es necesario comentar las siguientes lineas para que no falle la compilacion del kernel.

        ![kernel8][kernel8]

        [kernel8]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/uefi-error.png

    8. Utilizar los siguiente comando para inicializar la compilacion.El numero 3 es numero maximo de hilos que tiene capacidad mi VM.

        ```bash
        $ make -j 3
        ```

        ![kernel9][kernel9]

        [kernel9]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/make-j3.png

         ```bash
        $ make modules -j 3
        ```

        ![kernel10][kernel10]

        [kernel10]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/make-modules.png

         ```bash
        $ sudo make modules_install
        ```

        ![kernel11][kernel11]

        [kernel11]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/sudoMakeModulesInstall.png

         ```bash
        $ sudo make install
        ```

        ![kernel12][kernel12]

        [kernel12]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/makeInstalll.png

         ```bash
        $ sudo update-initramfs -c -k 5.2.13 && sudo update-grub 
        ```

        ![kernel13][kernel13]

        [kernel13]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/ultimos-kernel.png

    9. Reiniciar la VM y se podra ver en el GRUB el nuevo kernel.

        ![kernel14][kernel14]

        [kernel14]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/grub.png

## Modulos para el kernel
Por ultimo se desarrollaron dos modulos para poder montarlos en el kernel.
Para poder crear modulos se necesitan de los Headers de linux, por lo que se descargan de la siguiente forma.
```bash
$ sudo apt-get install build-essential linux-headers-$(uname -r)
```
### Modulo CPU
Este modulo sobreesribira un archivo en /proc con mi informacion y informacion de los procesos del sistema.
El codigo del modulo es el siguiente.
```c
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


```
El codigo del archivo make es el siguiente.
```c
obj-m += cpu_201222626.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```
### Modulo de memoria
Este modulo desplegara informacion personal y informacion de la memoria ram.El codigo del modulo es el siguiente.
```c
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
#include <linux/quicklist.h>
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


```
El codigo del archivo make es el siguiente.
```c
obj-m += mem_201222626.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

```
### Pasos para compilar los modulos,cargarlos al kernel y removerlos del kernel.

1. Para compilar cada modulo solo basta con ejecutar el archivo make lo que generara los respectivos archivos de salida.
En la siguiente imagen se observa que los modulos haun no se ahan cargado.

![modulos][modulos]

[modulos]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/lsmodsinmodulos.png

2. Para cargar el modulo al kernel se utiliza el siguiente comando.

```bash
$ sudo insmod mem_201222626.ko
$ sudo insmod cpu_201222626.ko     
```
![modulos2][modulos2]

[modulos2]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/cargademodmed.png

3. Luego de cargarlos ya se pueden observar en la lista de modulos y la salida cuando se cargaron.
```bash
$ sudo lsmod
$ sudo insmod cpu_201222626.ko     

```

![modulos3][modulos3]

[modulos3]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/losdosmodulosCargados.png

![modulos4][modulos4]

[modulos4]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/salidamodules.png

4. Para remover un modulo es el siguiente comando.
```bash
$ sudo rmmod mem_201222626
```

5. La salidas de los modulos son los siguientes.

![modulos5][modulos5]

[modulos5]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/nano-mem.png


![modulos6][modulos6]

[modulos6]: https://raw.githubusercontent.com/byronjl2003/SO1/master/imagenes/nano_cpu.png