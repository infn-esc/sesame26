---
title: School exercise environment
layout: main
section: basic
---

The goal of this step is for you to get familiar with the ESC working
environment and check that everything is correctly set up for the
school exercises.

If something doesn't work as expected, please ask one of the
organizers.

When using these computing resources you agree to comply with the "Regulation on
the use of INFN computing resources", available in
[Italian](https://www.cnaf.infn.it/wp-content/uploads/2020/03/Disciplinare_2020_IT.pdf)
and
[English](https://www.cnaf.infn.it/wp-content/uploads/2020/03/Disciplinare_2020_EN.pdf).
Please read also the "General information note on processing of personal data by
INFN", available in
[Italian](https://dpo.infn.it/wp-content/uploads/2019/01/Informativa_generale_INFN_181204.pdf)
and
[English](https://dpo.infn.it/wp-content/uploads/2020/07/Informativa_generale_181204_EN.pdf).

## Wi-Fi access

You can use either the `eduroam` federation, ask the organization for an alternative access upon your arrival.

## SSH access to School computers

You have been assigned a personal account to access computing resources
hosted at INFN-CNAF. Credentials are that of your INFN AAI account.

Most of the exercises will be done on the following host: esc26s.cloud.cnaf.infn.it (IP: 131.154.98.210, that also has the esc26s-01 hostname).

To log on this server, prepared for the School, just SSH on it using your AAI username.

```shell
[me@mylaptop ~]$ ssh username@esc26s.cloud.cnaf.infn.it
username@esc26s.cloud.cnaf.infn.it's password:
Last login: ...
[username@esc26s-01 ~]$
```

From `esc26s-01` you can then log onto the other School computers. There
are two servers available, named `esc2s-01` and `esc26s-02`. Both
are equipped with four nVidia H100 80GB GPUs, 192 physical CPU cores and 1.5 TB of
RAM. We will use the second one just for the MPI exercises, and you should not log on it unless the tutors tell you to this explicitely.

The names of the machines can be tedious to type, but you can get around
it by creating a config file for ssh. With the ssh configuration **on
your laptop** you can also avoid the explicit jump through the
`esc26s-01` host and forward the X11 display and the SSH agent.

```shell
[me@mylaptop ~]$ cat ~/.ssh/config
Host esc1
  Hostname esc26s.cloud.cnaf.infn.it
  User username

Host esc2
  Hostname esc26s-02
  User username
  ProxyJump esc1

Host *
  ForwardX11 yes
  ForwardAgent yes


[me@mylaptop ~]$ ssh esc1
username@esc26s-01's password:
Last login: ...
[username@esc26-01 ~]$
```


NOTE that esc26s-02 is not a public hostname and it is known only within esv26s-01.

To further simplify the login to `esc1`, you can use an SSH key. You first need
to create it (if you don't have one already), copy it remotely on both `esc1`
and `esc2` and set it in your SSH configuration.

```shell
[me@mylaptop ~]$ ssh-keygen -C username@esc1 -f ~/.ssh/id_rsa_student_esc
Generating public/private rsa key pair.
...
[me@mylaptop ~]$ ssh-copy-id -i ~/.ssh/id_rsa_student_esc esc1
/usr/bin/ssh-copy-id: INFO: Source of key(s) to be installed: "/home/me/.ssh/id_rsa_student_esc.pub"
...
username@esc26s-01's password: 
...

[me@mylaptop ~]$ cat ~/.ssh/config
Host esc1
  Hostname esc26s.cloud.cnaf.infn.it
  User username
  IdentityFile ~/.ssh/id_rsa_student_esc

Host esc2
  Hostname esc26s-02
  User username
  IdentityFile ~/.ssh/id_rsa_student_esc

Host *
  ForwardX11 yes
  ForwardAgent yes

[me@mylaptop ~]$ ssh esc1
Last login: ...
[username@esc26s-01 ~]$ 
```

Note that we removed the ProxyJump line from the esc2 section.

Now copy the key pair and the config file in ~/.ssh/ of esc1

```shell
[me@mylaptop ~]$ scp .ssh/id_rsa_student_esc esc1:.ssh/
[me@mylaptop ~]$ scp .ssh/id_rsa_student_esc esc1:.ssh/
[me@mylaptop ~]$ scp .ssh/config esc1:.ssh/
```

Now you should be able to login passwordless: 

* from your laptop to esc1
* from esc1 to esc2.
* from esc2 to esc1

This passwordless login(s) will be important for the MPI exercises.

Try the following, type yes when asked to add a fingerprint (this will be asked only during the first attempt).

```shell
[me@mylaptop ~]$ ssh esc1
[username@esc26s-01 ~]$ ssh esc2
[username@esc26s-02 ~]$ ssh esc1
[username@esc26s-01 ~]$ 
```


Your shell is [`bash`](https://www.gnu.org/s/bash).

Please note that:

* Each `esc` computer is not assigned exclusively to a student.
* Your home directory is shared between all `esc` computers
* Your home directory will be wiped out and your `username` account will
  be disabled soon after the end of the School. Please make sure to save
  somewhere else what you want to keep.

## School material

All the school hands-on material is included in a git repository. Get it using:

```shell
[username@esc26s-01 ~]$ git clone {{ site.github.clone_url }}
```

The repository contains also these pages.

## Check the environment

Once logged into `esc`, verify that the environment is set up correctly:

```shell
[username@esc26s-01 ~]$ gcc --version
gcc (GCC) 11.5.0 20240719 (Red Hat 11.5.0-5)
...
[username@esc26s-01 ~]$ gdb --version
GNU gdb (AlmaLinux) 16.3-2.el9
...
[username@esc26s-01 ~]$ valgrind --version
valgrind-3.25.1
[username@esc26s-01 ~]$ scl enable gcc-toolset-14 -- gcc --version
gcc (GCC) 14.2.1 20250110 (Red Hat 14.2.1-12)
...
```

As you can see from the last command, beside the native compiler (gcc v. 11.5), there is also a more recent version (gcc v. 14.2), available through a so-called software collection.

To always work with the more recent version, enable the toolset in a new shell:

```shell
[username@esc26s-01 ~]$ scl enable gcc-toolset-14 bash
[username@esc26s-01 ~]$ gcc --version
gcc (GCC) 14.2.1 20250110 (Red Hat 14.2.1-7)
...
```

## Editing source code

### Editing locally

On `esc` you can find several editors available, such as `vim` and `emacs`. If
the X display is available, graphical editors will open a window on your laptop;
the network latency however may not be good enough to give you a fine
experience. Just try.

### Editing remotely

The best option to edit files remotely is to use [Visual Studio
Code](https://code.visualstudio.com/) with its [remote development
extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.vscode-remote-extensionpack).

Alternatively you could edit the source code for the exercises on your laptop,
synchronizing them with the `esc` machine, for example using one of the
following:

* `scp`: You can copy files remotely using `scp`, in both directions.

* `sshfs`: You can mount your `esc` home directory on your laptop via `sshfs`.

* `rsync`: You can synchronize your local workspace with the one you keep on `esc`
  using [`rsync`](http://rsync.samba.org/). Please refer to the `rsync` manual to
  fully understand the meaning of the different options, so to avoid mistakes
  that could cause loss of data.

For all these options to work seamlessly, however, you should first simplify
your use of SSH to connect to `esc`, as described above.

## Environment configuration for MPI hands-on

### .bashrc configuration to locate openmpi bins and libs
OpenMPI is installed on the nodes, but it is needed to add to the .bashrc the following exports:

```shell
export PATH=$PATH:/usr/lib64/openmpi/bin/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/openmpi/lib
```

### Enabling passwordless ssh between the compute nodes
To run the multinode exercises passwordless ssh must be enabled among all the nodes involved in the computation.

An easy way to implement this is to create a new ssh rsa key in one of the nodes and add the newly created public key to the authorized_key file, in the very same way you already did to enable passwordless ssh between the bastion node and the compute nodes, i.e.:

```shell
[me@esc1 ~]$ ssh-keygen -C username@esc -f ~/.ssh/id_rsa_student_escMPI
Generating public/private rsa key pair.
...
[me@mesc1 ~]$  cat ~/.ssh/id_rsa_student_escMPI.pub >> ~/.ssh/authorized_keys

[me@esc1 ~]$ cat ~/.ssh/config

Host esc1
  Hostname esc25-a100-1
  User username
  IdentityFile ~/.ssh/id_rsa_student_escMPI


Host esc2
  Hostname esc25-a100-2
  User username
  IdentityFile ~/.ssh/id_rsa_student_escMPI

Host *
  ForwardX11 yes
  ForwardAgent yes


[me@esc1 ~]$ ssh esc2
[me@esc2 ~]$ ssh esc1

```

## Environment configuration for TBB hands-on

### Set the environment to locate the TBB library
```shell
[me@esc1 ~]$ source /opt/intel/oneapi/tbb/latest/env/vars.sh
[me@esc1 ~]$ echo $TBBROOT
/opt/intel/oneapi/tbb/2022.2/env/..
```

## Enviroment configuration for CUDA hands-on

### Set the enviroment to locate the CUDA toolkit
```shell
[me@esc1 ~]$ export PATH=/usr/local/cuda-13/bin:$PATH
[me@esc1 ~]$ export LD_LIBRARY_PATH=/usr/local/cuda-13/lib64:$LD_LIBRARY_PATH
[me@esc1 ~]$ nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2025 NVIDIA Corporation
Built on Wed_Aug_20_01:58:59_PM_PDT_2025
Cuda compilation tools, release 13.0, V13.0.88
Build cuda_13.0.r13.0/compiler.36424714_0
```
