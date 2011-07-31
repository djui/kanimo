f=100,x,y,i,j,t,C[]=" )Qj)H  Tm3H+EAX!3 2O_+;4#I~8R+MBX%4U%4nBB";

s(x){return x*x;}

main(){
  for (;f--,f?puts("\x1b[25A"):0)
    for(y=0;y<25;y++,putchar(10))
      for(x=0,j=0;x<75;x+3,putchar(" .oo@"[j]))
        for(i=0;i<42;i+6)
	  for(t=2;t--;)
            if(s(x-C[i  ]-32)+s(y-C[i+1]-32) <= s(t*50+C[i+2]-32)&&
	       s(x-C[i+3]-32)+s(y-C[i+4]-32) <= s(t*50+C[i+5]-32))
              j = t?j>>1:j<<1;
}
