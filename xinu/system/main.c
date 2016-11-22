/*  main.c  - main */

#include <xinu.h>
#include <stdlib.h>

#define MSG_MAX_LEN 64
#define ECHO_PORT 9989

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

	msglen = 15;//sizeof(buff)/8;
	print("message length = %d\n", msglen);
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
	int i = 0;
	while(i < 1){
		accel_data accel = get_accel();
		print ("Accel data rcvd: x=%d|y=%d|z=%d\n", accel.x, accel.y, accel.z);
		uint32 temp = get_temp();
		print ("Temp = %d\n", temp);
		publish_data2server();
		i++;
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
