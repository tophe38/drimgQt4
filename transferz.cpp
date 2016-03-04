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

#include "transferz.h"
#include "iniz.h"




typedef unsigned char  byte;
typedef unsigned long ULONG;
typedef unsigned int  UINT;


typedef struct MCATREC {
	char mcat_ch ;
	unsigned int mcat_ofs ;
	char mcat_fds ;
	unsigned int mcat_tots ;
	char mcat_fill[6];
} MCATREC, *PMCATREC ;


byte CATbuf[131072] ; //for 256 sectors
unsigned char LoadBuf[52000] ;
unsigned char SaveBuf[52000] ;
unsigned char z80h[30] ;
unsigned char TAPh[24] ; //TAP header
unsigned char eibuf[512] ; 
char driv8b ; // Flag for read drive in 8-bit mode


unsigned int seldcha,  catlen, catpoi, zxfstart, zxflen, filepos, snastack, restrsp ;
int seldpos ;
int hdfhea = 0;
ULONG secoff ;

byte firsttap = 0 ; 
char incha[32];
int selfl[32];
int selcount=0;
int copift, copifz  ;


char zxpath[256];
int zxpal ;
int p,  fscanp ;
char curzx[128];

char lastf ;
char fullpn[180]; 
char fext[] = "scr" ;
int pfilelen, mcstep;

unsigned char zxhead[32] ;
unsigned int filecha, c2 ;
int seldpos2 ;
char zxname[15] ;
unsigned int zxass, zxsecc, poscmr, nextar, endpoint  ;
ULONG zxpos ;

byte tapflag = 0;
unsigned int cmrlen, ahead ;
int oddflag, drih ;

ULONG loadlen;
long possp ;

//DWORD dwRead ;

//HANDLE ftozx ;

unsigned int firstfs, totsea, fdata, aused, afree ;
//ULONG aused, afree ;

unsigned char hlnamc = 0 ; //for auto filename
char hlnt[] = "hl" ;
unsigned char renamc = 0 ;
char rent[] = "re" ;


char toclearf = 0;
int o = 0;
char renamfl = 0 ;
extern char geohit;
 
bool sbin ;

QString litet;

QStringList SeldFiles;

//-----------------

extern int selected;
extern int c,n,k;
extern char detDev[9][16] ;
extern  char physd[9];
extern char loadedF[256] ;
extern int form;
extern int secsize, clust ;
extern int status ;

extern char segflag ;
extern ULONG SecCnt, ChaSecCnt ;
extern ULONG OffFS;
extern bool ov2ro;

extern QString op, qhm;

extern char dstr[60];


extern UINT sectr ;
extern ULONG cylc ;
extern UINT heads ; 

extern FILE *finp, *fout, *ftout, *flout ;

extern unsigned char bufr[640];


/*
int fileclo(int fd)
{
	return	close(fd); 
}

*/


             // Print out used and free space on charea:
void transferZ::printuf() {                  
                 
                 
	strcpy (dstr, "Used: ") ;
                      
	firstfs = CATbuf[1]*256+CATbuf[0] ;
	totsea =  CATbuf[3]*256+CATbuf[2] ;
	fdata = CATbuf[4] ;
                      
	if (clust > 1) fdata = CATbuf[8] ;
                      
	aused = firstfs-fdata ;
	afree = totsea-firstfs ;
                      
	if (clust == 1) {
		aused = aused/2;
		afree = afree/2;
		if ( form == 2 ) { aused = aused/2 ;
			afree = afree/2 ; }
	}
                                            
	qhm.setNum(aused);
	//itoa ( aused, khm, 10 );
	strcat (dstr, qhm );
	qhm.setNum(afree);
	//itoa (afree, khm, 10);
	strcat (dstr, " KB, Free: ");
	strcat (dstr, qhm );
	strcat (dstr, " KB") ;
                      
	usfreLa->setText(dstr);
	//SetDlgItemText(hDW, UFinf, dstr); 

}




  // Z80 decompress
void transferZ::decomp() {
	possp = 0 ;
	poscmr = 30 ; //**
	int l, c ;
	while ( possp < 49152 ) {
		if ( LoadBuf[poscmr] == 0xED )
		{
			poscmr++ ;
			if ( LoadBuf[poscmr] == 0xED )
			{           // is second ED in row
				poscmr++;
				l = LoadBuf[poscmr++] ;
				for (c=0; c<l; c++) SaveBuf[possp++] = LoadBuf[poscmr] ;
				poscmr++ ;
			} 
			else { SaveBuf[possp++] = LoadBuf[poscmr-1];
				SaveBuf[possp++] = LoadBuf[poscmr++]; 
			}
             
             
		} else { SaveBuf[possp++] = LoadBuf[poscmr++]; } 
	}
       
}             




  // Here update of current (last) displayed CAT!
                
void transferZ::updatcc() {  
     
	int c;
	secoff = (256*bufr[seldpos*mcstep+2]+bufr[seldpos*mcstep+1])*sectr*heads ;
                           
	catlen = bufr[seldpos*mcstep+3]; 
            
            
                    //first clear list, then load whole CAT
                         
	//SendMessage(GetDlgItem(hDW, catLB), LB_RESETCONTENT, 0, 0);  //Empty list
           catLB->clear();
                   
                     
	if ( driv8b ) {
		lseek(drih, secoff*512, SEEK_SET );
                    
		for ( c2=0;c2<catlen;c2++) {
			read(drih, eibuf, 512);                      
			for (n = 0; n<256; n++) CATbuf[n+256*c2] = eibuf[n*2];
		}
	}
                          
	else { lseek(drih, secoff*secsize+hdfhea, SEEK_SET) ; 
		read(drih, CATbuf, secsize*catlen);  }
                   
                                 
                              //Now list files
            
		catpoi = secsize ;
                  
		while ( CATbuf[catpoi+1] )  {
            
			if ( catpoi > (secsize*catlen-1) ) break ;
                  
			for( c = 0; c < 60; c++ ) {
				dstr[c] = 32; } //Fill string with spaces  
            
				for (n=1; n<16; n++) {
					dstr[n-1] = CATbuf[catpoi+n] ;
				}
                     
				if ( CATbuf[catpoi] == 0) dstr[17] = 'B';
				else dstr[17] = 'C';
                    
				zxfstart = 256*CATbuf[catpoi+19]+CATbuf[catpoi+18] ;
				zxflen = 256*CATbuf[catpoi+17]+CATbuf[catpoi+16] ;
                    
				qhm.setNum(zxfstart);
				//itoa(zxfstart, khm, 10 );
				dstr[19] = 0;
				strcat(dstr,qhm) ;
                    
                    
				for (c = strlen(dstr); c<24; c++) dstr[c] = 32;
                    
                    
				dstr[24] = 44;
                    
				qhm.setNum(zxflen);
				//itoa(zxflen, khm, 10 );
				dstr[25] = 0;
				strcat(dstr,qhm) ;
                    
                    
				dstr[30] = 0;
                     
				catLB->insertItem(dstr,-1) ; 
				//SendMessage(GetDlgItem(hDW, catLB), LB_ADDSTRING, 0, (LPARAM) dstr);
				catpoi = catpoi+32;
           
		} 
                     
		printuf() ;           

}
















transferZ::transferZ(QWidget *parent, const char *name)
    :tranZDlg(parent, name)
{
	chaLB->clear();
	catLB->clear();
	selLB->clear();
	//toclearf = 1;
	
	// List mcat with some datas
	
	status = 0;
       
	
	if (selected<16) 
	{  for (k=0;k<9;k++) physd[k] = detDev[k][selected];
        	
	if (ov2ro)  if ( SecCnt>2097152 ) {
		 drih = open(physd, O_RDONLY );
	} 
	else
	drih = open(physd, O_RDWR );
	
	}
	
	  else drih = open(loadedF, O_RDWR  );
         
                            
	   if (drih <0) {   		    
		KMessageBox::error( this, "Drive/file open error ", "Problem" );
		
		goto tradlgex;
		
		//return ;  // Not good!!!
		 }
                 
	
	
	
		secsize = 512 ;
		mcstep = 12 ;
		hdfhea = 0;
                   
		driv8b = 0 ;
		if ( selected<10 & form == 2 ) driv8b = 1; // Flag
                   
		if ( ( selected == 99) && (form>0) ) {
			lseek(drih, 128,  SEEK_SET );
			hdfhea = 128;  }
                  
			if ( form == 2) { secsize = 256 ; mcstep = 6; } // for SIF (8-bit simple if files)
                                        
                  
			if ( driv8b ) { read(drih, eibuf, 512); 
				for (n = 0; n<256; n++) bufr[n] = eibuf[n*2];
			}
                                       
			else read(drih, bufr, 512);
                   
                  // Test is BigSectorish:
			clust = 1 ;
			if ( form == 2 ) { if (bufr[224] == 'B') if (bufr[225] == 'S')
				clust = 4 ;  }       
				else { if (bufr[448] == 'B') if (bufr[450] == 'S')
					clust = 2 ; }        
                
                
                
					//MCATREC pdg;
					int c, ca1;
                  
                
					for( n = 0; n < 36; n++ ) {
                  
						for( c = 0; c < 60; c++ ) {
							dstr[c] = 0; } //Clear string  
                    
							dstr[0] = bufr[n*mcstep];
							dstr[1] = 58; //space
							dstr[2] = 32; 
							dstr[3] = 32; 
							ca1 = 256*bufr[n*mcstep+5]+bufr[n*mcstep+4] ;  // 
                                                            
                                         
							if ( clust == 1 ) {  
								ca1 = ca1/2 ; // get KB
								if ( form == 2 ) ca1 = ca1/2 ;
							}
							qhm.setNum(ca1);
							
                
                
							strcat(dstr,qhm);
							strcat(dstr," KB");
                
                   // dstr[1] = 0;
                    
                    
							chaLB->insertItem(dstr,-1) ; 
           
           						           
           
					}        

	
	
	tradlgex: ;
}






void transferZ::exitBu_clicked()
{
	
	if (!drih <0) fileclo(drih);
	transferZ::close() ;
	
}


//void transferZ::chaLB_selectionChanged(QListBoxItem*)

void transferZ::chaLB_clicked(QListBoxItem*)
{
	k = chaLB->currentItem() ;
	
	if (k<0) goto chalbex;
	
	//char Buffer[32];
	litet = chaLB->text(k);
	
	//int index = SendMessage((HWND)lParam, LB_GETCARETINDEX, 0, 0);
	//SendMessage((HWND)lParam, LB_GETTEXT, (LPARAM)index, (WPARAM)Buffer);
                            
	strcpy(dstr,litet);
	
	seldcha = dstr[0] ;
                            
	seldpos = seldcha -'A' ; //for A-Z
	if (seldpos<0) seldpos = seldpos+17+26 ; //for 0-9
            
                            //get offset
	secoff = (256*bufr[seldpos*mcstep+2]+bufr[seldpos*mcstep+1])*sectr*heads ;
                           
	catlen = bufr[seldpos*mcstep+3]; 
	
	  //first clear list, then load whole CAT
                         
	catLB->clear();//Empty list
           
           
                 
                     //load CAT in CATbuf
                    
	if ( driv8b ) {
		lseek(drih, secoff*512, SEEK_SET );
                    
		for ( c2=0;c2<catlen;c2++) {
			read(drih, eibuf, 512);                      
			for (n = 0; n<256; n++) CATbuf[n+256*c2] = eibuf[n*2];
		}
	}
                     
	else { lseek(drih, secoff*secsize+hdfhea, SEEK_SET );
                     //fseek(finp,secoff*512+hdfhea,0) ;
		read(drih, CATbuf, secsize*catlen);
	}
	
	
	                     
            //Now list files
            
	catpoi = secsize ;
                  
	while ( CATbuf[catpoi+1] )  {
            
		if ( catpoi > (secsize*catlen-1) ) break ;
                  
		for( c = 0; c < 60; c++ ) {
			dstr[c] = 32; } //Fill string with spaces  
            
			for (n=1; n<16; n++) {
				dstr[n-1] = CATbuf[catpoi+n] ;
			}
                     
			if ( CATbuf[catpoi] == 0) dstr[17] = 'B';
			else dstr[17] = 'C';
                    
			zxfstart = 256*CATbuf[catpoi+19]+CATbuf[catpoi+18] ;
			zxflen = 256*CATbuf[catpoi+17]+CATbuf[catpoi+16] ;
                    
			qhm.setNum(zxfstart);
			//itoa(zxfstart, khm, 10 );
			dstr[19] = 0;
			strcat(dstr,qhm) ;
                    
                    
			for (c = strlen(dstr); c<24; c++) dstr[c] = 32;
                    
                    
			dstr[24] = 44;
                    
			qhm.setNum(zxflen);
			//itoa(zxflen, khm, 10 );
			dstr[25] = 0;
			strcat(dstr,qhm) ;
                    
                    
			dstr[30] = 0;
                     
			catLB->insertItem(dstr,-1) ; 
			//SendMessage(GetDlgItem(hDlgWnd, catLB), LB_ADDSTRING, 0, (LPARAM) dstr);
			catpoi = catpoi+32;
           
	}
	
	printuf() ;
                    
	
	
	
	
  chalbex: ;	
}


//void transferZ::catLB_pressed(QListBoxItem*,const QPoint& )

//void transferZ::catLB_selected(QListBoxItem*)

void transferZ::catLB_clicked(QListBoxItem* item)
{
	
	if (item) {
	
	if (toclearf) {
		selLB->clear();
		selcount = 0; 
		}
		toclearf = 0;
                   
                   
		if (selcount == 32) goto selfin ; // Max 32 selectable ????!!!! break?
                
				
		
		//k = catLB->clicked();
		k = catLB->currentItem() ;
		if (k<0) goto selfin ;
		
		selfl[selcount] = k ; // store position
		incha[selcount] = seldpos; // Store Charea where is file
		selcount++ ;
                   
                   
		litet = catLB->text(k);
                            
                strncpy(dstr,litet,15);                     
		
		//for (n=0; n<15; n++)  dstr[n] = Buffer[n] ;
		dstr[15] = 0;
                     
                   // itoa( selfl[selcount-1],dstr, 10) ;  
                     
		//SendMessage(GetDlgItem(hDlgWnd, selLB), LB_ADDSTRING, 0, (LPARAM) dstr); 
                  
		selLB->insertItem(dstr,-1) ; 
	
	selfin:  ;
	}
}

void transferZ::clearBu_clicked()
{
	selLB->clear();
	selcount = 0;
	
}



void transferZ::saveBu_clicked()
{
	
	 // Saving selections:
               
            
            // Get SaveBin checkbox state:
	sbin = 0;
	if( savbCB->isChecked() ) sbin=1;
	 
            
	firsttap = 0 ; //flag for first TAP entry
	copift = copifz =  0;
              
	for (n = 0; n<selcount; n++) {
                   
		secoff = (256*bufr[incha[n]*mcstep+2]+bufr[incha[n]*mcstep+1])*sectr*heads ;   
		catlen = bufr[incha[n]*mcstep+3]; 
                 
		if ( driv8b ) {
			lseek(drih, secoff*512, SEEK_SET );
                    
			for ( c2=0;c2<catlen;c2++) {
				read(drih, eibuf, 512);                      
				for (k = 0; k<256; k++) CATbuf[k+256*c2] = eibuf[k*2];
			}
		}
                
                
		else { lseek(drih, secoff*secsize+hdfhea, SEEK_SET );
			read(drih, CATbuf, secsize*catlen ); }
                
               
                 //Get CAT record of file:
                       
			catpoi = secsize+32*selfl[n];      
                
			catlen = clust*CATbuf[catpoi+24] ;  // Here used as file sector count 
			filepos = clust*(256*CATbuf[catpoi+23]+CATbuf[catpoi+22])+secoff ;
                 
			if ( driv8b ) {
				lseek(drih, filepos*512, SEEK_SET );
                    
				for ( c2=0;c2<catlen;c2++) {
					read(drih, eibuf, 512);                      
					for (k = 0; k<256; k++) LoadBuf[k+256*c2] = eibuf[k*2];
				}
			}
                                  
                 
			else { lseek(drih, filepos*secsize+hdfhea, SEEK_SET );
				read(drih, LoadBuf, secsize*catlen ); }
                
                
              
                // if bin:
				if (sbin) goto savebin ;     
                                                 
                //Start with snapshots to Z80:
              
				if ( CATbuf[catpoi+26] )  {  // Snapshot if SP high byte > 0
                 
                 // Get files length in sectors, position on disk, name
                 
                 
                 
					snastack = 256*CATbuf[catpoi+26]+CATbuf[catpoi+25]-16384 ;
                 
                 // Fill Z80 header:
                         
					z80h[0] = LoadBuf[0x15+snastack] ;       
					z80h[1] = LoadBuf[0x14+snastack] ;
					z80h[2] = LoadBuf[0xE +snastack] ;
					z80h[3] = LoadBuf[0xF +snastack] ;
					z80h[4] = LoadBuf[0xA +snastack] ;
					z80h[5] = LoadBuf[0xB +snastack] ;
					z80h[6] = LoadBuf[0x16+snastack] ;
					z80h[7] = LoadBuf[0x17+snastack] ;
                
					restrsp = snastack+0x4018 ;
					z80h[8] = restrsp ;
					z80h[9] = restrsp>>8 ;
                 
					z80h[10] = LoadBuf[0x13+snastack] ;
					z80h[11] = 55 ;  // R register - irrelevant here
					z80h[12] = 14 ;  // Border, fixed white here
					z80h[13] = LoadBuf[0xC +snastack] ;
					z80h[14] = LoadBuf[0xD +snastack] ;
					z80h[15] = LoadBuf[0x8 +snastack] ;
					z80h[16] = LoadBuf[0x9 +snastack] ;
					z80h[17] = LoadBuf[0x6 +snastack] ;
					z80h[18] = LoadBuf[0x7 +snastack] ;
					z80h[19] = LoadBuf[0x4 +snastack] ;
					z80h[20] = LoadBuf[0x5 +snastack] ;
                
					z80h[21] = LoadBuf[0x11+snastack] ;
					z80h[22] = LoadBuf[0x10+snastack] ;
					z80h[23] = LoadBuf[0 +snastack] ;
					z80h[24] = LoadBuf[0x1 +snastack] ;
					z80h[25] = LoadBuf[0x2 +snastack] ;
					z80h[26] = LoadBuf[0x3 +snastack] ;
                 
					if ( 4 & LoadBuf[0x12+snastack] ) z80h[27] = 255;
					else z80h[27] = 0;
					z80h[28] = LoadBuf[0x12 +snastack] ;
                 
					if ( LoadBuf[0x13 +snastack] == 0x3f ) z80h[29] = 1;
					else z80h[29] = 2 ;
                 
                 //Now save file
                 
     savebin:         
		     for (c=1; c<16; c++) 
		     dstr[c-1] = CATbuf[catpoi+c] ;
                  
                  // Maybe look to spaces from end....
                
     for (c=14; c>0; c--)                      
	     if ( !(dstr[c] == ' ') ) break  ; 
                    
     c++ ;
                
     dstr[c] = 0;
     if (sbin) strcat(dstr,".bin") ;
     else strcat(dstr,".z80") ;
                  
                  //Open save file:
                         
                          
     fout = fopen(dstr,"wb");
     if (fout == NULL) { 
             //  printf("File open error!\n"); 
	    // close(drih); // ???  
	     goto copy1end ;   //!!!!
	 //    return 0;
     }       
                
     if (sbin)   fwrite(LoadBuf,1, CATbuf[catpoi+16]+256*CATbuf[catpoi+17], fout);
     else {    
	     fwrite(z80h,1,30,fout); 
	     fwrite(LoadBuf,1,49152,fout);  
     }
     fclose(fout); 
     copifz++ ; 
                
	copy1end: ;			} 
              
				else   {    // Here deal with TAPes
                  
                 
					TAPh[0] = 0x13;
					TAPh[1] = 0;
					TAPh[2] = 0;
                //copy type and name
                
					for (c=0; c<11; c++) TAPh[c+3] = CATbuf[catpoi+c] ;
                
                
					for (c=1; c<16; c++) 
						dstr[c-1] = CATbuf[catpoi+c] ;
                  
                  // Maybe look to spaces from end....
                
					for (c=14; c>0; c--)                      
						if ( !(dstr[c] == ' ') ) break  ; 
                    
					c++ ;
                
					dstr[c] = 0;
					strcat(dstr,".tap") ;  // This is PC filename for TAP - only one at save selected
                 
					if ( firsttap == 0 ) {
                                         
						ftout = fopen(dstr,"wb");
						if (ftout == NULL) { 
                 //  printf("File open error!\n"); 
							//close(drih);   //??? 
							//return 0;
							goto copy2end ;
						}   
						firsttap = 1; }   
                  
                  //Now put length, begin and length without variables
                  
						for (c=16; c<22; c++) TAPh[c-2] = CATbuf[catpoi+c];
                 
                  //calculate header checksum:
                  
						byte zxchs = 0 ;            
						for (c=3; c<20; c++)  zxchs = zxchs^TAPh[c] ;
						TAPh[20] = zxchs ;
                 
						unsigned int helen;
                 
						helen = 256*CATbuf[catpoi+17]+CATbuf[catpoi+16]+2 ;            
						TAPh[21] = helen ;
						TAPh[22] = helen>>8;
						TAPh[23] = 255 ;
                 
						fwrite(TAPh,1,24,ftout);
                 
                 //checksum of data 
                 
						zxchs = 0xFF ;            
						for (c2=0; c2<helen-2; c2++)  zxchs = zxchs^LoadBuf[c2] ;
						LoadBuf[helen-2] = zxchs ;
                 
                 
						fwrite(LoadBuf,1,helen-1,ftout);
                                
				}
                                  
	}                                
               
	if ( firsttap ) { fclose(ftout) ; copift++ ; }
               
                  //Now should be on last charea....
                  
	
	copy2end: 
	
	
	secoff = (256*bufr[seldpos*mcstep+2]+bufr[seldpos*mcstep+1])*sectr*heads ;
                           
	catlen = bufr[seldpos*mcstep+3]; 
                        
                    //load CAT in CATbuf
                    
                   
	if ( driv8b ) {
		lseek(drih, secoff*512, SEEK_SET );
                    
		for ( c2=0;c2<catlen;c2++) {
			read(drih, eibuf, 512);                      
			for (k = 0; k<256; k++) CATbuf[k+256*c2] = eibuf[k*2];
		}
	}
                 
                                  
	else { lseek(drih, secoff*secsize+hdfhea, SEEK_SET) ; 
                   
		read(drih, CATbuf, secsize*catlen); } // Load back visible CAT
                  
                
                qhm.setNum(copift);
		//ltoa(copift, khm, 10) ;
		strcpy(dstr,qhm) ;
		strcat(dstr," TAP, ");
		qhm.setNum(copifz);
		//ltoa(copifz, khm, 10) ;
		strcat(dstr, qhm) ;
		strcat (dstr," Z80/bin file saved."); 
                
		trinfLa->setText(dstr);
		toclearf = 1;  // Sign to clear list when  selecting files
	
	
	
	
	
	
}









void transferZ::eraBu_clicked()
{
	
	copifz = 0; // counter 
                    
	for (n = 0; n<selcount; n++) {
                   
		secoff = (256*bufr[incha[n]*mcstep+2]+bufr[incha[n]*mcstep+1])*sectr*heads ;   
		catlen = bufr[incha[n]*mcstep+3]; 
                 
		if ( driv8b ) {
			lseek(drih, secoff*512, SEEK_SET );
                    
			for ( c2=0;c2<catlen;c2++) {
				read(drih, eibuf, 512);                      
				for (k = 0; k<256; k++) CATbuf[k+256*c2] = eibuf[k*2];
			}
		}
                
                
                
		else { lseek(drih, secoff*secsize+hdfhea,SEEK_SET );
			read(drih, CATbuf, secsize*catlen ); }
                 
			catpoi = secsize+32*selfl[n]; // position in CAT
                       
              
              //see is last file
                                       
			zxass =  secsize*CATbuf[4] ; 
			if ( catpoi >= zxass ) goto itslast ;
              
			if ( CATbuf[catpoi+33] ) {               
				for (c=0;c<15;c++) dstr[c] = CATbuf[catpoi+1+c] ;
				dstr[15] = 0;
				
				KMessageBox::error( this, "Not last file!", dstr );
				
				continue;   
			}
       itslast: 
                              
		       zxass = CATbuf[0]+256*CATbuf[1]-CATbuf[catpoi+24] ; // correct first free sector
              
       CATbuf[0] = zxass ;
       CATbuf[1] = zxass>>8 ;
               
       for (c = 0; c<32;c++) CATbuf[catpoi+c] = 0 ;  // erase cat record    
              
                      
                // Write to image/drive:
               
                //Ensure whole sector write on phys. drive:
                         
       zxass = catpoi/secsize ;
                
       if ( driv8b ) {
	       lseek(drih, secoff*512+zxass*512, SEEK_SET );                                        
	       for (k = 0; k<256; k++) eibuf[k*2] = CATbuf[k+zxass*secsize];
	       write(drih, eibuf, 512);                      
                     
	       lseek(drih, secoff*512, SEEK_SET );                                        
	       for (k = 0; k<256; k++) eibuf[k*2] = CATbuf[k];
	       write(drih, eibuf, 512);
                                    
       } 
                              
                              
       else { lseek(drih, secoff*secsize+hdfhea+zxass*secsize, SEEK_SET) ; 
	       write(drih, CATbuf+zxass*secsize, secsize); 
                 
	       lseek(drih, secoff*secsize+hdfhea, SEEK_SET) ; 
	       write(drih, CATbuf, secsize); }
              
	       copifz++ ;                  
               
                   
	} // erase selcount loop end   
                    
                  
	qhm.setNum(copifz);
	strcpy(dstr, qhm) ;
	strcat (dstr," files erased."); 
                
	trinfLa->setText(dstr);   
	selcount = 0;
	toclearf = 1;  
                   
	 
	updatcc() ;
	
	
}




void transferZ::gueBu_clicked()
{
	
	// Try to guess drive geometry:
             
	zxass = 32 ; // starting value of Head * Sec/tr
	geohit = 0;
             
               //driv8b = 1 ; // testing
               
             
	for (n = zxass; n< 4096; n++) {
               
                 //now see is A & B & C on place:
                             
		qhm.setNum(n);
		
		strcpy(dstr,qhm) ;
		strcat(dstr," is Sec/tr*Heads ");
                                  
		trinfLa->setText(dstr); // print what guessing...
             
              //get s. location of A charea CAT:
                    
		catpoi = bufr[1]+256*bufr[2] ;  
		secoff = secsize*catpoi*n+hdfhea ;
                  
		if ( driv8b ) { 
			lseek(drih, secoff*2, SEEK_SET );
			read(drih, CATbuf, 512);
             
			if ( CATbuf[10] != 'A' ) continue ;                                  
		}
                  
		else { lseek(drih, secoff, SEEK_SET );
			read(drih, CATbuf, secsize);
             
			if ( CATbuf[5] != 'A' ) continue ;  }
                  
			catpoi = bufr[1+mcstep]+256*bufr[2+mcstep] ;  
			secoff = secsize*catpoi*n+hdfhea ;
                  
			if ( driv8b ) { 
				lseek(drih, secoff*2, SEEK_SET );
				read(drih, CATbuf, 512);
             
				if ( CATbuf[10] != 'B' ) continue ;                                  
			}
                  
                  
			else { lseek(drih, secoff,  SEEK_SET );
				read( drih, CATbuf, secsize);
             
				if ( CATbuf[5] != 'B' ) continue ;  }
                  
				catpoi = bufr[1+2*mcstep]+256*bufr[2+2*mcstep] ;  
				secoff = secsize*catpoi*n+hdfhea ;
                  
				if ( driv8b ) { 
					lseek(drih, secoff*2,  SEEK_SET );
					read(drih, CATbuf, 512);
             
					if ( CATbuf[10] == 'C' ) { geohit =1 ; break ;  }                                
				}
                  
				else { lseek(drih, secoff,  SEEK_SET );
					read(drih, CATbuf, secsize);
             
					if ( CATbuf[5] == 'C' ) { geohit =1 ; 
                  
						break; } }
                                  
	}
                
                if ( geohit) {  // now get SecTr and Heads:
                     
			qhm.setNum(n);
			strcpy(dstr,qhm) ;
			strcat(dstr," guess... "); 
                     
			zxass = 17 ; // common Sec/tr value
                 
			c = n/zxass ;
			if ( ( c*zxass) == n ) {  sectr = zxass ; heads = c;                                               
			}
                   
			else {
				zxass = 32;      
				c = n/zxass ;
				if ( ( c*zxass) == n ) {  sectr = zxass ; heads = c; } 
			}
                     
			catLB->clear();
			//SendMessage(GetDlgItem(hDlgWnd, catLB), LB_RESETCONTENT, 0, 0);                  
			
			chaLB->clearSelection();
			//SendMessage(GetDlgItem(hDlgWnd, chaLB), LB_SETCURSEL, -1, 1) ;
                     
		}
                
		else  trinfLa->setText( "Not guessed." );  
	
	
	
	
	
	
	
}










void transferZ::addBu_clicked()
{
		
	litet = hlEd->text();
	strncpy(hlnt,litet,2);
	hlnt[2] = 0;
	
	litet = reEd->text();
	strncpy(rent,litet,2);
	rent[2] = 0;
	
        // Miltiple file open dialog:
	     
	
	//SeldFiles = KFileDialog::getOpenFileNames(0, i18n("*.tap|ZX TAP files\n*.z80|Z80 snapshot //files\n*.sna|SNA snapshot files\n*.scr|ZX screen files\n*.bin|Bin files\n*.*|All files"), this, //i18n("Files to add"));
	
	
	SeldFiles = KFileDialog::getOpenFileNames(0, i18n("*.tap *.z80 *.sna *.scr|ZX tap, z80, sna, scr files\n*.bin|Bin files\n*.*|All files"), this, i18n("Files to add"));
	
	
	
	
                 
	if(!SeldFiles.isEmpty())  {
                                              
		kapp->processEvents() ; // refresh dialog content   
                      
                                              
		selLB->clear();   // clear list
		selcount = 0; 
		toclearf = 1; // flag to clear when select zx-pc   
                   
                // get filenames from returned list:
                              
		
		QStringList::Iterator iq = SeldFiles.begin();
		for (; iq != SeldFiles.end(); ++iq)
		
		{
		
		strcpy(zxpath, *iq);	// Filename with path
		
		
		
		
		//strcpy(zxpath, szFileName) ;
		zxpal = strlen(zxpath) ;
                 
                
		//if ( szFileName[zxpal+1] ) lastf = 0;
		//else {lastf = 1;
			//strcpy(fullpn, zxpath);
                
                

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
                                
			
			trinfLa->setText(curzx);   
			              
		      // SetDlgItemText(hDlgWnd, TRinf, curzx);
                
			//kapp->processEvents() ;
			//sleep(1);
			
			
              
              //load file in LoadBuf:                         
                  
       	
			finp = fopen(zxpath,"rb");  
	  
	  
			if (finp == NULL) { 
				KMessageBox::error( this, "File open error ", "Damn!" );
				goto islastf2;
                 
			}
           
                       
                              //get filelength :
                              fseek(finp, 0, SEEK_END) ;
                              pfilelen = ftell(finp); //Filelength got
           
			      fseek(finp,0,SEEK_SET); // Back to filestart dude!
              
       c = strlen(curzx); 
       if (c>15) c=15 ;         
       for (n = 0; n<c-4 ;n++) zxname[n] = curzx[n] ;
       for (n = c-4; n<15 ; n++) zxname[n] = ' ' ;    // Assuming regular extension          
                        
              
              // branch by file extension:
              
       n = strlen(curzx) ;
       fext[0] = curzx[n-3] ;
       fext[1] = curzx[n-2] ;
       fext[2] = curzx[n-1] ;
              
       if (fext[2]>0x40) fext[2] = 0xDF & fext[2] ; // make all capital  
              
             
       switch (fext[2]) {
              
	       case 'T' :
		       goto bincas ; // TXT file for instance
             
		       case 'N' : // BIN
             
             
			       bincas:     
				       if ( (pfilelen > 49152)) {    
				       KMessageBox::error( this, "Too long file!", "Error" );
				       		
				       goto islastf ;  }  
             
             
              
				       status = fread( LoadBuf,1, pfilelen, finp);
				       //fclose(finp);
                
               //copy to SaveBuf 
               
				       for (n=0; n<pfilelen; n++) SaveBuf[n] = LoadBuf[n] ;
                             
               // write some parameters in zxheader:
                        
              
				       if (clust>1) {
					       poscmr = pfilelen/1024 ;
					       if ( poscmr*1024 < pfilelen) poscmr++ ; }        
					       else { poscmr = pfilelen/secsize ;
						       if ( poscmr*secsize < pfilelen) poscmr++ ; }
                
						       zxhead[16] = pfilelen;
						       zxhead[17] = pfilelen>>8 ; 
              
               //begin address always 25000
						       zxhead[18] = 0xA8 ;
						       zxhead[19] = 0x61 ;
						       zxhead[24] = poscmr ; 
               
						       for (n=25; n<32; n++) zxhead[n] = 0 ;
           
             
						       goto  comssz  ;
             
             
             
						       case 'R' : // SCR
                   
							       if ( !(pfilelen == 6912)) {    
								       							       
								       KMessageBox::error( this, "Incorrect SCR file!", "File format error" );
				       				      								       
								       goto islastf ;  }  
               
              
              
								       status = fread( LoadBuf, 1, 6912, finp);
								                      
               //copy to SaveBuf and add 256 zero to printer buffer
               
								       for (n=0; n<6912; n++) SaveBuf[n] = LoadBuf[n] ;
								       for (n=6912; n<6912+256; n++) SaveBuf[n] = 0 ;
               
               // write some parameters in zxheader:
                        
								       zxhead[16] = 0;
								       zxhead[17] = 0x1b ; 
								       zxhead[18] = 0 ;
								       zxhead[19] = 0x40 ;
               
								       if (clust>1) zxhead[24] = 7 ;
								       else { zxhead[24] = 14 ; // 14 sectors for 6912 bytes
									       if ( secsize == 256 ) zxhead[24] = 27 ; // for HDF256
								       }
               
								       for (n=25; n<32; n++) zxhead[n] = 0 ;
              
                   
             // break ;     
								       goto comssz ;
              
              
								       case 'A' : // SNA
              
									       if ( !(pfilelen == 49179)) {    
													
										KMessageBox::error( this, "Incorrect SNA file!", "File format error" );
																		
										goto islastf ;  }  
               
										status = fread( LoadBuf,1, 49179, finp);
										 
               
										for (n=0; n<49152; n++) SaveBuf[n] = LoadBuf[n+27] ;
              
              
              //transfer register contents to z80 stack:
                         
										possp = 256*LoadBuf[24]+LoadBuf[23]-16384 ;
              
										if (possp<24) { 
																												KMessageBox::error( this, "Incorrect SNA file!","Stack in ROM..." );
										goto islastf ;   }
          
          
										SaveBuf[possp-1] = LoadBuf[22] ; //A reg
										SaveBuf[possp-2] = LoadBuf[21] ; // F reg
										SaveBuf[possp-3] = LoadBuf[0] ;  // I reg
              
										if ( LoadBuf[19] ) SaveBuf[possp-4] = 255 ;
										else SaveBuf[possp-4] = 0 ; // ~IFF ???
              
										SaveBuf[possp-5] = LoadBuf[7] ;// A'
										SaveBuf[possp-6] = LoadBuf[8] ;// F'
										SaveBuf[possp-7] = LoadBuf[14];
										SaveBuf[possp-8] = LoadBuf[13] ; //BC reg 
										SaveBuf[possp-9] = LoadBuf[12] ;
										SaveBuf[possp-10] = LoadBuf[11] ; //DE
										SaveBuf[possp-11] = LoadBuf[10] ;
										SaveBuf[possp-12] = LoadBuf[9] ;  //HL
										SaveBuf[possp-13] = LoadBuf[6] ;
										SaveBuf[possp-14] = LoadBuf[5] ; //BC'
										SaveBuf[possp-15] = LoadBuf[4] ;
										SaveBuf[possp-16] = LoadBuf[3] ; //DE'
										SaveBuf[possp-17] = LoadBuf[2] ;
										SaveBuf[possp-18] = LoadBuf[1] ; //HL'
              
										SaveBuf[possp-19] = LoadBuf[18] ;
										SaveBuf[possp-20] = LoadBuf[17] ; // IX
										SaveBuf[possp-21] = LoadBuf[16] ;
										SaveBuf[possp-22] = LoadBuf[15] ; // IY
              
										SaveBuf[possp-23] = 0x3A ;
										SaveBuf[possp-24] = 0x4C ; //Restr address
              
              //SaveBuf[possp-25] = 0x4 ;
              //SaveBuf[possp-26] = 0xC3 ; // Calc - not needed 
              
										possp = possp+16384-22 ;
										zxhead[25] = possp ;
										zxhead[26] = possp>>8 ;
              
              
										goto commsnaz8 ;            
              
            
            
            
            
            
										case '0' : // Z80 
             
          
          
										if ( pfilelen > 49200) {    
										
										KMessageBox::error( this, "Too long Z80 file!", "Error" );
																	
										goto islastf ;  }  
               
										status = fread( LoadBuf,1, pfilelen, finp);
										             
               //check is old snapshot 
										if ( LoadBuf[6] != 0 ) goto z80145 ; 
          
             
              // Z80 ver 2.01 or 3
              
										if ( LoadBuf[30] == 23 ) { c = 30+25 ; goto arealoop ;}                  
            
										if (LoadBuf[30] == 54 ) { c = 30+56 ; goto arealoop ; }
                
										KMessageBox::error( this, "Incorrect/unknown Z80 file!", "Error" );
										
										goto islastf ;  
               
            
										arealoop:      cmrlen = 256*LoadBuf[c+1]+LoadBuf[c] ;
										c = c+2 ;
										if ( LoadBuf[c] == 8) goto ar4 ;
										if ( LoadBuf[c] == 4) goto ar8 ;  
										if ( LoadBuf[c] == 5) goto arc ;  
               
             //skip other areas
             
										c = c+1+cmrlen ;
										zendtest:     if ( c<pfilelen) goto arealoop;
             
										c=32; goto pushr ;
             
										ar4:    possp = 0 ; goto areac;       
										ar8:    possp = 0x4000; goto areac;
										arc:    possp = 0x8000; 
   
										areac:  c++ ;
										nextar = c+cmrlen ;
										endpoint = possp+0x4000 ;      
             
              
  
										int l, o ;
										while ( possp < endpoint ) {
										if ( LoadBuf[c] == 0xED ) {
										c++ ;
										if ( LoadBuf[c] == 0xED ) { // is second ED in row
										c++;
										l = LoadBuf[c++] ;
										for (o=0; o<l; o++) SaveBuf[possp++] = LoadBuf[c] ;
										c++ ;
										} 
										else { SaveBuf[possp++] = LoadBuf[c-1];
										SaveBuf[possp++] = LoadBuf[c++]; 
										}
										} else { SaveBuf[possp++] = LoadBuf[c++]; } 
          
          
										}
             
										if ( c != nextar) {                  
										                       //CloseHandle(ftozx);
										KMessageBox::error( this, "Incorrect Z80 file!", "File format error" );
										
										goto islastf ;   }
             
										goto zendtest ;
          
        
          
               //test is data compressed:
      z80145:
             
		      if ( (0x20 & LoadBuf[12]) )  decomp() ; // bit 5 test
         
               // uncomperessed Z80:               
      else { for (n=0; n<49152; n++) SaveBuf[n] = LoadBuf[n+30] ;}
           
                        
             // Deal with CPU registers:
                     
      c = 6 ; // PC offset by ver 1.45 
             
      pushr:      possp = 256*LoadBuf[9]+LoadBuf[8]-16384 ;
              
              // if it is lower than 26 it goes to ROM!
      if (possp < 26) {          
	                           
	      KMessageBox::error( this, "Incorrect Z80 file!", "Stack in ROM..." );
	      
	      goto islastf ;   }
              
          
	      SaveBuf[possp-1] = LoadBuf[1+c] ; 
	      SaveBuf[possp-2] = LoadBuf[c] ; // PC
                            
	      SaveBuf[possp-3] = LoadBuf[0] ;  // A reg
	      SaveBuf[possp-4] = LoadBuf[1] ;  // F
	      SaveBuf[possp-5] = LoadBuf[10] ; // I reg
              
	      if ( LoadBuf[27] ) SaveBuf[possp-6] = 255 ;
	      else SaveBuf[possp-6] = 0 ; // ~IFF ???
              
              
	      SaveBuf[possp-7] = LoadBuf[21];
	      SaveBuf[possp-8] = LoadBuf[22] ; //AF' reg 
	      SaveBuf[possp-9] = LoadBuf[3] ;
	      SaveBuf[possp-10] = LoadBuf[2] ; //BC
	      SaveBuf[possp-11] = LoadBuf[14] ;
	      SaveBuf[possp-12] = LoadBuf[13] ;  //DE
	      SaveBuf[possp-13] = LoadBuf[5] ;
	      SaveBuf[possp-14] = LoadBuf[4] ; //HL
	      SaveBuf[possp-15] = LoadBuf[16] ;
	      SaveBuf[possp-16] = LoadBuf[15] ; //BC'
	      SaveBuf[possp-17] = LoadBuf[18] ;
	      SaveBuf[possp-18] = LoadBuf[17] ; //DE'
              
	      SaveBuf[possp-19] = LoadBuf[20] ;
	      SaveBuf[possp-20] = LoadBuf[19] ; // HL'
	      SaveBuf[possp-21] = LoadBuf[26] ;
	      SaveBuf[possp-22] = LoadBuf[25] ; // IX
	      SaveBuf[possp-23] = LoadBuf[24] ;
	      SaveBuf[possp-24] = LoadBuf[23] ; // IY
             
	      SaveBuf[possp-25] = 0x3A ;
	      SaveBuf[possp-26] = 0x4C ; //Restr address
              
                           
	      possp = possp+16384-24 ;
	      zxhead[25] = possp ;
	      zxhead[26] = possp>>8 ;          
              
                        
                           
              
               // write SNA, Z80 parameters in zxheader:
                        
	      commsnaz8:   zxhead[16] = 0;
	      zxhead[17] = 0xC0 ; 
	      zxhead[18] = 0 ;
	      zxhead[19] = 0x40 ;
              
	      if (clust == 1) {
		      zxhead[24] = 96 ;
		      if ( secsize == 256 ) zxhead[24] = 192 ;
	      }
	      else zxhead[24] = 48 ;
	      for (n=27; n<32; n++) zxhead[n] = 0 ;
              
              
              
               // common for SCR SNA Z80:
                
  comssz:      
               
		  zxhead[0] = 3; //CODE 
              
  for (n = 1; n<16 ; n++) zxhead[n] = zxname[n-1] ;
              
  zxhead[21] = 0 ;
  zxhead[20] = 0 ;
                 
        
        
              // Now load zx CAT by begin letter:
                     
  filecha = curzx[0] ; //It is for SCR, SNA & Z80, not for TAP!
  tapflag = 0 ;
             
             
              // Tap entry point:***
             
  comforall:     // if (filecha > 0x40) filecha = 0x5F & filecha ; // make all capital and clear bit 7            
                 
		  seldpos2 = 35 ; // charea 9
  zxhead[1] = '9' ; // presets for case of invalid first char
                 
  if ( (filecha > 0x60) && (filecha < 0x7B) ) {
	  seldpos2 = filecha -'a' ; //for a-z
	  zxhead[1] = filecha; }
	  if ( (filecha > 0x40) && (filecha < 0x5B) ) {
		  seldpos2 = filecha -'A' ; //for A-Z
		  zxhead[1] = filecha; }
		  if ( (filecha < 0x3A) && (filecha > 0x2F) ) {
			  seldpos2 = filecha -'0' +26 ; // for 0-9
			  zxhead[1] = filecha; }
                     
                
                 
                 // replace codes above 127 with ? in whole zxhead name:
			  for (n=1;n<16;n++) if ( zxhead[n]>127) zxhead[n] = '?' ;         
			  for (n=1;n<16;n++) if ( zxhead[n]<32) zxhead[n] = '?' ;
    
                    //get offset
			  secoff = (256*bufr[seldpos2*mcstep+2]+bufr[seldpos2*mcstep+1])*sectr*heads ;
                   
			  catlen = bufr[seldpos2*mcstep+3]; 
     
                     //load CAT in CATbuf
                    
			  if ( driv8b ) {
				  lseek(drih, secoff*512, SEEK_SET );
                    
				  for ( c2=0;c2<catlen;c2++) {
					  read(drih, eibuf, 512);                      
					  for (n = 0; n<256; n++) CATbuf[n+256*c2] = eibuf[n*2];
				  }
			  }
                                  
			  else { lseek(drih, secoff*secsize+hdfhea,SEEK_SET );
				  read(drih, CATbuf, secsize*catlen ); }
                     
                                                         
                        
              // Check is enough place:
                       
              
				  zxass = CATbuf[0]+256*CATbuf[1] ;
				  if ( (zxass+zxhead[24])> (CATbuf[2]+256*CATbuf[3]) ) {
					  					  
					  KMessageBox::error( this, "No place in charea!", "Save error" );
					  
					  goto islastf ;  }  
              
              
              //seek first free cat slot & look is name exists!!!!
					  catpoi = secsize;
              
              
					  while ( catpoi < secsize*CATbuf[4] ) {
						  if ( CATbuf[catpoi+1] == 0 ) break ;
              // compare names:
						  p = 0;
						  for (c = 0; c<15; c++) {
							  if ( zxhead[c+1]== CATbuf[catpoi+c+1] ) p++ ; }
							  if ( p==15 ) {
                // name already exists case
                //try to give some other, but only once, only for TAP
                
								  if ( tapflag ) {
									  if ( renamfl ) {
                     
										renamc++ ;
										qhm.setNum(renamc);
										//itoa (renamc, khm, 10 );
										strcpy(dstr, rent) ;
										strcat(dstr, qhm) ;
                   
										if ( strlen(zxname) <15 ) {
										for (c= strlen(zxname);c<15;c++) zxname[c] = ' ' ;
										}
										o = 0;
                    //add zxname(as many fits) to it:
										for (c=strlen(dstr); c<15 ; c++) { dstr[c] = zxname[o];  
										o++; }
                        
										for (c = 0; c<15 ;c++) zxhead[c+1] = dstr[c] ;
              
										renamfl = 0;
										filecha = zxhead[1];
										goto comforall ; //to area CAT load...
                                   
              
									  }
								  }
								  for (c = 0; c<15; c++) dstr[c] = zxhead[c+1];
								  dstr[15] = 0;
								           	
								  trinfLa->setText(dstr);  
								  
								  KMessageBox::error( this, "Name already exists!", dstr );
								  								  
								  goto islastf ;  }    
              
              
								  catpoi = catpoi+32 ; }
              
								  if ( (catpoi+1) > (secsize*CATbuf[4]) ) {
									 
									  KMessageBox::error( this, "No place in CAT!", "Save error" );
									  
									  goto islastf ;  }  
             
              // name is in header
              
              // put ss in header
              
              
									  zxhead[22] = zxass ;
									  zxhead[23] = zxass>>8 ;
             
									  for (c=0; c<32; c++) CATbuf[catpoi+c] = zxhead[c] ;
             
              //write file to disk/image:                      
             
									  zxpos = zxass*clust ;
									  zxsecc = clust*zxhead[24] ;
             
									  if ( driv8b ) {
										lseek(drih, secoff*512+zxpos*512, SEEK_SET );
                    
										for ( c=0;c<zxsecc;c++) {
										for (n = 0; n<256; n++) eibuf[n*2] = SaveBuf[n+256*c];
										write(drih, eibuf, 512);                      
										}
									  } 
             
                          
									  else { lseek(drih, secoff*secsize+hdfhea+zxpos*secsize, SEEK_SET) ; 
										write(drih, SaveBuf,secsize*zxsecc);  }
              
                            
										zxass = zxass+zxhead[24] ;
              
										CATbuf[0] = zxass ;
										CATbuf[1] = zxass>>8 ;
              
                      
                // Write to image/drive:
               
                //Ensure whole sector write on phys. drive:
                         
										zxass = catpoi/secsize ;
               
										if ( driv8b ) {
										lseek(drih, secoff*512+zxass*512, SEEK_SET );                                        
										for (n = 0; n<256; n++) eibuf[n*2] = CATbuf[n+zxass*secsize];
										write(drih, eibuf, 512);                      
                     
										lseek(drih, secoff*512, SEEK_SET );                                        
										for (n = 0; n<256; n++) eibuf[n*2] = CATbuf[n];
										write(drih, eibuf, 512);
                                    
										} 
               
                                            
               
										else { lseek(drih, secoff*secsize+hdfhea+zxass*secsize, SEEK_SET) ; 
										write(drih, CATbuf+zxass*secsize, secsize); 
                 
										lseek(drih, secoff*secsize+hdfhea, SEEK_SET) ; 
										write(drih, CATbuf, secsize);  }
              
                // put in list what is copied:
										for (c = 0; c<15; c++) dstr[c] = zxhead[c+1];
										dstr[15] = 0;
										                
										selLB->insertItem(dstr,-1) ; 
                        
              
										if (tapflag == 1) goto tapcont ;
             
             
										break ; // common end
         
         
         
										case 'P' :  // TAP file
            
          
										possp = 0;
                          
   taploop: 
		   status = fread(z80h, 1, 2, finp);
             
   cmrlen = 256*z80h[1]+z80h[0] ;
             
   if ( cmrlen == 0 ) {               
	   
	   KMessageBox::error( this, "Faulty TAP file", "Problem" );
	  
	   goto islastf;  // this is because of faulty TAP files - longer than should be
   }
                    
                    
   if ( !(cmrlen == 19) ) goto headerless ;
                                                   
   status = fread( LoadBuf, 1, 19, finp);
   possp = possp+21 ; // count loaded bytes
             
   if ( LoadBuf[0] ) goto hlespec ; // if flag is not ZERO
             
             
   for (c=0; c<11; c++) zxhead[c] = LoadBuf[c+1] ; // skip header flag (0) and count
   for (c=11; c<16; c++) zxhead[c] = ' ' ; //fill 5 extra with spaces
   for (c=11; c<17; c++) zxhead[c+5] = LoadBuf[c+1] ;
            
             
             // now write len in sectors in header:
             
   nextar = 256*zxhead[17]+zxhead[16] ; // Filelength in bytes
   status = fread( z80h, 1, 2, finp);          
   cmrlen = z80h[1]*256 + z80h[0] ;
             
             
    commont:   
                
            
		    if ( cmrlen > 49154) goto invalidt ;
                                   
    status = fread( LoadBuf, 1, cmrlen, finp);
    possp = possp+2+cmrlen ; 
             
    datacop:     for (c=0; c<cmrlen-2; c++) SaveBuf[c] = LoadBuf[c+1] ;
    if ( zxhead[0] == 0 ) SaveBuf[cmrlen-2] = 128 ; // Basic end marker
           
               
    for (c=22; c<32; c++) zxhead[c] = 0 ; // clear zxhead end                       
               
    if (clust>1) {
	    c = nextar/1024 ;
	    if ( (c*1024)<nextar) c++ ; }       
	    else { c = nextar/secsize ;
		    if ( (c*secsize)<nextar) c++ ; }               
               
		    zxhead[24] = c; // sector count 
                
                    // Print out zx filenames in selected window    :
                    //todo
                 
                 
		    tappf:         tapflag =1 ;             
		    filecha = zxhead[1] ; // Begin letter
		    renamfl = 1 ; // sign that first entry in check name equality.
             
		    goto comforall ;
         
     tapcont:      
             
		     if ( possp > (pfilelen-1) ) 
		        goto islastf ;
     goto taploop ;
         
         
        
                
          
          
     invalidt:    
     
     KMessageBox::error( this, "Invalid tape file!", "Save error" );
     
     goto islastf ;   
       
            
      headerless:    
		      if (cmrlen > 49154) goto invalidt ;
                   
                   //build zx header
                   
      zxhead[0] = 3 ; //CODE
      hlnamc++ ;
      qhm.setNum(hlnamc);
      //itoa (hlnamc, khm, 10 );
      strcpy(dstr, hlnt) ;
      strcat(dstr, qhm) ;
                   
      if ( strlen(zxname) <15 ) {
	      for (c= strlen(zxname);c<15;c++) zxname[c] = ' ' ;
      }
      o = 0;
                    //add zxname(as many fits) to it:
      for (c=strlen(dstr); c<15 ; c++) { dstr[c] = zxname[o];  
	      o++; }
                        
	      for (c = 0; c<15 ;c++) zxhead[c+1] = dstr[c] ;
                    
                   // for (c = 0; c<strlen(dstr) ;c++) zxhead[c+1] = dstr[c] ;
                    //for (c = strlen(dstr)+1 ; c<16; c++) zxhead[c]=' ' ;
                    
	      nextar = cmrlen-2 ;
	      zxhead[16] = nextar;
	      zxhead[17] = nextar>>8 ; // length
	      zxhead[18] = 0;
	      zxhead[19] = 0x40; // start fixed to screen
                    
	      goto commont ;
                
         
        
      hlespec: 
                // data is elready loaded
                
                
		      zxhead[0] = 3 ; //CODE
      hlnamc++ ;
      qhm.setNum(hlnamc);
      //itoa (hlnamc, khm, 10 );
      strcpy(dstr, hlnt) ;
      strcat(dstr, qhm) ;
                    
      if ( strlen(zxname) <15 ) {
	      for (c= strlen(zxname);c<15;c++) zxname[c] = ' ' ;
      }
      o = 0;
                    //add zxname(as many fits) to it:
      for (c=strlen(dstr); c<15 ; c++) { dstr[c] = zxname[o];  
	      o++; }
                        
	      for (c = 0; c<15 ;c++) zxhead[c+1] = dstr[c] ;
                    //for (c = 0; c<strlen(dstr) ;c++) zxhead[c+1] = dstr[c] ;
                    //for (c = strlen(dstr)+1 ; c<16; c++) zxhead[c]=' ' ;
                    
	      nextar = 17 ;
	      zxhead[16] = nextar;
	      zxhead[17] = nextar>>8 ; // length
	      zxhead[18] = 0;
	      zxhead[19] = 0x80; // start fixed to half mem.
                    
	      goto datacop ;
       
              
          
	      break ;
              
                         
              
       } // fext sw. end
                
                
       islastf:   fclose(finp); // close input file
		       
	islastf2:	       kapp->processEvents() ; // For seeing what is copied instantly
                  
		 
			} // iq loop end
               
                
                  
			
			updatcc() ;           
                     
		
		
		
	} 
	
     
	
	
}






void transferZ::charBu_clicked()
{
	        
	OffFS = sectr*heads*(256*bufr[mcstep*seldpos+2]+bufr[mcstep*seldpos+1]) ;
	ChaSecCnt = clust*(256*bufr[mcstep*seldpos+5]+bufr[mcstep*seldpos+4]) ;
	segflag = 1 ;
	
}
