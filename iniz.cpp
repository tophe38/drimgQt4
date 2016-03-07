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


#include "iniz.h"
#include <qcursor.h>
#include <qlabel.h>
#include <qlineedit.h>

#include "ui_initZdlg4.h"

typedef unsigned char  byte;

typedef unsigned long  ULONG;
typedef unsigned int  UINT;


extern int  clust, secsize ;
extern byte oversiz;

extern int n, c, i, k, s;
extern ULONG f, g, m, u ; // general var
 
extern  long fr ;

extern UINT step, cyld ;
extern UINT sectr;
extern   ULONG cylc ;
extern   UINT heads ;  //CHS parameters
extern   int  status;
extern byte datars;

extern int form ;
extern QString op, qhm;
extern int hdfhea ;
extern bool ov2ro;

extern ULONG fsecc, rest;

extern char dstr[60];
extern QString litet;
extern ULONG SecCnt; 

extern char physd[9];
extern char detDev[9][16] ;
extern int selected;

extern char loadedF[256] ;
extern int pfilelen, mcstep;

extern unsigned char eibuf[512] ; 
extern char driv8b ;
extern unsigned char  bufr[640];
extern unsigned char  buf2[512];


 int driv;

//_IO_FILE& drih;


UINT chaszs[36] ;
byte catsiz[36] ;

  int seldCha;

  char chas[36] = {'A','B','C','D','E','F','G','H','I','J',
	  'K','L','M','N','O','P','Q','R','S','T',
	  'U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9'} ;

 int proms[36] = { 60,60,40,35, 30,40,35,30, 10,20,20,40,
		  45,30,10,35, 10,45,90,60, 15,30,30,10, 10,10,15,15,
		  15,15,15,15, 15,15,15,15 } ; // total 100 promils




// This is because of function name 'close' conflict!!!!!		  
int fileclo(int fd)
{
 return	close(fd); 
}	  
		  
		  
		  
		  
		  
void iniZ::calctot()
{		  
		         
		  m = 0 ;
		  for (n=0;n<36;n++) {             
            
			  m = m+chaszs[n]*step ;
            
		  }  

		  fr = g-m ;
		  m = m/2 ;
		  if ( form == 2 ) m = m/2 ; 
		  qhm.setNum(m);
            //ltoa(m,khm,10) ;
		  totsizesInf->setText(qhm);
             //SetDlgItemText(hDlgWnd, TotSizD, khm);
             
		  fr = fr/2;
		  if ( form == 2 ) fr = fr/2 ; 
		  qhm.setNum(fr);
             //ltoa(fr,khm,10) ;
		  freeInf->setText(qhm);
            
		  
}		  






void iniZ::getkb1()
		
{
	if (form == 2) u = u/2 ;
        qhm.setNum(u);
        //ltoa(u,khm,10) ;
	datsizEB->setText(qhm);
        //SetDlgItemText(hDlgWnd, SizeE, khm); 

        // get % of size:
             
               
	m = chaszs[seldCha]*step*1000/g ;
	qhm.setNum(m);
         //ltoa(m,khm,10) ;
	promInf->setText(qhm);
         //SetDlgItemText(hDlgWnd, SizePerD, khm);        
          
         // Correct text in ListBox:
                        
	dstr[0] = chas[seldCha] ; 
	dstr[1] = 0 ;
	strcat(dstr," - ");
        
                
	u = chaszs[seldCha]  ; // get cyl count
	u = u*step/2 ;
	if ( form == 2 ) u = u/2 ;   
	qhm.setNum(u);
        //ltoa(u,khm,10) ;        
        
	c = strlen(qhm);
        
	for (k=c;k<5;k++) strcat(dstr," ") ;
        
	strcat(dstr,qhm) ;
	strcat(dstr," KB  ");
        
	f  = catsiz[seldCha];
	f = f*16 ;
	if ( form == 2 ) f = f/2 ;
	qhm.setNum(f);
        //itoa(f,khm,10) ;
	c = strlen(qhm);
	for (k=c;k<4;k++) strcat(dstr," ") ;
	strcat(dstr,qhm) ;  // CAT slot count
            
               
	listBox1->changeItem(dstr,seldCha) ;
	
	              
}






iniZ::iniZ(QWidget *parent) :
    QDialog(parent),
    ui( new Ui::initZbase)
{
    //WM_INITDIALOG:

          clust = 1;
          oversiz = 0 ;
        
       // Calc total capacity in KB:
               
             g = sectr*heads*cylc ;
             u = g/2 ;
             if ( form == 2 ) u = u/2 ;
               
             qhm.setNum(u);   
             //ltoa(u,khm,10) ;
             strcpy(dstr,qhm) ;
             strcat(dstr," KB") ;
            
             totdcLab->setText(dstr);
             //SetDlgItemText(hDlgWnd, TotDisCD, dstr );
    
    
          //SetDlgItemText(hDlgWnd, DatsE, "1"); // Initially 1
    
          step = sectr*heads ;
    
          g = g-step ; // Need EDbox for set start cyl for A - done
        
            u = g/2 ;
             if ( form == 2 ) u = u/2 ;
              
             qhm.setNum(u);  
             //ltoa(u,khm,10) ;
             strcpy(dstr,qhm) ;
             strcat(dstr," KB") ;
             placLab->setText(dstr);
             //SetDlgItemText(hDlgWnd, PlacD, dstr );
        
        
         cyld = cylc-1 ;
    
        m = 1 ; //cyl counter
        rest = 0 ; // count rest sectors
    
    
        listBox1->clear();
    // Fill listbox with 36 char:
            
        for (n=0;n<36;n++) {    
        
        dstr[0] = chas[n] ; 
        dstr[1] = 0 ;
        strcat(dstr," - ");
        
        // Calc capacity of charea in KB :
        // Sector size goes in steps sectr*heads!
                
      
        u =  cyld*proms[n]/1000 ; // cylinders for charea
      
        // Sector count for charea can't be over 65536 
      
        if ( (u*step)>65536 ) { oversiz = 1 ;
                  
           // Calc u to get max 65536 sectors:
            f = 65536/step ;
                          
            rest = rest+u*step-f*step ;
            u = f ;      
         }
      
        else { rest = rest + cyld*proms[n]*step/1000 - u*step ;
      
        while ( rest>(step-1) ) 
        { u++ ; rest = rest-step ; 
        if ( (u*step)>65536 ) { u-- ; rest = rest+step ; break ;}                   
           }
        
             }
        
        // Calculate CAT size in sectors:
         f = u*step/244 ; // !!!! value----            
         if ( f>254 ) f = 254 ;
        
        catsiz[n] = f ;
        chaszs[n] = u ; // store cyl count
        u = u*step/2 ;
        if ( form == 2 ) u = u/2 ;   
        
        qhm.setNum(u);
        //ltoa(u,khm,10) ;
       
        c = strlen(qhm);
        
        for (k=c;k<5;k++) strcat(dstr," ") ;
        
        strcat(dstr,qhm) ;
        strcat(dstr," KB  ");
        
        f = f*16 ;
        if ( form == 2 ) f = f/2 ;
        qhm.setNum(f);
        //itoa(f,khm,10) ;
        c = strlen(qhm);
        for (k=c;k<4;k++) strcat(dstr," ") ;
        strcat(dstr,qhm) ;  // CAT slot count
              
         listBox1->insertItem(dstr,-1) ; 
            
         //SendMessage(GetDlgItem(hDlgWnd, INLB), LB_ADDSTRING, 0, (LPARAM) dstr );
     

            }
            
          
          // Calculate total sizes sum:
        
          m = 0 ;
          for (n=0;n<36;n++) {             
            
            m = m+chaszs[n]*step ;
            
            }  

            fr = g-m ;
            m = m/2 ;
            if ( form == 2 ) m = m/2 ; 
            qhm.setNum(m);
            //ltoa(m,khm,10) ;

            totsizesInf->setText(qhm);
             //SetDlgItemText(hDlgWnd, TotSizD, khm);
             
             fr = fr/2;
             if ( form == 2 ) fr = fr/2 ; 
             qhm.setNum(fr);
             //ltoa(fr,khm,10) ;
             freeInf->setText(qhm);
             //SetDlgItemText(hDlgWnd, FreeD, khm);
             seldCha = 77 ; // Give impossible value

    
        
    
}







void iniZ::exitBut_clicked()
{
}




 
void iniZ::listBox1_clicked(QListBoxItem*)
{
        if (g==0) goto libclend;               
	
        seldCha = listBox1->currentItem() ; // seldCha instead index!!!

                            
      //  u =  g*proms[index]/2000 ; // 
      //  if ( form == 2 ) u = u/2 ;   
        
        u = (chaszs[seldCha]*step-catsiz[seldCha]-1)/2 ;
        
	
	
	if (form == 2) u = u/2 ;
        qhm.setNum(u);
        datsizEB->setText(qhm);
        
        // get % of size:
	 
               
         m = chaszs[seldCha]*step*1000/g ;
         qhm.setNum(m);
         promInf->setText(qhm);
	 
         c = catsiz[seldCha]*16 ;
         if (form == 2) c = c/2 ;
         
                
         caslEB->setText(qhm.setNum(c)) ; //!!!!
        
	 libclend: ;
}



  // Data area start setting:

  // case DatasSB:
void iniZ::dataatBut_clicked()
{   
                     
          litet = dataatEB->text();
          
             //GetDlgItemText(hDlgWnd, DatsE , szText , (LPARAM) 64); 
                 datars = atoi(litet);       
                 
                 if ( datars == 0 ) { datars = 1 ;    
                 dataatEB->setText("1"); }
                 //SetDlgItemText(hDlgWnd, DatsE, "1"); } // No 0 allowed!
                 
                 // Now update all :
                 
               
                // Calc total capacity in KB:
               
             g = sectr*heads*cylc ;
             u = g/2 ;
             if ( form == 2 ) u = u/2 ;
               
             qhm.setNum(u);   
             //ltoa(u,khm,10) ;
             strcpy(dstr,qhm) ;
             strcat(dstr," KB") ;
             totdcLab->setText(dstr);
             //SetDlgItemText(hDlgWnd, TotDisCD, dstr );
    
           
    
          g = g-step*datars ; // 
        
            u = g/2 ;
             if ( form == 2 ) u = u/2 ;
               
             qhm.setNum(u);   
             //ltoa(u,khm,10) ;
             strcpy(dstr,qhm) ;
             strcat(dstr," KB") ;
             placLab->setText(dstr);
             //SetDlgItemText(hDlgWnd, PlacD, dstr );
        
        
         cyld = cylc-datars ;
    
            
     // Clear listbox:
             
           listBox1->clear();   
           //SendMessage(GetDlgItem(hDlgWnd, INLB), LB_RESETCONTENT, 0, 0);  //Empty list
    
    // Fill listbox with 36 char:
            
        for (n=0;n<36;n++) {    
        
        dstr[0] = chas[n] ; 
        dstr[1] = 0 ;
        strcat(dstr," - ");
        
                
        u = chaszs[n]  ; // get cyl count
        u = u*step/2 ;
        if ( form == 2 ) u = u/2 ;   
        
        qhm.setNum(u);
        //ltoa(u,khm,10) ;
                
        c = strlen(qhm);
        
        for (k=c;k<5;k++) strcat(dstr," ") ;
        
        strcat(dstr,qhm) ;
        strcat(dstr," KB  ");
        
        f  = catsiz[n];
        f = f*16 ;
        if ( form == 2 ) f = f/2 ;
        qhm.setNum(f);
        //itoa(f,khm,10) ;
        c = strlen(qhm);
        for (k=c;k<4;k++) strcat(dstr," ") ;
        strcat(dstr,qhm) ;  // CAT slot count
       
          listBox1->insertItem(dstr,-1) ;   
         //SendMessage(GetDlgItem(hDlgWnd, INLB), LB_ADDSTRING, 0, (LPARAM) dstr );
     
     
            }
            seldCha = 77 ; // Give impossible value
          
          // Calculate total sizes sum:
        
         calctot() ;
             
       

}

  
void iniZ::dasdBut_clicked()
{
	               
              if ( seldCha > 35 ) goto dasdend ;   
                     
          n = chaszs[seldCha] ;
          n-- ;
          if ( n<0 ) n = 0 ;
         
          chaszs[seldCha] = n ; 
          u = (n*step-catsiz[seldCha]-1)/2 ;
	  
	  getkb1();    
	                   
          calctot() ; 

                           
	 dasdend: ;                 
}



void iniZ::dasuBut_clicked()
{

               
                                   
              if ( seldCha > 35 )goto dasuend ;
               
              n = chaszs[seldCha] ;
              n++ ;
          // Check for over 65536 sectors:
               
             if ( (n*step)>(clust*65536) ) n-- ;      
                 
	     chaszs[seldCha] = n ; 
	     u = (n*step-catsiz[seldCha]-1)/2 ;
	  
	      getkb1();    
	      calctot() ; 
                                    
                   
	dasuend: ;
}




void iniZ::casdBut_clicked()
{
                     
              if ( seldCha > 35 ) goto casdend ; 
                 
              c = catsiz[seldCha] ;
              c-- ;
              if ( c<1 ) c = 1 ;
	      
         catsiz[seldCha] = c ;
              c = c*16 ;
         if (form == 2) c = c/2 ;
         qhm.setNum(c);
         //itoa(c,khm,10) ;
         caslEB->setText(qhm);
         //SetDlgItemText(hDlgWnd, CATsloE, khm);
          u = (chaszs[seldCha]*step-catsiz[seldCha]-1)/2 ;    
                
	  
	  getkb1();    
	                   
	  calctot() ;            

	casdend: ;
}    

  
  

void iniZ::casuBut_clicked()
{
              
                     
                if ( seldCha > 35 ) goto casuend ;     

              c = catsiz[seldCha] ;
              c++ ;
              if ( c>254 ) c = 254 ;
	      
	      catsiz[seldCha] = c ;
	      c = c*16 ;
	      if (form == 2) c = c/2 ;
	      qhm.setNum(c);
         //itoa(c,khm,10) ;
	      caslEB->setText(qhm);
         //SetDlgItemText(hDlgWnd, CATsloE, khm);
	      u = (chaszs[seldCha]*step-catsiz[seldCha]-1)/2 ;    
                
	  
	      getkb1();    
	                   
	      calctot() ;   	      
	      
	      
	      casuend: ;
}





void iniZ::bigsCB_clicked()
{
                    
       // Get checkbox state:
              
          if ( bigsCB->isChecked() )
           {   
          // Set LBA geo:
           SecCnt = sectr*heads*cylc;
                
            sectr = 32 ;
            heads = 8 ;
            cylc = SecCnt/sectr/heads ;           
          
          clust = 2; // 1, 2 or 4...
          if ( form == 2 ) clust = 4 ;      
           }
         else clust = 1 ;
      
          oversiz = 0 ;
        
       // Calc total capacity in KB:
               
             g = sectr*heads*cylc ;
             u = g/2 ;
             if ( form == 2 ) u = u/2 ;
               
             qhm.setNum(u);   
             //ltoa(u,khm,10) ;
             strcpy(dstr,qhm) ;
             strcat(dstr," KB") ;
             totdcLab->setText(dstr);
             //SetDlgItemText(hDlgWnd, TotDisCD, dstr );
                     
          step = sectr*heads ;   
       
          g = g-step*datars ; // 
        
            u = g/2 ;
             if ( form == 2 ) u = u/2 ;
                
             qhm.setNum(u);
             //ltoa(u,khm,10) ;
             strcpy(dstr,qhm) ;
             strcat(dstr," KB") ;
             placLab->setText(dstr);
             //SetDlgItemText(hDlgWnd, PlacD, dstr );
        
        
         cyld = cylc-datars ;
        
        listBox1->clear(); 
        //SendMessage(GetDlgItem(hDlgWnd, INLB), LB_RESETCONTENT, 0, 0);  //Empty list
    
        m = 1 ; //cyl counter
        rest = 0 ; // count rest sectors
    
    // Fill listbox with 36 char:
            
        for (n=0;n<36;n++) {    
        
        dstr[0] = chas[n] ; 
        dstr[1] = 0 ;
        strcat(dstr," - ");
        
        // Calc capacity of charea in KB :
        // Sector size goes in steps sectr*heads!
                
      
        u =  cyld*proms[n]/1000 ; // cylinders for charea
      
        // Sector count for charea can't be over 65536 
      
        if ( (u*step)>(clust*65536) ) { oversiz = 1 ;
                  
           // Calc u to get max 65536 sectors:
            f = (clust*65536)/step ;
                          
            rest = rest+u*step-f*step ;
            u = f ;      
         }
      
        else { rest = rest + cyld*proms[n]*step/1000 - u*step ;
      
        while ( rest>(step-1) ) 
        { u++ ; rest = rest-step ; 
        if ( (u*step)>(clust*65536) ) { u-- ; rest = rest+step ; break ;}                   
           }
        
             }
        
        // Calculate CAT size in sectors:
         f = u*step/244 ; // !!!! value----            
         if ( f>254 ) f = 254 ;
        
        catsiz[n] = f ;
        chaszs[n] = u ; // store cyl count
        u = u*step/2 ;
        if ( form == 2 ) u = u/2 ;   
        qhm.setNum(u);
        //ltoa(u,khm,10) ;        
        
        c = strlen(qhm);
        
        for (k=c;k<5;k++) strcat(dstr," ") ;
        
        strcat(dstr,qhm) ;
        strcat(dstr," KB  ");
        
        f = f*16 ;
        if ( form == 2 ) f = f/2 ;
        qhm.setNum(f);
        //itoa(f,khm,10) ;
        c = strlen(qhm);
        for (k=c;k<4;k++) strcat(dstr," ") ;
        strcat(dstr,qhm) ;  // CAT slot count
              
        listBox1->insertItem(dstr,-1) ;    
        // SendMessage(GetDlgItem(hDlgWnd, INLB), LB_ADDSTRING, 0, (LPARAM) dstr );
     

            }
            
          
          // Calculate total sizes sum:
        
          m = 0 ;
          for (n=0;n<36;n++) {             
            
            m = m+chaszs[n]*step ;
            
            }  

            fr = g-m ;
            m = m/2 ;
            if ( form == 2 ) m = m/2 ; 
            qhm.setNum(m);
            //ltoa(m,khm,10) ;

            totsizesInf->setText(qhm);
             //SetDlgItemText(hDlgWnd, TotSizD, khm);
             
             fr = fr/2;
             if ( form == 2 ) fr = fr/2 ; 
             qhm.setNum(fr);
             //ltoa(fr,khm,10) ;
             freeInf->setText(qhm);
             //SetDlgItemText(hDlgWnd, FreeD, khm);
             seldCha = 77 ; // Give impossible value
                   
            
}                 
                   
                   

void iniZ::performBut_clicked()
{
                        
                
              // Warning message Box !!!
              
	k = KMessageBox::warningContinueCancel(this, "Are you sure?", "Data loss danger!");      
	
	if (k==2) goto performex;
        
	  
              
	/*
	qhm.setNum(SecCnt); //testing
	freeInf->setText(qhm); //testing
	*/
	
	
	if (selected<16) 
	{  for (k=0;k<9;k++) physd[k] = detDev[k][selected];
	
	
	if (ov2ro)  if ( SecCnt>2097152 ) {
		driv = open(physd, O_RDONLY );
	 }
	
	else 
		driv = open(physd, O_RDWR );
	}
	
	else driv = open(loadedF, O_RDWR  );
         
                            
	if (driv <0) {   		    
		KMessageBox::error( this, "Drive/file open error", "Problem" );
		goto performex;
		
		//return ;  // Not good!!!
	}
		setCursor(QCursor(Qt::WaitCursor));
                   //SetCursor( jc) ;
                
                   secsize = 512 ;
                   mcstep = 12 ;
                   hdfhea = 0;
                   
                   driv8b = 0 ;
                   if ( selected<10 & form == 2 ) driv8b = 1; // Flag
                   
                   if ( ( selected == 99) && (form>0) ) {
                   //SetFilePointer(drih, 128, 0, FILE_BEGIN ); //Unnecessary
                   hdfhea = 128;  }
                  
                   if ( form == 2) { secsize = 256 ; mcstep = 6; } // for SIF (8-bit simple if files)
                
                // Create Master CAT:"Data loss danger!"
                          
                  for (n=0;n<512;n++) bufr[n] = 0 ; // clear bufr
                  
                  i = datars ; // Start cyl of charea A
                 
                  for (n=0;n<36;n++) {
                      
                  bufr[n*mcstep] = chas[n] ;
                  bufr[n*mcstep+1] = i ;  // LSB of cyl offset
                  bufr[n*mcstep+2] = i>>8 ;  // MSB
                  bufr[n*mcstep+3] = catsiz[n]+1 ; // FDS          
                  c = chaszs[n] ;
                  i = i+c ; // advance for next charea
                  m = (c*step)/clust ; // Size in BIG sectors (KB actually)
                  if ( m>65535 ) m = 65535 ; // Max value for 2 byte
                  bufr[n*mcstep+4] = m ; // LSB of total sec count
                  bufr[n*mcstep+5] = m>>8 ;  // MSB
                                 
                  } // n loop end
                
                // Add string JR for 8-bit ones:
                       
                    if ( form == 2) { bufr[222] = 'J' ; bufr[223] = 'R' ; }
		    else { bufr[444] = 0 ; bufr[446] = 0 ; }
                // Sign for BIG sectors:
                 if ( clust == 4)   { bufr[224] = 'B' ; bufr[225] = 'S' ; }
                 if ( clust == 1) { bufr[224] = 0 ; bufr[225] = 0 ; }
                  
                  if ( clust == 2) { bufr[448] = 'B' ; bufr[450] = 'S' ; }    
                  if ( clust == 1) { bufr[448] = 0 ; bufr[450] = 0 ; }
                
                 // Write it to disk/image:
                          
                    if ( driv8b ) {
                      lseek(driv, 0, SEEK_SET );                    
                     
                      for (n = 0; n<256; n++) eibuf[n*2] = bufr[n];
                      write(driv, eibuf, 512);                      
                   
                   // Write copy sect 2 too:            
                     lseek(driv, 1024, SEEK_SET );          
                     write(driv, eibuf, 512);                               
                                    }
                                
              else { lseek(driv, hdfhea, SEEK_SET) ; 
              write(driv, bufr, secsize); 
              
              // Copy:
              lseek(driv, secsize*2+hdfhea, SEEK_SET) ; 
              write(driv, bufr, secsize); 
                   }        
              
              // Now create all CAT startsectors and clear whole CATs:
                     
                
                for (n=0;n<512;n++) buf2[n] = 0 ; // clear buf2
                
                
                for (i=0;i<36;i++) {     
                     
                n = bufr[i*mcstep+3] ;
                c = n/clust ; // Calc BIG if is it...
                if ( ((n/clust)*clust)<n ) c++ ;
                buf2[0] = c ; // First free (BIG) sector init value LSB
                buf2[8] = c ; // This will be used by CAT command used/free calc
                buf2[1] = 0 ; // MSB 0 always
                buf2[2] = bufr[i*mcstep+4] ; //Total sect count LSB
                buf2[3] = bufr[i*mcstep+5] ;
                buf2[4] = bufr[i*mcstep+3] ;  // FDS by sectors always
                buf2[5] = bufr[i*mcstep] ;   // Char   
                buf2[6] = bufr[i*mcstep+1] ; // Cyl offset    
                buf2[7] = bufr[i*mcstep+2] ;
            
              // Write it:
                       
                  if ( driv8b ) {
                  lseek(driv, (buf2[6]+256*buf2[7])*step*512, SEEK_SET );
                                     
                  for (n = 0; n<256; n++) eibuf[n*2] = buf2[n];
                  write(driv, eibuf, 512);                      
                                    }
                                           
              else { lseek(driv, (buf2[6]+256*buf2[7])*step*secsize+hdfhea, SEEK_SET) ; 
              write(driv, buf2, secsize);  }      
            
              // Clear all CAT sectors:
                       
               for (n=0;n<10;n++) buf2[n] = 0 ; // clear buf2 begin
               
                if ( driv8b ) {
                        for ( c=0;c<(bufr[i*mcstep+3]-1);c++) {                                
                  for (n = 0; n<256; n++) eibuf[n*2] = buf2[n];
                  write(driv, eibuf, 512);                      
                                }    }
                                           
              else  { for ( c=0;c<(bufr[i*mcstep+3]-1);c++)
                     write(driv, buf2, secsize); 
                    }
               
                          
                 } // i loop end
                 
			 
             fileclo(driv); 
	     setCursor(QCursor(Qt::ArrowCursor)); // 
	performex: ;
	
	
}
          
                   
                   
                   
                   
              
            






#include "iniz.moc"
