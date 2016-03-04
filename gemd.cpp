/***************************************************************************
 *   Copyright (C) 2006 by P. Putnik   *
 *   pp@ppest.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <qlabel.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <kmessagebox.h>
#include <qtextedit.h>
#include <kapplication.h>
#include <qdir.h>
#include <qcursor.h>
//#include <string.h>
//#include <sys/stat.h>
#include <utime.h>

#include "gemd.h"
#include "iniz.h"




typedef unsigned char  byte;
typedef unsigned long ULONG;
typedef unsigned int  UINT;




 
   
extern int Filesys; // 0-unknown, 1-DOS, 2-PlusIDE, 3-PlusIDE 256
    // 4-7 Charea ;  11-GemDos

extern char Swf ; // Sub filesystem 1-Swapped High/Low
   
extern  int Fsub ;  // Swap request flag - by read or write drive

extern   QString fileName;
	
extern   QString op, qhm;
extern   FILE *finp, *fout, *ftout, *flout ;
extern   ULONG filelen, m, u ;

extern ULONG ChaSecCnt ;

extern int drih;
extern int selected;
extern int c,n,k,i, o, p, s, fscanp ; 
extern char detDev[9][16] ;
extern  char physd[9];
extern char loadedF[256] ;
extern int form;
extern int secsize, clust ;
extern int status ;

extern char segflag ;
extern ULONG SecCnt ;
extern ULONG OffFS;

extern QString op, qhm;

extern char dstr[60];


extern UINT sectr ;
extern ULONG cylc ;
extern UINT heads ; 
extern bool ov2ro;

extern FILE *finp, *fout, *ftout, *flout ;

extern unsigned char bufr[640];
extern  unsigned char  buf2[524];
extern  unsigned char  bufsw[524];

extern QString litet;

char subdnam[13] ;
char filext[20] ;


 int sublev , reculev, selcnt, selP, j, d, e ;
 unsigned int oe, direc, zz, staclu, fileClucnt ;
  
  ULONG epos, eposIn, partpos, stfilelen, copied ;
  ULONG Gpartp[24] ; // GemDos part postions
  ULONG Gparsiz[24] ; // GemDos part sizes
  
  unsigned int pdirpos[8] ; // Position in parent dir, by recursive extraction
  
  UINT fatsec , dirsec, clusec, FdatS,  DirBeg, ClunOfParent  ;
  long tosave, remain ;
                
  unsigned int dirlen, CurDirLen, fx, pusec, pfrec, ResSec ; // Used clusters, free clusters of partition
    
  char subnam[12][8] ;
  char subpath[256];
  char res ;
  bool timestCur ;
  
  int fdat, ftim ;
  
  extern char zxpath[256];
  extern int zxpal ;
  extern int p,  fscanp ;
  extern char curzx[128];
  
  char fnamb[128];
  
  extern  QStringList SeldFiles;
  char DestDir[256];
    
  ULONG  PartSec ,PartClu ;
                
  ULONG  dirflen, oldclun ;
  
  unsigned long msapos, msapos2 ;
  
  unsigned int  dirpos[512] ;
  
  unsigned int dpos, logsec, nextf, fatTime, fatDate, SecSiz ;

  unsigned int credirCnt; //cluster count of just created DIR
  
  QString currentPath;
  
      
  
  char utbuuf[64];
  char buffer [1000];

  unsigned int DirClus[98] ; // For storing DIR cluster spread
  unsigned int credirClu[16] ; // Created DIR cluster spread store
  
  unsigned char dirbuf[512*100];
  unsigned char fatbuf[132000] ;
  unsigned char trasb[65536] ;  // 128 sector
  
  unsigned long selbuf[256] ;
  

  
  
  
  void fatlosn(unsigned int clun) // For FAT 16 MS DOS
{
	int  fp ;
    
	fp = 2*clun  ; // double of...
       
	nextf = fatbuf[fp]+256*fatbuf[fp+1] ; 
     

	logsec = FdatS+(clun-2)*clusec ; //first sector of cluster

}



int loadsecs( int lsec, int secn, unsigned char *ldbuf)
{

	lseek(drih, partpos+lsec*SecSiz, 0 );
	
	
	if (Swf) { j = secn*SecSiz/512 ;
		for (e=0;e<j;e++)
		{ read(drih, bufsw, 512);  
		for (d=0;d<512;d=d+2) { ldbuf[d+e*512]=bufsw[d+1] ; ldbuf[d+1+e*512]=bufsw[d]; }
		}
       
		}
	else
	
	read(drih, ldbuf, secn*SecSiz);  
	return 1;
}
  
  
  
ULONG writsecs( int lsec, int secn, unsigned char *ldbuf)
{

     
	lseek(drih, partpos+lsec*SecSiz, 0 );
	
	if (Swf) { j = secn*SecSiz/512 ; u = 0 ;
		for (e=0;e<j;e++)
		{  for (d=0;d<512;d=d+2) { bufsw[d+1]=ldbuf[d+e*512] ; bufsw[d]=ldbuf[d+1+e*512]; }
		u = u+ write(drih, bufsw, 512);             
		}
		return u ;
	}
	else  
	
	
	return  write(drih, ldbuf, secn*SecSiz);  

}
  
  
  
  
  
  
int PaddFn(char *Instr)
{

  // Look where is .
  
	k = strlen(Instr);
	for (n=0;n<k;n++) {
		if ( Instr[n] == '.' ) break; 
	}
	if (k>8) {
		if (n>8) return 0 ; }// Invalid filename
   
		strncpy(dstr,Instr,n+1) ;
   // Padd name with  spaces by need
		for (c=n;c<8;c++) dstr[c]=' ';
   
   // Extension if is
		for (i=8;n<k-1;i++) {dstr[i]=Instr[n+1]; n++; }
   
		for (c=i;c<11;c++) dstr[c]=' ';
		dstr[c]= 0;
      
  
		return 1;



}  
  
  
  
  
  
int SamNam(char *gs)
{
               
	if (sublev==0) dpos = 0;
	else dpos=64; // By subdir is 2 first slot occupied
               
	for ( fx = dpos; fx<(CurDirLen*SecSiz);fx=fx+32) {
      
      
		for (n=0;n<11;n++) {
          
			if ( gs[n]!=dirbuf[fx+n]) break;
		}
		if (n==11) return 0; // Same name found
                    
	}
	return 1; // Not found same name  
      
      
}     
  
  
  
int GetFfds()
{
             
	if (sublev==0) fx = 0;
	else fx=64; // By subdir is 2 first slot occupied
        
              
	for ( dpos = fx; dpos<(CurDirLen*SecSiz);dpos=dpos+32) {
          
		if (dirbuf[dpos]==0) return 1;   
 
	}
             
      //If not found 0 seek E5 for deleted slot:
           
                   
	for ( dpos = fx; dpos<(CurDirLen*SecSiz);dpos=dpos+32) {
          
		if (dirbuf[dpos]==0xE5) return 1;   
 
	}           
             
	return 0 ;       
 
             
}           
               
     
int SeekFfc() 
{
              
	for( m = 4; m < (2*PartClu+4);  m = m+2 ) {
         
		fx = fatbuf[m]+256*fatbuf[m+1] ;
     
		if ( fx == 0 ) break  ; // free cluster
       
	}            
              
	if ( m==(2*PartClu+4) ) return 0 ; // Not found         
              
	m = m/2 ; // This is first free cluster #         
	return 1;         
             
}


int SeekNfc(ULONG ne) 
{
	ne = 2*ne+2 ; // Twice cluster # and go ahead to next
	for( m = ne; m < (2*PartClu+4);  m = m+2 ) {

		fx = fatbuf[m]+256*fatbuf[m+1] ;
     
		if ( fx == 0 ) break  ; // free cluster
       
	}            
              
	if ( m==(2*PartClu+4) ) { m=m/2;return 0 ; } // Not found         
              
	m = m/2 ; // This is first free cluster #         
	return 1;         
      
                  
}
 
 void MarkCluR(ULONG clun, ULONG oldclun) 
{
      
	oldclun = oldclun*2 ;
	fatbuf[oldclun]= clun;
	fatbuf[oldclun+1]= clun>>8;     
      
     
      
} 
 
 
 
 
 
 
 
 void MarkCluL(ULONG clun) 
{
      
	clun = clun*2 ;
	fatbuf[clun]= 255;
	fatbuf[clun+1]= 255;     
      
     
      
} 
 
 
 
 
 
 
 
 
int MakeSubF(UINT Clun) // Create start cluster of subdirectory
{

  // Calculate logical sector by cluster #
  
	logsec = FdatS+(Clun-2)*clusec ;
  
  // Create content:
     // First clear:
      
	for ( n=0;n<(SecSiz*clusec);n++) trasb[n]=0 ;        
     
            
	trasb[0] = 0x2E ;
	for ( n =1;n<11;n++ ) trasb[n] = ' ' ;
	trasb[11] = 16 ; // SubDir flag
     
     	
	n = fatTime ;
	trasb[22] = n;
	trasb[23] = n>>8;         
          
	n = fatDate ;
	trasb[24] = n;
	trasb[25] = n>>8; 
       
	
	n = Clun ;
	trasb[26] = n;
	trasb[27] = n>>8; // Start cluster of itself
    
      
	trasb[32] = 0x2E ;
	trasb[33] = 0x2E ;
	for ( n =34;n<32+11;n++ ) trasb[n] = ' ' ;
	trasb[43] = 16 ; // SubDir flag
    
	n = ClunOfParent ; //!!! zero if parent is ROOT
	trasb[26+32] = n;
	trasb[27+32] = n>>8; // Start cluster of parent
    
      // Write it to drive or image:
               
	res = writsecs( logsec, clusec, trasb); 
       
      /* 
	if ( res != (clusec*512) )   {
           
	MessageBox(hDlgWnd, "Write error!", "Problem",
	MB_OK | MB_ICONSTOP);
               
}
             
      */    
    // Some check....!!!!
    
    return 1;
}

 
 
void setFiledt(int fdat, int ftim)
{
		
	struct utimbuf fildt;
	time_t tim ;	
	struct tm tout;
	
	
	tout.tm_sec = ftim & 0x1F ;
	tout.tm_min = (ftim>>5) & 0x3F ;
	tout.tm_hour = (ftim>>11) & 0x1F ;
	
	tout.tm_mday = fdat & 0x1F ;
	tout.tm_mon = ((fdat>>5) & 0x0F) - 1 ;
	tout.tm_year = ((fdat>>9) & 0x7F) + 80 ;
	
	tout.tm_isdst = 0 ;
	
		
	//dirLabel->setText(asctime( &tout));
		
	tim = mktime( &tout) ;
	
	fildt.actime = tim;
	fildt.modtime = tim;
	
	utime(dstr, &fildt) ;
	
	
	
}
 
   
  
  
  
  
  
  
  
void gemd::subdirh()
{
	  
	dstr[11] = 0 ;
               
	ClunOfParent = staclu ; // Keep this value for SubDIR creation  
                      
	
	if ( sublev ) {
		
			int spafl;
			subpath[0] = '/';
			c=1;	
			
		for (o=0;o<sublev;o++) 
		{ 			
			spafl = 0;
			
			for (i=0;i<11;i++) {
				
				k = subnam[i][o+1] ;
				
				if ( k )  
				{
					if ( k == ' ' )  spafl++;
											
					else
					{     if ( spafl>0 )
					{ subpath[c++] = '.' ; spafl = -111 ; }
					
						subpath[c++] = k ;
					}
							
				}
			}
			subpath[c++]='/';
		}	
			subpath[c]= 0;
				
		
	
	//strcat(subpath,dstr);
	
	dirLabel->setText(subpath);
	}
	
	else 	dirLabel->setText(dstr);
			//SetDlgItemText(hDlgWnd,CurDir,dstr);   
               
              
              // Now load it to dirbuf
              
	msapos = 0;
	n = 0; // Counter for FatClus
              
	fatop2:    fatlosn(staclu) ;             
           //load cluster
          
           // Store loaded cluster start sector numbers for further writes!!
                      
	DirClus[n] = logsec ;
           
	res = loadsecs(logsec, clusec, dirbuf+msapos); 
          
	if (!res) goto subdirhend ;
                     
             
	msapos = msapos+clusec*SecSiz ; 
	n++ ;              
	if ( nextf>0xFFF0 ) goto subdirf ; // For FAT 16
	staclu = nextf ;
	if (msapos<512*98) goto fatop2 ;    
      
      subdirf: 
             
		      CurDirLen = n*clusec ; // Current DIR length in sectors  
            //Clear following sector for sure
            
      for (o=0;o<512;o++) dirbuf[msapos+o] = 0 ;  
              
      subdirl:        filesLB->clear();
		      //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_RESETCONTENT, 0, 0);  //Empty list
        
       
      direc = 0 ;
               
        
      //for (n=0;n<512*98; n=n+32) {
        
      for (n=0;n<CurDirLen*SecSiz; n=n+32) {
	      
	      if (dirbuf[n] == 0) break ;
	      if (dirbuf[n] == 0xE5) continue ; // Skip deleted
	      if (dirbuf[n] == 0x2E) continue ; // Skip subdir marks
        
	      dirpos[direc] = n ; // Store pos (bytewise) in DIR
	      direc++ ;
        
	      for (k=0;k<8;k++) {
		      dstr[k] = dirbuf[n+k] ; }
           
		      dstr[8] = '.' ;
		      dstr[9] = dirbuf[n+8];
		      dstr[10] = dirbuf[n+9];
		      dstr[11] = dirbuf[n+10];
		      dstr[12] = ' ';
		      dstr[13] = ' ';
              
		      dstr[14] = 0 ;
        
		      o = dirbuf[n+11] ;    
		      if ( (o & 16) == 16 ) strcat(dstr, "DIR" );
                              
		      else                    
			      if ( (o & 8) == 8 ) strcat(dstr, "VOL" );   
          // File lenght in bytes from DIR fields:           
		      else {          
			      dirflen = 16777216*dirbuf[n+31]+65536*dirbuf[n+30]+256*dirbuf[n+29]+dirbuf[n+28] ; // Len!!!
			      qhm.setNum(dirflen);
			      //ltoa(dirflen, khm, 10 );
			      strcat(dstr,qhm) ;  }         
          
                
        
         // Put filename in list
			      
			      filesLB->insertItem(dstr,-1);
			      //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_ADDSTRING, 0, (LPARAM) dstr);
            
      }
	
	
	
      subdirhend: ; // Here some error message!!! TODO
		
	
	
	
	
	
}
  
  
  
void gemd::subdl()  
  
{
	 
	filesLB->clear();
		      //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_RESETCONTENT, 0, 0);  //Empty list
               
	direc = 0 ;
                       
	for (n=0;n<CurDirLen*SecSiz; n=n+32) {
        
		if (dirbuf[n] == 0) break ;
		if (dirbuf[n] == 0xE5) continue ; // Skip deleted
		if (dirbuf[n] == 0x2E) continue ; // Skip subdir marks
        
		dirpos[direc] = n ; // Store pos (bytewise) in DIR
		direc++ ;
        
		for (k=0;k<8;k++) {
			dstr[k] = dirbuf[n+k] ; }
           
			dstr[8] = '.' ;
			dstr[9] = dirbuf[n+8];
			dstr[10] = dirbuf[n+9];
			dstr[11] = dirbuf[n+10];
			dstr[12] = ' ';
			dstr[13] = ' ';
              
			dstr[14] = 0 ;
        
			o = dirbuf[n+11] ;    
			if ( (o & 16) == 16 ) strcat(dstr, "DIR" );
                              
			else                    
				if ( (o & 8) == 8 ) strcat(dstr, "VOL" );   
        	  // File lenght in bytes from DIR fields:           
			else {          
				dirflen = 16777216*dirbuf[n+31]+65536*dirbuf[n+30]+256*dirbuf[n+29]+dirbuf[n+28] ; // Len!!!
				qhm.setNum(dirflen);
			     
				strcat(dstr,qhm) ;  }         
                         
         // Put filename in list
			      
				filesLB->insertItem(dstr,-1);
			      
            
	}	
	
	
	
	
}
  
  
  
  
  
  
  
  
  
  
  
    
void gemd::ShowUF()  
{
	
	int SecsFact ;
               
 // Walk FAT for free clusters
 
   //msapos = 4 ; // pos of first FAT entry
	dpos = 0 ;
	SecsFact = SecSiz/512 ;
    
	for( n = 4; n < (2*PartClu+4);  n = n+2 ) {
         
		fx = fatbuf[n]+256*fatbuf[n+1] ;
     
		if ( fx == 0 ) dpos++  ;
       
	}
     
	pfrec = dpos ;
	pusec = PartClu-dpos ;  
       
          
	strcpy(dstr,"Used: ");
	
	qhm.setNum( (pusec*clusec)/2*SecsFact);
	//ltoa( (pusec*clusec)/2 ,khm, 10);
	strcat(dstr,qhm);
	strcat(dstr, " KB  Free: ");
	qhm.setNum( (dpos*clusec)/2*SecsFact );
	//ltoa( (dpos*clusec)/2 ,khm, 10);
     //ltoa( fatbuf[2244]+256*fatbuf[2245] ,khm, 10);
	strcat(dstr,qhm);                  
	strcat(dstr, " KB");
     
	usfrLabel->setText(dstr);
	//SetDlgItemText(hDlgWnd,pufre,dstr);          
              
}
  
  
  
  
  
  
                  
void	gemd::opensubd()
{      
				   
	int litc = filesLB->count() ;
	if (litc==0) goto opsubdend;  
	
	
	for (p=0;p<litc;p++) {        
                   
			// see is item selected:
		if ( filesLB->isSelected(p) ) 
		{
				
			dpos = dirpos[p] ; // Position in DIR, only first selection    
		o = dirbuf[dpos+11] ;    
		if ( (o & 16) != 16 ) break;    // If not SubDir break
                  
		//SetCursor( jc) ;
		sublev++ ;
              
		staclu = dirbuf[dpos+26]+256*dirbuf[dpos+27] ;    
              
              //Get dir name:
                    
		for (o=0;o<11;o++) { dstr[o] = dirbuf[dpos+o] ;
              // store SUBdir name
			subnam[o][sublev] = dirbuf[dpos+o]; }
              
		subdirh() ;
	           
	
	
	
		
      break;    }  // perform only for first selected item!!!
      }  
              
                
      opsubdend: ;
      
}               
                  
  
             
             
void gemd::loadroot()
{             
                   
             
   lseek(drih, partpos+DirBeg*SecSiz, 0 );
   read(drih, dirbuf, dirsec*SecSiz);                
                        
      
   if (Swf)
   {   // Swap low/high bytes:
                 
	   for (m=0;m<dirsec*SecSiz;m=m+2)
	   { k=dirbuf[m] ; dirbuf[m]=dirbuf[m+1];dirbuf[m+1]=k; }
   }              
   
   
               // List files from ROOT dir:
                       
                       
   dirLabel->setText("ROOT");
  //SetDlgItemText(hDlgWnd,CurDir,"ROOT");    
   CurDirLen = dirsec ;
                         
   listdtest:      filesLB->clear();
		  //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_RESETCONTENT, 0, 0);  //Empty list
        
   sublev = 0;
   ClunOfParent = 0 ;
   direc = 0 ;
               
        
   for (n=0;n<SecSiz*dirsec;n = n+32) {
        
	   if (dirbuf[n] == 0) break ;
	   if (dirbuf[n] == 0xE5) continue ; // Skip deleted
        
	   dirpos[direc] = n ; // Store pos (bytewise) in DIR
	   direc++ ;
        
	   for (k=0;k<8;k++) {
		   dstr[k] = dirbuf[n+k] ; }
           
		   dstr[8] = '.' ;
		   dstr[9] = dirbuf[n+8];
		   dstr[10] = dirbuf[n+9];
		   dstr[11] = dirbuf[n+10];
		   dstr[12] = ' ';
		   dstr[13] = ' ';
              
		   dstr[14] = 0 ;
        
		   o = dirbuf[n+11] ;    
		   if ( (o & 16) == 16 ) strcat(dstr, "DIR" );
                              
		   else                    
			   if ( (o & 8) == 8 ) strcat(dstr, "VOL" );   
          // File lenght in bytes from DIR fields:           
		   else {          
			   dirflen = 16777216*dirbuf[n+31]+65536*dirbuf[n+30]+256*dirbuf[n+29]+dirbuf[n+28] ;
			   qhm.setNum(dirflen);
			  //ltoa(dirflen, khm, 10 );
			   strcat(dstr,qhm) ;  }         
          
        
         // Put filename in list
			   filesLB->insertItem(dstr,-1) ;
			  //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_ADDSTRING, 0, (LPARAM) dstr);
            
   }     
              
            
   ShowUF() ;                          
    
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  



gemd::gemd(QWidget *parent, const char *name)
    :gemdBase(parent, name)
{
	
	
	
	 // Open file or physical drive:
	
	if (selected<16) 
	{  for (k=0;k<9;k++) physd[k] = detDev[k][selected];
	
	if (ov2ro)  if ( SecCnt>2097152 ) {
		drih = open(physd, O_RDONLY );
	} 
	else
	drih = open(physd, O_RDWR  );
	}
	
	else drih = open(loadedF, O_RDWR  );
         
                            
	if (drih <0) {   		    
		KMessageBox::error( this, "Drive/file open error ", "Problem" );
		goto gemdpex;
		
		//return ;  // Not good!!!
	}
	
	
	
	// Load bootsector
 
	read(drih, bufr, 512); 


	if ( Filesys == 13 )
	{
   
		Gpartp[0] = 0 ;
           
		m = bufr[19]+256*bufr[20] ;
		strcpy(dstr,"DOS16 part. ");
             
             
		if ( m == 0 ) {
			m = bufr[32]+bufr[33]*256 + bufr[34]*65536 +bufr[35]*16777216 ;
			strcpy(dstr,"BIGDOS part. ");
		}
               
		n = (bufr[11]+256*bufr[12])>>9 ;   // Sector size
		Gparsiz[0] = m ;
       
		qhm.setNum(Gparsiz[0]/2*n);
		strcat(dstr,qhm);
		strcat(dstr," KB");
              
		partLB->insertItem(dstr,-1) ;
       
       
	}
	
	
	
	
   // Detect partition system - if DOS (PC) then look only DOS16 and BIGDOS partitions
   // And in extend. partitions
  
  
	if ( Filesys == 1 ) {
  
    
		p = 0 ; // Partition counter (index)
   
		for (i=0x1BE;i<0x1EF;i=i+16) { // Four primary slots
      
			if ( bufr[i+4] == 6 ) {  // BigDOS code   
				strcpy(dstr,"BigDOS prim.: ");
				goto dgetprip; }
          
          
				if ( bufr[i+4] == 4 ) {  // DOS16 code   
					strcpy(dstr,"DOS16 prim.: ");
					goto dgetprip; }
          
					goto dlookext;
  
           // Position, absolute, in sectors
					dgetprip:    Gpartp[p] = bufr[i+8]+bufr[i+9]*256 + bufr[i+10]*65536 +bufr[i+11]*16777216 ;
            
            // Get size:
                   
					Gparsiz[p] = bufr[i+12]+bufr[i+13]*256 + bufr[i+14]*65536 +bufr[i+15]*16777216 ;       
             // Not looking CHS entries!

             // Size in KB:
               
					qhm.setNum(Gparsiz[p]/2);
					//ltoa(Gparsiz[p]/2,khm,10) ;
					strcat(dstr,qhm);
					strcat(dstr," KB");
              
					partLB->insertItem(dstr,-1) ; 
					//SendMessage(GetDlgItem(hDlgWnd, GemP), LB_ADDSTRING, 0, (LPARAM) dstr );       
					p++ ;
					if ( p> 24 ) break ;

           
     dlookext:      
		     if ( bufr[i+4] == 5 ) {  // Extended       
             
       
       
        // Position, absolute, in sectors
	     epos = bufr[i+8]+bufr[i+9]*256 + bufr[i+10]*65536 +bufr[i+11]*16777216 ;
          
   dosfp1:   
             
			   strcpy(dstr,"Ext. ");
            
   lseek(drih, epos*512, 0);
   read(drih, buf2, 512);
          
            // First entry must be regular partition
         
         
   if ( buf2[0x1C2] == 6 ) {  // BigDOS code   
	   strcat(dstr,"BigDOS: ");
	   goto dgetexp; }
          
          
	   if ( buf2[0x1C2] == 4 ) {  // DOS16 code   
		   strcat(dstr,"DOS16: ");
		   goto dgetexp; }
          
		   goto dlookext2;
           
          
            //Get pos, absolute:
                  
		   dgetexp:    Gpartp[p] =  epos + buf2[0x1C6]+buf2[0x1C7]*256 + buf2[0x1C8]*65536 +buf2[0x1C9]*16777216 ;     
            
            // Get size:
                   
		   Gparsiz[p] = buf2[0x1CA]+buf2[0x1CB]*256 + buf2[0x1CC]*65536 +buf2[0x1CD]*16777216 ;       

             // Size in KB:
               
		   qhm.setNum(Gparsiz[p]/2);
		   //ltoa(Gparsiz[p]/2,khm,10) ;
		   strcat(dstr,qhm);
		   strcat(dstr," KB");
              
		   partLB->insertItem(dstr,-1) ; 
		   //SendMessage(GetDlgItem(hDlgWnd, GemP), LB_ADDSTRING, 0, (LPARAM) dstr );       
		   p++ ;
		   if ( p> 24 ) break ;
              // Look for further Extended slots:
                      
              
		   dlookext2:     if ( buf2[0x1D2] == 5 ) {
              
              
			   epos = epos+buf2[0x1D6]+buf2[0x1D7]*256 + buf2[0x1D8]*65536 +buf2[0x1D9]*16777216 ;
              
			   goto dosfp1 ;              
		   }
              
		   if ( buf2[0x1D2] == 0 ) break ; // End of chain
              
		     }

         
                    
                    
  
		} // i loop end
  
 
  
  
  
	} //if end
  
   
   // See is swapped High/Low... TODO



   // Detecting GemDos partitions
    
	else {
    
		if (Swf)
		{   // Swap low/high bytes:
                 
			for (n=0;n<512;n=n+2)
			{ k=bufr[n] ; bufr[n]=bufr[n+1];bufr[n+1]=k; }
		}  
		
       
		p = 0 ; // Partition counter (index)
   
		for (i=0x1C6;i<0x1F7;i=i+12) { // Four primary slots
      
		if (  ((bufr[i+1] == 'G') & (bufr[i+2] == 'E') & (bufr[i+3] == 'M')) | ((bufr[i+1] == 'B') & (bufr[i+2] == 'G') & (bufr[i+3] == 'M')) )
			{ 
            // Primary GemDos part

				strcpy(dstr,"Primary: ");
            
            // Position, absolute, in sectors
				Gpartp[p] = bufr[i+7]+bufr[i+6]*256 + bufr[i+5]*65536 +bufr[i+4]*16777216 ;
            
            // Get size:
                   
				Gparsiz[p] = bufr[i+11]+bufr[i+10]*256 + bufr[i+9]*65536 +bufr[i+8]*16777216 ;       

             // Size in KB:
				qhm.setNum(Gparsiz[p]/2);
				//ltoa(Gparsiz[p]/2,khm,10) ;
				strcat(dstr,qhm);
				strcat(dstr," KB");
				partLB->insertItem(dstr,-1) ;
				//SendMessage(GetDlgItem(hDlgWnd, GemP), LB_ADDSTRING, 0, (LPARAM) dstr );       
				p++ ;
				if ( p> 24 ) break ;

					}


					if (bufr[i+1] == 'X')
						if (bufr[i+2] == 'G')       
							if (bufr[i+3] == 'M') { 
            // Extended GemDos slot

          
            
            // Position, absolute, in sectors
						eposIn = bufr[i+7]+bufr[i+6]*256 + bufr[i+5]*65536 +bufr[i+4]*16777216 ;
						epos = eposIn ;
						
				   gemsfp1:     strcpy(dstr,"Extended: ");
            
						lseek(drih, epos*512, 0 );
						read(drih, buf2, 512);
          
						
						if (Swf)
						{   // Swap low/high bytes:
                 
							for (n=0;n<512;n=n+2)
							{ k=buf2[n] ; buf2[n]=buf2[n+1];buf2[n+1]=k; }
						} 
						
            // First entry must be regular partition
         
             
				if ( ((buf2[0x1C7] == 'G') & (buf2[0x1C8] == 'E')  &  (buf2[0x1C9] == 'M'))| ((buf2[0x1C7] == 'B') & (buf2[0x1C8] == 'G')  &  (buf2[0x1C9] == 'M')) ) 
         
						{                   
         
          
            //Get pos, absolute:
                  
							Gpartp[p] = epos + buf2[0x1CD]+buf2[0x1CC]*256 + buf2[0x1CB]*65536 +buf2[0x1CA]*16777216 ;     
            
            // Get size:
                   
							Gparsiz[p] = buf2[0x1D1]+buf2[0x1D0]*256 + buf2[0x1CF]*65536 +buf2[0x1CE]*16777216 ;       

             // Size in KB:
							qhm.setNum(Gparsiz[p]/2);
							//ltoa(Gparsiz[p]/2,khm,10) ;
							strcat(dstr,qhm);
							strcat(dstr," KB");
              
							partLB->insertItem(dstr,-1) ;
							//SendMessage(GetDlgItem(hDlgWnd, GemP), LB_ADDSTRING, 0, (LPARAM) dstr );       
							p++ ;
							if ( p> 24 ) break ;
              // Look for further Extended slots:
                      
              
							if (buf2[0x1D3] == 'X')
								if (buf2[0x1D4] == 'G')       
									if (buf2[0x1D5] == 'M') { 
              
								epos = eposIn+buf2[0x1D9]+buf2[0x1D8]*256 + buf2[0x1D7]*65536 +buf2[0x1D6]*16777216 ;
              
								goto gemsfp1 ;
              
									}
              
								}

							}





		} // i loop end

	} // else end
   
	
	
	
	
	
	
	
	
	
	
	
	
	
	gemdpex: ;
	
}





void gemd::quitP_clicked(int,int)
{

	fileclo(drih);
	gemd::close() ;
	
}




void gemd::partLB_clicked(QListBoxItem*)
{
	
	 selP = partLB->currentItem() ;
		         
                   //  Load bootsector of partition:
                
	partpos =  Gpartp[selP]*512 ;  
	       
	lseek(drih, partpos, 0 );
	read(drih, buf2, 512);     
              
	    
	if (Swf)
	{   // Swap low/high bytes:
                 
		for (n=0;n<512;n=n+2)
		{ k=buf2[n] ; buf2[n]=buf2[n+1];buf2[n+1]=k; }
	} 
	
	
              // Get BPB parameters:
	SecSiz = buf2[11]+256*buf2[12] ;              
	fatsec = buf2[22] ;
	
	n = SecSiz>>9 ;
	dirsec = (buf2[17]/16 + 16*buf2[18])/n ;
	
	clusec = buf2[13] ;  
	ResSec = buf2[14]+256*buf2[15] ;
	FdatS = ResSec+fatsec*2+dirsec ;
	DirBeg = ResSec+fatsec*2 ;
               
	PartSec = buf2[19]+256*buf2[20] ;
               
	if ( PartSec == 0 ) 
		PartSec = buf2[32]+buf2[33]*256 + buf2[34]*65536 +buf2[35]*16777216 ;
               
	PartClu = (PartSec-FdatS)/clusec ;
               
                            
        // Display begin sector and count of sectors of selected part. :
	       
	qhm.setNum( Gpartp[selP] );
	strcpy(dstr,qhm);
	strcat(dstr,", ");
	
	qhm.setNum( Gparsiz[selP] );
	if ( Filesys == 13) qhm.setNum( PartSec );
	
	strcat(dstr,qhm);
	
	partinfLab->setText(dstr);
	
	
               
               
               // Load FAT #1
	lseek(drih, partpos+SecSiz*ResSec, 0) ;               
	read(drih, fatbuf, fatsec*SecSiz);  
               
             
	if (Swf)
	{   // Swap low/high bytes:
                 
		for (m=0;m<fatsec*SecSiz;m=m+2)
		{ k=fatbuf[m] ; fatbuf[m]=fatbuf[m+1];fatbuf[m+1]=k; }
	}     
                // Testing:
               /* flout = fopen("TheFat.bin","wb");
	fwrite(fatbuf,1,fatsec*512 ,flout); 
	fclose(flout);
	       */
               
               // Load root DIR
  		loadroot() ;    
	
}




void gemd::filesLB_clicked(QListBoxItem*)
{
	
}

	
	
void gemd::dirupP_clicked(int,int)
{
	
	                 
             //are we in subdir? :
		if (dirbuf[0] != 0x2E) goto dirupend ;
		
		//SetCursor( jc) ;
		staclu = dirbuf[32+26]+256*dirbuf[32+27] ;             
                   
		if (staclu == 0 )  loadroot() ;     //it is root then
            
		else {
		sublev-- ;
            
		for (o=0;o<11;o++)  dstr[o] = subnam[o][sublev] ;
            
		subdirh() ;         
                        
		}
            
		dirupend: ;
	
	
	
}













void gemd::opdirP_clicked(int,int)
{
	opensubd();
}






void gemd::extrP_clicked(int,int)
{
	   setCursor(QCursor(Qt::WaitCursor));
		
	                   
		reculev = 0 ;
		pdirpos[0] = 0;
          
          //Clear sel buffer:
                  
		//for (s=0;s<256;s++)  selbuf[s] = 0 ;      
              
              // Get count of items in listbox
		
		int litc = filesLB->count() ;
		if (litc==0) goto extrend;
          
		
		
		//selcnt = SendMessage(GetDlgItem(hDlgWnd, GemF), LB_GETSELCOUNT, 0, 0);
		//if (selcnt == 0) break ;
           
           //if (strlen(dir2)>0) SetCurrentDirectory(dir2) ;
             
		//SetCursor( jc) ;
        
        
		//k =  SendMessage(GetDlgItem(hDlgWnd, GemF), LB_GETSELITEMS, 256, (LPARAM) selbuf);          
            
                              
		for (p=0;p<litc;p++) {        
                   
			// see is item selected:
			if ( filesLB->isSelected(p) ) {
			
			
			dpos = dirpos[p] ; // Position in DIR
                         
			
			
          //avoid VOL extraction
			o = dirbuf[dpos+11] ;    
			if ( (o & 8) == 8 )  continue ; 
               
			msapos = 0 ;  
               
               //Get filename:
                     
			for(i=0;i<8;i++) {
				if (dirbuf[dpos+i] == 32) break ;               
				dstr[i] = dirbuf[dpos+i] ;    }
                
				if (dirbuf[dpos+8] == 32) goto namegot ;
                
				dstr[i] = '.' ;
				i++ ;
                     
				for(k=8;k<11;k++) {
					if (dirbuf[dpos+k] == 32) break ;               
					dstr[i] = dirbuf[dpos+k] ; 
					i++ ;   } 
                   
                   
					namegot:    dstr[i] = 0 ; // String term.  
					
					exwht->setText(dstr);
					//SetDlgItemText(hDlgWnd,exwht,dstr);
					errti->setText(" ");
					//SetDlgItemText(hDlgWnd,errti," ");    
          
       					kapp->processEvents();
          
					//sleep(1);
					
							
             
              
              // Recursive subdir extraction
              // Open SubDir, show files, and extract
              // Need to store level, parent & pos in parent
              
               
					o = dirbuf[dpos+11] ;  
               
                 
					if ( (o & 16) == 16 ) { 
                       
                 
              // Store pos in parent, here by selection number
						pdirpos[sublev] = p ;
              
						sublev++ ; reculev++ ;
              
						strcpy(subdnam,dstr) ;
              
						staclu = dirbuf[dpos+26]+256*dirbuf[dpos+27] ;    
              //ClunOfParent = staclu ; // Keep this value for SubDIR creation  
                  
              //Get dir name:
                    
						for (o=0;o<11;o++) { dstr[o] = dirbuf[dpos+o] ;
              // store SUBdir name
							subnam[o][sublev] = dirbuf[dpos+o]; }
              
							sublevup:        dstr[11] = 0 ;
              
                         
							dirLabel->setText(dstr);
							//SetDlgItemText(hDlgWnd,CurDir,dstr);   
            
              
              
              // Now load it to dirbuf
              
							msapos = 0;
							n = 0;    
							recfatop2:    fatlosn(staclu) ;             
           //load cluster
       
							DirClus[n] = logsec;
							res = loadsecs(logsec, clusec, dirbuf+msapos); 
          
							if (!res) break ;
                     
             
							msapos = msapos+clusec*SecSiz ;  
							n++ ;             
							if ( nextf>0xFFF0 ) goto recsubdirf ;
							staclu = nextf ;
							if (msapos<512*98) goto recfatop2 ;    
      
      recsubdirf: 
              
		      CurDirLen = n*clusec ;  
            //Clear following sector for sure
            
      for (o=0;o<512;o++) dirbuf[msapos+o] = 0 ;  
              
      filesLB->clear();
      //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_RESETCONTENT, 0, 0);  //Empty list
        
     // currentPath = QDir::homeDirPath();
     // currentPath.append("/");  
      
      
      getcwd(DestDir,256);
      
    //  exwht->setText(DestDir); // testing  
            
        
        // Change current DIR:
         
    
    //  currentPath.append(subdnam);
      //strcat(DestDir, subdnam) ;         
       
       // Create that DIR:
                 
      
      o = mkdir(subdnam,0777);
      //o = CreateDirectory(subdnam, 0) ;
       
      
      o = chdir(subdnam);
      //SetCurrentDirectory(subdnam) ; // Funny
         
      
      // SetDlgItemText(hDlgWnd,exwht,DestDir);  //testing
      
      
      zz = 0 ; // When goes deeper in sub levels
               
      recbackn:        direc = 0 ;
       
      
      			
      
    
      
      
      for (n=zz;n<SecSiz*dirsec; n=n+32) {
        
 
	      recdirsc:     if (dirbuf[n] == 0) break ;
	      if (dirbuf[n] == 0xE5) continue ; // Skip deleted
	      if (dirbuf[n] == 0x2E) continue ; // Skip subdir marks
        
         // dirpos[direc] = n ; // Store pos (bytewise) in DIR
	      direc++ ;
        
	      for (k=0;k<8;k++) {
		      dstr[k] = dirbuf[n+k] ; }
           
		      dstr[8] = '.' ;
		      dstr[9] = dirbuf[n+8];
		      dstr[10] = dirbuf[n+9];
		      dstr[11] = dirbuf[n+10];
		      dstr[12] = ' ';
		      dstr[13] = ' ';
              
		      dstr[14] = 0 ;
        
		      o = dirbuf[n+11] ;    
		      if ( (o & 16) == 16 ) strcat(dstr, "DIR" );
                              
		      else                    
			      if ( (o & 8) == 8 ) strcat(dstr, "VOL" );   
          // File lenght in bytes from DIR fields:           
		      else {          
			      dirflen = 16777216*dirbuf[n+31]+65536*dirbuf[n+30]+256*dirbuf[n+29]+dirbuf[n+28] ;
			      qhm.setNum(dirflen);
			      //ltoa(dirflen, khm, 10 );
			      strcat(dstr,qhm) ;  }      
        
         // Put filename in list
			      filesLB->insertItem(dstr,1);
			      //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_ADDSTRING, 0, (LPARAM) dstr);
         // Highlight it
         
         			//TODO
			      
			      //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_SETTOPINDEX, direc-1, 0); 
			      
			      //UpdateWindow(hDlgWnd) ; 
        
         // Perform extraction immediately:
         // But only from position after SubDir if back from SubDir extract
                    
        
           //Get filename:
                     
			      for(i=0;i<8;i++) {
				      if (dirbuf[n+i] == 32) break ;               
				      dstr[i] = dirbuf[n+i] ;    }
                
				      if (dirbuf[n+8] == 32) goto recnamegot ;
                
				      dstr[i] = '.' ;
				      i++ ;
                     
				      for(k=8;k<11;k++) {
					      if (dirbuf[n+k] == 32) break ;               
					      dstr[i] = dirbuf[n+k] ; 
					      i++ ;   } 
                
                         
                                 
					      recnamegot:    dstr[i] = 0 ; // String term.  
					      
					      exwht->setText(dstr);
					      errti->setText(" ");
					      //SetDlgItemText(hDlgWnd,exwht,dstr);
					      //SetDlgItemText(hDlgWnd,errti," ");       
        
        
        
            //  Sub-SubDir extraction - must reenter
               
					      o = dirbuf[n+11] ;    
					      if ( (o & 16) == 16 ) {
                    
						      pdirpos[sublev] = n ;   // Store pos in dir  
                    
              
						      sublev++ ; reculev++ ;
              
						      strcpy(subdnam,dstr) ;
              
						      staclu = dirbuf[n+26]+256*dirbuf[n+27] ;    
                  
              //Get dir name:
                    
						      for (o=0;o<11;o++) { dstr[o] = dirbuf[n+o] ;
              // store SUBdir name
							      subnam[o][sublev] = dirbuf[n+o]; }
              
                          
							      goto sublevup ;                  
                    
                    
                    
					      } // !!!!
               
               
					      msapos = 0 ;  
               
                     
                  //Now load file from drive/image:
           
                      
					      staclu = dirbuf[n+26]+256*dirbuf[n+27] ;
					      dirflen = 16777216*dirbuf[n+31]+65536*dirbuf[n+30]+256*dirbuf[n+29]+dirbuf[n+28] ;              
					      remain = dirflen; // remain to save   
                      
					      fdat = dirbuf[n+24]+256*dirbuf[n+25] ;
					      ftim = dirbuf[n+22]+256*dirbuf[n+23] ;
         // Open save file:   
					      flout = fopen(dstr,"wb");
					      if (flout == NULL) { 
						      
						      KMessageBox::error( this, "File open error.", dstr );
						      //MessageBox(NULL, "File open error.", dstr, MB_OK | MB_ICONERROR); 
                          
						      goto recextrer ; }  
            
						      msapos2 = 0 ;  
						      tosave = SecSiz*clusec ;
        
        
           // Get sector(s) to load and next cluster
           
						      recnextclu:         fatlosn(staclu) ;   
           
                    
						      res = loadsecs(logsec, clusec, trasb); 
						      if (!res) goto recextrer ;              
						      if ( remain<(SecSiz*clusec) ) tosave = remain ; 
                
                                         
						      msapos2 = msapos2+fwrite(trasb,1,tosave ,flout);   
         
               //msapos = msapos+tosave ;
						      remain = remain-SecSiz*clusec;
						      if ( remain<0 ) goto recsaveexf;
               
           //msapos = msapos+clusec*512 ;               
						      if ( nextf>0xFFF0 ) goto recsaveexf ;
						      staclu = nextf ;
						      goto recnextclu;
  
  
           
   recsaveexf:            
          // res = fwrite(clulist,1,1536,flout); //testing
		   fclose(flout); 
   
   if (msapos2 != dirflen ) goto recextrer ; // If not match len
            
     if (!timestCur)   setFiledt(fdat, ftim); 
   
   
   recnextfil:     o=1;        // next file    
                   
      }  // n loop end
                        
              
             // When over, go back to parent DIR 
              
      sublev-- ; reculev-- ;
          
          // Need to set dest DIR, ST Dir name, reload DIR in dirbuf 
          
          
      
      chdir("..");
      //SetCurrentDirectory("..") ; 
      //TODO ???
      //o = GetCurrentDirectory(MAX_PATH, DestDir) ;
         
          // Back ST dir level:
                  
           //are we in subdir? :
      if (dirbuf[0] != 0x2E) break ;
              
      staclu = dirbuf[32+26]+256*dirbuf[32+27] ;             
                   
             // if (staclu == 0 ) goto loadroot ;     //it is root then!!!!!!!****!!!
            
              //sublev-- ;
            
      for (o=0;o<11;o++)  dstr[o] = subnam[o][sublev] ;
                            
      dstr[11] = 0 ;             
                      
          // Print out ST dir:
          
      dirLabel->setText(dstr);
      //SetDlgItemText(hDlgWnd,CurDir,dstr);  
       
          // Setting back pos:
       
       
         // If back to root:
         // Need to reload root dir and jump to next pos extract
       
      if (!sublev) {
       
        //logsec = DirBeg ;
        //seccnt = dirsec ;
	      res = loadsecs(DirBeg, dirsec, dirbuf);
        // Error test...!!!!
	      dirLabel->setText("ROOT");
	      //SetDlgItemText(hDlgWnd,CurDir,"ROOT");    
                         
	      backcontent:    filesLB->clear();
			      //SendMessage(GetDlgItem(hDlgWnd, GemF), LB_RESETCONTENT, 0, 0);  //Empty list
        
        
	      direc = 0 ;
               
        
	      for (n=0;n<SecSiz*dirsec;n = n+32) {
        
		      if (dirbuf[n] == 0) break ;
		      if (dirbuf[n] == 0xE5) continue ; // Skip deleted
		      if (dirbuf[n] == 0x2E) continue ; // Skip subdir marks
        
		      dirpos[direc] = n ; // Store pos (bytewise) in DIR
		      direc++ ;
        
		      for (k=0;k<8;k++) {
			      dstr[k] = dirbuf[n+k] ; }
           
			      dstr[8] = '.' ;
			      dstr[9] = dirbuf[n+8];
			      dstr[10] = dirbuf[n+9];
			      dstr[11] = dirbuf[n+10];
			      dstr[12] = ' ';
			      dstr[13] = ' ';
              
			      dstr[14] = 0 ;
        
			      o = dirbuf[n+11] ;    
			      if ( (o & 16) == 16 ) strcat(dstr, "DIR" );
                              
			      else                    
				      if ( (o & 8) == 8 ) strcat(dstr, "VOL" );   
          // File lenght in bytes from DIR fields:           
			      else {          
				      dirflen = 16777216*dirbuf[n+31]+65536*dirbuf[n+30]+256*dirbuf[n+29]+dirbuf[n+28] ;
				      qhm.setNum(dirflen);
				      //ltoa(dirflen, khm, 10 );
				      strcat(dstr,qhm) ;  }         
          
                
        
         // Put filename in list
				      filesLB->insertItem(dstr,-1);
				    //  SendMessage(GetDlgItem(hDlgWnd, GemF), LB_ADDSTRING, 0, (LPARAM) dstr);
            
            
	      }
       
	      
	      kapp->processEvents();  
       
	      goto nextfil ;
       
       
      }
       
     
            
           
            // must reload dirbuf and continue loop
            
      msapos = 0;
      n = 0;
      fatop3:    fatlosn(staclu) ;             
           //load cluster
      DirClus[n] = logsec;
      res = loadsecs(logsec, clusec, dirbuf+msapos); 
          
      if (!res) break ;
                     
             
      msapos = msapos+clusec*SecSiz ; 
      n++ ;              
      if ( nextf>0xFFF0 ) goto subdirf3 ;
      staclu = nextf ;
      if (msapos<512*98) goto fatop3 ;    
      
      subdirf3: 
		      CurDirLen = n*clusec ; 
            //Clear following sector for sure
            
      for (o=0;o<512;o++) dirbuf[msapos+o] = 0 ;  
   
  
   // Here is dir up, not to root:
            
            
      if (reculev)  {
            
	      zz = pdirpos[sublev] ;
	      zz = zz+32 ; // to next entry
            
	      goto recbackn ;  // if not to level of selections!!!
   
      }
   
      
            // If to level of selections (not root)   
      goto backcontent ; // continue via root section
     
     
                    
             
  
     // SetDlgItemText(hDlgWnd,exwht,DestDir);  //testing
                 
     // break ; // testing
        
              
           
      recextrer:   errti->setText("Error!");
		      //SetDlgItemText(hDlgWnd,errti,"Error!");
      sleep(1) ; // Pause to allow seeing error message
      goto recnextfil ;
          
                      
        
        
					} // = 16 if end
              
                         
                    
                  //Now load file from disk/image file:
           
                      
					staclu = dirbuf[dpos+26]+256*dirbuf[dpos+27] ;
					dirflen = 16777216*dirbuf[dpos+31]+65536*dirbuf[dpos+30]+256*dirbuf[dpos+29]+dirbuf[dpos+28] ;            
					remain = dirflen; // remain to save     
            
					fdat = dirbuf[dpos+24]+256*dirbuf[dpos+25] ;
					ftim = dirbuf[dpos+22]+256*dirbuf[dpos+23] ;
					
                    // Open save file:   
					opensf:           flout = fopen(dstr,"wb");
					if (flout == NULL) { 						
						KMessageBox::error( this, "File open error.", dstr );
						//MessageBox(NULL, "File open error.", dstr, MB_OK | MB_ICONERROR); 
                          
						goto extrer ; }  
            
						msapos2 = 0 ;  
						tosave = SecSiz*clusec ;      
           // Get sector(s) to load and next cluster
           
						nextclu:            fatlosn(staclu) ;   
       // testing:
      //   res = loadsecs(logsec, 750, trasb+msapos); 
       //        if (!res) break ;
       //   goto opensf ;
        // testing end
               
                          
						res = loadsecs(logsec, clusec, trasb); 
						if (!res) goto extrer ;              
						if ( remain<(SecSiz*clusec) ) tosave = remain ; 
                
                                         
						msapos2 = msapos2+fwrite(trasb,1,tosave ,flout);   
         
               //msapos = msapos+tosave ;
						remain = remain-SecSiz*clusec;
						if ( remain<0 ) goto saveexf;
               
           //msapos = msapos+clusec*512 ;               
						islast:     if ( nextf>0xFFF0 ) goto saveexf ;
						staclu = nextf ;
						goto nextclu;
           
   saveexf:             
          // res = fwrite(clulist,1,1536,flout); //testing
		   fclose(flout); 
   if ( msapos2 != dirflen ) goto extrer ;
         
   if (!timestCur)  setFiledt(fdat, ftim); 
     
   
   
   
   
   
            
   nextfil:     ;     } // if selected end 
                   
		}  // p loop end
        
        
                                       
		goto extrend;
           
		extrer:    errti->setText("Error!");
				//SetDlgItemText(hDlgWnd,errti,"Error!");
		sleep(1) ; // Pause to allow seeing error message
		goto nextfil ;
    
    
    		
		
	
		extrend:  setCursor(QCursor(Qt::ArrowCursor));
	
		
}







void gemd::setddP_clicked(int,int)
{
	
	
	fileName = KFileDialog::getExistingDirectory(0,  this, "Dest. dir:");
	
	
	if( !fileName.isEmpty() )           
	
              {        
              		strcpy(DestDir,fileName);
			//strncpy(DestDir,szFileName,strlen(szFileName)-1);     
			//DestDir[strlen(szFileName)-1] = 0 ;
			
			chdir(DestDir);
			//SetCurrentDirectory(DestDir) ;
                                          
              
		}
              
        
}





void gemd::addfP_clicked(int,int)
{
	// 1. Multiple file selector
	// 2. Process by files
	// 3. Get file's length and check is enough space
	// 4. Convert filename to 8.3 Capitals
	// 5. Check is name exists
	// etc....
	
	// Miltiple file open dialog:
		
	SeldFiles = KFileDialog::getOpenFileNames(0, i18n("*.prg *.tos *.ttp *.app|ST PRG, TOS, TTP, APP files\n*.bin|Bin files\n*.* *|All files"), this, i18n("Files to add"));
		
	          
	if(!SeldFiles.isEmpty())  {
		
		
		struct stat fparms ;
		time_t tim ;
		tm tout;                              
		
		kapp->processEvents() ; // refresh dialog content 
		setCursor(QCursor(Qt::WaitCursor));
		
		QStringList::Iterator iq = SeldFiles.begin();
		for (; iq != SeldFiles.end(); ++iq)
		
		{	
			strcpy(zxpath, *iq);	// Filename with path?
				
			zxpal = strlen(zxpath) ;
                 
                           

		// extract filename only:
				
					
			p = 0;          
			for (c=zxpal-1; c>0; c--)                      
				if ( zxpath[c] == '/' ) break  ; 
			c++ ;
			while (c<zxpal) 
			{ curzx[p++] = zxpath[c++];
                 //p++ ; c++ ;
			curzx[p] = 0;
			}
                                
			
			// Convert filename to 8.3 cap format
			
		
			zxpal = strlen(curzx) ;		
			
			o = 0;
			i = 0;
			for (c=0;c<zxpal;c++)
			{ k=curzx[c];
				if (k>64) k = k&0xDF ; // Make all capital
				
				if (k==' ') k='_' ; // Space to under
			
			if (!o)	if (k=='.') o = c; // get pos of first .
			if (o) { filext[i] = k; i++; } // build extension	
			
			fnamb[c] = k ;
			}
			
			fnamb[c] = 0;
			filext[i]=0;
			
			// Limit filename base to 8 length	
			
			if (o) {
			fnamb[o] = 0;
			if (o>8) fnamb[8] = 0 ; // max 8 char
			
			if (strlen(filext)>4) filext[4]=0;
			strcat(fnamb,filext);
			}
			else if (zxpal>8) fnamb[8] = 0 ;
			
			
			
			//exwht->setText(fnamb);
			
			
			res = PaddFn(fnamb) ;
			
			
			exwht->setText(dstr);   // show corrected filename
			              
			kapp->processEvents(); 
			//sleep(1); //testing
	
			
		// Open file and get it's length:
				
			
			// Ignore directories:			
			
			 stat( zxpath , &fparms ); 
				if (S_IFDIR & fparms.st_mode )  break ;  
			
			
			
			finp = fopen(zxpath,"rb");
			
			if (finp == NULL) { 
				KMessageBox::error( this, "File open error ", zxpath );
				break;
			}		
					
			
	
                              //get filelength :
			fseek(finp, 0, SEEK_END) ;
			stfilelen = ftell(finp); //Filelength got
           
			fseek(finp,0,SEEK_SET); // Back to filestart dude!
			
			// Get needed cluster count:
			
			n = clusec*SecSiz;
			fileClucnt = stfilelen/n ;
			if ( stfilelen % n) fileClucnt++ ;
			
			
			
			
			  // Check is there enough space:
           
			if ( pfrec<fileClucnt ) {
           
				KMessageBox::error( this, "No place for file", zxpath );
				fclose(finp); goto addfend; 
			}
       
       			// Is name already in cur DIR?    
			res = SamNam(dstr) ;
        
			if (!res) {
           
				KMessageBox::error( this, "Name exists!", "Name already in Dir.");
				fclose(finp); goto addfend; 
				
			}
        
			
			  // Now seek first free slot in DIR:
                 
			res = GetFfds() ;
                 
			if (!res) {
           
				KMessageBox::error( this, "DIR full!", "Error");
				fclose(finp); goto addfend; 
				}
           
           // Copy name at dpos, add timestamp and directory flag
           
			for (n=0;n<11;n++) dirbuf[dpos+n]=dstr[n];
			//Enter filelength in filerecord:
			
			m = stfilelen;
			dirbuf[dpos+28] = m;
			dirbuf[dpos+29] = m>>8;
			dirbuf[dpos+30] = m>>16;
			dirbuf[dpos+31] = m>>24;
						   
          // Add timestamp:         		
		
			//time_t tim ;
			time(&tim);
					
			
			//struct tm tout;
			
			if (timestCur) localtime_r( &tim, &tout); // Current time for timestamp
			else localtime_r( &fparms.st_mtime, &tout); // Filetime for timestamp
			 					
			
			n = (tout.tm_sec/2)|(tout.tm_min<<5)|(tout.tm_hour<<11) ;
			fatTime = n ;
				
			//qhm.setNum(fileClucnt);
			//errti->setText(qhm); //testing
		
		
			dirbuf[dpos+22] = n;
			dirbuf[dpos+23] = n>>8;         
          					
			n = (tout.tm_mday)|((tout.tm_mon+1)<<5)|( (tout.tm_year-80)<<9) ;
			fatDate = n;
		
			dirbuf[dpos+24] = n;
			dirbuf[dpos+25] = n>>8;  
			
			//Set start cluster #          
        		  // Seek first free cluster
          
			SeekFfc() ;
   
			n = m;
                   
			dirbuf[dpos+26] = n;
			dirbuf[dpos+27] = n>>8;       
			
			//Copy fileclusters in order:
			
			msapos = 0; // pos in file
			
			if ( (stfilelen)<(clusec*SecSiz) )  { k = stfilelen ;
			// First clear:      
				for ( n=0;n<(SecSiz*clusec);n++) trasb[n]=0 ; // clear trasb for have zeros after file end 
			}
			
			else  k =  clusec*SecSiz ; 				
			
			res=fread(trasb,1,k,finp);
			
			logsec = FdatS+(m-2)*clusec ;
			
			writsecs( logsec, clusec, trasb);
			
			// if only 1 cluster mark it as last:
			if (fileClucnt==1) {
				MarkCluL(m) ; 
				goto addfclos ;				
			}
			else {	msapos = clusec*SecSiz;	
				o = 0 ; //flag		
				for (n=1;n<fileClucnt;n++)
				{
					if ( (stfilelen-msapos)<(clusec*SecSiz) ) { k = stfilelen-msapos ;
						for ( i=0;i<(SecSiz*clusec);i++) trasb[i]=0 ; // clear trasb for have zeros after file end 
						o = 1; // flag for last cluster
					}	
					else k = clusec*SecSiz ;
			
					res=fread(trasb,1,k,finp);
			
					oldclun = m;	
										
					if (!SeekNfc(m) ) { MarkCluL(oldclun) ; break ;} // Error message?
								  
					  // write into FAT
										
					MarkCluR(m, oldclun);
					if ( o ) MarkCluL(m) ;	
										
						//oldclun = m;
									
					logsec = FdatS+(m-2)*clusec ;
			
					writsecs( logsec, clusec, trasb);
					msapos = msapos+SecSiz*clusec ;			
				}
				
				
				
			}
			
						
			
			
			
			
	
			addfclos:	fclose(finp);
			
			
			// Write parent DIR with new entry to drive/image:
			
			// need to get pos first (to write 1 cluster only):
			n = dpos/(SecSiz*clusec) ;
			
			if ( sublev == 0 ) writsecs( DirBeg+n*clusec, clusec, dirbuf); 
			
			else  { 			
				writsecs( DirClus[n], clusec, dirbuf);      
			}
			
		
		
		
		
		} // iq for loop end	
		
		
           //Finally write FAT back:
                  
		writsecs( ResSec, fatsec, fatbuf);
          //Second FAT
		writsecs( ResSec+fatsec, fatsec, fatbuf);
          
          
          
          // Refresh Used/free indicator:
                     
		ShowUF() ; 
           
          // Need to refresh listbox content
		subdl() ;
				
		
	} // if Seldfiles end
	
	addfend:  setCursor(QCursor(Qt::ArrowCursor))    ;
}




void gemd::newfP_clicked(int,int)
{
	
	
           //  1. Padd entered filename with zeros by need
           //  2. Check is space on partiton
           //  3. Check is name exists
           //  4. Add it to current DIR
           //  5. Create FAT entries - size (1 clust only) ??
           //  6. Clear sectors and add SubDir .. and 2E at start
           //  7. Rewrite medias DIR and FAT sectors!!!
           //  8. Refresh ListBox with DIR content
           //  9. Refresh Used/Free info at bottom

		litet = dirED->text();
		//GetDlgItemText(hDlgWnd, GemCreD , szText , (LPARAM) 64); 
           
		if ( strlen(litet)==0 ){           
			KMessageBox::error( this, "Invalid filename!", "Enter something" );
			//MessageBox(hDlgWnd, "Invalid filename!", "Enter something",
				   //MB_OK | MB_ICONSTOP);
			
			return;
			//goto credirend;
		}  
           
            
            // Look is name base 8 space:
                    
		c=0;       
		for (n=0;n<8;n++) {
			if (litet[n]==' ') c++ ;
		}
             
		if (c==8) {           
			
			KMessageBox::error( this, "Invalid filename!", "All the spaces..." );
			//MessageBox(hDlgWnd, "Invalid filename!", "All the spaces...",
				 //  MB_OK | MB_ICONSTOP);
			goto credirend;
		}  
                    
            
		
        	strcpy(subdnam,litet);    
		res = PaddFn(subdnam) ;
		if (!res) {
           
			KMessageBox::error( this, "Invalid filename!", "Must have dot before ext." );
			//MessageBox(hDlgWnd, "Invalid filename!", "Must have dot before ext.",
				   //MB_OK | MB_ICONSTOP);
			goto credirend;
		} 
             
            
            // Testing
		exwht->setText(dstr);
		//SetDlgItemText(hDlgWnd,exwht,dstr);

           // On free cluster at least needed
           
		if ( pfrec<1 ) {
           
			KMessageBox::error( this, "Partition full!", "No place");
			//MessageBox(hDlgWnd, "Partition full!", "No place",
				//   MB_OK | MB_ICONSTOP);
			goto credirend; 
		}
       
           
		res = SamNam(dstr) ;
        
		if (!res) {
           
			KMessageBox::error( this, "Name exists!", "Name already in Dir.");
			//MessageBox(hDlgWnd, "Name exists!", "Name already in Dir.",
				 //  MB_OK | MB_ICONSTOP);
			goto credirend;  
		}
        
        
          // Now seek first free slot in DIR:
                 
		res = GetFfds() ;
                 
		if (!res) {
           
			KMessageBox::error( this, "DIR full!", "Error");
			//MessageBox(hDlgWnd, "DIR full!", "Error",
				//   MB_OK | MB_ICONSTOP);
			goto credirend; 
		}
           
           // Copy name at dpos, add timestamp and directory flag
           
		for (n=0;n<11;n++) dirbuf[dpos+n]=dstr[n];
		dirbuf[dpos+11]=16; // DIR flag
           
          // Add timestamp:         		
		
		time_t tim ;
		//tim = time(NULL);
		time(&tim);
		
		struct tm tout;
		localtime_r( &tim, &tout);
		
		//GetSystemTime(&syt);              
		//res = SystemTimeToFileTime( &syt, &fit) ;
             
                        
                  
		//res = FileTimeToDosDateTime(
		//		&fit,	// pointer to 64-bit file time 
		//&FatDate,	// pointer to variable for MS-DOS date  
		//&FatTime 	// pointer to variable for MS-DOS time  
		//			   );
          
     
     
         // itoa(FatDate,dstr,10);
          //SetDlgItemText(hDlgWnd,exwht,dstr); // Testing
     
     
		n = (tout.tm_sec/2)|(tout.tm_min<<5)|(tout.tm_hour<<11) ;
		fatTime = n ;
				
		qhm.setNum(n);
		errti->setText(qhm); //testing
		
		
		dirbuf[dpos+22] = n;
		dirbuf[dpos+23] = n>>8;         
          
		//n = int(FatDate) ;
		
		n = (tout.tm_mday)|((tout.tm_mon+1)<<5)|( (tout.tm_year-80)<<9) ;
		fatDate = n;
		
		dirbuf[dpos+24] = n;
		dirbuf[dpos+25] = n>>8;  
          
         
          
          //Set start cluster #
          
          // Seek first free cluster
          
		SeekFfc() ;
   
		n = m;
                   
		dirbuf[dpos+26] = n;
		dirbuf[dpos+27] = n>>8;       
   
		credirClu[0] = m ; // store spread of created
		credirCnt = 1;
		
		// seek for next 15 free cluster:
		oldclun = m;
		n = SecSiz>>9 ;
		
		while ( credirCnt<(32/(clusec*n)) ) 
		{
		  	
		  if (!SeekNfc(m) ) break ;
		  credirClu[credirCnt] = m ;	
		  
		  // write into FAT
		  MarkCluR(m, oldclun);
		  
		  
		  credirCnt++ ;	
		  oldclun = m;
		}
		
		// mark as last:
                 
		MarkCluL(oldclun) ;       
   
           // Write parent DIR with new entry to drive/image:
		if ( sublev == 0 ) writsecs( DirBeg, dirsec, dirbuf); 
		
		else  { 
			// need to get pos first:
			n = dpos/(SecSiz*clusec) ;
			
			writsecs( DirClus[n], clusec, dirbuf);      
		}
                   
          // Create and write empty SubDir:
		// first cluster of:
		MakeSubF(credirClu[0]);
          
		
   		  // First clear:      
		for ( n=0;n<(SecSiz*clusec);n++) trasb[n]=0 ;  
		
		// clear following clusters of DIR:
		for ( i=1;i<credirCnt;i++)
		{		
		 // Calculate logical sector by cluster #
  		logsec = FdatS+(credirClu[i]-2)*clusec ;
  
		res = writsecs( logsec, clusec, trasb); 	
			
		}        
     
		
		
		
          
          
           //Write FAT back:
                  
		writsecs( ResSec, fatsec, fatbuf);
          //Second FAT
		writsecs( ResSec+fatsec, fatsec, fatbuf);
          
          
          
          // Refresh Used/free indicator:
                     
		ShowUF() ; 
           
          // Need to refresh listbox content
		subdl() ;
           	 
		
		
	credirend: ;
	
	
	
}






void gemd::desallP_clicked(int,int)
{
	filesLB->clearSelection();
	
}

void gemd::filesLB_doubleClicked(QListBoxItem*)
{
	opensubd();
	
}

void gemd::timeCB_clicked()
{
	if( timeCB->isChecked() ) timestCur=1;
	else timestCur=0;
	
}

void gemd::partsBut_clicked()
{
	
	
	OffFS = Gpartp[selP] ;
	ChaSecCnt = Gparsiz[selP] ;
	segflag = 1 ;
	
	
	 //Fileselector call
	fileName = KFileDialog::getSaveFileName(0, i18n("*.img|Img image files\n*.raw|Raw image files\n*.*|All files"), this, i18n("File to save"));
               
       
	if( !fileName.isEmpty() )  {

		fout = fopen(fileName,"wb");
		if (fout == NULL)
		{                   
			KMessageBox::error( this, "File open error ", "Damn!" );
			return;                  
		}
                    
                    
		partpos =  Gpartp[selP]*512 ;         
		
		lseek(drih, partpos, SEEK_SET );
		
                    
		kapp->processEvents() ; // refresh dialog content 
		setCursor(QCursor(Qt::WaitCursor));
		
		copied = 0 ;
		for (m=0;m<Gparsiz[selP];m++)
		{ 
                      
			read(drih, trasb, 512);

			if (Swf)  // by SW detection!
			{
				for (n = 0; n<512; n=n+2) { buf2[n] = trasb[n+1]; buf2[n+1] = trasb[n] ; }    
				status = fwrite(buf2,1,512,fout);                               
			}                  
			else	status = fwrite(trasb,1,512,fout);
        
			copied = copied+status;
		}
		fclose(fout); 
		setCursor(QCursor(Qt::ArrowCursor))    ;
	}

}
	
	
	
	
	
	
	
	




















#include "gemd.moc"
