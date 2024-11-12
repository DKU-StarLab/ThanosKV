#!/bin/bash

export TEST_TMPDIR=/mnt/pmemdir
export NUMA_AFFINITY_4=4
export NUMA_AFFINITY_6=6

#Numa binding 
export APP_PREFIX="numactl --membind=$NUMA_AFFINITY_4 --cpunodebind=$NUMA_AFFINITY_4"
export APP_PREFIX="numactl --membind=$NUMA_AFFINITY_6 --cpunodebind=$NUMA_AFFINITY_6"