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



2. Asegurar que se cumpla con un versionamiento SEMVER.
El verionamiento semver se puede obervar en los artefactos construidos con docker y que se publicaron en los siguientes repositorios
- https://cloud.docker.com/repository/docker/byronjl2003/sa-piloto
- https://cloud.docker.com/repository/docker/byronjl2003/sa-cliente
- https://cloud.docker.com/repository/docker/byronjl2003/sa-ubicacion
- https://cloud.docker.com/repository/docker/byronjl2003/sa-esb
3.  Crear las ramas básicas master / develop en un nuevo repositorio GitLab o GitHub (puede mantenerse el mismo historial git)
desde elrepositorio se puede ver las ramas credas en el repo.
4. Se utilizo docker junto con un script bash que se ejecuta en el servidor de CI. El script se llama
Instrucciones:
- Inicial el servidor de OpenESB
- deployar en el servidorel proyecto que se llama build-script.sh
5. Se utilizo un servidor Sonarqube para el testing statico del codigo. En el siguinte enlace podran ver los resultados.
- http://35.184.187.153:5001/dashboard?id=Tarea5SA

### Se adjunta imagenes del resultado de un pull request fallido.
[diagrama][logo]

[logo]: https://raw.githubusercontent.com/byronjl2003/SA-Tarea5/Develop/github.com_byronjl2003_SA-Tarea5_pull_4.png "pull request"