/*
 * interface.c
 *
 *  Created on: 2014年1月2日
 *      Author: root
 */
#include "interface.h"

#include <stdio.h>
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <memory.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

pid_t pid = -1;

//////////////////////////////////////////////////////////////
//begin uac interface

/*filled by yaoyao*/ // get sdp, fill in INVITE, send to media server by client
int uac_get_Playsdp(char *sdp_data)
{/*
	snprintf(sdp_data,1024,
			"v=0\r\n"
            "o=josua  0 0 IN IP4 192.168.1.1\r\n"
            "s=Playback\r\n"
			  "u=34020000001310000054:3\r\n"
            "c=IN IP4 192.168.1.1\r\n"
            "t=11111 22222\r\n"
            "m=audio 8000 RTP/AVP 0 8 101\r\n");*/
	snprintf(sdp_data,1024,
				"v=0 \r\n"
"o=- 0 0 IN IP4 127.0.0.1 \r\n"
"s=Play \r\n"
"c=IN IP4 0.0.0.0 \r\n"
"t=0 0 \r\n"
"a=tool:libavformat 55.12.102 \r\n"
"m=video 0 RTP/AVP 96 \r\n"
"a=rtpmap:96 H264/90000 \r\n"
"a=fmtp:96 packetization-mode=1 \r\n"
"a=control:streamid=0 \r\n"
);
	return 0;
}

/*filled by yaoyao*/ // handle sdp received from media server in client
int uac_handle_Playsdp(char *sdp_data)
{
	// nothing to do, just print
	printf("sdp_data in uac_handle_Playsdp: %s\n",sdp_data);
	return 0;
}


/*filled by yaoyao*/ // start request: media receiving process from media server in client
int uac_receive_Playmedia()
{
	// start ffplay process
    if(pid < 0){
            char *ffplay_prog_dir="";//"/home/yaoyao/ffmpeg_sources/ffplay/";
            char ffplay_cmd[256];
            char *ffplay_cmd_ptr = ffplay_cmd;
            snprintf(ffplay_cmd_ptr, 255,
            "%sffplay rtsp://192.168.115.42:5454/live.h264 >/dev/null 2>/dev/null",
            ffplay_prog_dir);

            printf(ffplay_cmd_ptr);
            printf("\n");

            if((pid = fork()) < 0){
                    perror("fork()");
            }else if(pid == 0){
                    if(execl("/bin/sh", "sh", "-c", ffplay_cmd, (char *)0) < 0){
                            perror("execl failed");
                    }
                    pid++;
            }else{}
    }
	return 0;
}

/*filled by yaoyao*/ // close media receiving process from media server in client
int uac_close_Playmedia()
{
	// terminate ffplay process
    printf("kill %d\n",pid);
    kill(pid,SIGABRT);
    wait(NULL);
    pid++;
    printf("kill %d\n",pid);
    kill(pid,SIGABRT);
    wait(NULL);

    pid = -1;
	return 0;
}

// get rtsp data, fill in INFO for sending to media server by client
int uac_get_rtsp(char *rtsp_data, struct st_rtsptype  *ptr_st_rtsptype)
{
	// if rtsp datatype in {"PLAY", "FAST", "SLOW"}: check scale
	snprintf(rtsp_data,1024,
				"this is test data!");
	return 0;
}

// handle MESSAGE, received from media server in client
int uac_handle_message(char *message)
{
	return 0;
}

//end uac interface

//////////////////////////////////////////////////////////////
//begin uas interface

int uas_function_run(funcP fun_name,void(*arg))
{
	(*fun_name)(arg);
	return 0;
	}

/*filled by yaoyao*/ // handle sdp data via INVITE received from client in media server
int uas_handle_Playsdp(char *sdp_data)
{
	// nothing to do, just print
	printf("sdp_data in uas_handle_Playsdp: %s\n",sdp_data);
	return 0;
}

/*filled by yaoyao*/ // get sdp data for sending to client in media server
/*filled by yaoyao*/ // p -> 1024 bytes
int uas_get_Playsdp(char *sdp_data)
{
	/*
	snprintf(sdp_data, 1024,
			"v=0\r\n"
			"o=%s 0 0 IN IP4 \r\n"
			"s=PLAY\r\n"
			"c=IN IP4 \r\n"
			"t=0 0\r\n"
			"m=video  STP/AVP 96\r\n"
			"a=sendonly\r\n"
			"a=rtpmap:96 H264/90000\r\n"
			"f=\r\n");*/
	snprintf(sdp_data,1024,
					"v=0 \r\n"
	"o=- 0 0 IN IP4 127.0.0.1 \r\n"
	"s=Play \r\n"
	"c=IN IP4 0.0.0.0 \r\n"
	"t=0 0 \r\n"
	"a=tool:libavformat 55.12.102 \r\n"
	"m=video 0 RTP/AVP 96 \r\n"
	"a=rtpmap:96 H264/90000 \r\n"
	"a=fmtp:96 packetization-mode=1 \r\n"
	"a=control:streamid=0 \r\n"
	);

	return 0;
}

/*filled by yaoyao*/ // start response: media sending process to client in media server
int uas_send_Playmedia()
{
	// Nothing to do, because ffserver is running before uas started.
	return 0;
}

/*filled by yaoyao*/ // close media sending process to client in media server
int uas_close_Playmedia()
{
	// Nothing to do, because ffserver will be running all the time.
	return 0;
}

// handle rtsp data via INFO, received from client by media server
int uas_handle_rtsp(char *rtsp_data)
{
	return 0;
}

// get MESSAGE for sending to client in media server
// p -> 1024 bytes
int get_message(char *message, char *message_type)
{
	// message_type: "EOF"
	snprintf(message,1024,
			"<?xml version=\"1.0\"?>"
			"<Notify>"
			"<CmdType>MediaStatus</CmdType>"
			"<SN>8</SN>"
			"<DeviceID>000</DeviceID>"
			"<NotifyType>121</NotifyType>"
			"</Notify>");
	return 0;
}

int interface_init()
{
	uas_handle_invite_sdp=uas_handle_Playsdp;
	uas_get_invite_sdp=uas_get_Playsdp;
	uas_start_transport=uas_send_Playmedia;
	uas_handle_Message=uas_handle_rtsp;
	uas_stop_transport=uas_close_Playmedia;
	//uas_get_info=uas_get_message;
	return 0;
}

//end uas interface



