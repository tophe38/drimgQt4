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


#include <stdlib.h>
#include <qfileinfo.h>
#include <qlabel.h>


#include <qlineedit.h>
#include <qprogressbar.h>
#include <qcursor.h>
 



#include "drimgwidget.h"
#include "about.h"
#include "transferz.h"
#include "iniz.h"
#include "gemd.h"



 
 
 

typedef unsigned char  byte;

typedef unsigned long  ULONG;
typedef unsigned int  UINT;

unsigned char drv;  //selected drv
 
   char act = 0;
   char abortf = 0;
   
   char khm[14]; //for string operations

   char dstr[60]; ////for string operations
   //
   int n, c, i, k, s;  //loop counter, general
   int  clust, secsize ;
   byte datars = 1 ;
   byte oversiz = 0 ; // Oversize flag for INIT
 
   ULONG f, g, m, u ; // general var
 
   long fr ;

   ULONG newsec;
   ULONG SecCnt = 1;
   ULONG ChaSecCnt ;
   ULONG fsecc, rest;
    
   ULONG OffFS = 0 ;
   char segflag = 0;
   
   UINT step, cyld ;
   UINT sectr = 32;
   ULONG cylc = 1 ;
   UINT heads = 8;  //CHS parameters
   int  status=0;
   bool ov2ro = 1;
   char geohit = 0;
   char loadedF[256] ;
   int selected = 111; //set to non existing drive for begin
   int form = 0; //Format 0-raw, 1-hdf, 2-hdf256
   int Filesys = 0; // 0-unknown, 1-DOS, 2-PlusIDE, 3-PlusIDE 256
    // 4-7 Charea ;  11-GemDos ; 13-FAT16 partition itself 
   char Swf=0 ; // Sub filesystem 1-Swapped High/Low
   
   int Fsub = 0;  // Swap request flag - by read or write drive
   
     
   
   QString fileName;
	
   QString op, qhm;
   FILE *finp, *fout, *ftout, *flout ;
   ULONG filelen;
   
    //Ramsoft hdf header template:
   char rsh[128] = {0x52,0x53,0x2d,0x49,0x44,0x45,0x1a,0x10,0,0x80,0,0,0,0,0,0,0,0,0,0,0,0,
	   0x8a,0x84,0xd4,3, //last two is cyl count 
	   0,0,8,0,0,0,
	   0,2,32,0,3,0,0, 0xd4,0,0, 
	   32,32,32,32,0x64,0x72,0x69,0x6d,32,32,0x6d,0x61,0x64,0x65,32,32,
	   32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	   32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	   32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
	   32,32,32,32,32,32,32,32,32,32,
	   1,0,0,0,0,3,0,0,0,2,0,0} ; 
   
   unsigned char  bufr[640];
   unsigned char  buf2[524];
   unsigned char  bufsw[524] ;
   
   
   char devStr[9] = "/dev/sda" ;
   int detCount;
   char detDev[9][16] ;
   char physd[9];
   
   ULONG exdl[16]  ; //Drive seccnt list, empty at start 
   
   
  
   
   


//Alternative size detection:

  
   void detSD()
{
	int drih;
	unsigned long long drisize;
	
	drih = open(devStr,  O_RDONLY | O_NONBLOCK) ; 

	if (drih>0) {

	//drisize = lseek(drih,0,SEEK_END) ;
			
	drisize = lseek64( drih, 0,  SEEK_END );
	
	close(drih) ;
	
	// TODO CD, DVD detection!!!
	
	if (drisize) {
		
		finp = fopen(devStr,"wb"); // CD ROMs will not open
		if (finp != NULL)  		
			{
			
			exdl[detCount] = drisize/512; // Sector count
			c=1;
			qhm.setNum((double)drisize/1048576);
			strcat(dstr, qhm);
								
			strcat(dstr," MB"); 
			fclose(finp);
			}
		}
	
	
	}
	
}
   
      

   
void drimgWidget::detSDloop()
{
	   
	listBox1->clear(); 
	detCount = 0;
	
	for (n=0;n<16;n++) 
	{ 
		
		if (n<8) { devStr[5] = 's' ;
			devStr[7] = 'a'+n;
		}
		
		else { devStr[5] = 'h' ;
			devStr[7] = 'a'+n-8;
		}
		   
		   //printf("\n",devStr);
		   
		strcpy(dstr,devStr);
		strcat(dstr,"  ");
		
		c=0; // hit flag
		
		detSD() ;
		
		
		if (c) {
		listBox1->insertItem(dstr,-1) ;
		for (k=0;k<9;k++) detDev[k][detCount]=devStr[k]; // Store dev path
		detCount++;
		}	
	}
	
}
   
   
   
   
   
   
   

    drimgWidget::drimgWidget(QWidget* parent, const char* name, Qt::WindowFlags fl)
       : drimgWidgetBase(parent,name,fl)
     // drimgWidget::drimgWidget(QWidget* parent, const char* name)
	//: drimgWidgetBase(parent,name)
			
{
		
	detSDloop() ; // Detect SCSI (USB) drives attached

	
}












drimgWidget::~drimgWidget()
{}



// Functions:


void drimgWidget::getForm()
{
	if( rawRB->isChecked() ) form=0;
	if( hdfRB->isChecked() ) form=1;
	if( h256RB->isChecked() ) form=2;
	
		
}


void drimgWidget::getCHS()
{
	qhm = edSecTr->text() ;
	sectr = atoi(qhm);
	
	qhm = edHeads->text() ;
	heads = atoi(qhm);
                                
	qhm = edCyl->text() ;
	cylc = atol(qhm);
}



/*$SPECIALIZATION$*/


/*
void drimgWidget::button_clicked()
{
	
}
*/


void drimgWidget::quitButton_clicked()
{
	drimgWidget::close() ;
}




void drimgWidget::FileTrButton_clicked()
{
	
	//Filesys = 1 ; //Testing!!!!
	
	
	if (!act)  {
	
	getForm();
	getCHS();	
		
	segflag = 0;	
	
	if (Filesys == 11 | Filesys == 13 | Filesys == 1) {	
	
		gemd().exec() ;
	
	}
	
	else {	
	transferZ().exec() ;  //ZX Charea dlg call
	
	if (geohit) {       
	
		qhm.setNum(sectr);
		edSecTr->setText(qhm);
			
		qhm.setNum(heads);
		edHeads->setText( qhm );	
         	
		cylc = SecCnt/(sectr*heads) ;               
		qhm.setNum(cylc);
		edCyl->setText( qhm ); 
	 
		}
	}
			
	
	
	
	if (segflag)
	{ inf3Label->setText(" "); // Clear selected sec count message	
	
	qhm.setNum(OffFS);
	secofEdit->setText(qhm);
	                
	qhm.setNum(ChaSecCnt);
	seccnEdit->setText(qhm);    
	}
	// correct CHS for case of successfull geo guess:
                                 
       
	}
	
	
}



void drimgWidget::openIfButton_clicked()
{
	if (!act) {	
	
	getForm();	
		
	if (form==0) {
	fileName = KFileDialog::getOpenFileName(0, i18n("*.raw *.img|RAW & IMG image files\n*.*|All files"), this, i18n("File to load"));
	}
	else fileName = KFileDialog::getOpenFileName(0, i18n("*.hdf|Hdf image files\n*.*|All files"), this, i18n("File to load"));
	
		
	
	if( !fileName.isEmpty() )
	{
				
		
	  fout = fopen(fileName,"rb");  
	  
	  
	  if (fout == NULL) { 
		  KMessageBox::error( this, "File open error ", "Damn!" );
		  return;
                 
	  }
                  
                 //Load 512+128 bytes
                 
                 //status=
	  fread(bufr,1,640,fout);
                
                 // get filelen
                      
	  fseek(fout, 0, SEEK_END) ;
                       
	  
                
	  filelen = ftell(fout); //Filelength got
                               
	  fclose(fout);
                
	  strcpy(loadedF, fileName);
	  inf1Label->setText( "File selected" );
	 
	 
	  
	  selected = 99; //flag
                  
                // Get here CHS from hdf header if...
                   
	  int ofset = 0;
                       
	  if (form > 0) ofset = 128;
                   
                   
                
	  if (form>0) {   
		  heads = bufr[28]; 
		  sectr = bufr[34];                               
		  cylc = 256*bufr[25]+bufr[24] ;
		  SecCnt = (filelen-ofset)/512 ;
		  if ( bufr[8] == 1 ) SecCnt = (filelen-ofset)/256 ;
	  }
	  else { heads = 8; 
		  sectr = 32; 
		  SecCnt = filelen/512 ;
		  cylc = SecCnt/(sectr*heads) ;
                  
                  
	  }
             
	  
	  qhm.setNum(sectr);	  
	  edSecTr->setText(qhm);
	  	 
	  qhm.setNum(heads);
	  edHeads->setText( qhm );
	  
	  qhm.setNum(cylc);
	  edCyl->setText( qhm );
	
	  
	  qhm.setNum(SecCnt);
	  seccnEdit->setText( qhm );
	                         
	  inf5Label->setText( " ");  // Clear                   
                                 
              
	  OffFS = 0 ;
	  secofEdit->setText( "0" );
	  
	 	  
	  Filesys = 0;
	  Swf = 0 ;   
	  
	  
	  char PLUSIIS[]= "PLUSIDEDOS      ";
	  char OSID[16];
	 	  
	  int matc = 0;
                       
                       
                         //Get first 16 char:
                                          
	  for( n = 0 ; n < 16; n++ ) OSID[n]=bufr[n+ofset];
	  for( n = 0; n < 16; n++ ) {if (OSID[n]==PLUSIIS[n]) matc++ ;}
                      
	  if (matc == 16) {              
                          
                      //Check is 8-bit IDEDOS:                                      
                        
		  if ((form>0) && ( bufr[8] == 1 )){                       
                        
			  inf2Label->setText( "PLUSIDEDOS 256 byte sect");
			  goto sethdf256 ; }
                      
                          
                      
                      
			  else { inf2Label->setText( PLUSIIS); 
				  goto sethdf512 ; }
                                           
	  }                      
   
   
   
   
                          // Check for GemDos:
                                   
	  if (bufr[0x1c7] == 'G')
		  if (bufr[0x1c8] == 'E') 	 	  
		     if (bufr[0x1c9] == 'M') {   
		  inf2Label->setText("GemDos"); 
		  Filesys = 11 ; }
   
		  if (bufr[0x1c7] == 'X')
			  if (bufr[0x1c8] == 'G')       
				  if (bufr[0x1c9] == 'M') {   
			  inf2Label->setText("GemDos"); 
			  Filesys = 11 ; }   
   
			  
			  if (bufr[0x1c7] == 'B')
				  if (bufr[0x1c8] == 'G')       
					  if (bufr[0x1c9] == 'M') {   
				  inf2Label->setText("GemDos");
				  Filesys = 11 ; }
   
			  
	
                              // Check for GemDos by swapped H/L :
                                       
                                       
				  for (n=0;n<512;n=n+2) { bufsw[n] = bufr[n+1];  bufsw[n+1] = bufr[n]; }          
   
				  if (bufsw[0x1c7] == 'G')
					  if (bufsw[0x1c8] == 'E')       
						  if (bufsw[0x1c9] == 'M') {   
					  inf2Label->setText("GemDos SW");  
					  Filesys = 11 ; Swf = 1; }
   
					  if (bufsw[0x1c7] == 'X')
						  if (bufsw[0x1c8] == 'G')       
							  if (bufsw[0x1c9] == 'M') {   
						  inf2Label->setText("GemDos SW"); 
						  Filesys = 11 ; Swf = 1;}   
   
   
   
						  if (bufsw[0x1c7] == 'B')
							  if (bufsw[0x1c8] == 'G')       
								  if (bufsw[0x1c9] == 'M') {   
							  inf2Label->setText("GemDos SW");
							  Filesys = 11 ; Swf = 1;}   				  
				
			      
                            // See is FAT16 partition:
                                   
			  if (bufr[21] == 0xF8) {                              
				  inf2Label->setText("FAT16 part.");            
				  Filesys = 13 ;  goto removh ; }
			  
			  
                        
  
                        //See is DOS mbr:
                              
			  if (bufr[510+ofset] == 0x55) {
				  if (bufr[511+ofset] == 0xAA) {
					  inf2Label->setText( "DOS mbr"); 
					  Filesys = 1 ;           
					  goto removh ; }     
			  }
	 	  
   
   
                          
                      // Check for PP Charea doS

			  if ( bufr[0+ofset] == 0x41 )   //A
				  if ( bufr[12+ofset] == 0x42 )  //B
					  if ( bufr[24+ofset] == 0x43 )  {  //C
				  strcpy(dstr,"Charea DoS 16-bit");
                               
				  if ( bufr[ofset+448] == 'B' ) if ( bufr[ofset+450] == 'S' )
						  strcat(dstr," BigSect.");
                                      
				  inf2Label->setText( dstr);
				  goto sethdf512 ; }
   
   
   
				  if ( bufr[0+ofset] == 0x41 )   //A
					  if ( bufr[6+ofset] == 0x42 )  //B
						  if ( bufr[12+ofset] == 0x43 ) { //C
                               
					  strcpy(dstr,"Charea DoS 8-bit");
                               
					  if ( bufr[ofset+224] == 'B' ) if ( bufr[ofset+225] == 'S' )
							  strcat(dstr," BigSect.");
                               
					  inf2Label->setText( dstr);
					  goto sethdf256 ; }
                 
                 
					  if ( form >0 ) {                 
						  if ( bufr[8] == 1 ) goto sethdf256 ;
						  else goto sethdf512 ;             
					  }     
                 
					  goto removh ;
					  
					  sethdf256:         h256RB->setChecked(1);
					  		hdfRB->setChecked(0);
							   
					  goto removh ;
                 
					  sethdf512:   if ( form ) {  h256RB->setChecked(0);
						  hdfRB->setChecked(1);  }
          
		
					removh:   
							
					listBox1->clearSelection();			
					//  SendMessage(GetDlgItem(hDlgWnd, ListBox), LB_SETCURSEL, -1, 1) ; // remove highlight of drive selection
	
					qhm.setNum(SecCnt);
					
					strcpy(dstr,"Sector count: ") ;

					strcat(dstr,qhm);
		
					inf3Label->setText(dstr);
	
					
				//Print out loaded filename:
					inf6Label->setText(fileName );
				
                    
	}
	  
	 

	
	
	}
	
}


void drimgWidget::readButton_clicked()
{
	
	//selected = 2; // testing!!!
	
	
	if (!act) {
		act = 1;	
	
                   //Set CHS according to Edit boxes first:     	
	getCHS();
              
                 //Get OffSet and Sector count:
	qhm = secofEdit->text() ; 
	OffFS = atol(qhm);
	
	qhm = seccnEdit->text() ; 
	SecCnt = atol(qhm);
               
                   
	qhm.setNum(SecCnt); 
	strcpy(dstr,"Sector count: ") ;                
	strcat(dstr,qhm);                           
					
	inf3Label->setText(dstr );
	
	getForm(); //Set format flag according to RB-s
	
	if ( selected == 99 ) { //Special case - hdf to raw or vice versa
                 
                 
		
		if ( (form==0) & (Fsub) ) // Swap L/H and save in new file
		{
      
			fileName = KFileDialog::getSaveFileName(0, i18n("*.raw|Raw image files\n*.img|Img image files\n*.*|All files"), this, i18n("File to save"));
			
				
			if( !fileName.isEmpty() )  {
	
				kapp->processEvents() ;
                
                                
				int  status=0;
				ULONG copied = 0;
               
              // ULONG cophh = 0; 
                      
              
				finp = fopen(loadedF,"rb");
				if (finp == NULL) {                
					KMessageBox::error( this, "File open error ", "Damn!" );
					act=0; return;
				}
  
               
				fout = fopen(fileName,"wb");
				if (fout == NULL) { 
                 //  printf("File open error!\n"); 
					fclose(finp);   act=0;  
					return;
				}
  
   //  SecCnt = exdl[selected];
    
            //message at bottom
				curopLabel->setText("Swapping L-H..."); 
				                
				fseek(finp, 0, SEEK_END) ;
                       
				ULONG filelen;
                
				filelen = ftell(finp); //Filelength got   
                
				fseek(finp, 0, SEEK_SET) ;
				SecCnt = filelen/512 ;
                
                  //ULONG m;                 
				int prog = 0;
				int prnxt = 1;
     
				progressBar1->setProgress( 0 );
				setCursor(QCursor(Qt::WaitCursor));
				
				for( m = 0; m < SecCnt; m++ ) { 
        
						status=fread(bufr,1,512,finp);            
      
						for (n = 0; n<512; n=n+2) { buf2[n] = bufr[n+1]; buf2[n+1] = bufr[n] ; }            
         
						status = fwrite(buf2,1,512,fout);
						kapp->processEvents() ;
						copied = copied+status;
      
						if (abortf) { abortf=0 ; goto closefilesSW; }
						
						prog = 100*m/SecCnt;
						if (prog>prnxt) {    
							progressBar1->setProgress( prog+1 );    
							prnxt = prnxt+1 ;
						} 
      
				}
				
		closefilesSW:			fclose(finp);
				fclose(fout);
				setCursor(QCursor(Qt::ArrowCursor));
       //Print out info about data transfer:
                 
				for( n = 0; n < 60; n++ ) dstr[n] = 0; //Clear string     
        
				qhm.setNum(m); 
					
				strcat(dstr,qhm);
				strcat(dstr," sectors swapped L-H into file of ");
				qhm.setNum(copied);
				strcat(dstr, qhm);
				strcat(dstr," bytes.");
					
				curopLabel->setText(dstr);
				        
			}     
      
      
      
		}
               
		else 
		{
		
		
               
               //Fileselector call
		
               
	if (form > 0)  // contra selection here - to make conversion
			fileName = KFileDialog::getSaveFileName(0, i18n("*.raw|Raw image files\n*.img|Img image files\n*.*|All files"), this, i18n("File to save"));
			
			else 	fileName = KFileDialog::getSaveFileName(0, i18n("*.hdf|Hdf image files\n*.*|All files"), this, i18n("File to save"));
               		
				
			if( !fileName.isEmpty() )  {
	
				kapp->processEvents() ;
					int  status=0;
					ULONG copied = 0;
               
					ULONG cophh = 0; 
                      
              
					finp = fopen(loadedF,"rb");
					
					 
					if (finp == NULL) { 
						KMessageBox::error( this, "File open error ", "Damn!" );
						act=0; return;
                 
					}
  
               
					fout = fopen(fileName,"wb");
					if (fout == NULL) { 
                 //  printf("File open error!\n"); 
						fclose(finp);   act=0;  
						return;
					}
  
  
    					
            //Message at bottom
					curopLabel->setText("File conversion..."); 
					//SetDlgItemText(hDlgWnd, StaRWinf, "File conversion... Right click to abort"); 
      
         
					if (form == 0) {  //if hdf wanted 
      
       //Insert CHS in hdf header:
             
						rsh[28] = heads; 
						rsh[24] = cylc;  
						rsh[25] = cylc>>8;  
						rsh[34] = sectr ;
       
      // if (form == 2) { //Hdf 256
       //   rsh[8] = 1; }
       // else   // no hdf 256 support here
						rsh[8] = 0;
        
                   
						for( n = 0; n < 128; n++ ) {          
							fputc(rsh[n],fout);
						}
						cophh = 128;
 
					}
					else fseek(finp,128,0) ; // just skip hdf header
         
       //ULONG m;                 
					int prog = 0;
					int prnxt = 1;
     
					progressBar1->setProgress( 0 );
					//SendMessage(hPb, PBM_SETPOS, 0, 0);if (!act) {
					
      
					for( m = 0; m < SecCnt; m++ ) {
                 
								
						setCursor(QCursor(Qt::WaitCursor));				
						//SetCursor( jc) ;                    
        
							status=fread(bufr,1,512,finp);
            
       // if (form == 2) {
         //    for (n = 0; n<256; n++) buf2[n] = bufr[n*2];
         //    status = fwrite(buf2,1,256,fout);
         //    }         
        //else
							status = fwrite(bufr,1,512,fout);
							
							kapp->processEvents() ;
							
														
							copied = copied+status;
      
							if (abortf) { abortf=0 ; goto closefiles; }
							
							
							prog = 100*m/SecCnt;
							if (prog>prnxt) {    
								progressBar1->setProgress( prog+1 );    
								prnxt = prnxt+1 ;
							} 
							
							
					}   
		closefiles:		fclose(finp);
					fclose(fout);
     
					setCursor(QCursor(Qt::ArrowCursor));
       //Print out info about data transfer:
                 
					for( n = 0; n < 60; n++ ) dstr[n] = 0; //Clear string     
        
					qhm.setNum(m); 
					
					strcat(dstr,qhm);
					strcat(dstr," sectors copied from img. file into file of ");
					qhm.setNum(copied+cophh);
					strcat(dstr, qhm);
					strcat(dstr," bytes.");
					
					curopLabel->setText(dstr);
					
   					
                 
                 
				} // filesel success end
		} // else after Fsub if end
	}   // vice versa end 
                                          
	
	   // Here comes read from drives, medias...:
	else   { 
		
		
		
		if (form == 0)  
			fileName = KFileDialog::getSaveFileName(0, i18n("*.raw|Raw image files\n*.img|Img image files\n*.*|All files"), this, i18n("File to save"));
			
		else 	fileName = KFileDialog::getSaveFileName(0, i18n("*.hdf|Hdf image files\n*.*|All files"), this, i18n("File to save"));
		
		if( !fileName.isEmpty() )  {
	
			kapp->processEvents() ;
						  
			
			
			int  status=0;
			ULONG copied = 0;
   
			ULONG cophh = 0; 
          
			for (k=0;k<9;k++) physd[k] = detDev[k][selected];
    
						
			inf4Label->setText(physd); //testing!!!
			
			finp = fopen(physd,"rb");
			if (finp == NULL) { 
   
				KMessageBox::error( this, "File open error ", "Damn!" );
				act=0; return; 
			}
		
		
		
			fout = fopen(fileName,"wb");
			if (fout == NULL) { 
     //  printf("File open error!\n"); 
				fclose(finp);     
				act=0; return ;
			}
	
			curopLabel->setText("Reading from drive...");
		
			if (form >0) {  //if hdf wanted 
  
   //Insert CHS in hdf header:
         
				rsh[28] = heads; 
				rsh[24] = cylc;  
				rsh[25] = cylc>>8;  
				rsh[34] = sectr ;
       
				if (form == 2) { //Hdf 256
					rsh[8] = 1; }
					else rsh[8] = 0;
        
                   
					for( n = 0; n < 128; n++ ) {
          
						fputc(rsh[n],fout);
					}
					cophh = 128;
 
			}
		
	
			int prog = 0;
			int prnxt = 1;
			   
			fseek(finp, OffFS*512, SEEK_SET);
			progressBar1->setProgress( 0 );
			setCursor(QCursor(Qt::WaitCursor));
			
			//return;
			
			
			for( m = 0; m < SecCnt; m++ ) {
	
	
				status=fread(bufr,1,512,finp);
            
				if (form == 2) {
					for (n = 0; n<256; n++) buf2[n] = bufr[n*2];
					status = fwrite(buf2,1,256,fout);
				}         
				else 
				{
					if (Fsub) {       
      					 // Swap L/H
					for (n = 0; n<512; n=n+2) { buf2[n] = bufr[n+1]; buf2[n+1] = bufr[n] ; }          
					status = fwrite(buf2,1,512,fout);
					}
					else   status = fwrite(bufr,1,512,fout);
        
					}  			
				
				copied = copied+status;
				
	
				kapp->processEvents() ;
				      
				if (abortf) { abortf=0 ; goto closef3; }
							
							
				prog = 100*m/SecCnt;
				if (prog>prnxt) {    
					progressBar1->setProgress( prog+1 );    
					prnxt = prnxt+1 ;
				} 
	
			}
			
		closef3:	fclose(finp);
				fclose(fout);
				
				setCursor(QCursor(Qt::ArrowCursor));
				
				for( n = 0; n < 60; n++ ) dstr[n] = 0; //Clear string     
        
				qhm.setNum(m); 
					
				strcat(dstr,qhm);
				strcat(dstr," sectors copied from drive into file of ");
				qhm.setNum(copied+cophh);
				strcat(dstr, qhm);
				strcat(dstr," bytes.");
					
				curopLabel->setText(dstr);	
				
			
	
		}
	}
	
	
	
	act=0; }
	
	
	
}

void drimgWidget::sizeCHSButton_clicked()
{
	if (!act) {
	
	getCHS();

	newsec = sectr*heads*cylc;
	SecCnt = newsec;
	qhm.setNum(newsec);
	
	strcpy(dstr,"Sec. count set: ");           
                
	strcat(dstr,qhm);                         
	inf3Label->setText(dstr);
	//SetDlgItemText(hDlgWnd, Sta3, text1);
        	                                             
	seccnEdit->setText(qhm);
	OffFS = 0 ;
	secofEdit->setText("0");
	
	}

}




void drimgWidget::lbaButton_clicked()
{
	if (!act) {
	
	getCHS();
	                
	SecCnt = sectr*heads*cylc;
                
	sectr = 32 ;
	heads = 8 ;
	cylc = SecCnt/sectr/heads ;   
                
	qhm.setNum(sectr);	  
	edSecTr->setText(qhm);
	  	 
	qhm.setNum(heads);
	edHeads->setText( qhm );
	  
	qhm.setNum(cylc);
	edCyl->setText( qhm );                        
        
	}
}

void drimgWidget::creimfButton_clicked()
{
	
	if (!act) {
		act=1;
	
 // Create empty image file in format by selection & by CHS settings
               
	getForm(); //Set format flag according to RB-s
	 // Get CHS parameters from EDboxes:
	getCHS();    // CHS too  
	SecCnt = sectr*heads*cylc;      

	qhm.setNum(SecCnt);
	seccnEdit->setText( qhm );
	                         
	//inf5Label->setText( " ");  // Clear                   
                                 
              
	OffFS = 0 ;
	secofEdit->setText( "0" );
	
	// Open fileselector for giving name:
         
	if (form == 0)  
		
		fileName = KFileDialog::getSaveFileName(0, i18n("*.raw|Raw image files\n*.img|Img image files\n*.*|All files"), this, i18n("File to create"));
				
		else  	fileName = KFileDialog::getSaveFileName(0, i18n("*.hdf|Hdf image files\n*.*|All files"), this, i18n("File to create")); 
		
		if( !fileName.isEmpty() )  {                          
               
			
			
			//update() ;
			kapp->processEvents() ;			
			
			
			//UpdateWindow(hDlgWnd); 
			
			fout = fopen(fileName,"wb");
				if (fout == NULL) { act=0;
					return; }
                   
                  
				int prog = 0;
				int prnxt = 1;      
            
				
            
				curopLabel->setText( "Creating image file..."); 
				progressBar1->setProgress( 0 );
				
				setCursor(QCursor(Qt::WaitCursor));
				//SetCursor( jc) ;
             
				
				m = 0 ; 
               
				if ( form > 0 ) { // HDF images 
             // Set values in HDF header:
                       
					rsh[28] = heads; 
					rsh[24] = cylc;  
					rsh[25] = cylc>>8;  
					rsh[34] = sectr ;
               
					if (form == 2)   rsh[8] = 1; //Hdf 256
					else rsh[8] = 0;    
              
					status = fwrite(rsh,1,128,fout);          
					m = m+128 ;            
                          
				}
             
				if ( form == 2 ) secsize = 256 ;
				else secsize = 512 ;
             // Just make file with totalsectors*512 (or 256) 0 bytes
            
              //clear buffer:              
                
				for (n=0;n<secsize;n++) bufr[n] = 0 ;
               							
				g = sectr*heads*cylc ;
               
				for (f=0;f<g;f++) {
                  
					status = fwrite(bufr,1,secsize,fout);      
					    
					kapp->processEvents() ;	
					
					m = m+secsize ;
					      
					if (abortf) { abortf=0 ; goto crefclos; }
					
					//u = m/secsize ;
					prog = (100*(m/secsize))/SecCnt;
					if (prog>prnxt) {    
						progressBar1->setProgress( prog+1 );    
						prnxt++ ;
					} 
							
                   
				}  // some check needed!
             
                        
             
            			 //Close file:
                     
		crefclos:		fclose(fout);       
             
		   		
		       		setCursor(QCursor(Qt::ArrowCursor));
			 
				for( n = 0; n < 60; n++ ) dstr[n] = 0; //Clear string     
        
				qhm.setNum(m);
        
				strcpy(dstr,"Image file of ");
				strcat(dstr,qhm);
                
				strcat(dstr," bytes created.");
        
				curopLabel->setText(dstr); 
             
							
             
			} // if filesel end	
	
	
			//curopLabel->setText(dstr); 
	
	act=0;
	
	}
}





void drimgWidget::initzButton_clicked()
{
	
	getForm();
	getCHS();
	
	iniZ().exec();
	
	// Correct CHS if Big Sector used:
	qhm.setNum(sectr);	  
	edSecTr->setText(qhm);
	  	 
	qhm.setNum(heads);
	edHeads->setText( qhm );
	  
	qhm.setNum(cylc);
	edCyl->setText( qhm );
	
	
}




void drimgWidget::aboutButton_clicked()
{
	
	
	about().exec() ;
	
	/*
	aboutDlg = new KAboutDialog( KAboutDialog::AbtProduct, "Drive Image", KDialogBase::Ok, KDialogBase::Ok, 0, "Drive Image", true );
	aboutDlg->setProduct( "Drive Image V ", "0.1", "P. Putnik", "2006" ); 
	aboutDlg->show();
	*/ 
	
}

void drimgWidget::refrButton_clicked()
{
	
	detSDloop() ; // Detect SCSI (USB) drives attached
	
	
	/*
	
	 finp = fopen(devc,"rb") ;
	
	
	 if (finp) {
		 
		 KMessageBox::error( this, "Drive opened", "Yes!" );
	 
	fread(bufr,1,512,finp) ;
	
	fclose(finp);
	
	
	
	 }
	*/
	
	
}

void drimgWidget::abortButton_clicked()
{
	if (act) abortf = 1 ;
	
}

void drimgWidget::listBox1_clicked(QListBoxItem*)
{
	
	selected = listBox1->currentItem() ;
	
	
	           
	qhm.setNum(selected); 
	strcpy(dstr,"Drive: ") ;                
	strcat(dstr,qhm);                           
					
	inf1Label->setText(dstr ); //Info about selected drive - better write here /dev...
	inf2Label->setText(" ");
	
	SecCnt = exdl[selected];
	inf3Label->setText(" ");   //Clear info line 3  
	
	// Dev descriptor: TODO
	
	
                      // Insert in EDsecC field SecCnt :
                    
	qhm.setNum(SecCnt);
	seccnEdit->setText(qhm);
	
	//ltoa(SecCnt,khm,10);                                              
	//SetDlgItemText(hDlgWnd, EDsecC, khm);
                    
	
                       //Insert by sector count to edit boxes:
	UINT secw;
	sectr = 32;
                                              
	secw = (SecCnt-1000)/512 ;
                       
	heads = 2;
	if (secw>64) heads = 4 ;
	if (secw>128) heads = 8;
	if (secw>512) heads = 16;
	if (secw>1024) heads = 32;
                       
	cylc = SecCnt/(heads*sectr) ; //Sector per track is fixed to 32
	
	     
//Get sector #0 and see filesystem on drive:
                                                                                    
                      // Clear buffer ahead:
	for (n=0;n<512;n++) bufr[n] = 0 ;         
                      
	for (k=0;k<9;k++) physd[k] = detDev[k][selected];
    
						
	inf4Label->setText(physd); //testing!!!
			
	finp = fopen(physd,"rb");
	if (finp == NULL) { 
   
		KMessageBox::error( this, "Drive open error ", "Root?" );
		act=0; return; 
	}
	
	
                      
	fread(bufr,1,512,finp);   
	fclose(finp);
                 
	OffFS = 0 ;
	secofEdit->setText("0");
	
	
	
	// Detectiing used file(partition) system:
	
	Filesys = 0;
	Swf = 0 ;
                       
	char PLUSIIS[]= "PLUSIDEDOS      ";
	char OSID[16];
	int matc = 0;
                       
                       //Get first 16 char:
                                          
	for( n = 0; n < 16; n++ ) OSID[n]=bufr[n];
	for( n = 0; n < 16; n++ ) {if (OSID[n]==PLUSIIS[n]) matc++ ;}
                      
	if (matc == 16) {                     
		
		inf2Label->setText(PLUSIIS);
		//SetDlgItemText(hDlgWnd, Sta2, PLUSIIS);            
                           
                             //Get CHS from disk's boot sector:
		heads = bufr[34];
		sectr = bufr[35];
		cylc = bufr[32]+256*bufr[33];
                          
	}                 
                      
                      //Check is 8-bit IDEDOS:
	matc = 0;
	for( n = 0; n < 16; n++ ) OSID[n]=bufr[n*2];        
	for( n = 0; n < 16; n++ ) {if (OSID[n]==PLUSIIS[n]) matc++ ;}
                      
	if (matc == 16) {                     
		inf2Label->setText("PLUSIDEDOS 256 byte sect");
                      
		heads = bufr[68];
		sectr = bufr[70];
		cylc = bufr[64]+256*bufr[66];
                          
		// Check Hdf256:
		h256RB->setChecked(1);
		hdfRB->setChecked(0);
		rawRB->setChecked(0);
		
	}
                     
  
                          
                         
                           //Put CHS values in ED boxes
	qhm.setNum(sectr);	  
	edSecTr->setText(qhm);
	  	 
	qhm.setNum(heads);
	edHeads->setText( qhm );
	  
	qhm.setNum(cylc);
	edCyl->setText( qhm );
  
                  
                  
   
                          // Check for GemDos:
                                   
	if (bufr[0x1c7] == 'G')
		if (bufr[0x1c8] == 'E')       
			if (bufr[0x1c9] == 'M') {   
		inf2Label->setText("GemDos"); 
		Filesys = 11 ; }
   
		if (bufr[0x1c7] == 'X')
			if (bufr[0x1c8] == 'G')       
				if (bufr[0x1c9] == 'M') {   
			inf2Label->setText("GemDos"); 
			Filesys = 11 ; }   
   
                  
			
			if (bufr[0x1c7] == 'B')
				if (bufr[0x1c8] == 'G')       
					if (bufr[0x1c9] == 'M') {   
				inf2Label->setText("GemDos"); 
				Filesys = 11 ; }   
			
				// Check for GemDos by swapped H/L :
                                       
                                       
				for (n=0;n<512;n=n+2) { bufsw[n] = bufr[n+1];  bufsw[n+1] = bufr[n]; }          
   
				if (bufsw[0x1c7] == 'G')
					if (bufsw[0x1c8] == 'E')       
						if (bufsw[0x1c9] == 'M') {   
					inf2Label->setText("GemDos SW");  
					Filesys = 11 ; Swf = 1; }
   
					if (bufsw[0x1c7] == 'X')
						if (bufsw[0x1c8] == 'G')       
							if (bufsw[0x1c9] == 'M') {   
						inf2Label->setText("GemDos SW"); 
						Filesys = 11 ; Swf = 1;}   
   
   
   
						if (bufsw[0x1c7] == 'B')
							if (bufsw[0x1c8] == 'G')       
								if (bufsw[0x1c9] == 'M') {   
							inf2Label->setText("GemDos SW");
							Filesys = 11 ; Swf = 1;}   		
			
			
			
                            // See is FAT16 partition:
                                   
			if (bufr[21] == 0xF8) {                              
				inf2Label->setText("FAT16 part.");            
				Filesys = 13 ; goto clrfilenam ; }
			
			
			
                  
                        //See is DOS mbr:
                              
			if (bufr[510] == 0x55) {
				if (bufr[511] == 0xAA) {
					inf2Label->setText("DOS mbr");            
					Filesys = 1 ; }
			}     
  
  
   
                          
                      // Check for PP Charea doS

			if ( bufr[0] == 0x41 )   //A
				if ( bufr[12] == 0x42 )  //B
					if ( bufr[24] == 0x43 )  { //C
				strcpy(dstr,"Charea DoS");
                                                      
                         // Check is 8-bit simple IF drive/media:
                     
                      
				if ( bufr[0x1BC] == 'J' )      
					if ( bufr[0x1BE] == 'R' )   { 
                            
					strcat(dstr," 8-bit");
                       
					// Check Hdf256:
					h256RB->setChecked(1);
					hdfRB->setChecked(0);
					rawRB->setChecked(0);     
					}  
					
					if ( bufr[448] == 'B' )      
						if ( bufr[450] == 'S' ) strcat(dstr, " BigSect.");                        
					inf2Label->setText(dstr);
                      
					}
            
	clrfilenam:		inf6Label->setText(" "); // Clear filename info
}





void drimgWidget::writeButton_clicked()
{
	
	
	if (!act) {
		act = 1;

	        
		getForm() ;
		
		if (form == 0)  
			fileName = KFileDialog::getOpenFileName(0, i18n("*.raw *.img|RAW & IMG image files\n*.*|All files"), 0, i18n("File for write")); // 0 instead this here !!!
			
		else 	fileName = KFileDialog::getOpenFileName(0, i18n("*.hdf|Hdf image files\n*.*|All files"), this, i18n("File for write"));
		
		if( !fileName.isEmpty() )  {
	
			kapp->processEvents() ;

			if ( selected==99 )  strcpy(dstr,"Write to selected file!");
			else {	strcpy(dstr,"Write to selected drive ");
				qhm.setNum(selected); 
				strcat(dstr,qhm);            
				  
			}
// File to drive

			k = KMessageBox::warningContinueCancel(this, "Are you sure?", dstr);      
	
			if (k==2) { act=0; return;  }

                      int hDevice;            // handle to the drive or file for write on
     
			
		      if ( selected==99) 
		      {
			      
			      hDevice = open(loadedF, O_WRONLY   );       
			      
		      }
		      
		      else 
		      {
		      for (k=0;k<9;k++) physd[k] = detDev[k][selected];

			if ( ov2ro ) 
				if ( SecCnt>2097152) 
				{ KMessageBox::error( this, "Read only for large drives!", "Stop" );
				act=0; return;  }
			
       
			hDevice = open(physd, O_WRONLY   ); // | O_FSYNC );
		      }
			
			if (hDevice <0) // cannot open the drive
			{         
				KMessageBox::error( this, "Drive open error ", "Admin?" );
				act=0; return;  
			}
		     
 
       // Open image file for read:
			fout = fopen(fileName,"rb");
			if (fout == NULL) { 
     //  printf("File open error!\n"); 
				fileclo(hDevice);   
				act=0; return ;
			}
   
   	
			if ( selected==99) curopLabel->setText("Writing to file..."); 
			else curopLabel->setText("Writing to drive..."); 
       
			setCursor(QCursor(Qt::WaitCursor));
			ULONG cophh = 0; 
  // SecCnt = exdl[selected];  //Already set **** - Maybe changed.... see it
   
   
			if (form > 0) {  //if hdf to load
        
				cophh = 128;
			}
          
			fseek(fout, 0, SEEK_END) ;
           
			ULONG filelen;
    
			filelen = ftell(fout); //Filelength got
  
			fseek(fout, cophh, SEEK_SET) ; // Set to 0 by raw, or to 128 by hdf
    
			if (form == 2)  fsecc = (filelen-cophh)/256 ;
			else fsecc = (filelen-cophh)/512 ;
      			
			
			qhm = seccnEdit->text() ; 
			SecCnt = atol(qhm); 
			
			if (SecCnt < fsecc)  fsecc = SecCnt;
			
			qhm.setNum(fsecc);
			inf4Label->setText(qhm) ; // testing!!!
			
            
			//DWORD bytesw;
  
    //ULONG m;
			int prog = 0;
			int prnxt = 1;
			
			int  status=0;
			ULONG copied = 0;
    
			//*** Need to get value from edLine!!!
			qhm = secofEdit->text() ; 
			OffFS = atol(qhm);
			lseek(hDevice, OffFS*512, SEEK_SET );
     
     
     
			progressBar1->setProgress( 0 );
     
			    
			for( m = 0; m < fsecc; m++ ) {
        
       
             
				if (form == 2) {
					status=fread(buf2,1,256,fout);
					if (status<256) break;
					for (n = 0; n<256; n++)  bufr[n*2] = buf2[n];
				}         
				else {      
      
					status=fread(bufr,1,512,fout);
					if (status<512) break;
				}
       
				
				if (Fsub) {
       
       				// Swap L/H
					for (n = 0; n<512; n=n+2) { buf2[n] = bufr[n+1]; buf2[n+1] = bufr[n] ; }          
					copied = copied + write(hDevice,buf2,512) ;    
				}
         
				else	copied = copied+write(hDevice,bufr,512) ;
												
				kapp->processEvents() ;
                  
				if (abortf) { abortf=0 ; goto closef4; }
							
							
				prog = 100*m/fsecc;
				if (prog>prnxt)
					{  // Workaround because of delayed write   
					// SYNC mode is too slow
										
					if (selected==99) goto nocloopen ;
					
					n = fileclo(hDevice);
										
					hDevice = open(physd, O_WRONLY ); // To ensure correct progress bar when writing to Flash media
					
				nocloopen:	lseek(hDevice, OffFS*512+copied, SEEK_SET ); // restore pos
					
					progressBar1->setProgress( prog+1 );    
					prnxt = prnxt+1 ;
					} 
			} 
          
	closef4:        // sleep(1);
			     
			fclose(fout);
			
			n = fileclo(hDevice);
			setCursor(QCursor(Qt::ArrowCursor));
			
			
   //Print out info about data transfer:
                 
			
			//for( n = 0; n < 60; n++ ) {
			//	dstr[n] = 0; } //Clear string     
        
				qhm.setNum(copied);
				strcpy(dstr,qhm);
				strcat(dstr," bytes (");
				qhm.setNum(m);
				strcat(dstr, qhm);
				
				if (selected==99) strcat(dstr," sectors ) written to file.");				
				else strcat(dstr," sectors ) written to drive.");
        
				curopLabel->setText(dstr); 
          
   
		}

	}

act = 0;
	
	
}

void drimgWidget::helpButton_clicked()
{
	
	KProcess *kfm = new KProcess();
	*kfm << "firefox" << "help/drimlus.htm";
	//*kfm << "firefox" << "http://localhost/";
	
	kfm->start( KProcess::DontCare );
	
}

void drimgWidget::ov2roCB_clicked()
{
	// Set flag according to CB state:
	if( ov2roCB->isChecked() ) ov2ro = 1;
	else ov2ro = 0 ;
	
}

void drimgWidget::swapCB_clicked()
{
	if( swapCB->isChecked() ) Fsub = 1;
	else Fsub = 0 ;
	
}
