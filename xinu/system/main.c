/*  main.c  - main */

#include <xinu.h>
#include <stdlib.h>

void chk(int line){
	print ("Line %d: Chk .. \n", line);
}

/* Message struct to be sent to server */
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
int32 slot_subscribe;

/* Stub Message */
char *buff = "Hello from Xinu";

typedef struct {
	int32 x;
	int32 y;
	int32 z;
}accel_data;

/* Stub Function */
accel_data get_accel(){
	accel_data a;
	a.x = 5;
	a.y = 6;
	a.z = 7;
	return a;
}

/* Stub Function */
int32 get_temp(){
	int temp = 10;
	return 10;
}
//#if 0
process local_daemon_publish(){
	print ("Starting Publish Daemon Process .. \n");

	uint8 temp_prev = 0, temp_threshold = 1;
	do_accel_init(); do_tmp_init();
	accel_data accel_prev;
	accel_prev.x = 0; accel_prev.y = 0; accel_prev.z = 0;
	uint8 inertia = 3; uint8 accel_threshold = 3;

	/* UDP Pipeline init */
	chk(__LINE__);
	int32	retval;						/* return value from sys calls	*/
	//uint32	des_ip = 0xC0A80065;	/* destination IP address		*/
	uint32	des_ip = 0xC0A80064;		/* destination IP address		*/
	uint16	des_port = 5154;			/* destination UDP port			*/
	uint16	subscribe_port = 5155;
	int32	msglen;						/* length of outgoing message	*/
	int32	slot;						/* slot in UDP table 			*/
	uint16	localport= 6;				/* port number for UDP echo		*/

	slot = udp_register(des_ip, des_port, localport);
	if (slot == SYSERR)	{
		print(stderr, "Could not reserve UDP port with 0x%x:%d & local port:%d\n",
				des_ip, des_port, localport);
		return 1;
	}
	chk(__LINE__);

	slot_subscribe = udp_register(des_ip, subscribe_port, subscribe_port);
	if (slot == SYSERR)	{
		print(stderr, "Could not reserve UDP port with 0x%x:%d & local port:%d\n",
				des_ip, des_port, localport);
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

	/* type = 1: Subscribe with Server*/
	msg msg_2nd;
	msg_2nd.type = 1;					/* 1 corresponds to subscribe */
	msg_2nd.class = 3;					/* 3 corresponds to LED service as defined  in server */
	msg_2nd.group = 0;
	msg_2nd.topic = 3;					/* 3 corresponds to LED service as defined  in server */
	msg_2nd.src = 255;
	memset(&msg_2nd.data, 0, 64);
	retval = udp_send(slot, &msg_2nd, MSGLEN);
	if (retval == SYSERR) {
		print(stderr, "udp_send failed\n");
		return 1;
		//break;
	}

	/* Start Publishing */
	uint8 timer = 0;
	uint8 temp_awake = 0;
	uint8 sleep_flag = 0; 					/* Initially assumed awake */
	while(1){
		sleep(1); timer++;

		/* Poll and send temp every minute*/
		if (timer%10 == 0) {
			msg temp_read;
			temp_read.type = 0;							/* 0 corresponds to publish */
			temp_read.group = 0;
			temp_read.src = 255;
			temp_read.class = 0; temp_read.topic = 0;	/* 0 corresponds to TEMP service as defined  in server */
			memset(&temp_read.data, 0, 64);
			int temp_c; do_tmp_read(&temp_c);
			uint8 local_temp = temp_c; temp_read.data[0] = local_temp;
			print ("Temp = %d\n", local_temp);
			uint8 diff = temp_read.data[0]-temp_prev;
			if (diff < 0)
				diff = -1*diff;

			//if (diff > temp_threshold){
				temp_prev = temp_read.data[0];
				retval = udp_send(slot, &temp_read, MSGLEN);
				if (retval == SYSERR) {
					print(stderr, "udp_send failed\n");
					return 1;
				}
			//}

		}


		/* Poll and send accel every 10 second*/
		if (timer%10 == 0) {
			chk(__LINE__);
			msg accel_read;
			accel_read.type = 0;						/* 0 corresponds to publish */
			accel_read.group = 0;
			accel_read.src = 255;
			accel_read.class = 1; accel_read.topic = 1; /* 1 corresponds to ACCEL service as defined  in server */
			memset(&accel_read.data, 0, 64);

			accel_data accel;
			do_accel_read(&accel_read);
			uint8 x = accel.x; accel_read.data[0] = x;
			uint8 y = accel.y; accel_read.data[1] = y;
			uint8 z = accel.z; accel_read.data[2] = z;
			print("Accel: x=%d\ty=%d\tz=%d\n");
			uint8 diff = (accel_prev.x + accel_prev.y + accel_prev.z) - (accel.x + accel.y + accel.z);

			if (diff < 0)
				diff = -1*diff;

			//if (diff > accel_threshold) {
				accel_prev = accel;
				retval = udp_send(slot, &accel_read, MSGLEN);
				if (retval == SYSERR) {
					print(stderr, "udp_send failed\n");
					return 1;
				}
			//}
		}
#if 0
		/* Poll accel at every 10 seconds for generating data
		 * over 5 minutes for sleep event */
		if (timer%10 == 0) {
			chk(__LINE__);
			accel_data accel;
			do_accel_read(&accel);

			if ((accel.x + accel.y + accel.z) > inertia)
				temp_awake++;
			else
				temp_awake--;

			/* Generate a sleep event at every 5minutes Seconds */
			if (timer%300 == 0){
				chk(__LINE__);
				msg sleep_event;
				sleep_event.type = 0;
				sleep_event.group = 0;
				sleep_event.src = 255;
				sleep_event.class = 2;
				sleep_event.topic = 2;
				memset(&sleep_event.data, 0, 64);

				/* person awake and last event was sleep */
				if (temp_awake > 0 && sleep_flag == 1){
					chk(__LINE__);
					sleep_flag = 0;

					/* Data: sleep_flag, current temperature, current accel*/
					sleep_event.data[0] = sleep_flag;

					int temp_c; do_tmp_read(&temp_c);
					uint8 local_temp = temp_c; sleep_event.data[1] = local_temp;
					print("Temp = %d\n", sleep_event.data[1]);

					uint8 x = accel.x; sleep_event.data[2] = x;
					uint8 y = accel.y; sleep_event.data[3] = y;
					uint8 z = accel.z; sleep_event.data[4] = z;
					print("Accel: x=%d\ty=%d\tz=%d\n", x, y, z);

					retval = udp_send(slot, &sleep_event, MSGLEN);
					if (retval == SYSERR) {
						print(stderr, "udp_send failed\n");
						return 1;
					}
				}

				/* person sleep and last event was awake */
				if (temp_awake < 0 && sleep_flag == 0){
					chk(__LINE__);
					sleep_flag = 1;

					/* Data: sleep_flag, current temperature, current accel*/
					sleep_event.data[0] = sleep_flag;

					int temp_c; do_tmp_read(&temp_c);
					uint8 local_temp = temp_c; sleep_event.data[1] = local_temp;
					print("Temp = %d\n", sleep_event.data[1]);

					uint8 x = accel.x; sleep_event.data[2] = x;
					uint8 y = accel.y; sleep_event.data[3] = y;
					uint8 z = accel.z; sleep_event.data[4] = z;
					print("Accel: x=%d\ty=%d\tz=%d\n", x, y, z);

					retval = udp_send(slot, &sleep_event, MSGLEN);
					if (retval == SYSERR) {
						print(stderr, "udp_send failed\n");
						return 1;
					}
				}
				temp_awake = 0;
			}

		}
#endif
	}
}

process local_daemon_subscribe () {
	print ("Starting Subscribe Daemon Process .. \n");
	do_led_init(EP9_15);
	int32	retval;
	uint16	localport= 7;				/* port number for UDP echo		*/
	int32	delay	= 2000;				/* reception delay in ms		*/
	char	inbuf[69];					/* buffer for incoming reply	*/



	/* Wait for Data */
	char toggle_flag = '0'; 		/* Assumed initially Off*/
	chk(__LINE__);
	while (1) {
		chk(__LINE__);
		retval = udp_recv(slot_subscribe, inbuf, sizeof(inbuf), delay);
		chk(__LINE__);
		if (retval == TIMEOUT) {
			//fprintf(stderr, "%s: timeout...\n");
			continue;
		} else if (retval == SYSERR) {
			//fprintf(stderr, "%s: error from udp_recv \n");
			return 1;
		}

		if (inbuf[5] == 1) {
			if (toggle_flag == '0') {
				//do_led_on();
				print("LED ON..\n");
				toggle_flag = '1';
			}
			else {
				//do_led_off();
				print ("LED OFF .. \n");
				toggle_flag = '0';
			}
		}
	}
}
//#endif
process	main (void) {
	recvclr();

	print("**************************************\n");
	print("*Sanity Check: Xinu up and running :D*\n");
	print("**************************************\n");


	pid32 procA = create(local_daemon_publish,8192,20,"ProcA",0);
	resume(procA);
	sleep(5);
	pid32 procB = create(local_daemon_subscribe, 8192, 20, "ProcB", 0);
	resume(procB);
	chprio(getpid(),2);
	return OK;
}
