#include<reg51.h>
#include<stdio.h>

#define lcd P1

sbit mot1=P0^0;
sbit mot2=P0^1;

sbit rs=P3^0;
sbit e=P3^2;

sbit sensor=P0^6;
sbit buzzer=P0^7;

sbit r0=P2^0;   
sbit r1=P2^1;
sbit r2=P2^2;
sbit r3=P2^3;
sbit c0=P2^4;   
sbit c1=P2^5;
sbit c2=P2^6;

unsigned char key[4][3]={1,2,3,4,5,6,7,8,9,'S',0,'E'};

unsigned char pass[16];

unsigned int t;
unsigned int s=0;

void delay (int);
void cmd (unsigned char);
void display (unsigned char);
void init (void);
void string (char *);
unsigned char keypress (void);
void dooropen(void);
void doorclose(void);
void stop(void);
void checkfire(void);
void createpassword(void);
unsigned int checkpassword(void);


void main()
{
	unsigned int c=3;
	unsigned char choice;
	buzzer=0;
  sensor=0;
	
	init();
	delay(100);
	if(s==0){
	createpassword();}
	while(1)
	{
		if(c!=0){
		cmd(0x01);//Clear screen
		cmd(0x80);//Force cursor to the beginning of 1nd line
		string("OPEN *");
		cmd(0xc0);//Force cursor to the beginning of 2nd line
		string("CLOSE #");}
		checkfire();
		choice=keypress();
		checkfire();
		if(choice=='O')
		{
			key[3][2]='E';
			if(checkpassword())
			{
				dooropen();
				key[3][2]='C';
			}
			else
			{
				cmd(0x01);//Clear screen
				cmd(0x80);//Force cursor to the beginning of 1nd line
				string("INVALID PASSWORD");
				c=c-1;
				if(c==0)
				{
					cmd(0x01);//Clear screen
					cmd(0x83);//Force cursor to the beginning of 1nd line
					string("ALERT");
					cmd(0xc0);//Force cursor to the beginning of 2nd line
					string("DOOR LOCKED");
					key[3][0]='S';
					key[3][2]='E';
					delay(200);
					cmd(0x01);//Clear screen
					cmd(0x80);//Force cursor to the beginning of 1nd line
					string("ENTER * TO SET");
					cmd(0xc0);//Force cursor to the beginning of 2nd line
					string("ENTER # TO ENTER");
				}
				else{
				cmd(0xc0);//Force cursor to the beginning of 2nd line
				string("LOOSING CHANCES");
				}	
				delay(200);
			}
		}
		else if(choice=='C')
		{
			doorclose();
		}
		else if(choice=='S')
		{
			s=0;
			main();
		}
	}
}


void cmd (unsigned char c)
{
    lcd=c;
    rs=0;
    e=1;
    delay(10);
    e=0;
}

void display (unsigned char c)
{
    lcd=c;
    rs=1;
    e=1;
    delay(10);
    e=0;
}

void string (char *p)
{
    while(*p)
    {
        display(*p++);
    }
}

void init (void)
{
    cmd(0x38);//Use 2 lines and 5×7 matrix
    cmd(0x01);//Clear screen
    cmd(0x0c);//Display ON, Cursor OFF
    cmd(0x80);//Force cursor to the beginning of  1st line
    
}

void delay (int d)
{
    unsigned char i;
    for(;d>0;d--)
    {
        for(i=250;i>0;i--);
        for(i=246;i>0;i--);
    }
		checkfire();
}

unsigned char keypress (void)
{
    unsigned char row=-1,col=-1;
  r0=r1=r2=r3=0;
    c0=c1=c2=1;
    while(c0&c1&c2);           
    
    
    
    r0=0;
    r1=r2=r3=1; 
    
    if((c0&c1&c2)==0)
    {
        row=0;            
        goto check;       
    }
    r1=0;
    r0=r2=r3=1;
    if((c0&c1&c2)==0)
    {
        row=1;
        goto check;
    }
    r2=0;
    r1=r0=r3=1;
    if((c0&c1&c2)==0)
    {
        row=2;
        goto check;
    }
    r3=0;
    r1=r2=r0=1;
    if((c0&c1&c2)==0)
    {
        row=3;
        goto check;
    }
    
    
    check: 
    if(c0==0)
        col=0;
    else if(c1==0)
        col=1;
    else if(c2==0)
        col=2;
    
    delay(20);
    
    while(!(c0&c1&c2)); 
    return key[row][col]; 
}

void dooropen(void)
{
	cmd(0x01);//Clear screen
	cmd(0x80);//Force cursor to the beginning of 1nd line
    string("DOOR OPENING");
	//checkfire();
	mot1=1;
  mot2=0;
	stop();
}

void doorclose(void)
{
	cmd(0x01);//Clear screen
	cmd(0x80);//Force cursor to the beginning of 1nd line
    string("DOOR CLOSING");
	//checkfire();
		mot1=0;
    mot2=1;
	stop();
}

void stop(void)
{
	delay(500);
	mot1=0;
  mot2=0;
	cmd(0x01);//Clear screen
}

void checkfire(void)
{
	if(sensor==1)
    {
		buzzer=1;
		cmd(0x01);//Clear screen
		string("FIRE OCCURED");
		buzzer=1;
		dooropen();
		cmd(0x01);//Clear screen
		cmd(0x80);//Force cursor to the beginning of  1st line
		string("GET OUT OF ROOM");
		cmd(0xc0);//Force cursor to the beginning of 2nd line
		string("SAVE YOUR SELF  "); 
		buzzer=0;
		sensor=0;
			main();
	}
}

void createpassword(void)
{
	unsigned int i=0;
	unsigned char j;
	key[3][0]='O';
	cmd(0x01);//Clear screen
	cmd(0x80);//Force cursor to the beginning of  1st line
    string("# AFTER ENTER");
	delay(200);
	cmd(0x01);//Clear screen
	string("SET PASSWORD");
	cmd(0xc0);//Force cursor to the beginning of 2nd line
	j=keypress(); 
	while(j!='E')
	{
		display(j+48);
		pass[i]=j;
		i=i+1;
		enter:
		j=keypress();
		if(j=='O')
		{
			goto enter;
		}
	}
	t=i;
	s=1;
	key[3][2]='C';
	doorclose();
}

unsigned int checkpassword(void)
{
	unsigned char temp[16];
	unsigned int i=0;
	unsigned char j;
	cmd(0x01);//Clear screen
	cmd(0x80);//Force cursor to the beginning of  1st line
    string("# AFTER ENTER");
	delay(200);
	cmd(0x01);//Clear screen
	cmd(0x80);//Force cursor to the beginning of  1st line
    string("ENTER PASSWORD");
    cmd(0xc0);//Force cursor to the beginning of 2nd line
	j=keypress(); 
	while(j!='E')
	{
		display(j+48);
		temp[i]=j;
		i=i+1;
		enter:
		j=keypress();
		if(j=='O')
		{
			goto enter;
		}
	}
	for(i=0;i<t;i++)  
    {
		 if(pass[i]!=temp[i])
		 {
			  return 0;
		 }
    }
	return 1;
}

