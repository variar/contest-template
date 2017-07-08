#ifndef MAC_SEM_TIMEDWAIT_H
#define MAC_SEM_TIMEDWAIT_H

int mac_sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

#endif
