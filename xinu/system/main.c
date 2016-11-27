/*  main.c  - main */

#include <xinu.h>
#include <stdlib.h>

void chk(int line){
	print ("Line %d: Chk .. \n", line);
}

typedef struct {
	uint8 type; 	/* 0 = publish, 1 = subscribe, other values left undefined for future expansions */
	uint8 class; 	/* Format class of the data field (temp=0, accel=1, sleep=2,...) */
	uint8 group; 	/* Broad topic, we set to 0 in this project, allows structure to be used when 256 subjects is not enough */
	uint8 topic; 	/* 'Topic' from our assignments (temp=0, accel=1, sleep=2,...) */
	uint8 src; 		/* Source ID, set dynamically by server only. Always set to 255 by BBB, 255 means no source. Server sets this field for all
  					   messages traveling through it */
	uint8 data[64]; /* class-defined data items, length is set to make the entire structure byte-addressable */
}msg;
#define MSGLEN 69

char *buff = "Hello from Xinu";

typedef struct {
	uint32 x;
	uint32 y;
	uint32 z;
}accel_data;

accel_data get_accel(){
	accel_data a;
	a.x = 5;
	a.y = 6;
	a.z = 7;
	return a;
}

uint32 get_temp(){
	return 10;
}


void publish_data2server(){
	chk(__LINE__);
	int32	retval;				/* return value from sys calls	*/
	uint32	des_ip = 0xC0A80065;/* destination IP address		*/
	uint16	des_port = 12345;	/* destination UDP port			*/
	int32	msglen;				/* length of outgoing message	*/
	int32	slot;				/* slot in UDP table 			*/
	uint16	localport= 6;		/* port number for UDP echo		*/

	slot = udp_register(des_ip, des_port, localport);
	if (slot == SYSERR) {
		print(stderr, "Could not reserve UDP port with 0x%x:%d & local port:%d\n",des_ip, des_port, localport);
		return 1;
	}
	chk(__LINE__);


	uint32 i = 0;
	while(i<1){
		//retval = udp_sendto(slot, des_ip, des_port, buff, msglen);
		retval = udp_send(slot, buff, msglen);
		if (retval == SYSERR) {
			print(stderr, "udp_send failed\n");
			return 1;
			break;
		}
		i++;
	}
	chk(__LINE__);
}


process local_daemon(){
	print ("Starting Daemon Process .. \n");

	uint8 temp_prev = 0, temp_threshold = 1;
	accel_data accel_prev;
	accel_prev.x = 0; accel_prev.y = 0; accel_prev.z = 0;
	uint8 accel_threshold = 3;

	/* UDP Pipeline init */
	chk(__LINE__);
	int32	retval;				/* return value from sys calls	*/
	uint32	des_ip = 0xC0A80065;/* destination IP address		*/
	uint16	des_port = 5154;	/* destination UDP port			*/
	int32	msglen;				/* length of outgoing message	*/
	int32	slot;				/* slot in UDP table 			*/
	uint16	localport= 6;		/* port number for UDP echo		*/

	slot = udp_register(des_ip, des_port, localport);
	if (slot == SYSERR)	{
		print(stderr, "Could not reserve UDP port with 0x%x:%d & local port:%d\n",des_ip, des_port, localport);
		return 1;
	}
	chk(__LINE__);

	/* type = 2: Establish Connection with Server*/
	msg msg_1st;
	msg_1st.type = 2;
	msg_1st.class = 0;
	msg_1st.group = 0;
	msg_1st.topic = 0;
	msg_1st.src = 255;
	memset(&msg_1st.data, 0, 64);
	retval = udp_send(slot, &msg_1st, MSGLEN);
	if (retval == SYSERR) {
		print(stderr, "udp_send failed\n");
		return 1;
		//break;
	}

	/* type = 1: Subscribe*/
	msg msg_2nd;
	msg_2nd.type = 1;
	msg_2nd.class = 2;
	msg_2nd.group = 0;
	msg_2nd.topic = 2;
	msg_2nd.src = 255;
	memset(&msg_1st.data, 0, 64);
	retval = udp_send(slot, &msg_1st, MSGLEN);
	if (retval == SYSERR) {
		print(stderr, "udp_send failed\n");
		return 1;
		//break;
	}

	/* Start Publishing */
	uint8 timer = 0;
	while(1){
		/* Poll and send temp*/
		if (timer%100 == 0) {
			msg temp_read;
			temp_read.type = 0;
			temp_read.group = 0;
			temp_read.src = 255;
			temp_read.class = 0;
			temp_read.topic = 0;
			memset(&temp_read.data, 0, 64);
			temp_read.data[0] = get_temp();
			do_led_init();
			do_led_on();
			/*
			uint8 diff = temp_read.data[0]-temp_prev;
			if (diff < 0)
				diff = -1*diff;

			if (diff > temp_threshold){
				temp_prev = temp_read.data[0];
				retval = udp_send(slot, &temp_read, MSGLEN);
				if (retval == SYSERR) {
					print(stderr, "udp_send failed\n");
					return 1;
				}
			}*/

		}
#if 0
		/* Poll and send accel*/
		if (timer%10 == 0) {
			msg accel_read;
			accel_read.type = 0;
			accel_read.group = 0;
			accel_read.src = 255;
			accel_read.class = 1;
			accel_read.topic = 1;
			memset(&accel_read.data, 0, 64);
			accel_data accel = get_accel();
			accel_read.data[0] = accel.x; accel_read.data[1] = accel.y; accel_read.data[2] = accel.z;
			uint8 diff = (accel_prev.x + accel_prev.y + accel_prev.z) - (accel.x + accel.y + accel.z);

			if (diff < 0)
				diff = -1*diff;

			if (diff > accel_threshold) {
				accel_prev = accel;
				retval = udp_send(slot, &accel_read, MSGLEN);
				if (retval == SYSERR) {
					print(stderr, "udp_send failed\n");
					return 1;
				}
			}
		}

		/* Compare and generate sleep event*/
		if (timer%10000 == 0) {


			/* Resetting Timer*/
			timer = 0;
		}
#endif
	}
	chk(__LINE__);
}

process	main (void) {
	recvclr();

	print("**************************************\n");
	print("*Sanity Check: Xinu up and running :D*\n");
	print("**************************************\n");


	pid32 procA = create(local_daemon,8192,20,"ProcA",0);
	resume(procA);


	chprio(getpid(),2);
	return OK;
}
