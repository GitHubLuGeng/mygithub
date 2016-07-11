#include <linux/init.h>  
#include <linux/module.h>  
#include <linux/timer.h>  
#include <linux/time.h>  
#include <linux/types.h>  
#include <net/sock.h>  
#include <net/netlink.h>   
  
#define NETLINK_TEST 25  
#define MAX_MSGSIZE 1024  
int pid;  
int err;  
struct sock *nl_sk = NULL;  
int flag = 0;  
  
int stringlength(const char *s)  
{  
    int slen = 0;  
    for(; *s; s++){  
        slen++;  
    }  
    return slen;  
}  
  
void sendnlmsg(void)  
{  
    struct sk_buff *skb_1;  
    struct nlmsghdr *nlh;  
    int len = NLMSG_SPACE(MAX_MSGSIZE);  
    int slen = 0;  
    char buffer[128];  
    const char *message="hello i am kernel";  
    if(!message || !nl_sk){  
        return ;  
    }  
    skb_1 = alloc_skb(len,GFP_KERNEL);  
    if(!skb_1){  
        printk(KERN_ERR "my_net_link:alloc_skb_1 error\n");  
    }  
    nlh = nlmsg_put(skb_1,0,0,0,MAX_MSGSIZE,0);  
  
    //NETLINK_CB(skb_1).pid = 0;  
    NETLINK_CB(skb_1).portid = 0;  
    NETLINK_CB(skb_1).dst_group = 0;  
  
    slen = stringlength(message);  
    memset(buffer,0,sizeof(buffer));  
    memcpy(buffer,message,slen);  
    memcpy(NLMSG_DATA(nlh),buffer,slen+1);  
    printk("my_net_link:send message '%s'.\n",(char *)NLMSG_DATA(nlh));  
  
    netlink_unicast(nl_sk,skb_1,pid,MSG_DONTWAIT);  
}  
  
void nl_data_ready(struct sk_buff *__skb)  
{  
    struct sk_buff *skb;  
    struct nlmsghdr *nlh;  
    char str[100];  
 //   struct completion cmpl;  
    int i=10;  
    skb = skb_get (__skb);  
    if(skb->len >= NLMSG_SPACE(0)){  
        nlh = nlmsg_hdr(skb);  
  
        memcpy(str, NLMSG_DATA(nlh), sizeof(str));  
        printk("Message received:%s\n",str) ;  
        pid = nlh->nlmsg_pid;  
        while(i--){  
            //init_completion(&cmpl);  
            //wait_for_completion_timeout(&cmpl,3 * HZ);  
            sendnlmsg();  
            break;  
        }  
        flag = 1;  
        kfree_skb(skb);  
    }  
  
 }  
  
static int netlink_k_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.input = nl_data_ready,
	};
	
	nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
	if(!nl_sk){
			printk(KERN_ERR "my_net_link: create netlink socket error.\n");
			return 1;
	}
	
	printk("my_net_link_3: create netlink socket ok.\n");
	return 0;
}
	
static int netlink_init(void)  
{
	printk("netlink_k init\n");  
    netlink_k_init();  
    return 0; 
}  
  
static void netlink_exit(void)  
{  
    if(nl_sk){  
		netlink_kernel_release(nl_sk);
    }  
  
    printk("my_net_link: self module exited\n");  
}  
  
module_init(netlink_init);  
module_exit(netlink_exit);  
  
MODULE_AUTHOR("wangpengqi");  
MODULE_LICENSE("GPL");  
