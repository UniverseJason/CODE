void *cpuSchedule(void *arg)
{
    // 1 sec timespec strcut
    struct timespec sleep_time;
    sleep_time.tv_sec = 1;
    sleep_time.tv_nsec = 0;

    // while will go through the CPU schedule algorithm
    while(true)
    {
        //if(isEmpty(data.ready_Q) == 1 && isEmpty(data.io_Q) == 1 && io_busy == 0 && cpu_busy == 0 && file_read_done == 1) break;
        if(isEmpty(data.ready_Q) == 1 && io_busy == 0 && cpu_busy == 0 && file_read_done == 1) break;

        if(algoFlag == FIFO)
        {
            int res = sem_timedwait(&sem_cpu, &sleep_time);
            if(res == -1 && errno == ETIMEDOUT) continue;

            // simulate the CPU burst
            cpu_busy = 1;

            // CONSUME: remove the node from the ready queue
            sem_wait(&full_cpu);
            sem_wait(&mutex_cpu);

            PCB_st *pcbNode = getFirst(data.ready_Q);
            if(pcbNode == NULL) continue;
            data.ready_Q = removeHead(data.ready_Q);

            sem_post(&mutex_cpu);
            sem_post(&empty_cpu);

            // CPU sleep for the CPU burst
            usleep(pcbNode->CPUBurst[pcbNode->cpuIndex] * 1000);
            pcbNode->cpuIndex++;

            // if this idx is the last CPU burst
            if(pcbNode->cpuIndex >= pcbNode->numCPUBurst)
            {
                freeNode(pcbNode);
                cpu_busy = 0;
            }
            else
            {
                // PRODUCE: add node to the IO queue
                //sem_wait(&empty_io);
                //sem_wait(&mutex_io);
                
                addNewPCB(data.io_Q, pcbNode);
                
                //sem_post(&mutex_io);
                //sem_post(&full_io);
                
                cpu_busy = 0;
                sem_post(&sem_io);
            }
        }
    }

    cpu_sch_done = 1;
    return NULL;
}