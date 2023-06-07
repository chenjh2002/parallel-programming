echo 'export PATH=/usr/local/cuda11.0/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/cuda11.0/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc


MANPATH=$MANPATH:/data3/cjh/nvidia/hpc_sdk/Linux_x86_64/23.5/compilers/man; export MANPATH
PATH=/data3/cjh/nvidia/hpc_sdk/Linux_x86_64/23.5/compilers/bin:$PATH; export PATH

export PATH=/data3/cjh/nvidia/hpc_sdk/Linux_x86_64/23.5/comm_libs/mpi/bin:$PATH
export MANPATH=$MANPATH:/data3/cjh/nvidia/hpc_sdk/Linux_x86_64/23.5/comm_libs/mpi/man

CUDA_HOME=/data3/cjh/nvidia/hpc_sdk/Linux_x86_64/23.5/cuda/12.1