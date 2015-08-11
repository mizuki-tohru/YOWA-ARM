#ifndef TASK_SEQUENCE_H
#define TASK_SEQUENCE_H

void sequence_set_rate(float rate);
void sequence_set_max_angle_deg(float deg);
void sequence_set_duration(float duration);
void sequence_dump(void);
void vSequenceControl(void *param);

#endif // TASK_SEQUENCE_H

